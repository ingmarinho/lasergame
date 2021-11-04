#pragma once

class DebugStats : public rtos::task<>
{
private:
   // hwlib::target::pin_in &buttonClear;
   hwlib::target::pin_in &buttonDump;

private:
   void main()
   {
      bool prevPress = true;
      while (true)
      {
         bool buttonReadDump = !buttonDump.read();
         // bool buttonReadClear = !buttonClear.read();

         if (buttonReadDump && !prevPress)
         {
            rtos::display_statistics();
            rtos::statistics_clear();
         }

         // if (buttonReadClear && !prevPress)
         // {
         //    rtos::statistics_clear();
         // }

         prevPress = buttonReadDump;
         hwlib::wait_ms(100);
      }
   }

public:
   // DebugStats(hwlib::target::pin_in &buttonClear, hwlib::target::pin_in &buttonDump, unsigned int priority)
   //     : rtos::task<>(priority, "DEBUGSTATS_TASK"), buttonClear(buttonClear), buttonDump(buttonDump)
   // {
   // }
   DebugStats(hwlib::target::pin_in &buttonDump, unsigned int priority)
       : rtos::task<>(priority, "DEBUGSTATS_TASK"), buttonDump(buttonDump)
   {
   }
};