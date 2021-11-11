#pragma once

class ButtonListener
{
public:
    virtual void ButtonPressed(int buttonid);
};

template <unsigned int maxNumberOfListeners>
class button : public rtos::task<>
{

private:
    std::array<ButtonListener *, maxNumberOfListeners> ButtonListeners;
    hwlib::target::pin_in &Trigger;
    unsigned int currentNumberOfListeners = 0;

public:
    button(hwlib::target::pin_in &button, unsigned int priority) : rtos::task<>(priority, "BUTTON_TAAK"), Trigger(button)
    {
        for (unsigned int i = 0; i < maxNumberOfListeners; i++)
        {
            ButtonListeners[i] = nullptr;
        }
    }

    void addListener(ButtonListener *listener)
    {
        if (currentNumberOfListeners < maxNumberOfListeners)
        {
            ButtonListeners[currentNumberOfListeners] = listener;
            currentNumberOfListeners++;
        }
    }

    void main()
    {
        for (;;)
        {
            bool t = false;

            while (!t)
            {
                t = Trigger.read();
                // hwlib::wait_ms(200);
            }

            for (unsigned int i = 0; i < maxNumberOfListeners; i++)
            {
                auto listener = ButtonListeners[i];
                if (listener != nullptr)
                {
                    listener->ButtonPressed(0);
                }
            }
            hwlib::wait_ms(200);
        }
    }
};