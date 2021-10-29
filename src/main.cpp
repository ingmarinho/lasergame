#include "hwlib.hpp"
#include "rtos.hpp"

class ir_receiver : public rtos::task<>
{
private:
   const int active = 100'000;
   uint_fast64_t last_signal = hwlib::now_us() - active;
   hwlib::target::pin_in &tsop_signal;
   hwlib::target::pin_out &led;

private:
   void main()
   {
      for (;;)
      {
         tsop_signal.refresh();
         if (tsop_signal.read() == 0)
         {
            last_signal = hwlib::now_us();
         }
         led.write((bool) (last_signal + active) > hwlib::now_us());
         led.flush();
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
   hwlib::target::d2_36kHz& ir;
   hwlib::target::pin_out& red;
   hwlib::target::pin_in& sw;

private:
   void main()
   {
      hwlib::wait_ms(1);

      sw.refresh();
      ir.write(!sw.read());
      red.write(!sw.read());
      ir.flush();
      red.flush();

      hwlib::wait_ms(1);
      ir.write(0);
      ir.flush();
   }

public:
   ir_sender(hwlib::target::d2_36kHz& ir, hwlib::target::pin_out& red, hwlib::target::pin_in& sw)
   : ir(ir), red(red), sw(sw)
   {
   }
};

int main(void)
{
   auto tsop_signal = hwlib::target::pin_in(hwlib::target::pins::d8);
   auto led = hwlib::target::pin_out(hwlib::target::pins::d9);

   ir_receiver receiver(tsop_signal, led);

   auto ir = hwlib::target::d2_36kHz();
   auto red = hwlib::target::pin_out(hwlib::target::pins::d42);
   auto sw = hwlib::target::pin_in(hwlib::target::pins::d43);

   ir_sender sender(ir, red, sw);

   rtos::run();
}
