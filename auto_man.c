#include <auto_man.h>
#include <mcp23016.c>

void PortsInit(){
//**************************************
	int i = 0;
   for(i = 0; i < 3; i++)
   {
      InitMCP23016(i,0x00,0x00); 
        delay_ms(350);
      SetGPx(i, 0x00, 0x00);
   }
//**************************************
}

void TimeDelay(unsigned int16 _Time){
	StartTime = true;
	Time_Delay = _Time;
	Time_Ready = false;
}

void ThrowOut(int1 operation){
//********************ScalesThrowOutOpen_Subprogram******************
		if(operation)
			
			SetGPx(0x01, Output2.Out2.Low, Output2.Out2.High);

//********************ScalesThrowOutClose_Subprogram*******
	    else{
			SetGPx(0x01, 0x00, 0x00);	
		}
//	Output2.Out2 = 0x0000;	
}

void ThrowOutVerh(int1 operation){
//********************ScalesThrowOutOpen_Subprogram******************
		if(operation)
		{
			SetGPx(0x02, Output3.Out3.Low, Output3.Out3.High);
		}
//********************ScalesThrowOutClose_Subprogram*******
	    else{
			SetGPx(0x02, 0x00, 0x00);	
		}
//	Output3.Out3 = 0x0000;	
}

void Vibro(operation){

//*****************************************
		if(operation){
			bit_set(Output1.Out1,14);
			SetGPx(0x00, Output1.Out1.Low, Output1.Out1.High);
		}
//********************ScalesThrowOutClose_Subprogram******************
	    else{
			SetGPx(0x00, 0x00, 0x00);
		}
//	Output1.Out1 = 0x0000;

}