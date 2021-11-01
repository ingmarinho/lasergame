#pragma once

class MessageDecoder : public rtos::task<>, public IRReceiverListener
{

   enum state_t
   {
      IDLE,
      MESSAGE
   };

private:
   state_t state = IDLE;

   rtos::channel<int, 1024> pausesChannel;
   rtos::clock pauseClock(100);
   // receiveIRController &receiveIRctrl;
   IRReceiver<2> &IRReceiverObject;

public:
   MessageDecoder(IRReceiver<2> &IRReceiverObject) :
   pausesChannel(this), pauseClock(this, 100 * rtos::ms), IRReceiverObject(IRReceiverObject)
   {
      IRReceiverObject.addListener(this);
   }

   void pauseDetected(int pause) override
   {
      pausesChannel.write(pause);
   }

private:
   bool isValid(const int &pause)
   {
      return (200 > pause && pause > 2000);
   }

   void main()
   {
      for (;;)
      {
         switch (state)
         {
         case IDLE:
            wait(pauseClock);
            if (pausesChannel.read())
            {
               state = MESSAGE;
            }
            break;

         case MESSAGE:
            int pause = pausesChannel.read();

            if (pause > 4000)
            {
               state = IDLE;
            }
            break;
         }
      }
      // if !isValid(pause) {state = IDLE; break;}

      //    if (pause <)

      //       switch ()

      //          break;

      // case default:

      //    break;
      // }
   }
};