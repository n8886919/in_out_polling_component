#include <Wire.h>
#include <VL53L1X.h>
#include <StopWatch.h>
#include "esphome.h"

class MyCustomSensor : public Component {
 public:

  VL53L1X VL53L1X_1, VL53L1X_2;
  uint16_t last_mm1 = 0;
  uint16_t last_mm2 = 0;
  uint16_t mm1, mm2, change;
  uint32_t elapsed_1, elapsed_2;
  Sensor *tof_1 = new Sensor();
  Sensor *tof_2 = new Sensor();

  StopWatch SWarray[2];
  Sensor *timer_1 = new Sensor();
  Sensor *timer_2 = new Sensor();

  void setup() override {
    // This will be called by App.setup()
    SWarray[0].start();
    SWarray[1].start();
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C

    if (!VL53L1X_1.init()) {
      ESP_LOGE("VL53L1X 1 custom sensor", "Failed to detect and initialize sensor!");
      while (1);
    }
    VL53L1X_1.setAddress(42);
    VL53L1X_1.setTimeout(500);
    VL53L1X_1.setDistanceMode(VL53L1X::Long);
    VL53L1X_1.setMeasurementTimingBudget(50000);
    VL53L1X_1.startContinuous(50);

    pinMode(2, OUTPUT);
    pinMode(2, INPUT);
    if (!VL53L1X_2.init()) {
      ESP_LOGE("VL53L1X 2 custom sensor", "Failed to detect and initialize sensor!");
      while (1);
    }
    VL53L1X_2.setTimeout(500);
    VL53L1X_2.setDistanceMode(VL53L1X::Long);
    VL53L1X_2.setMeasurementTimingBudget(50000);
    VL53L1X_2.startContinuous(50);
  }

  void loop() override {
    mm1 = VL53L1X_1.read();
    if (!VL53L1X_1.timeoutOccurred()) {
      change = abs(last_mm1 - mm1);
      if (change > 100) {
        SWarray[0].stop();

        tof_1->publish_state(mm1);
        elapsed_1 = SWarray[0].elapsed();

        if (elapsed_1 < 5000) {
          timer_1->publish_state(elapsed_1);
        }
        last_mm1 = mm1;
        SWarray[0].reset();
        SWarray[0].start();
        ESP_LOGI("VL53L1X 1 custom sensor", "address: %x,\tchange: % u,\tdt: %u",
                 VL53L1X_1.getAddress(), change, elapsed_1);

      } else {
        ESP_LOGD("VL53L1X 1 custom sensor", "change  %u mm", change);
      }
    } else {
      ESP_LOGE("VL53L1X 1 custom sensor", "Timeout during read().");
    }

    mm2 = VL53L1X_2.read();
    if (!VL53L1X_2.timeoutOccurred()) {
      change = abs(last_mm2 - mm2);
      if (change > 100) {
        SWarray[1].stop();

        tof_2->publish_state(mm2);
        elapsed_2 = SWarray[1].elapsed();
        if (elapsed_2 < 5000) {
          timer_2->publish_state(elapsed_2);
        }
        last_mm2 = mm2;
        SWarray[1].reset();
        SWarray[1].start();
        ESP_LOGW("VL53L1X 2 custom sensor", "address: %x,\tchange: % u,\tdt: %u",
                 VL53L1X_2.getAddress(), change, elapsed_2);
      } else {
        ESP_LOGD("VL53L1X 2 custom sensor", "change  %u mm", change);
      }
    } else {
      ESP_LOGE("VL53L1X 2 custom sensor", "Timeout during read().");
    }
  }

};
