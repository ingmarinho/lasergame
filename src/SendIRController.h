#pragma once

class Led
{
private:
	hwlib::target::pin_out &led public : Led(hwlib::target::pin_out &led)
		: led(led)
	{
	}

	void ZetAan()
	{
		red.write(1);
		red.flush();
	}

	void ZetUit()
	{
		red.write(0);
		red.flush();
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

	void ZetAan()
	{
		IR.write(1);
		IR.flush();
	}

	void ZetUit()
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
	IRTransmitter IR;
	Led red;

public:
	SendIRController(hwlib::target::d2_36kHz &IR, hwlib::target::pin_out &red)
		: IR(IR), red(red)
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
				case GETBIT:

					break;

				case SEND0_PAUSE:

					break;

				case SEND0_SIGNAL:

					break;

				case SEND1_PAUSE:

					break;

				case SEND1_SIGNAL:

					break;
				}
				break;
			}
		}
	}
};