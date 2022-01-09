//==============================================================================
// File        : obrazldr.c
// Project     : Loader KREDO RAM
// Revision    : $Revision$
// Author      : NIL-1112
// Date        : $Date$ 26/05/2021
// Description : functions loader KREDO RAM
//==============================================================================
//******************************************************************************
//INCLUDE
#include <stdio.h>
#include "inc/obrazldr.h"
#include "inc/elcus/tmk_kk.h"
#include "inc/options.h"
#include "inc/milstd_def.h"
#include "inc/potok/potok.h"
//******************************************************************************
//******************************************************************************
// GLOBAL VARS
extern struct _option option;
//******************************************************************************
//******************************************************************************
// LOCAL DEFINE
//******************************************************************************
//******************************************************************************
// LOCAL TYPEDEF
//******************************************************************************
//******************************************************************************
// LOCAL VARS
static unsigned short cntErrAccess;
static FILE *fp;
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
// Загрузка файла в ОЗУ
//------------------------------------------------------------------------------
char ObrazFileLoad(unsigned addr) {	//Загрузка файла ppo.dat
  size_t n, all, done =0;
	unsigned short offset;
	unsigned short segment;
	unsigned short massWords[32];
  
  all = FileSize(fp);
  
  offset = 0x0000;
  segment = (unsigned short)(addr / 0x10);
  
  while((n = fread(&massWords[3], 1, SIZE_DATA_LOAD, fp))) {
		if (offset > 0xFF00) {
			offset -= 0xFF00;
			segment += 0x0FF0;
		}
		massWords[0] = WRITE_RAM;
		massWords[1] = offset;
		massWords[2] = segment;
    
		if(option.device) {
      if(PKKOU(option.channel, ADDR_OU, SUBADDR_DATA, massWords, n/2 + 3)) {
        if(option.progressBar)
          printf("\n");
        return DATA_LOAD_FAIL;
      }
    }
    else {
      if(KKOU(ADDR_OU, SUBADDR_DATA, massWords, n/2 + 3)) {
        if(option.progressBar)
          printf("\n");
        return DATA_LOAD_FAIL;
      }
    }
    offset += SIZE_DATA_LOAD;
    
    done+=n;
    if(option.progressBar)
      printf("\rdone = %d%%", ((100*done)/all));
	}
  if(option.progressBar)
    printf("\n");
	return DATA_LOAD_OK;
}

char FileOpen(char *fname) {
  fp = fopen(fname,"rb");
  if(fp == NULL)
    return FILE_OPEN_FAIL;
  else
    return FILE_OPEN_OK;
}

void FileClose() {
  fclose(fp);
}

//------------------------------------------------------------------------------
// Экспресс тест прибора
//------------------------------------------------------------------------------
char ExpressTest() {
  unsigned short massWords[NUMWORD_EXPRESS_TEST];
  
  if(option.device) {
    if(POUKK(option.channel, ADDR_OU, SUBADDR_EXPRESS_TEST, massWords, NUMWORD_EXPRESS_TEST))
      return EXPRESS_TEST_FAIL;
  }
  else {
    if(OUKK(ADDR_OU, SUBADDR_EXPRESS_TEST, massWords, NUMWORD_EXPRESS_TEST))
      return EXPRESS_TEST_FAIL;
  }
	cntErrAccess = massWords[1];
	if(massWords[0] == WORD_NORM) 
		return EXPRESS_TEST_OK;
	return EXPRESS_TEST_FAIL;
}

//------------------------------------------------------------------------------
// Инициализация прибора
//------------------------------------------------------------------------------
char InitializationDevice() {
  int i;
  unsigned short massWords[32];
  unsigned short numberStartsDevice;
  
	if (ExpressTest() != EXPRESS_TEST_OK) {
		return EXPRESS_TEST_FAIL;
	}
	numberStartsDevice = cntErrAccess;
	delay(1000);	

	massWords[0] = WORD_INITIALIZATION;
	for (i = 1; i < NUMWORD_INITIALIZATION; i++) {
		massWords[i] = SECURITY_CODE;
	}

	while (numberStartsDevice < NUM_ALLOWABLE_STARTS) {
    if(option.device) {
      if(PKKOU(option.channel, ADDR_OU, SUBADDR_INITIALIZATION, massWords, NUMWORD_INITIALIZATION))
        return INITIALIZATION_DEVICE_FAIL;
    }
    else {
      if(KKOU(ADDR_OU, SUBADDR_INITIALIZATION, massWords, NUMWORD_INITIALIZATION))
        return INITIALIZATION_DEVICE_FAIL;
    }
    delay(1000);  
		if (ExpressTest() != EXPRESS_TEST_OK) {
			return EXPRESS_TEST_FAIL;
		}
		if (numberStartsDevice == cntErrAccess) { 
			return INITIALIZATION_DEVICE_OK;
		}
		numberStartsDevice = cntErrAccess;
	} 
	return INITIALIZATION_DEVICE_FAIL;
}

//------------------------------------------------------------------------------
// Старт загруженного файла
//------------------------------------------------------------------------------
char RunOZU(unsigned addr) {
  unsigned short massWords[3];
	massWords[0] = RUN_RAM;
	massWords[1] = 0x0000;        // смещение
	massWords[2] = addr / 0x10;   // сегмент
  
  if(option.device) {
    if(PKKOU(option.channel, ADDR_OU, SUBADDR_RUN, massWords, 3))
      return RUN_FAIL;
  }
  else {
    if(KKOU(ADDR_OU, SUBADDR_RUN, massWords, 3))
      return RUN_FAIL;
  }
  return RUN_OK;
}

//------------------------------------------------------------------------------
//Вернуть размер файла
//------------------------------------------------------------------------------
size_t FileSize( FILE *fp ) {
    size_t size_of_file;
    fseek(fp, 0L, SEEK_END);
    size_of_file = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return(size_of_file);
}