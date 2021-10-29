#include "hwlib.hpp"
#include "rtos.hpp"

class ir_receiver : public rtos::task<>
{
private:
   uint16_t message = 0;
   const int active = 100'000;
   uint_fast64_t last_signal = hwlib::now_us() - active;
   hwlib::target::pin_in &tsop_signal;
   hwlib::target::pin_out &led;

private:
   // void main()
   // {
   //    for (;;)
   //    {
   //       tsop_signal.refresh();

   //       led.write(tsop_signal.read());
   //       led.flush();
   //       hwlib::wait_us(100);
   //       // led.write((bool) (last_signal + active) > hwlib::now_us());

   //       // hwlib::wait_ms(10);
   //    }
   // }
   void main()
   {
      for (;;)
      {
         tsop_signal.refresh();
         if (tsop_signal.read())
         {
            for (unsigned int i = 0; i < 16; i++)
            {

               message <<= tsop_signal.read();
            }
         }

         led.write(tsop_signal.read());
         led.flush();
         hwlib::wait_us(100);
      }
   }

public:
   ir_receiver(hwlib::target::pin_in &tsop_signal, hwlib::target::pin_out &led) : tsop_signal(tsop_signal), led(led)
   {
   }
};

class ir_sender : public rtos::task<>
{
private:
   hwlib::target::d2_36kHz &ir;
   hwlib::target::pin_out &red;
   hwlib::target::pin_in &sw;

public:
   ir_sender(hwlib::target::d2_36kHz &ir, hwlib::target::pin_out &red, hwlib::target::pin_in &sw)
       : ir(ir), red(red), sw(sw)
   {
   }

private:
   void main()
   {
      for (;;)
      {

         for (unsigned int i = 0; i < 16; i++)
         {
            red.write(1);
            ir.write(0);

            hwlib::wait_us(1600);

            if (i % 2 == 0)
            {
               ir.write(1);

               hwlib::wait_us(800);
            }
            red.write(0);

            red.flush();

            hwlib::wait_ms(2000);
         }

         // sw.refresh();
         // ir.write(!sw.read());
         // red.write(!sw.read());
         // ir.flush();
         // red.flush();

         // hwlib::wait_ms(1);
         // ir.write(0);
         // ir.flush();
      }
   }
};

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
   :  rtos::task<>(priority, "DEBUG_TASK"), buttonClear(buttonClear), buttonDump(buttonDump)
   {
   }
};

// class message_decoder : public rtos::task<>,
//                         ir_receiver_listener
// {
//    enum state_t =
//        {
//            idle,
//            message};

// private:
//    state_t state = idle;

//    rtos::channel pausesChannel;
//    receiveIRController &receiveIRctrl
//        IRReceiver IRReceiver // Hier moet de constructor worden aangeroepen.
//        public : message_decoder()
//    {
//    }

// private:
// }
// for (;;)
// {
//    switch ( )
//    {
//    case idle:

//       break;

//    default:
//       break;
//    }
// }

int main(void)
{
   // debug stats
   auto Dbutton = hwlib::target::pin_in(hwlib::target::pins::d10);
   auto Cbutton = hwlib::target::pin_in(hwlib::target::pins::d11);

   statistics_debug SD(Dbutton, Cbutton, 0);

   auto tsop_signal = hwlib::target::pin_in(hwlib::target::pins::d8);
   auto led = hwlib::target::pin_out(hwlib::target::pins::d9);

   ir_receiver receiver(tsop_signal, led);

   auto ir = hwlib::target::d2_36kHz();
   auto red = hwlib::target::pin_out(hwlib::target::pins::d42);
   auto sw = hwlib::target::pin_in(hwlib::target::pins::d43);

   ir_sender sender(ir, red, sw);
   rtos::run();
}
