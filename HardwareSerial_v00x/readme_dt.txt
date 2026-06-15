
HardwareSerial_v00x
-------------------------------------------------------------------------------------------

Der "offizielle" Arduino-Core von WCH

                     https://github.com/openwch/arduino_core_ch32

ist in vielerlei Hinsicht unvollstaendig, was leider auch auf HardwareSerial zutrifft,
welche das Standardobjekt Serial zur Verfuegung stellt. 

Serial kann hier nur schreibend verwendet werden, jedoch nicht lesend, sodass Serial.read
und Serial.available nicht funktionieren.

HardwareSerial_v00x stellt eine Ersatzimplementierung bereit, mittels dieser ein 
funktionsfaehiges serielles Objekt erzeugt werden kann:

    HardwareSerial_v00x mySerial;
    
In Setup kann ein mySerial analog zu Serial mit

    mySerial.begin(19200);
    
initialisiert werden. Da HardwareSerial_v00x den Stream von Arduino erbt, stehen die
ueblichen Stream-Methoden zur Verfuegung:

    mySerial.print(...)
    mySerial.println(...)
    mySerial.read()
    mySerial.available()
    
Wie grundsaetzlich ein Arduino-Core fuer CH32V003 installiert werden kann, ist in

                     https://www.jjflash.de/ch32v003/ch32v003_arduino.html#001401ard
                     
beschrieben.

JJFlash im Juni 2026
                     
