# About

Esp8266 reads pulses (1 pulse/100Wh) from Levato DME D100 T1 energy meter and sends them to MQTT.

## Prerequisites

* https://github.com/esp8266/Arduino
* https://github.com/plerup/makeEspArduino

## Build

Prepare and edit `config.h`:
```
cp src/config.h.example config.h
```

Build:

```
make clean
make flash
```
