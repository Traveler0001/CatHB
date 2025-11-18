#ifndef __APP_MAX31865_H
#define __APP_MAX31865_H

#include "Drivers/drv_max31865.h"
// #include 

MAX31685_STATUS app_MAX31865Init(void);
MAX31685_STATUS app_MAX31865Read(void);
MAX31685_STATUS app_MAX31865ReadConfig(void);
MAX31685_STATUS app_MAX31865SetConfig(void);
void app_MAX31865Test(void);

#endif // !__APP_MAX31865_H
