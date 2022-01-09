//==============================================================================
// File        : main.c
// Project     : Loader KREDO RAM
// Revision    : $Revision$
// Author      : NIL-1112
// Date        : $Date$ 26/05/2021
// Description : main
//==============================================================================
//******************************************************************************
//INCLUDE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "inc/obrazldr.h"
#include "inc/options.h"

#ifdef PTK
  #include "inc/ptkdrv.h"
#else
  #include "inc/tmkdrv.h"
#endif

#include "inc/elcus/tmk_kk.h"
#include "inc/potok/potok.h"
//******************************************************************************
//******************************************************************************
// GLOBAL VARS
struct _option option;
//******************************************************************************
//******************************************************************************
// LOCAL DEFINE
#define LOAD_ADDR   0x1000
#define START_ADDR  0x1000
enum bool {FALSE,TRUE};
//******************************************************************************
//******************************************************************************
// LOCAL TYPEDEF
//******************************************************************************
//******************************************************************************
// LOCAL VARS
//******************************************************************************
//******************************************************************************
// LOCAL PROTO
//******************************************************************************
//******************************************************************************
// LOCAL MACRO
//******************************************************************************
//******************************************************************************
// FUNCTIONS
void GetOptions(int argc, char** argv);  //����㧪� ��砫��� ��ࠬ��஢
void PrintResult(char result);           //�⮡ࠦ���� १���⮢ ࠡ��� �㭪権
//******************************************************************************
//------------------------------------------------------------------------------
//MAIN
//------------------------------------------------------------------------------
int main(int argc, char * argv[]) {
  char errorCode; // ��� �訡��
  printf("Obraz Loader [%s %s]\n",__DATE__,__TIME__);
  
  //����㧪� ��砫��� ��ࠬ��஢
  GetOptions(argc,argv);
  
  // ���뢠�� TMK ����� ��� ��⮪� 
  if(option.device)
    errorCode = PInit_KK(option.channel);
  else
    errorCode = IniTmk_KK(option.tmkPath);
  if(option.verbose)
    PrintResult(errorCode);
  if(errorCode != TMK_OPEN_OK)  { FileClose(); return(EXIT_FAILURE); }
  
  // ����㧪� ������ � ��� �ਡ��
  if(option.write) {
    
    // ���뢠�� 䠩� ��� �⥭��
    errorCode = FileOpen(option.fileName);
    if(option.verbose)
      PrintResult(errorCode);
    if(errorCode != FILE_OPEN_OK)  return(EXIT_FAILURE);
    
    // ���樠������ �ਡ�� � ��ॢ�� � �孮�����᪨� ०��
    errorCode = InitializationDevice(); 
    if(option.verbose)
      PrintResult(errorCode);
    if(errorCode != INITIALIZATION_DEVICE_OK)  { 
      FileClose(); 
      if(option.device) PotokClose(option.channel);
      else CloseTmk();
      return(EXIT_FAILURE); 
    }
    
    // ����㧪� ������ � ��� �ਡ��
    errorCode = ObrazFileLoad(option.loadAddr);
    if(option.verbose)
      PrintResult(errorCode);
    if(errorCode != DATA_LOAD_OK) { 
      FileClose(); 
      if(option.device) PotokClose(option.channel);
      else CloseTmk();
      return(EXIT_FAILURE); 
    }
  }
  
  // ���� ����㦥���� ������
  if(option.run) {
    errorCode = RunOZU(option.startAddr);
    if(option.verbose)
      PrintResult(errorCode);
    if(errorCode != RUN_OK)  { 
      FileClose(); 
      if(option.device) PotokClose(option.channel);
      else CloseTmk(); 
      return(EXIT_FAILURE); 
    }
  }
  printf("\n");
  FileClose();
  if(option.device) PotokClose(option.channel);
  else CloseTmk();
  return(EXIT_SUCCESS);
}
//------------------------------------------------------------------------------
//END MAIN
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ������ ��㬥�⮢ 
//------------------------------------------------------------------------------
void GetOptions(int argc, char** argv) {
  int errflag = 0;
  int arg;
  
  option.progressBar=TRUE;        //-p �⮡ࠦ���� 室� �믮������
  option.device = FALSE;          //-m ���ன�⢮ milstd ( 0 - elcus/ 1 - potok) default = 0
  option.channel = 0;             //-c ����� ������ ����� ��⮪ (⮫쪮 ��� ��⮪�)
  option.tmkPath = NULL;          //-t ���� � tmk (/dev/tmk0 �� 㬮�砭��)
  option.write = FALSE;           //-w ����㧪� ��� �ਡ��
  option.run = FALSE;             //-R ���� �ਡ�� 
  option.verbose = TRUE;          //-v �뢮� 室� ࠡ��� �ணࠬ��
  option.loadAddr = LOAD_ADDR;    //-l ���� ����㧪� ��ࠧ� � �ਡ��
  option.startAddr = START_ADDR;  //-b ���� ���� �ਡ��
  option.fileName=NULL;           //-f ��� 䠩��
  
  while((arg = getopt(argc, argv, "pd:c:t:wrvl:s:f:")) != -1) { 
    switch( arg ) {
      case 'p': option.progressBar =  FALSE;                      break;
      case 'd': option.device =       strtoul(optarg, NULL, 16);  break;
      case 'c': option.channel =      strtoul(optarg, NULL, 16);  break;
      case 't': option.tmkPath =      strdup(optarg);             break;
      case 'w': option.write =        TRUE;                       break;
      case 'r': option.run =          TRUE;                       break;
      case 'v': option.verbose =      FALSE;                      break;
      case 'l': option.loadAddr =     strtoul(optarg, NULL, 16);  break;
      case 's': option.startAddr =    strtoul(optarg, NULL, 16);  break;
      case 'f': option.fileName =     strdup(optarg);             break;
      case '?': ++errflag;                                        break;
    }
  }
  
  if(option.tmkPath==NULL) {
    option.tmkPath=strdup("/dev/tmk0");
  }
  
  if(option.fileName==NULL) {
    option.fileName=strdup("file.bin");
  }
  
  if(option.verbose) {
    printf("\n");
    printf("ProgressBar:   %d\n",   option.progressBar);
    printf("Device:        %d\n",   option.device);
    printf("tmkPath:       %s\n",   option.tmkPath);
    printf("write:         %d\n",   option.write);
    printf("run:           %d\n",   option.run);
    printf("Verbose:       %d\n",   option.verbose); 
    printf("LoadAddr:      0x%X\n", option.loadAddr);
    printf("StartAddr:     0x%X\n", option.startAddr);  
    printf("FileName:      %s\n",   option.fileName);
  }
  
  return;
}  
//------------------------------------------------------------------------------
//END ������ ��㬥�⮢ 
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Print Error
//------------------------------------------------------------------------------

void PrintResult(char result){
#ifndef LANG_RUS
  switch(result) {
    case FILE_OPEN_OK:                  printf("OPEN FILE OK\n");                   break;
    case FILE_OPEN_FAIL:                printf("OPEN FILE ERROR\n");                break;
    case EXPRESS_TEST_OK:               printf("EXPRESS TEST OK\n");                break;
    case EXPRESS_TEST_FAIL:             printf("EXPRESS TEST ERROR\n");             break;
    case INITIALIZATION_DEVICE_OK:      printf("INITIALIZATION DEVICE OK\n");       break;
    case INITIALIZATION_DEVICE_FAIL:    printf("INITIALIZATION DEVICE ERROR\n");    break;
    case DATA_LOAD_OK:                  printf("DATA LOAD OK\n");                   break;
    case DATA_LOAD_FAIL:                printf("DATA LOAD ERROR\n");                break;
    case RUN_OK:                        printf("RUN OK\n");                         break;
    case RUN_FAIL:                      printf("RUN FAIL\n");                       break;
    case TMK_OPEN_OK:                   printf("TMK OPEN OK\n");                    break;
    case TMK_OPEN_FAIL:                 printf("TMK OPEN ERROR\n");                 break;
    default:                            printf("UNDEFINED ERROR CODE\n");           break;
  }
#else  
   switch(result) {
    case FILE_OPEN_OK:                  printf("���� �ᯥ譮 �����\n");            break;
    case FILE_OPEN_FAIL:                printf("�訡�� ������ 䠩��\n");          break;
    case EXPRESS_TEST_OK:               printf("������ ��� �ਡ�� �ன���\n");  break;
    case EXPRESS_TEST_FAIL:             printf("�訡�� ������ ��� �ਡ��\n");  break;
    case INITIALIZATION_DEVICE_OK:      printf("���樠������ �ਡ�� �ன����\n"); break;
    case INITIALIZATION_DEVICE_FAIL:    printf("�訡�� ���樠����樨 �ਡ��\n");   break;
    case DATA_LOAD_OK:                  printf("����� �ᯥ譮 ����㦥��\n");       break;
    case DATA_LOAD_FAIL:                printf("�訡�� ����㧪� ������\n");         break;
    case RUN_OK:                        printf("���� �ਡ�� �ன���\n");          break;
    case RUN_FAIL:                      printf("�訡�� ���� �ਡ��\n");          break;
    case TMK_OPEN_OK:                   printf("������祭�� � ���� �믮�����\n");  break;
    case TMK_OPEN_FAIL:                 printf("�訡�� ������祭�� � ����\n");     break;
    default:                            printf("����।������ ��� �訡��\n");      break;
  }
#endif 
}
//------------------------------------------------------------------------------
//END Print Error
//------------------------------------------------------------------------------