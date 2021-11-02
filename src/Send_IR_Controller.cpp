include "hwlib.hpp"
include "rtos.hpp"


class led
{
private:
	hwlib::target::pin_out &red
public:
	led( hwlib::target::pin_out &red)
		: red(red)
	{
	}
	
	void ZetAan(){
		red.write(1);
		red.flush();
	}
	
	void ZetUit(){
		red.write(0);
		red.flush();
	}
};



class IRTransmitter
{
private:
	hwlib::target::d2_36kHz &ir;

public:
	IRTransmitter(hwlib::target::d2_36kHz &ir)
		: ir(ir)
	{
	}
	
	void ZetAan(){
		ir.write(1);
		ir.flush();
	}
	
	void ZetUit(){
		ir.write(0);
		ir.flush();
	}
};


class SendIRController : public rtos::task<>
{
	enum state_t
	{
		IDLE,
		SENDBITS
	};
	enum state_s
	{
		getbit,
		send1_signal,
		send1_pause,
		send0_signal,
		send0_pause
	};

private:
	state_t state = IDLE;
	state_s stateS = getbit
	IRTransmitter IR;
	led Red;

public:
	SendIRController(hwlib::target::d2_36kHz &ir, hwlib::target::pin_out &red)
		: IR(ir), Red(red)
	{
	}
	
private:
	void main()
	{
		
		for (;;)
		{
			switch (state)
			{
				case IDLE:
					
					
					break;
				
				case SENDBITS:
					switch (stateS)
					{
						case getbit:
							
							break;
						
						case send0_pause:
							
							break;
						
						case send0_signal:
							
							break;
						
						case send1_pause:
							
							break;
						
						case send1_signal:
							
							break;
					}
					break;
			}
		}
	}
};