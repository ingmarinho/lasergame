#pragma once

class Led
{
private:
	hwlib::target::pin_out &led;

public:
	Led(hwlib::target::pin_out &led)
		: led(led)
	{
	}

	void ZetAan()
	{
		led.write(1);
		led.flush();
	}

	void ZetUit()
	{
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

	void ZetAan()
	{
		ir.write(1);
		ir.flush();
	}

	void ZetUit()
	{
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
	state_s stateS = getbit;
	rtos::timer delay;
	rtos::channel<uint16_t, 1024> sendChannel;
	IRTransmitter IR;
	Led led;

	int sendcounter = 0;
	int huidig = 0;
	uint16_t msg = 0x0;

public:
	SendIRController(hwlib::target::d2_36kHz &ir, hwlib::target::pin_out &led)
		: delay(this, "delay"), sendChannel(this, "SEND_CHANNEL"), IR(ir), led(led)
	{
	}

	void SendMessage(uint16_t msg)
	{
		sendChannel.write(msg);
	}

private:
	void main()
	{
		uint16_t masker = 0x1;
		uint16_t res = masker & msg;
		
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
					if (huidig >= 15)
					{
						huidig = 0;
						sendcounter++;
						if (sendcounter == 2)
						{
							sendcounter = 0;
							state = IDLE;
						}
					}
					masker = 0x1;
					masker = masker << huidig;
					res = masker & msg;
					huidig++;
					if (res > 0)
					{
						stateS = send1_signal;
					}
					else
					{
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
					stateS = send1_pause;
					break;

				default:
				hwlib::wait_ms(1);
				hwlib::cout << "kapot" << hwlib::endl;
				break; 
				}
				break;
				default:
				 hwlib::wait_ms(1);
				hwlib::cout << "kapot2" << hwlib::endl;
				break; 
			}
		}
	}
};