#pragma once

#include <array>
#include "rtos.hpp"

class IRReceiverListener
{

public:
	virtual void pauseDetected(int pause);
};

/// reveives IR signals and communicates the pauses inbetween bits to messagedecoder
template <unsigned int maxNumberOfListeners>
class IRReceiver : public rtos::task<1500>
{
	enum state_t
	{
		IDLE,
		SIGNAL
	};

private:
	state_t state = IDLE;

	rtos::clock mainClock; /// rtos clock for precise timing

	std::array<IRReceiverListener *, maxNumberOfListeners> IRReceiverListenerArr;
	unsigned int currentNumberOfListeners = 0;
	hwlib::target::pin_in &tsopSignal; /// pin connected to data pin of IR receiver
	hwlib::target::pin_out &led;

public:
	IRReceiver(hwlib::target::pin_in &tsopSignal, hwlib::target::pin_out &led, unsigned int priority)
		: rtos::task<1500>(priority, "IRRECEIVER_TASK"), mainClock(this, 100 * rtos::us, "IRRECEIVER_CLOCK"), tsopSignal(tsopSignal), led(led)
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
		bool IRSignal;

		for (;;)
		{
			switch (state)
			{
			case IDLE:

				wait(mainClock);
				IRSignal = !tsopSignal.read();

				if (!IRSignal)
				{
					n += 100;
					state = IDLE;
				}
				else if (IRSignal)
				{
					for (auto &listener : IRReceiverListenerArr)
					{
						listener->pauseDetected(n);
					}
					state = SIGNAL;
				}
				break;

			case SIGNAL:
				wait(mainClock);

				IRSignal = !tsopSignal.read();

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
