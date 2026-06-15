/* -----------------------------------------------------
                   HardwareSerial_v00x.cpp

    provides UART / Serial functions that are not 
    available in the core

    MCU   :  CH32V003
    Clock :  48 MHz

    30.11.2025  R. Seelig
  ------------------------------------------------------ */

#ifdef CH32V00x

#include "HardwareSerial_v00x.h"

// -----------------------------------------------------------
//   Ring buffer for serial interface USART
// -----------------------------------------------------------
#define RECV_BUF_SIZE                64    // ring buffer size
uint8_t _recv_buf[RECV_BUF_SIZE];

volatile uint8_t _recv_buf_tail = 0;
volatile uint8_t _recv_buf_head = 0;

/* --------------------------------------------------------
                   _recv_bufferadd

     adds a new character to the ring buffer

     Parameters:
          c : character to insert
   -------------------------------------------------------- */
void _recv_bufferadd(uint8_t c)
{
  int next = (_recv_buf_head + 1) & (RECV_BUF_SIZE - 1);

  if (next == _recv_buf_tail)
  {
    _recv_buf_tail = (_recv_buf_tail + 1) & (RECV_BUF_SIZE - 1); // overwrite old
  }

  _recv_buf[_recv_buf_head] = c;
  _recv_buf_head = next;
}   

/* --------------------------------------------------------
                   _recv_bufferget

     reads a character from the ring buffer

     Parameters:
        *c  : pointer to uint8_t where the character
              will be stored

     Return:
           0 => no character available
           1 => character was read
   -------------------------------------------------------- */
uint8_t _recv_bufferget(uint8_t *c)
{
  if (_recv_buf_head == _recv_buf_tail) return 0;

  *c = _recv_buf[_recv_buf_tail];
  _recv_buf_tail = (_recv_buf_tail + 1) & (RECV_BUF_SIZE - 1);

  return 1;
}

/* --------------------------------------------------------
                        ISR USART1

      must be declared as extern "C" because the
      interrupt handling is outside Arduino core and
      is not fully implemented for CH32V00x
   -------------------------------------------------------- */
extern "C" void USART1_IRQHandler(void) __attribute__((interrupt));
void USART1_IRQHandler(void)
{
  int cx;             // timeout counter

  {
    cx= 0;
    do
    {
      if (USART1->STATR & USART_STATR_RXNE)           // if a received character triggered the interrupt
      {
        // store received character into ring buffer
        _recv_bufferadd(USART1->DATAR);

      }
      cx++;
    } while (((USART1->STATR & USART_STATR_RXNE) != 0) && (cx< 10000));
  }
}

// --------------------------------------------------------------------
//                      v00x_Serial (constructor)
// --------------------------------------------------------------------
HardwareSerial_v00x::HardwareSerial_v00x()
{
}

/* --------------------------------------------------------------------
                               begin

     initializes serial interface with 8N1 protocol
     TxD on pin PD5, RxD on pin PD6

     Parameters:
        baudrate : baud rate to set
   ---------------------------------------------------------- */
void HardwareSerial_v00x::begin(uint32_t brate)
{
  #define pinnr   6            // PD6

  baudrate= brate;
  // Enable UART and GPIOD
  RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1;

  // Push-Pull, 10MHz output on D5, alternate function (TxD)
  GPIOD->CFGLR = (GPIOD->CFGLR & ~(0x0f << (4*5))) |
                ((GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<( 4*5 ));

  // Enable pull-up on PD6
  GPIOD->BSHR |= (1 << pinnr);

  // PD6 as input
  GPIOD->CFGLR &= ~(0x0f << (4 * pinnr));
  GPIOD->CFGLR |= ((GPIO_CNF_IN_PUPD) << (4 * pinnr));

  GPIOD->BSHR |= (1 << pinnr);

  // Setup UART for 8N1
  USART1->CTLR1 = USART_WordLength_8b | USART_Parity_No | USART_Mode_Tx | USART_Mode_Rx;
  USART1->CTLR2 = USART_StopBits_1;

  // Set baud rate and enable UART
  USART1->BRR = ((CH32V00X_SYSTEM_CORE_CLOCK) + (baudrate)/2) / (baudrate);
  USART1->CTLR1 |= CTLR1_UE_Set;
  
  // Enable USART1 interrupts
  NVIC_EnableIRQ(USART1_IRQn);            
  USART1->CTLR1 |= USART_CTLR1_RXNEIE;    // enable receive interrupt flag  
}

/* ----------------------------------------------------------
                          putchar

     sends a character via serial interface

     Parameters:
       ch : character to send
   ---------------------------------------------------------- */
void HardwareSerial_v00x::putchar(uint8_t ch)
{
   while(!(USART1->STATR & USART_FLAG_TXE));
   USART1->DATAR = ch;
}

/* ----------------------------------------------------------
                           getchar

     waits until a character is received and reads it

     Return:
       received character
   ---------------------------------------------------------- */
uint8_t HardwareSerial_v00x::getchar(void)
{
  uint8_t c = 0;
  
  _recv_bufferget(&c);
  return c;
}

/* ----------------------------------------------------------
                           ischar

     checks if a character is available in the serial buffer
     but does not consume it

     Return:
       1 : character available
       0 : no character available
   ---------------------------------------------------------- */
uint8_t HardwareSerial_v00x::ischar(void)
{
  return (_recv_buf_head != _recv_buf_tail);  
}

/* ------------------------------------------------------------
                           Stream methods
  ------------------------------------------------------------ */
int HardwareSerial_v00x::available()
{
  return (_recv_buf_head - _recv_buf_tail + RECV_BUF_SIZE)
         & (RECV_BUF_SIZE - 1);
}

int HardwareSerial_v00x::read()
{
  return getchar();
}

int HardwareSerial_v00x::peek()
{
  if (_recv_buf_head == _recv_buf_tail)
  {
    return -1;
  }
  return _recv_buf[_recv_buf_tail];
}

size_t HardwareSerial_v00x::write(uint8_t ch)
{
  putchar(ch);
  return 1;
}

#endif
