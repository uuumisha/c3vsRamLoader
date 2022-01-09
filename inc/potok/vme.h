//******************************************************************************
// INCLUDE
//******************************************************************************
//******************************************************************************
// DEFINE
#define VMEBUS2408      "/dev/vmebus/A24/D08"
#define VMEBUS2416      "/dev/vmebus/A24/D16"
#define VMEINT_MILSTD1  "/dev/vmebus/int/milstd1"
#define VMEINT_MILSTD2  "/dev/vmebus/int/milstd2"
//******************************************************************************
//******************************************************************************
// TYPEDEF
//******************************************************************************
//******************************************************************************
// GLOBAL VARS
//******************************************************************************
//******************************************************************************
// GLOBAL PROTO
unsigned char OpenVmeFdDrv2408();
unsigned char OpenVmeFdDrv2416();
unsigned char OpenVmeIntMilstd1();
unsigned char OpenVmeIntMilstd2();

void CloseVmeFdDrv2408();
void CloseVmeFdDrv2416();
void CloseVmeIntMilstd1();
void CloseVmeIntMilstd2();

void SetReg(unsigned short regs, unsigned short data);
void SetByte(unsigned regs, unsigned char data);

unsigned short GetReg(unsigned short regs);

void SetDataWord(unsigned short regs, unsigned short* data, unsigned short size);
void SetDataByte(unsigned short regs, unsigned char*  data, unsigned short size);

void GetDataWord(unsigned short regs, unsigned short* data, unsigned short size);
void GetDataByte(unsigned short regs, unsigned char*  data, unsigned short size);

short SetProxyMilstd1(pid_t proxy);
short SetProxyMilstd2(pid_t proxy);

void GetIntCnt(unsigned short nm, unsigned long *ptr);

//******************************************************************************
//******************************************************************************
// MACRO
//******************************************************************************
//******************************************************************************
// END OF FILE
//******************************************************************************





