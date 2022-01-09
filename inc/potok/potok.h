#define	RT_RECEIVE		0
#define RT_TRANSMIT		1

#define SREQ_MASK 0x0100

#define BC_COMMAND_WORD( a, b, c, d ) ((((a) & 0x1F) << 11) | (((b) & 1) << 10) | (((c) & 0x1F) << 5) | ((d) & 0x1F))

typedef struct {
  unsigned short nm;
  unsigned short addr;
  unsigned short direction;
  unsigned short subaddr;
  unsigned short numwords;
  unsigned short *ptr;
} _msg_kk;


char PInit_KK(unsigned short nm);
void PBCPutCmd(_msg_kk* msg);
void PBCPutBlk(_msg_kk* msg);
void PBCStart(unsigned short nm);
void PBCGetblk(_msg_kk* msg);
unsigned short PGetOS(unsigned short nm);
char PKKOU(unsigned short nm, unsigned short addr, unsigned short subAddr, unsigned short *mass, unsigned short numberOfWords);
char POUKK(unsigned short nm, unsigned short addr, unsigned short subAddr, unsigned short *mass, unsigned short numberOfWords);
void PotokClose(unsigned short nm);
