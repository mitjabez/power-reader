# About

Esp8266 app which reads pulses (1 pulse/100Wh) from Levato DME D100 T1 energy meter and sends them to MQTT. Data is then processed by Node-RED and monitored in Grafana. The whole solution is used for monitoring the energy consumption of my heat pump.

## Prerequisites

* https://github.com/esp8266/Arduino
* https://github.com/plerup/makeEspArduino

If using Arduino IDE you can skip the above.

## Build

Prepare and edit `config.h`:
```
cp src/config.h.example config.h
```

Build using Arduino IDE or make:

```
make clean
make flash
```
