/* -----------------------------------------------------
                        HardwareSerial_v00x.h

    Header for functions of the CH32V003 thar are
    not available in the core

    MCU   :  CH32V003
    Clock :  48 MHz

    30.11.2025  R. Seelig
  ------------------------------------------------------ */

#ifdef CH32V00x

  #ifndef in_v00x_serial
    #define in_v00x_serial

    #include <stdint.h>
    #include "Arduino.h"

    class HardwareSerial_v00x : public Stream
    {

      #define  CH32V00X_SYSTEM_CORE_CLOCK  48000000l
      #define  GPIO_CNF_OUT_PP_AF          8
      #define  GPIO_CNF_IN_PUPD            8
      #define  CTLR1_UE_Set                ((uint16_t)0x2000) // USART enable mask


      public:
        HardwareSerial_v00x();

        void begin(uint32_t brate);

        void putchar(uint8_t ch);
        uint8_t getchar(void);
        uint8_t ischar(void);

        // Stream Methoden
        int available() override;
        int read() override;
        int peek() override;
        size_t write(uint8_t) override;
        operator bool()
        {
          return 1;
        }
        void end ()
        {
        }

      private:
        uint32_t baudrate;

    };

  #endif

#endif
