#include "hwlib.hpp"
#include "rtos.hpp"

#include "SendTest.h"
#include "Logger.h"

#include "ReceiveIRController.h"
#include "RunGameController.h"
#include "SendIRController.h"
#include "SpeakerController.h"
#include "OledDisplay.h"
#include "Toetsenbord4x4.hpp"
#include "InitGameController.h"

int main()
{
   // logger
   auto dumpButton = hwlib::target::pin_in(hwlib::target::pins::d11);
   Logger logger(dumpButton, 0);

   // rungamecontroller
   RunGameController runGameController;


   // receiveircontroller
   auto tsopSignal = hwlib::target::pin_in(hwlib::target::pins::d8);
   auto led = hwlib::target::pin_out(hwlib::target::pins::d9);

   ReceiveIRController receiveIRcontroller(runGameController, tsopSignal, led, logger, 5, 2, 1); // priority MessageDecoder, priority IRReceiver

   // sendircontroller
   auto IR = hwlib::target::d2_36kHz();
   auto red = hwlib::target::pin_out(hwlib::target::pins::d42);
   // auto sw = hwlib::target::pin_in(hwlib::target::pins::d43);
   SendIRController sendIRcontroller(IR, red);

   // speakercontroller
   auto speaker = hwlib::target::pin_out(hwlib::target::pins::d12);
   SpeakerController speakerController(speaker, 6);

   auto out0 = hwlib::target::pin_oc(hwlib::target::pins::a0);
   auto out1 = hwlib::target::pin_oc(hwlib::target::pins::a1);
   auto out2 = hwlib::target::pin_oc(hwlib::target::pins::a2);
   auto out3 = hwlib::target::pin_oc(hwlib::target::pins::a3);

   auto in0 = hwlib::target::pin_in(hwlib::target::pins::a4);
   auto in1 = hwlib::target::pin_in(hwlib::target::pins::a5);
   auto in2 = hwlib::target::pin_in(hwlib::target::pins::a6);
   auto in3 = hwlib::target::pin_in(hwlib::target::pins::a7);

   auto out_port = hwlib::port_oc_from(out0, out1, out2, out3);
   auto in_port = hwlib::port_in_from(in0, in1, in2, in3);
   auto matrix = hwlib::matrix_of_switches(out_port, in_port);
   auto keypad = hwlib::keypad<16>(matrix, "123A456B789C*0#D");

   Toetsenbord4x4<2> Toetsenbord(keypad, 3);

   InitGameController IGC(Toetsenbord);

   // oleddisplay
   // auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
   // auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
   // auto i2cBus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
   // auto oled = hwlib::glcd_oled(i2cBus, 0x3c);
   // auto font = hwlib::font_default_8x8();
   // auto display = hwlib::terminal_from(oled, font);

   // OledDisplay oledDisplay(display);

   // testing IR
   SendTest sendTest(sendIRcontroller, speakerController, 4);

   rtos::run();
}
