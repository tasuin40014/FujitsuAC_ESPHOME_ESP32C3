import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, uart
from esphome.const import CONF_ID

# 引用同目錄下的命名空間
from . import fujitsu_ac_ns

# 綁定 C++ 類別名稱
FujitsuACClimate = fujitsu_ac_ns.class_(
    "FujitsuACClimate", climate.Climate, cg.Component
)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(FujitsuACClimate),
    }
).extend(uart.UART_DEVICE_SCHEMA).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    await uart.register_uart_device(var, config)
