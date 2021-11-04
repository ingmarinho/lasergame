#pragma once

#include <array>

class Logger : public rtos::task<>
{
private:
    hwlib::target::pin_in &buttonDump;
    std::array<int, 50> logs;
    unsigned int index = 0;

private:
    void main()
    {
        bool prevPress = true;
        while (true)
        {
            bool buttonReadDump = !buttonDump.read();

            if (buttonReadDump && !prevPress)
            {
                for (auto &log : logs)
                {
                    // for (unsigned int i = 0; i < 50; i++)
                    // {
                    if (log == 983)
                        continue;
                    hwlib::cout << log << hwlib::endl;
                }
                logs.fill(983);
                index = 0;
            }
            hwlib::wait_ms(100);

            prevPress = buttonReadDump;
        }
    }

public:
    Logger(hwlib::target::pin_in &buttonDump, unsigned int priority)
        : rtos::task<>(priority, "LOGGER_TASK"), buttonDump(buttonDump)
    {
        logs.fill(983);
        // addLog("I am da Best");
        // addLog("I am da Best2");
        // addLog("I am da Best3");
    }

    void addLog(int log)
    {
        logs[index] = log;
        index++;
        if (index > 49)
        {
            index = 0;
        }
    }
};