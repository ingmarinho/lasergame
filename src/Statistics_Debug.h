#include "hwlib.hpp"
#include "rtos.hpp"

class statistics_debug : public rtos::task<>
{
private:
   hwlib::target::pin_in &buttonClear;
   hwlib::target::pin_in &buttonDump;

private:
   void main()
   {
      bool prevPress = true;
      while (true)
      {
         bool buttonReadDump = !buttonDump.read();
         bool buttonReadClear = !buttonClear.read();

         if (buttonReadDump && !prevPress)
         {
            rtos::display_statistics();
         }

         if (buttonReadClear && !prevPress)
         {
            rtos::statistics_clear();
         }

         prevPress = buttonReadDump;
         hwlib::wait_ms(100);
      }
   }

public:
   statistics_debug(hwlib::target::pin_in &buttonClear, hwlib::target::pin_in &buttonDump, unsigned int priority)
       : rtos::task<>(priority, "DEBUG_TASK"), buttonClear(buttonClear), buttonDump(buttonDump)
   {
   }
};