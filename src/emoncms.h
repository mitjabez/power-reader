#ifndef EMONCMS_H
#define EMONCMS_H

void initWiFi();
bool sendPowerUsage(unsigned long countHeater, unsigned long countHeatPump);

#endif
