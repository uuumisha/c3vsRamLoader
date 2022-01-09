//****************************************************************************
//*					Драйвер модуля "ПОТОК" под QNX 4.25						 *
//*  обеспечивающий сопряжение по мультиплексному каналу ГОСТ 26765.52-87    *
//****************************************************************************

#include <stdio.h>
#include <sys/kernel.h>
#include <sys/wait.h>
#include <conio.h>
#include <unistd.h>
#include <sys/types.h>
#include <process.h>
#include <time.h>
#include <math.h>
#include <sys/mouse.h>
#include <i86.h>
#include <ctype.h>
#include <sys/sched.h>
#include <fcntl.h>
#include <sys/irqinfo.h>

#include "inc/options.h"
#include "inc/potok/vme.h"
#include "inc/potok/potok.h"
#include "inc/potok/potok_regs.h"

static pid_t proxy;

//----------------------------------------------------------------------------------------------------------
//---------------------------  Функции для работы с контроллером канала ------------------------------------
//----------------------------------------------------------------------------------------------------------

// инициализация адаптера в режим КК
char PInit_KK(unsigned short nm) {
  
  unsigned short na;
  unsigned char *zeros;
  
  proxy = qnx_proxy_attach(0,0,0,-1);
  if(!nm) {
    // открытие файла прерываний для чтения и записи
    if(OpenVmeIntMilstd1()) {
      //printf("Unable to open VMEINT\n");
      return TMK_OPEN_FAIL;
    }
    
    // сообщение proxy обработчику прерываний
    if(SetProxyMilstd1(proxy)) {
      //printf("VMEINT is full. Can't start program.\n");
      return TMK_OPEN_FAIL;
    }
  }
  else {
    // открытие файла прерываний для чтения и записи
    if(OpenVmeIntMilstd2()) {
      //printf("Unable to open VMEINT\n");
      return TMK_OPEN_FAIL;
    }
    
    // сообщение proxy обработчику прерываний
    if(SetProxyMilstd2(proxy)) {
      //printf("VMEINT is full. Can't start program.\n");
      return TMK_OPEN_FAIL;
    }
  }
  
  // открытие шины A24 8 бит
  if(OpenVmeFdDrv2408()) {
    //printf("Unable to open VMEBUS2408\n");
    return TMK_OPEN_FAIL;
  }
  
  // открытие шины A24 16 бит
  if(OpenVmeFdDrv2416()) {
    //printf("Unable to open VMEBUS2416\n");
    return TMK_OPEN_FAIL;
  }
  
  if(nm) na=NA_ADDR1;	    // Определение начального адреса для 2-й микросхемы
	else   na=NA_ADDR0;			// Определение начального адреса для 1-й микросхемы
  
  // обнуление ОЗУ микросхемы 
  zeros = (unsigned char *) malloc(POTOK_MIC_SIZE - START_BASE_ADDR);
  memset(zeros, 0, sizeof(zeros));
  SetDataByte(na+START_BASE_ADDR, zeros, sizeof(zeros));
  free(zeros);
  
  SetByte(na+INTERRUPT_MASK, 0xFE);                     // разрешение команд приема/передачи без ошибок
  SetReg(na+POINTER_TABLE_ADDRESS, START_BASE_ADDR);    // адрес таблицы адресов баз
  SetReg(na+CONFIGURATION_1, 0x0100);                   // установка режима КК микросхемы
  SetReg(na+BASIC_STATUS, 0xF800);                      // может быть не надо
  //SetReg(na+CONTROL, 0x6080 + 0x00A0);                // таймаут ожидания ОС
  SetByte(na+CONFIGURATION_2, 0x00);
  SetReg(na+CONFIGURATION_3, 0x0003);                   // тоже непонятно что
  SetReg(na+RESET_FIFO, 0x0000);                        // сброс FIFO
  SetReg(na+FRAME_A_POINTER, START_BASE_ADDR>>1);       // таблица адресов
  return TMK_OPEN_OK;
}

void PBCPutCmd(_msg_kk* msg) {
	unsigned short na;
  if(msg->nm) na=NA_ADDR1;
	else        na=NA_ADDR0;
  
  SetReg(na+FRAME_A_LENGTH, 0x4001);               // Выдавать прерывание по окончании и кол-во сообщений
  SetReg(na+START_BASE_ADDR,(BASE_ADDR_KK>>1));    // адрес посылки сообщения
	
  SetReg(na+BASE_ADDR_KK+0x00, 0x0000); // CONTROL WORD
  SetReg(na+BASE_ADDR_KK+0x02, BC_COMMAND_WORD(msg->addr, msg->direction, msg->subaddr, msg->numwords));  // COMMAND WORD
  SetReg(na+BASE_ADDR_KK+0x04, 0x4000); // MESSAGE GAP WORD
  SetReg(na+BASE_ADDR_KK+0x06, 0x0000); // RTC HIGH
  SetReg(na+BASE_ADDR_KK+0x08, 0x0000); // RTC LOW
}
 
void PBCPutBlk(_msg_kk* msg) {
  unsigned short na;
  if(msg->nm) na=NA_ADDR1;
	else        na=NA_ADDR0;
  SetDataWord(na+BASE_ADDR_KK+0xA, msg->ptr, msg->numwords*sizeof(unsigned short));
}

void PBCStart(unsigned short nm) {
  unsigned short na;
  if(nm)  na=NA_ADDR1;
	else    na=NA_ADDR0;
  SetReg(na+CONFIGURATION_1, 0x900); // Запуск обмена
}

void PBCGetblk(_msg_kk* msg) {
	unsigned short na;
  if(msg->nm) na=NA_ADDR1;
	else        na=NA_ADDR0;
  
  GetDataWord(na+BASE_ADDR_KK+0xC, msg->ptr, msg->numwords*sizeof(unsigned short));
}

unsigned short PGetOS(unsigned short nm) {
  unsigned short na, w, numwords = 0;
	
  if(nm) na=NA_ADDR1;
  else   na=NA_ADDR0;
  
  w = GetReg(na+BASE_ADDR_KK+0x02);
  if(!(w&0x400)) {                // Если был обмен RT_TRANSMIT, то ОС лежит первым,                            
		numwords = w&31;              // RT_RECEIVE - после numwords слов данных
		if(!numwords)
			numwords=0x20;
	}
  
  w = GetReg(na+BASE_ADDR_KK+0xA+numwords*2);
	SetReg(na+BASE_ADDR_KK+0xA+numwords*2, 0x0000); // Обнуление ОС
  
  return w;
}

char PKKOU(unsigned short nm, unsigned short addr, unsigned short subAddr, unsigned short *mass, unsigned short numberOfWords) {
  unsigned short os;
  _msg_kk msg;
  
  msg.nm = nm;
  msg.addr = addr;
  msg.direction = RT_RECEIVE;
  msg.subaddr = subAddr;
  msg.numwords = numberOfWords;
  msg.ptr = mass;
  
  PBCPutCmd(&msg);
  PBCPutBlk(&msg);
  PBCStart(msg.nm);
  Receive(proxy, 0, 0);
  os = PGetOS(msg.nm);
  if(os == (msg.addr << 11))
    return 0;
  else
    return 1;
}

char POUKK(unsigned short nm, unsigned short addr, unsigned short subAddr, unsigned short *mass, unsigned short numberOfWords) {
  unsigned short os;
  _msg_kk msg;
  
  msg.nm = nm;
  msg.addr = addr;
  msg.direction = RT_TRANSMIT;
  msg.subaddr = subAddr;
  msg.numwords = numberOfWords;
  msg.ptr = mass;
  
  PBCPutCmd(&msg);
  PBCStart(msg.nm);
  Receive(proxy, 0, 0);
  os = PGetOS(msg.nm);
  if(os == (msg.addr << 11)) {
    PBCGetblk(&msg);
    return 0;
  }
  else
    return 1;
}

// Завершение работы с драйвером
void PotokClose(unsigned short nm) {
  CloseVmeFdDrv2408();
  CloseVmeFdDrv2416();
  if(nm)
    CloseVmeIntMilstd2();
  else
    CloseVmeIntMilstd1();
}
