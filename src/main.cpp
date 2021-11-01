#include "hwlib.hpp"
#include "rtos.hpp"

#include "DebugStats.h"
#include "IRReceiver.h"
#include "IRSender.h"
#include "MessageDecoder.h"


int main(void)
{
   // debug stats
   auto dumpButton = hwlib::target::pin_in(hwlib::target::pins::d10);
   auto clearButton = hwlib::target::pin_in(hwlib::target::pins::d11);
   DebugStats debugStatsObject(dumpButton, clearButton, 0);

   // auto tsop_signal = hwlib::target::pin_in(hwlib::target::pins::d8);
   // auto led = hwlib::target::pin_out(hwlib::target::pins::d9);

   // ir_receiver receiver(tsop_signal, led);

   // auto ir = hwlib::target::d2_36kHz();
   // auto red = hwlib::target::pin_out(hwlib::target::pins::d42);
   // auto sw = hwlib::target::pin_in(hwlib::target::pins::d43);

   // ir_sender sender(ir, red, sw);

   rtos::run();
}
