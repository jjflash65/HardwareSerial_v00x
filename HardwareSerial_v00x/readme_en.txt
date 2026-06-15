HardwareSerial_v00x
-------------------------------------------------------------------------------------------

The "official" Arduino core from WCH

                     https://github.com/openwch/arduino_core_ch32

is incomplete in several aspects, which unfortunately also applies to HardwareSerial,
which provides the standard Serial object.

Serial can only be used for transmission, but not for receiving data. Therefore,
Serial.read() and Serial.available() do not work.

HardwareSerial_v00x provides a replacement implementation that allows creating a
fully functional serial object:

    HardwareSerial_v00x mySerial;

In setup(), mySerial can be initialized in the same way as Serial:

    mySerial.begin(19200);

Since HardwareSerial_v00x inherits from Arduino Stream, the usual Stream methods
are available:

    mySerial.print(...)
    mySerial.println(...)
    mySerial.read()
    mySerial.available()

General information on how to install an Arduino core for the CH32V003 can be found at:

                     https://www.jjflash.de/ch32v003/ch32v003_arduino.html#001401ard

JJFlash, June 2026
