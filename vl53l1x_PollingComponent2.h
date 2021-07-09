#include "esphome.h"

// #include <Wire.h>
#include <VL53L1X.h>

VL53L1X tof_sensor2;

class MyCustomSensor2 : public PollingComponent, public Sensor {
 public:
  // constructor
  MyCustomSensor2() : PollingComponent(500) {} // poll1ing every 0.5s

  void setup() override {
    // This will be called by App.setup()
    // Wire.begin();
    // Wire.setClock(400000); // use 400 kHz I2C
    pinMode(2, OUTPUT);
    pinMode(2, INPUT);
    if (!tof_sensor2.init()) {
      ESP_LOGE("VL53L1X 2 custom sensor", "Failed to detect and initialize sensor!");
      while (1);
    }
    // tof_sensor2.setAddress(41);
    tof_sensor2.setTimeout(500);
    tof_sensor2.setDistanceMode(VL53L1X::Long);
    tof_sensor2.setMeasurementTimingBudget(50000);
    tof_sensor2.startContinuous(50);
  }

  void update() override {
    uint16_t mm = tof_sensor2.read();
    
    if (!tof_sensor2.timeoutOccurred()) {
      ESP_LOGI("VL53L1X 2 custom sensor", " val # %x",  tof_sensor2.getAddress());
      publish_state(mm);
    } else {
      ESP_LOGE("VL53L1X 2 custom sensor", "Timeout during read().");
    }
  }
};