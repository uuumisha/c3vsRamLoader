#define FILE_OPEN_OK                    0
#define FILE_OPEN_FAIL                  1
#define EXPRESS_TEST_OK                 2
#define EXPRESS_TEST_FAIL               3
#define INITIALIZATION_DEVICE_OK        4
#define INITIALIZATION_DEVICE_FAIL      5
#define DATA_LOAD_OK                    6
#define DATA_LOAD_FAIL                  7
#define RUN_OK                          8
#define RUN_FAIL                        9
#define TMK_OPEN_OK                     10
#define TMK_OPEN_FAIL                   11

struct _option{
  char progressBar;
  char device;
  char node;
  char channel;
  char write;
  char run;
  char verbose;
  unsigned long  loadAddr;
  unsigned long  startAddr;
  char *tmkPath;
  char *fileName;
};