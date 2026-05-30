#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/uart/uart.h"
// 引用原專案根目錄下 src 中的富士通核心庫
#include "../../src/FujitsuAC.h" 

namespace esphome {
namespace fujitsu_ac {

class FujitsuACClimate : public climate::Climate, public Component, public uart::UARTDevice {
 private:
  FujitsuAC ac_;

 public:
  void setup() override {
    // 初始化富士通空調控制實例，傳入 ESPHome 的 UART 介面
    this->ac_.begin(this); 
  }

  climate::ClimateTraits traits() override {
    auto traits = climate::ClimateTraits();
    traits.set_supports_current_temperature(true);
    traits.set_supported_modes({
      climate::CLIMATE_MODE_OFF,
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_HEAT,
      climate::CLIMATE_MODE_FAN_ONLY,
      climate::CLIMATE_MODE_DRY
    });
    traits.set_supported_fan_modes({
      climate::CLIMATE_FAN_AUTO,
      climate::CLIMATE_FAN_LOW,
      climate::CLIMATE_FAN_MEDIUM,
      climate::CLIMATE_FAN_HIGH
    });
    traits.set_visual_min_temperature(16.0);
    traits.set_visual_max_temperature(30.0);
    traits.set_visual_temperature_step(0.5);
    return traits;
  }

  void control(const climate::ClimateCall &call) override {
    if (call.get_mode().has_value()) {
      climate::ClimateMode mode = *call.get_mode();
      if (mode == climate::CLIMATE_MODE_OFF) {
        ac_.turnOff();
      } else {
        ac_.turnOn();
        // 這裡可依據原專案 ac_.setMode() 對應不同的模式
      }
    }
    if (call.get_target_temperature().has_value()) {
      float temp = *call.get_target_temperature();
      ac_.setTargetTemp((int)temp);
    }
    this->publish_state();
  }

  void loop() override {
    // 讓原專案的狀態機持續處理序列埠訊號
    ac_.loop(); 

    // 當冷氣狀態有變更時，即時回傳給 Home Assistant
    if (ac_.hasChanges()) {
      this->current_temperature = ac_.getCurrentTemp();
      this->target_temperature = ac_.getTargetTemp();
      // 您可以在此處繼續補上風速、擺葉等狀態同步
      this->publish_state();
    }
  }
};

}  // namespace fujitsu_ac
}  // namespace esphome
