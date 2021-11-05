#ifndef TOETSENBORD4X4_HPP
#define TOETSENBORD4X4_HPP

#include <array>

class KeyPadListener {
public:
	virtual void KeyPressed (char KeyID);
};

template <unsigned int maxNumberOfListeners>
class Toetsenbord4x4 public : rtos::task<>{

private:
	hwlib::keypad<16> &keypad;
	std::array< KeyPadListener, maxNumberOfListeners> keypadListeners;
	unsigned int currentNumberOfListeners = 0;
	
public:
	Toetsenbord4x4(hwlib::keypad<16> &keypad, unsigned int priority) : rtos::task<>(priority, "TOETSENBORD_TAAK"), keypad(keypad)
	{
	}

	void addListener(keypadListeners *listener)
	{
		if (currentNumberOfListeners < maxNumberOfListeners)
		{
			keypadListeners[currentNumberOfListeners] = listener;
			currentNumberOfListeners++;
		}
	}
	
private:
	void main(){
		for(;;){
			auto c = keypad.getc();
			for (auto &listener : KeyPadListener)
			{
				listener->KeyPressed(c);
			}
		}
	}
};

#endif //TOETSENBORD4X4_HPP
