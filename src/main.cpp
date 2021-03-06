#include "hwlib.hpp"
#include "rtos.hpp"

#include "SendTest.h"
#include "Logger.h"

#include "MessageDecoder.h"
#include "ReceiveIRController.h"
#include "RunGameController.h"
#include "SendIRController.h"
#include "SpeakerController.h"
#include "OledDisplay.h"
#include "Toetsenbord4x4.hpp"
#include "InitGameController.h"
#include "Speeltijd.h"
#include "ParametersController.h"
#include "IRReceiver.h"

int main()
{
  // logger
  // auto dumpButton = hwlib::target::pin_in(hwlib::target::pins::d10);
  // Logger logger(dumpButton, 13);

  // RunGameController &runGameController;

  //  sendircontroller
  auto IR = hwlib::target::d2_36kHz();
  auto red = hwlib::target::pin_out(hwlib::target::pins::d42);
  SendIRController sendIRcontroller(IR, red, 1);

  // speakercontroller
  auto speaker = hwlib::target::pin_out(hwlib::target::pins::d12);
  SpeakerController speakerController(speaker, 10);

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

  Toetsenbord4x4<1> Toetsenbord(keypad, 9);

  // InitGameController IGC(Toetsenbord, 8, sendIRcontroller);

  // // oleddisplay
  auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
  auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
  auto i2cBus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
  auto oled = hwlib::glcd_oled(i2cBus, 0x3c);
  auto font8x8 = hwlib::font_default_8x8();
  auto font16x16 = hwlib::font_default_16x16();
  auto display8x8 = hwlib::terminal_from(oled, font8x8);
  auto display16x16 = hwlib::terminal_from(oled, font16x16);

  OledDisplay oledDisplay(display8x8, display16x16);

  // testing IR
  // SendTest sendTest(sendIRcontroller, speakerController, oledDisplay, 12);
  //  rungamecontroller

  Speeltijd speeltijd;
  auto rungameled = hwlib::target::pin_out(hwlib::target::pins::d7);

  Timer countdown(oledDisplay, speeltijd, 6);


  HitLog hitLog;

  auto trigger = hwlib::target::pin_in(hwlib::target::pins::d43);
  InitShotController initShotController(trigger, sendIRcontroller, 3, 7);

  auto runGameLED = hwlib::target::pin_out(hwlib::target::pins::d45);
  RunGameController runGameController(oledDisplay, speeltijd, initShotController, runGameLED, countdown, hitLog, 2);

  ReceiveIRController receiveIRcontroller(runGameController, 5); // priority MessageDecoder, priority IRReceiver

  ParametersController parametersController(Toetsenbord, runGameController, 14);

  // messagedecoder
  auto tsopSignal = hwlib::target::pin_in(hwlib::target::pins::d8);
  auto led = hwlib::target::pin_out(hwlib::target::pins::d9);
  IRReceiver<1> irReceiver(tsopSignal, led, 0);

  MessageDecoder messageDecoder(irReceiver, receiveIRcontroller, 4);

  rtos::run();
}
