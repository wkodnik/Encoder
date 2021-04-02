#include<mcp23016.h>

void InitMCP23016(int8 AddressSelect, int8 P0, int8 P1) 
{ 
   i2c_start(); 
      delay_us(20); 
   i2c_write(MCP_write | (AddressSelect << 1)); 
      delay_us(20); 
   i2c_write(IODIR0); 
      delay_us(20); 
   i2c_write(P0); 
      delay_us(20); 
   i2c_write(P1); 
      delay_us(20); 
   i2c_stop(); 
      delay_us(20); 
   i2c_start(); 
      delay_us(20); 
   i2c_write(MCP_write | (AddressSelect << 1)); 
      delay_us(20); 
   i2c_write(IOCON0); 
      delay_us(20); 
   i2c_write(0x01); 
      delay_us(20); 
   i2c_stop(); 
      delay_us(50); 
} 
////////////////////////////////////////////////////////////////////////////// 

void GetGPx(int8 AddressSelect, int8 *P0, int8 *P1) 
{ 
   i2c_start(); 
      delay_us(20); 
   i2c_write(MCP_write | (AddressSelect << 1)); 
      delay_us(20); 
   i2c_write(GP0); 
      delay_us(20); 
   i2c_stop(); 
      delay_us(50); 
   i2c_start(); 
      delay_us(20); 
   i2c_write(MCP_read | (AddressSelect << 1)); 
      delay_us(20); 
   *P0 = i2c_read(); 
      delay_us(20); 
   *P1 = i2c_read(0); 
      delay_us(20); 
   i2c_stop(); 
      delay_us(50); 
} 
////////////////////////////////////////////////////////////////////////////// 

void SetGPx(int8 AddressSelect, int8 P0, int8 P1) 
{ 
   i2c_start(); 
      delay_us(20); 
   i2c_write(MCP_write | (AddressSelect << 1)); 
      delay_us(20); 
   i2c_write(OLAT0); 
      delay_us(20); 
   i2c_write(P0); 
      delay_us(20); 
   i2c_write(P1); 
      delay_us(20); 
   i2c_stop(); 
      delay_us(50); 
} 
//////////////////////////////////////////////////////////////////////////////

void GetINTGPx(int8 AddressSelect, int8 *P0, int8 *P1) 
{ 
   i2c_start(); 
      delay_us(20); 
   i2c_write(MCP_write | (AddressSelect << 1)); 
      delay_us(20); 
   i2c_write(INTCAP0); 
      delay_us(20); 
   i2c_stop(); 
      delay_us(50); 
   i2c_start(); 
      delay_us(20); 
   i2c_write(MCP_read | (AddressSelect << 1)); 
      delay_us(20); 
   *P0 = i2c_read(); 
      delay_us(20); 
   *P1 = i2c_read(0); 
      delay_us(20); 
   i2c_stop(); 
      delay_us(50); 
} 
////////////////////////////////////////////////////////////////////////////// 

void SetGPREG(int8 AddressSelect, int8 REG, int8 Data) 
{ 
   i2c_start(); 
      delay_us(20); 
   i2c_write(MCP_write | (AddressSelect << 1)); 
      delay_us(20); 
   i2c_write(REG); 
      delay_us(20); 
   i2c_write(Data); 
      delay_us(20); 
   i2c_stop(); 
      delay_us(50); 
} 

