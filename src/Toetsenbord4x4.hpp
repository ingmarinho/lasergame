#ifndef TOETSENBORD4X4_HPP
#define TOETSENBORD4X4_HPP

#include <array>

class KeyPadListener {
public:
	virtual void KeyPressed (char KeyID);
};

template <unsigned int maxNumberOfListeners>
class Toetsenbord4x4  : public rtos::task<2000>{

private:
	hwlib::keypad<16> &keypad;
	std::array< KeyPadListener*, maxNumberOfListeners> keypadListeners;
	unsigned int currentNumberOfListeners = 0;
	
public:
	Toetsenbord4x4(hwlib::keypad<16> &keypad, unsigned int priority) : rtos::task<2000>(priority, "TOETSENBORD_TAAK"), keypad(keypad)
	{
		unsigned int i=0;
		for(i=0;i<maxNumberOfListeners;i++)
		{
			keypadListeners[i]=nullptr;
		}
	}

	void addListener(KeyPadListener *listener)
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
			char c = keypad.getc();
			hwlib::cout << c << hwlib::endl;
			
			unsigned int i=0;
			for(i=0;i < maxNumberOfListeners;i++)
			{
				auto listener = keypadListeners[i];
				if(listener!=nullptr)
				{
				   listener->KeyPressed(c);	
				}
			}
			
			// for (auto &listener : keypadListeners)
			// {
				
			// 	listener->KeyPressed(c);
			// }
		}
	}
};



#endif //TOETSENBORD4X4_HPP
