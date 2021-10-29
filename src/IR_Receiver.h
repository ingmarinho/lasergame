#include "hwlib.hpp"
#include "rtos.hpp"

class IRReceiverListener
{

public:
	virtual void pause_detected(int pause) {}
};

template <unsigned int MAX_NOF_LISTENERS>
class IR_Receiver : public rtos::task<>
{
	enum state_t =
	{
		IDLE,
		SIGNAL
	}

	private : state_t state = IDLE;

	IRReceiverListener *IRreceiverlisteners[MAX_NOF_LISTENERS];
	unsigned int nof_listeners = 0;
	hwlib::target::pin_in &tsop_signal;
	hwlib::target::pin_out &led;

public:
	PauzeDetected(hwlib::target::pin_in &tsop_signal, hwlib::target::pin_out &led) : tsop_signal(tsop_signal), led(led)
	{
	}

	void addIR_receveiver_listener(IRReceiverListener *irl)
	{
		if (nof_listeners < MAX_NOF_LISTENERS)
		{
			IRreceiverlisteners[nof_listeners] = irl;
		}
	}

private:
	void main()
	{

		int n = 0;
		rtos::clock cl(100);

		for (;;)
		{

			switch (state)
			{
			case IDLE:

				wait(cl);
				bool IR_Signal = tsop_signal.read();

				if (!IR_Signal)
				{
					n += 100;
					state = IDLE;
				}
				else if (IR_Signal)
				{
					for (int i = 0; i < IRReceiverListener.size(); i++)
					{
						IRReceiverListener[i].pause_detected(n);
					}
					state = SIGNAL;

					break;

				case SIGNAL:
					wait(cl);

					bool IR_Signal = tsop_signal.read();

					if (IR_Signal)
					{
						state = SIGNAL;
					}
					else if (!IR_Signal)
					{
						n = 0;
						state = IDLE;
					}

					break;
				}
			}
		}
	}
};