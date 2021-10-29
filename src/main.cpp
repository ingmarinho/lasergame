#include "hwlib.hpp"
#include "rtos.hpp"

#include "Statistics_Debug.h"
#include "IR_Receiver.h"

class Message_Decoder : public rtos::task<>, IRReceiveListener
{
   enum state_t =
       {
           IDLE,
           MESSAGE};

private:
   state_t state = IDLE;

   rtos::channel pausesChannel;
   receiveIRController& receiveIRctrl;
   IR_Receiver& IRReceiver;
public:
   Message_Decoder()
   {
      IR_Receiver.addIR_receiver_listener(this);
   }

private:
   void pause_detected(int pause) override
   {
      pauseChannel.write(pause);
   }

   void main()
   {

      rtos::clock cl(100);

      for (;;)
      {
         switch (state)
         {
         case IDLE:
            wait(cl);
            if (pauseChannel.read() )
            {
               
            }
            else
            {
               state = IDLE
            }
            break;

         case MESSAGE:

            break;

         case default:

            break;
         }
      }
   }
};

int main(void)
{
   // debug stats
   auto Dbutton = hwlib::target::pin_in(hwlib::target::pins::d10);
   auto Cbutton = hwlib::target::pin_in(hwlib::target::pins::d11);
   statistics_debug SD(Dbutton, Cbutton, 0);

   // auto tsop_signal = hwlib::target::pin_in(hwlib::target::pins::d8);
   // auto led = hwlib::target::pin_out(hwlib::target::pins::d9);

   // ir_receiver receiver(tsop_signal, led);

   // auto ir = hwlib::target::d2_36kHz();
   // auto red = hwlib::target::pin_out(hwlib::target::pins::d42);
   // auto sw = hwlib::target::pin_in(hwlib::target::pins::d43);

   // ir_sender sender(ir, red, sw);


   rtos::run();
}
