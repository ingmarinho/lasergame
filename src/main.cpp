#include "hwlib.hpp"
#include "rtos.hpp"

#include "DebugStats.h"
#include "MessageDecoder.h"
#include "SendIRController.h"


int main()
{
   // debug stats
   auto dumpButton = hwlib::target::pin_in(hwlib::target::pins::d10);
   auto clearButton = hwlib::target::pin_in(hwlib::target::pins::d11);
   DebugStats debugStats(dumpButton, clearButton, 0);

   auto tsopSignal = hwlib::target::pin_in(hwlib::target::pins::d8);
   auto led = hwlib::target::pin_out(hwlib::target::pins::d9);
   MessageDecoder messageDecoder(tsopSignal, led, 2, 1); // priority MessageDecoder, priority IRReceiver

   auto IR = hwlib::target::d2_36kHz();
   auto red = hwlib::target::pin_out(hwlib::target::pins::d42);
   // auto sw = hwlib::target::pin_in(hwlib::target::pins::d43);
   SendIRController sendIRcontroller(IR, red);

   // ir_sender sender(ir, red, sw);

   rtos::run();
}
