
#define MCP_write 0B01000000 
#define MCP_read  0B01000001 

#define GP0     0x00 
#define GP1     0x01 
#define OLAT0   0x02 
#define OLAT1   0x03 
#define IPOL0   0x04    // INPUT POLARITY PORT REGISTER 0 
#define IPOL1   0x05    // INPUT POLARITY PORT REGISTER 1 
#define IODIR0  0x06    // I/O DIRECTION REGISTER 0 
#define IODIR1  0x07    // I/O DIRECTION REGISTER 1 
#define INTCAP0 0x08 // INTERRUPT CAPTURE REGISTER 0 
#define INTCAP1 0x09 // INTERRUPT CAPTURE REGISTER 1 
#define IOCON0  0x0A // I/O EXPANDER CONTROL REGISTER 0 
#define IOCON1  0x0B // I/O EXPANDER CONTROL REGISTER 1 

void InitMCP23016(int8 AddressSelect, int8 P0, int8 P1);
void GetGPx(int8 AddressSelect, int8 *P0, int8 *P1) ;
void SetGPx(int8 AddressSelect, int8 P0, int8 P1);
void GetINTGPx(int8 AddressSelect, int8 *P0, int8 *P1);
void SetGPREG(int8 AddressSelect, int8 REG, int8 Data); 
