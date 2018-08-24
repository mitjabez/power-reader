#ifndef EMONCMS_H
#define EMONCMS_H

void initWiFi();
bool sendPowerUsage(int wattHoursHeater, int wattHoursHeatPump);

#endif
