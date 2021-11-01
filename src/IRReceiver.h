#pragma once

#include <array>

class IRReceiverListener
{

public:
	virtual void pauseDetected(int pause);
};

template <unsigned int maxNumberOfListeners>
class IRReceiver : public rtos::task<>
{
	enum state_t
	{
		IDLE,
		SIGNAL
	};

private:
	state_t state = IDLE;

	rtos::clock mainClock;

	std::array<IRReceiverListener *, maxNumberOfListeners> IRReceiverListenerArr;
	unsigned int currentNumberOfListeners = 0;
	hwlib::target::pin_in &tsopSignal;
	hwlib::target::pin_out &led;

public:
	IRReceiver(hwlib::target::pin_in &tsopSignal, hwlib::target::pin_out &led) : mainClock(this, 100 * rtos::ms), tsopSignal(tsopSignal), led(led)
	{
	}

	void addListener(IRReceiverListener *listener)
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
		bool IRSignal = tsopSignal.read();

		for (;;)
		{
			switch (state)
			{
			case IDLE:

				wait(mainClock);
				IRSignal = tsopSignal.read();

				if (!IRSignal)
				{
					n += 100;
					state = IDLE;
				}
				else if (IRSignal)
				{
					// for (int i = 0; i < nof_listeners; i++)
					// {
					// 	IRReceiverListenerArr[i].pause_detected(n);
					// }
					for (auto &listener : IRReceiverListenerArr)
					{
						listener->pauseDetected(n);
					}
					state = SIGNAL;
				}
				break;

			case SIGNAL:
				wait(mainClock);

				IRSignal = tsopSignal.read();

				if (IRSignal)
				{
					state = SIGNAL;
				}
				else if (!IRSignal)
				{
					n = 0;
					state = IDLE;
				}
				break;
			}
		}
	}
};
