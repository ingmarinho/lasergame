#pragma once

#include "hwlib.hpp"
#include "rtos.hpp"

#include <array>

class IRReceiverListener
{

public:
	virtual void pauseDetected(int pause);
};

template <unsigned int maxNumberOfListeners>
class IRReceiver : public rtos::task<>
{
	enum state_t {IDLE, SIGNAL};

private:
	state_t state = IDLE;

	std::array<IRReceiverListener*, maxNumberOfListeners> IRReceiverListenerArr;
	unsigned int currentNumberOfListeners = 0;
	hwlib::target::pin_in &tsop_signal;
	hwlib::target::pin_out &led;

public:
	IR_Receiver(hwlib::target::pin_in &tsop_signal, hwlib::target::pin_out &led) : tsop_signal(tsop_signal), led(led)
	{
	}

	void addListener(IRReceiverListener &listener)
	{
		if (currentNumberOfListeners < maxNumberOfListeners)
		{
			IRReceiverListenerArr[currentNumberOfListeners] = listener;
			currentNumberOfListeners++;
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
					// for (int i = 0; i < nof_listeners; i++)
					// {
					// 	IRReceiverListenerArr[i].pause_detected(n);
					// }
					for(auto & IRListener : IRReceiverListenerArr)
					{
						IRListener->pause_detected(n);
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