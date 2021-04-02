#define USE_WITH_PC 1
#include <18f2580.h>
#fuses HS, NOWDT, NOMCLR
#use delay(clock=12M)
#use I2C(master,SDA = PIN_C4,SCL = PIN_C3)
#include <algoritm.c>
#include <auto_man.c>
#include <_can_18F4580.c>
#define BOARD1_ID  100

#define MODBUS_TYPE MODBUS_TYPE_SLAVE
#define MODBUS_SERIAL_RX_BUFFER_SIZE 64
#define MODBUS_SERIAL_BAUD 19200

#ifndef USE_WITH_PC
#define MODBUS_SERIAL_INT_SOURCE MODBUS_INT_EXT
#define MODBUS_SERIAL_TX_PIN PIN_B1   // Data transmit pin
#define MODBUS_SERIAL_RX_PIN PIN_B0   // Data receive pin
//The following should be defined for RS485 communication
//#define MODBUS_SERIAL_ENABLE_PIN   0   // Controls DE pin for RS485
//#define MODBUS_SERIAL_RX_ENABLE    0   // Controls RE pin for RS485
#else
#define MODBUS_SERIAL_INT_SOURCE MODBUS_INT_RDA
#endif

#include "_modbus.c"

#define MODBUS_ADDRESS 0xF7
#define open 1
#define close 0
#define start 1
#define stop 0

/*This function may come in handy for you since MODBUS uses MSB first.*/
int8 swap_bits(int8 c)
{
   return ((c&1)?128:0)|((c&2)?64:0)|((c&4)?32:0)|((c&8)?16:0)|((c&16)?8:0)
          |((c&32)?4:0)|((c&64)?2:0)|((c&128)?1:0);
}
   
   struct rx_stat rxstat; 
      int32 rx_id; 
      int8 rx_len = 7;                
      int8 buffer[8] = {0,0,0,0,0,0,0,0};
   int BOARD2_ID  = 0;

   int8 coils = 0b00000101;
   int8 inputs = 0b00001001;
   int16 hold_regs[60] = {0x0000};
   int16 input_regs[5] = {0x0000};
   int16 event_count = 0;

   unsigned int16 total = 0;

   unsigned int16 ScaleThrowOut_Time = 0; // Vremja zadergki na vysyp.
   unsigned int16 ScaleVibro_Time = 0;
   unsigned int16 Time = 0;
   unsigned int16 auto_man_en = false;
  
   unsigned int16 temp_out1 = 0;
   unsigned int16 temp_out2 = 0;
   unsigned int16 temp_out3 = 0;

   unsigned int16 startup = 0;   

#INT_TIMER1
void can_ask()
{
//*******************every 5ms interrupt*****************
      int i = 0;
      CLEAR_INTERRUPT(INT_TIMER1);
      SET_TIMER1(0);
      SET_TIMER1(0x8acf);//(0X159f);
//*******************Time_Delay_Subprogram**********************
      if(StartTime)
	  {
           if(Time < Time_Delay)
		   {Time += 1;
           }
           else
		   {Time = 0;
            Time_Delay = 0;
            Time_Ready = true;
            StartTime = false;
           }
      }
//*****************************************
      if(BOARD2_ID<15){
         BOARD2_ID = BOARD2_ID+1;
      }
      else{BOARD2_ID = 1;
          }
   
     buffer[0] = 105;

      if((hold_regs[57]>>8) == BOARD2_ID)
	  {  buffer[0] = hold_regs[57];      // command
         buffer[2] = hold_regs[58];       // data 1
         buffer[1] = hold_regs[58]>>8;   //data 0
         hold_regs[57] = 0x0000;
         hold_regs[58] = 0x0000;
      }
         can_putd(BOARD2_ID, buffer, 7, 1, 1, 0); 
         for (i = 0; i<7;i++)
             buffer[i] = 0;   // Clear the buffer byte 
}

void main()
{
//	delay_ms(500);
    modbus_init();
	PortsInit();
    can_init(); 
  
    delay_ms(250);
   
   for(i=0; i<15;i++)x[i] = 0;

    SETUP_TIMER_1(T1_INTERNAL|T1_DIV_BY_1);
    SET_TIMER1(0x8acf);//(0X159f);
    ENABLE_INTERRUPTS(INT_TIMER1);
    ENABLE_INTERRUPTS(GLOBAL);
//**************************************
   int t = 0;

   while(TRUE)
   {
// Call the weightfind subprogram from PC          
      if(hold_regs[56] == 1 && startup){
      hold_regs[56] = 0;
      temp_out1 = 0;
      temp_out2 = 0;
      temp_out3 = 0;

// poisk summy
//m1:
      total = summ();
        if(total){

         hold_regs[52] = (total<<8)| (total>>8);
           for (t = 0; t < 10; t++)
		   { 
            if(arrnumber[number][t] != 0)
			{
               bit_set(temp_out1,(arrnumber[number][t] - 1));
               bit_set(temp_out2,(arrnumber[number][t] - 1));
               bit_set(temp_out3,(arrnumber[number][t] - 1));
            }
           }
         Output1.Out1 = temp_out1;
         Output2.Out2 = temp_out2;
         Output3.Out3 = temp_out3;
      }
  
      if(auto_man_en && auto_vibro_time_delay )
	  {  ThrowOut(open);
         TimeDelay(50);
         while(!Time_Ready){;}
         ThrowOut(close);
         delay_ms(500);

         Vibro(start);
         TimeDelay(auto_vibro_time_delay);
         while(!Time_Ready){;}
         Vibro(stop);

         ThrowOutVerh(open);
         TimeDelay(50);
         while(!Time_Ready){;}
         ThrowOutVerh(close);   
      }
     }
      if(!auto_man_en){
//***************Start Vibro by time from PC****************
      if(hold_regs[53])
	  {
         ScaleVibro_Time = (hold_regs[53]<<8)|(hold_regs[53]>>8);
         hold_regs[53] = 0x0000;
         Vibro(start);
         TimeDelay(ScaleVibro_Time);
         while(!Time_Ready){;}
            Vibro(stop);
      }
//***************Start ThrowOut from PC**************************
      if(hold_regs[49])
	  {  ScaleThrowOut_Time = (hold_regs[49]<<8)|(hold_regs[49]>>8);
         hold_regs[49] = 0x0000;
         ThrowOut(open);
         TimeDelay(ScaleThrowOut_Time);
         while(!Time_Ready){;}
            ThrowOut(close);
      }

//**************Start VerhLotki from PC**************************
      if(hold_regs[48])
	  {  ScaleThrowOut_Time = (hold_regs[48]<<8)|(hold_regs[48]>>8);
         hold_regs[48] = 0x0000;
         ThrowOutVerh(open);
         TimeDelay(ScaleThrowOut_Time);
         while(!Time_Ready){;}
            ThrowOutVerh(close);
      }

//***************Select Vibro from PC*********************************************
       if(hold_regs[55] != 0)
	   {
         unsigned int16 tempreg = 0;
         tempreg = (hold_regs[55]<<8)|(hold_regs[55]>>8);
         hold_regs[55] = 0;
         Output1.Out1 = tempreg;
         tempreg = 0;
       }
//**************Select ThrowOut from PC*****************************************

       if(hold_regs[54] != 0)
	   {
         unsigned int16 tempreg3 = 0;
         tempreg3 = (hold_regs[54]<<8)|(hold_regs[54]>>8);
         hold_regs[54] = 0;
         Output2.Out2 = tempreg3;
         tempreg3 = 0;
       }

//**************Select ThrowOutVerh from PC*****************************************
       if(hold_regs[47] != 0)
	   {
         unsigned int16 tempreg4 = 0;
         tempreg4 = (hold_regs[47]<<8)|(hold_regs[47]>>8);
         hold_regs[47] = 0;
         Output3.Out3 = tempreg4;
         tempreg4 = 0;
       }

//***************Set limits from PC*********************************************
//      hilim
      if(hold_regs[51] != 0){
         unsigned int16 tempreg1 = 0;
      
         tempreg1 = (hold_regs[51]<<8)|(hold_regs[51]>>8);
         hold_regs[51] = 0;
//         xhilim = tempreg1;
      }
//      lolim
      if(hold_regs[50] != 0){
         unsigned int16 tempreg2 = 0;
         tempreg2 = (hold_regs[50]<<8)|(hold_regs[50]>>8);
         hold_regs[50] = 0;
//         xlolim = tempreg2;
      }
      }
// auto - man select
//      if (hold_regs[45] != 0){
         auto_man_en = (hold_regs[45]<<8)|(hold_regs[45]>>8);
//         hold_regs[45] = 0;
//      }
      
// zadanie na ves
      if (hold_regs[44] != 0)
	  {
         m = (hold_regs[44]<<8)|(hold_regs[44]>>8);
         m *= 10;
//         hold_regs[44] = 0;
         if(m <= 5000)
            auto_vibro_time_delay = 50;
          else if (m > 5000 && m <= 7500)
            auto_vibro_time_delay = 70;
         else if (m > 7500 && m <= 10000)
            auto_vibro_time_delay = 85;
         else 
            ;
      }
      if ((hold_regs[43] != 0) && (m > 3000))
	  {
         int count = 0;
         hold_regs[43] = 0x0000;
// osvobogdenie izmeritelnyh jacheek
         temp_out1 = 0xffff;
         temp_out2 = 0xffff;
         temp_out3 = 0xffff; 
         Output1.Out1 = temp_out1;
         Output2.Out2 = temp_out2;
         Output3.Out3 = temp_out3;
   
         ThrowOutVerh(open);  
         TimeDelay(100);
         while(!Time_Ready){;}
		 ThrowOutVerh(close);

		 delay_ms(100);
		 ThrowOut(open);
	     TimeDelay(100);
         while(!Time_Ready){;}
         ThrowOut(close);
         TimeDelay(100);
         while(!Time_Ready){;}
//*************************************

// kalibrovra 0 pri pervom vkluchenii   
         for(count = 1; count < 15; count++)
		 {
             buffer[0] = 102;      // command
             can_putd(count, buffer, 7, 1, 1, 0); 
            for (i = 0; i<7; i++) buffer[i] = 0;   // Clear the buffer byte
             delay_ms(1000);
         }
//**************************************
         Vibro(start);
         TimeDelay(auto_vibro_time_delay);
         while(!Time_Ready){;}
         Vibro(stop);
         ThrowOutVerh(open);
         TimeDelay(50);
         while(!Time_Ready){;}
         ThrowOutVerh(close);
         
//**************************************
         startup = 1;      
      }   
      else
         hold_regs[43] = 0x0000;

//*****************************CAN**************************************** 
        if(can_kbhit()){
          while(!can_kbhit());
         DISABLE_INTERRUPTS(INT_TIMER1);
         if(can_getd(rx_id, buffer, rx_len, rxstat)) 
           { 
               if(rx_id == BOARD1_ID)  // Is it for this board ? 
                { 
            //   putc(buffer[0]);  // If so, display the char
               hold_regs[buffer[0]] = buffer[3];
               hold_regs[buffer[0]] = hold_regs[buffer[0]]<<8;
               hold_regs[buffer[0]] += buffer[4]; 

// zapolnenie massiva sortirovki.
               x[buffer[0]] = buffer[4];
               x[buffer[0]] = x[buffer[0]]<<8;
               x[buffer[0]] += buffer[3];
                } 
            }
         
         SET_TIMER1(0);
         SET_TIMER1(0x8acf);//(0X159f);
         ENABLE_INTERRUPTS(INT_TIMER1);
          }

//******************************MODBUS*************************************   
     if(modbus_kbhit()){   
      
      //check address against our address, 0 is broadcast
      if((modbus_rx.address == MODBUS_ADDRESS) || modbus_rx.address == 0)
      {
         switch(modbus_rx.func)
         {
            case FUNC_READ_COILS:    //read coils
            case FUNC_READ_DISCRETE_INPUT:    //read inputs
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  int8 data;
                  
                  if(modbus_rx.func == FUNC_READ_COILS)
                     data = coils>>(modbus_rx.data[1]);      //move to the starting coil
                  else
                     data = inputs>>(modbus_rx.data[1]);      //move to the starting input

                  data = data & (0xFF>>(8-modbus_rx.data[3]));  //0 out values after quantity

                  if(modbus_rx.func == FUNC_READ_COILS)
                     modbus_read_discrete_input_rsp(MODBUS_ADDRESS, 0x01, &data);
                  else
                     modbus_read_discrete_input_rsp(MODBUS_ADDRESS, 0x01, &data);
                     
                  event_count++;
               }
               break;
            case FUNC_READ_HOLDING_REGISTERS:
            case FUNC_READ_INPUT_REGISTERS:
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 60 || modbus_rx.data[3]+modbus_rx.data[1] > 60)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  if(modbus_rx.func == FUNC_READ_HOLDING_REGISTERS)
                     modbus_read_holding_registers_rsp(MODBUS_ADDRESS,(modbus_rx.data[3]*2),hold_regs+modbus_rx.data[1]);
                  else
                     modbus_read_input_registers_rsp(MODBUS_ADDRESS,(modbus_rx.data[3]*2),input_regs+modbus_rx.data[1]);
                  
                  event_count++;
               }
               break;
            case FUNC_WRITE_SINGLE_COIL:      //write coil
               if(modbus_rx.data[0] || modbus_rx.data[3] || modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else if(modbus_rx.data[2] != 0xFF && modbus_rx.data[2] != 0x00)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_VALUE);
               else
               {
                  //coils are stored msb->lsb so we must use 7-address
                  if(modbus_rx.data[2] == 0xFF)
                     bit_set(coils,7-modbus_rx.data[1]);
                  else
                     bit_clear(coils,7-modbus_rx.data[1]);

                  modbus_write_single_coil_rsp(MODBUS_ADDRESS,modbus_rx.data[1],((int16)(modbus_rx.data[2]))<<8);
                  
                  event_count++;
               }
               break;
            case FUNC_WRITE_SINGLE_REGISTER:
               if(modbus_rx.data[0] || modbus_rx.data[1] >= 60)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  //the registers are stored in little endian format
                  hold_regs[modbus_rx.data[1]] = make16(modbus_rx.data[3],modbus_rx.data[2]);

                  modbus_write_single_register_rsp(MODBUS_ADDRESS,
                               make16(modbus_rx.data[0],modbus_rx.data[1]),
                               make16(modbus_rx.data[2],modbus_rx.data[3]));
                        //delay_us(1);
               }
               break;
            case FUNC_WRITE_MULTIPLE_COILS:
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  int i,j;

                  modbus_rx.data[5] = swap_bits(modbus_rx.data[5]);

                  for(i=modbus_rx.data[1],j=0; i < modbus_rx.data[1]+modbus_rx.data[3]; ++i,++j)
                  {  
                     if(bit_test(modbus_rx.data[5],j))
                        bit_set(coils,7-i);
                     else
                        bit_clear(coils,7-i);
                  }

                  modbus_write_multiple_coils_rsp(MODBUS_ADDRESS,
                                 make16(modbus_rx.data[0],modbus_rx.data[1]),
                                 make16(modbus_rx.data[2],modbus_rx.data[3]));
                  
                  event_count++;
               }
               break;
            case FUNC_WRITE_MULTIPLE_REGISTERS:
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 60 || modbus_rx.data[3]+modbus_rx.data[1] > 60)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  int i,j;

                  for(i=0,j=5; i < modbus_rx.data[4]/2; ++i,j+=2)
                     hold_regs[modbus_rx.data[1+i]] = make16(modbus_rx.data[j+1],modbus_rx.data[j]);

                  modbus_write_multiple_registers_rsp(MODBUS_ADDRESS,
                                 make16(modbus_rx.data[0],modbus_rx.data[1]),
                                 make16(modbus_rx.data[2],modbus_rx.data[3]));
               
                  event_count++;
               }
               break;           
            default:    //We don't support the function, so return exception
               modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_FUNCTION);
         }
      }
   }
 }
}