/* --------------------------------------------------------
                    hw_v003_serial_demo.ino

     Demo program for HardwareSerial_v00x.

     - text output on mySerial
     - reading from mySerial
     - counter

     HardwareSerial_v003.h /.cpp provides the functions
     not available in the core for the CH32V003.

     Note: the receive ring buffer has a size of 64 bytes,
     a transmit buffer has not been implemented
     
     14.06.2026   R. Seelig
   -------------------------------------------------------- */


#ifdef CH32V00x
  // CH32V003-specific instance

  #include "HardwareSerial_v00x.h"  
  HardwareSerial_v00x mySerial;
  
#else

  // all other cores, so that it also works on
  // compatible Arduino boards
  HardwareSerial &mySerial = Serial;
  
#endif

/* Color codes of the ESC sequences used in the terminal
   bk  bl  gn  cy  rd  mg  br  gy

   30, 34, 32, 36, 31, 35, 33, 37     foreground
   40, 44, 42, 46, 41, 45, 41, 47     background

   for bright background colors add 60 to the value

   Output string for color selection:

      "\033[brightness foreground_color; foreground_color; background_color"

   Note:
      the background color must be followed by the character 'm'

   Example for yellow text (bright brown) on light blue background:
      yellow (bright brown) = 33
      light blue            = 44 + 60 = 104
      output string         = "\033;1;33;104m"

*/

// The ANSI sequences are declared volatile here because SDCC, for whatever reason,
// performs an optimization without volatile that causes the clrscr string
// to no longer be stored correctly!

const char col_yellow_cyan[]  = "\033[;1;33;42m";    // yellow on cyan
const char col_white_black[]  = "\033[;1;37;40m";    // white on black
const char col_grey_black[]   = "\033[;0;37;40m";    // white on black
const char col_iblue_black[]  = "\033[;1;34;40m";    // light blue on black
const char col_igreen_black[] = "\033[;1;32;40m";    // light green on black

const char clrscr[]           = "\033[2J\033[H";     // string to clear terminal contents


/* -------------------------------------------------------------------
                               myReadln

     reads characters from mySerial into a character array

     Arguments:
        *str   : pointer to the array receiving the input                                   
        maxlen : maximum number of input characters
   ------------------------------------------------------------------- */
void myReadln(char *str, uint8_t maxlen)
{
  char ch;
  uint8_t cnt = 0;

  if (!maxlen) return;

  while (1)
  {
    while (!mySerial.available());                 // wait until a character is available
    ch = mySerial.read();
    mySerial.print(ch);                            // echo received character

    if ((ch == '\r') || (ch == '\n')) { break; }   // Return || Newline terminates input

    if (cnt < (maxlen - 1))
    {
      *str++ = ch;
      cnt++;
    }
  }

  *str = 0;
}

// --------------------------------------------------------
//                           setup
// --------------------------------------------------------
void setup() 
{
  mySerial.begin(115200);
  mySerial.print(clrscr);
  mySerial.print(col_yellow_cyan);
  mySerial.println("");
  mySerial.println("----------------------------------"); 
  mySerial.println("  CH32V003                        ");
  mySerial.println("  HardwareSerial_v00x / 115200bd  ");
  mySerial.println("----------------------------------");   
}

// --------------------------------------------------------
//                           loop
// --------------------------------------------------------
void loop() 
{
  static int count;
  char str[6];
  char ch;

  mySerial.print(col_grey_black);
  mySerial.println("\n\n\r");
  mySerial.print("start counter at [input]: ");
  mySerial.print(col_iblue_black);
  myReadln(str, 5);
  count = atoi(str);  
  mySerial.println("\n\r");
  mySerial.print(col_igreen_black);

  while(!mySerial.available())
  {
    mySerial.print("\r  counter: ");
    mySerial.print(count, DEC);
    count++;
    delay(1000);
  }
  
  while(mySerial.available()) ch= mySerial.read();  // clear receive buffer
}
