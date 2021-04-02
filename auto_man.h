int1 StartTime = false;
int1 Time_Ready = false;
unsigned int16 Time_Delay = 0;
unsigned int16 auto_vibro_time_delay = 0;


struct Port{
		unsigned Output0:1;
		unsigned Output1:1;
		unsigned Output2:1;
		unsigned Output3:1;
		unsigned Output4:1;
		unsigned Output5:1;
		unsigned Output6:1;
		unsigned Output7:1;
		unsigned Output8:1;
		unsigned Output9:1;
		unsigned Output10:1;
		unsigned Output11:1;
		unsigned Output12:1;
		unsigned Output13:1;
		unsigned Output14:1;
		unsigned Output15:1;
	};

	struct Out{
		unsigned int8 Low;
		unsigned int8 High;
	};
// vibratory
	union Output1{
		struct Port Port1;
		struct Out Out1;
	}Output1;

// lotki niz
	union Output2{
		struct Port Port2;
		struct Out Out2;
	}Output2;

// lotki verh
	union Output3{
		struct Port Port3;
		struct Out Out3;
	}Output3;

void TimeDelay(unsigned int16 _Time);
void ThrowOut(int1 operation);
void ThrowOutVerh(int1 operation);
void Vibro(operation);
void PortsInit();