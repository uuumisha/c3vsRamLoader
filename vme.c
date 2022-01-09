//==============================================================================
// File        : vme.c
// Project     : Loader Vulkan
// Revision    : $Revision$
// Author      : NIL-1112
// Date        : $Date$ 30/04/2021
// Description : function open, read, write, close vmeBus
//==============================================================================
//******************************************************************************
//INCLUDE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <i86.h>
#include "inc/potok/vme.h"
//******************************************************************************
//******************************************************************************
// GLOBAL VARS
//******************************************************************************
//******************************************************************************
// LOCAL DEFINE
//******************************************************************************
//******************************************************************************
// LOCAL TYPEDEF
//******************************************************************************
//******************************************************************************
// LOCAL VARS
static int fd2408, fd2416, fdmilstd1, fdmilstd2;

//******************************************************************************
//******************************************************************************
// LOCAL PROTO
//******************************************************************************
//******************************************************************************
// LOCAL MACRO
//******************************************************************************
//******************************************************************************
// FUNCTIONS
//******************************************************************************

//------------------------------------------------------------------------------
//Открыть файл для чтения и записи
//------------------------------------------------------------------------------

unsigned char OpenVmeFdDrv2408() {
  char path[40];
  sprintf(path, "//%d%s", getnid(), VMEBUS2408);
  if((fd2408 = open( path, O_RDWR )) == -1 ){ 
    return 1; 
  }

  return 0;
}

unsigned char OpenVmeFdDrv2416() {
  char path[40];
  sprintf(path, "//%d%s", getnid(), VMEBUS2416);
  if((fd2416 = open( path, O_RDWR )) == -1 ){ 
    return 1; 
  }
  return 0;
}

unsigned char OpenVmeIntMilstd1() {
  char path[40];
  sprintf(path, "//%d%s", getnid(), VMEINT_MILSTD1);
  if((fdmilstd1 = open( path, O_RDWR )) == -1 ){ 
    return 1; 
  }
  return 0;
}

unsigned char OpenVmeIntMilstd2() {
  char path[40];
  sprintf(path, "//%d%s", getnid(), VMEINT_MILSTD2);
  if((fdmilstd2 = open( path, O_RDWR )) == -1 ){ 
    return 1; 
  }
  return 0;
}

//------------------------------------------------------------------------------
//Закрыть файл
//------------------------------------------------------------------------------

void CloseVmeFdDrv2408() {
  close(fd2408);
}

void CloseVmeFdDrv2416() {
  close(fd2416);
}

void CloseVmeIntMilstd1() {
  close(fdmilstd1);
}

void CloseVmeIntMilstd2() {
  close(fdmilstd2);
}

//------------------------------------------------------------------------------
// Запись слова в regs
//------------------------------------------------------------------------------

void SetReg(unsigned short regs, unsigned short data) {
  lseek(fd2416,regs,0);
  write(fd2416, &data, 2);
}
//------------------------------------------------------------------------------
// Чтение слова из regs
//------------------------------------------------------------------------------

unsigned short GetReg(unsigned short regs) {
  unsigned short data;
  lseek(fd2416,regs,0);
  read(fd2416, &data, 2);
  return data;
}
//------------------------------------------------------------------------------
// Запись байта в regs
//------------------------------------------------------------------------------

void SetByte(unsigned short regs, unsigned char data) {
  lseek(fd2408, regs, 0);
  write(fd2408, &data, 1);
}

//------------------------------------------------------------------------------
// Запись массивов данных
//------------------------------------------------------------------------------

void SetDataWord(unsigned short regs, unsigned short* data, unsigned short size) {
  lseek(fd2416, regs, 0);
  write(fd2416, data, size);
}

void SetDataByte(unsigned short regs, unsigned char* data, unsigned short size) {
  lseek(fd2408, regs,0);
  write(fd2408, data, size);
}

//------------------------------------------------------------------------------
// Чтение массивов данных
//------------------------------------------------------------------------------

void GetDataWord(unsigned short regs, unsigned short* data, unsigned short size) {
  lseek(fd2416, regs, 0);
  read(fd2416, data, size);
}

void GetDataByte(unsigned short regs, unsigned char* data, unsigned short size) {
  lseek(fd2408, regs,0);
  read(fd2408, data, size);
}

//------------------------------------------------------------------------------
// Сообщение proxy для vmeint
//------------------------------------------------------------------------------
short SetProxyMilstd1(pid_t proxy) {
  return dev_arm(fdmilstd1, proxy, 0);
}

short SetProxyMilstd2(pid_t proxy) {
  return dev_arm(fdmilstd2, proxy, 0);
}

void GetIntCnt(unsigned short nm, unsigned long *ptr) {
  if(!nm)
    read(fdmilstd1, ptr, sizeof(unsigned long)*2);
  else 
    read(fdmilstd2, ptr, sizeof(unsigned long)*2);
}
