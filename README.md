# HardwareSerial_v00x

A lightweight replacement implementation of Arduino `HardwareSerial` for the CH32V003 core.

---

## Overview

The official Arduino core for WCH CH32 devices:

https://github.com/openwch/arduino_core_ch32

is incomplete in several areas, especially regarding serial communication.

The standard `Serial` object is often write-only in this environment, meaning that
functions like `Serial.read()` and `Serial.available()` are not functional.

This library provides a simple workaround by implementing a fully working serial
interface for CH32V003-based projects.

---

## Features

- Full `Stream`-compatible serial interface
- Supports:
  - `print()`
  - `println()`
  - `read()`
  - `available()`
- Simple drop-in replacement via a custom serial instance
- Designed specifically for CH32V003 (48 MHz)

---

## Usage

```cpp
#include "HardwareSerial_v00x.h"

HardwareSerial_v00x mySerial;

void setup()
{
  mySerial.begin(19200);
}

void loop()
{
  if (mySerial.available())
  {
    char c = mySerial.read();
    mySerial.print(c);
  }
}
```

---


### Manual installation (ZIP)

1. Download the file HardwareSerial_v00x.zip
2. Open Arduino IDE  
3. Go to:

```
Sketch → Include Library → Add .ZIP Library...
```

4. Select the downloaded ZIP file

---

## Example

Included example sketch:

```
examples/hw_v003_serial_demo/
```

It demonstrates:
- serial input handling
- counter output
- simple terminal interaction

---

## Notes

- RX buffer size: 64 bytes
- No TX buffer implemented
- Minimal footprint design
- Direct register-level implementation for CH32V003

---

## Compatibility

- CH32V003 (primary target)
- Can optionally be mapped to standard `Serial` on other Arduino cores

---

## Author

JJFlash  
June 2026
