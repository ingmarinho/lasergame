#pragma once

class led
{
private:
	hwlib::target::pin_out &led
public:
	led( hwlib::target::pin_out &led)
		: led(red)
	{
	}
	
	void ZetAan(){
		led.write(1);
		led.flush();
	}
	
	void ZetUit(){
		led.write(0);
		led.flush();
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
		getbit ,
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
	rtos::channel<uint16_t, 1024> sendChannel;
	int sendcounter = 0;
	int huidig = 0;
	uint16_t msg = 0x0;
	rtos::timer delay;

public:
	SendIRController(hwlib::target::d2_36kHz &ir, hwlib::target::pin_out &red)
		: IR(ir), Red(red)
	{
	}
	
	void SendMessage(uint16_t msg){
		sendChannel.write(msg);
	}
	
private:
	void main()
	{
		for (;;)
		{
			switch (state)
			{
				case IDLE:
					wait(sendChannel);
					msg = sendChannel.read();
					state = SENDBITS;
					break;
				
				case SENDBITS:
					switch (stateS)
					{
						case getbit:
							if(huidig >= 15){
								huidig = 0;
								sendcounter++;
								if(sendcounter == 2) {
									sendcounter = 0;
									state = IDLE;
								}
							}
							uint16_t masker = 0x1;
							masker = masker << huidig;
							uint16-t res = masker & msg;
							huidig++;
							if(res > 0){
								stateS = send1_signal;
							}else{
								stateS = send0_signal;
							}
							break;
						
						case send0_pause:
							IR.ZetUit();
							delay.set(1600);
							wait(delay);
							stateS = getbit;
							break;
						
						case send0_signal:
							IR.ZetAan();
							delay.set(800);
							wait(delay);
							stateS = send0_pause;
							break;
						
						case send1_pause:
							IR.ZetUit();
							delay.set(800);
							wait(delay);
							stateS = getbit;
							break;
						
						case send1_signal:
							IR.ZetAan();
							delay.set(1600);
							wait(delay);
							stateS = send1_pause
							break;
					}
					break;
			}
		}
	}
};