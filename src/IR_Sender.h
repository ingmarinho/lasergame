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