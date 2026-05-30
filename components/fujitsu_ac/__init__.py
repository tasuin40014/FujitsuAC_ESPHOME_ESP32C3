import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

# 定義這個組件的代號
CODEOWNERS = ["@your_github_handle"]
DEPENDENCIES = ["uart"]

fujitsu_ac_ns = cg.esphome_ns.namespace("fujitsu_ac")

CONFIG_SCHEMA = cv.Schema({}).extend(cv.COMPONENT_SCHEMA)
