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

	void turnOn()
	{
		led.write(1);
		led.flush();
	}

	void turnOff()
	{
		led.write(0);
		led.flush();
	}
};

class IRTransmitter
{
private:
	hwlib::target::d2_36kHz &IR;

public:
	IRTransmitter(hwlib::target::d2_36kHz &IR)
		: IR(IR)
	{
	}

	void turnOn()
	{
		IR.write(1);
		IR.flush();
	}

	void turnOff()
	{
		IR.write(0);
		IR.flush();
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
		GETBIT,
		SEND1_SIGNAL,
		SEND1_PAUSE,
		SEND0_SIGNAL,
		SEND0_PAUSE
	};

private:
	state_t state = IDLE;
	state_s stateS = GETBIT;
	rtos::timer delay;
	rtos::channel<uint16_t, 1024> sendChannel;
	IRTransmitter IR;
	Led led;

	int sendcounter = 0;
	int current = 0;
	uint16_t message = 0x0;

public:
	SendIRController(hwlib::target::d2_36kHz &IR, hwlib::target::pin_out &led)
		: delay(this, "delay"), sendChannel(this, "SEND_CHANNEL"), IR(IR), led(led)
	{
	}

	void SendMessage(uint16_t message)
	{
		sendChannel.write(message);
	}

private:
	void main()
	{
		uint16_t mask = 0x1;
		uint16_t res = mask & message;

		for (;;)
		{
			switch (state)
			{
			case IDLE:
				wait(sendChannel);
				message = sendChannel.read();
				state = SENDBITS;
				break;

			case SENDBITS:
				switch (stateS)
				{

				case GETBIT:
					if (current >= 15)
					{
						current = 0;
						sendcounter++;
						if (sendcounter == 2)
						{
							sendcounter = 0;
							state = IDLE;
						}
					}
					mask = 0x1;
					mask = mask << current;
					res = mask & message;
					current++;
					if (res > 0)
					{
						stateS = SEND1_SIGNAL;
					}
					else
					{
						stateS = SEND0_SIGNAL;
					}
					break;

				case SEND0_PAUSE:
					IR.turnOff();
					delay.set(1600);
					wait(delay);
					stateS = GETBIT;
					break;

				case SEND0_SIGNAL:
					IR.turnOn();
					delay.set(800);
					wait(delay);
					stateS = SEND0_PAUSE;
					break;

				case SEND1_PAUSE:
					IR.turnOff();
					delay.set(800);
					wait(delay);
					stateS = GETBIT;
					break;

				case SEND1_SIGNAL:
					IR.turnOn();
					delay.set(1600);
					wait(delay);
					stateS = SEND1_PAUSE;
					break;

				default:
					hwlib::wait_ms(1);
					hwlib::cout << "broken" << hwlib::endl;
					break;
				}
				break;
			default:
				hwlib::wait_ms(1);
				hwlib::cout << "broken2" << hwlib::endl;
				break;
			}
		}
	}
};