#include "esphome.h"

#include <Wire.h>
#include <VL53L1X.h>

VL53L1X tof_sensor1;

class MyCustomSensor1 : public PollingComponent, public Sensor {
 public:
  // constructor
  MyCustomSensor1() : PollingComponent(500) {} // polling every 15s

  void setup() override {
    // This will be called by App.setup()
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C

    if (!tof_sensor1.init()) {
      ESP_LOGE("VL53L1X 1 custom sensor", "Failed to detect and initialize sensor!");
      while (1);
    }
    tof_sensor1.setAddress(42);

    tof_sensor1.setTimeout(500);
    tof_sensor1.setDistanceMode(VL53L1X::Long);
    tof_sensor1.setMeasurementTimingBudget(50000);
    tof_sensor1.startContinuous(50);

  }

  void update() override {
    uint16_t mm = tof_sensor1.read();
    
    if (!tof_sensor1.timeoutOccurred()) {
      ESP_LOGI("VL53L1X 1 custom sensor", " val # %x",  tof_sensor1.getAddress());
      publish_state(mm);
    } else {
      ESP_LOGE("VL53L1X 1 custom sensor", "Timeout during read().");
    }
  }
};