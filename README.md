[![Build_special_firmware](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner-direct.svg)](https://github.com/vshymanskyy/StandWithUkraine/blob/main/docs/README.md)

![Tasmota logo](https://github.com/arendst/Tasmota/blob/development/tools/logo/TASMOTA_FullLogo_Vector.svg#gh-light-mode-only)![Tasmota logo](https://github.com/arendst/Tasmota/blob/development/tools/logo/TASMOTA_FullLogo_Vector_White.svg#gh-dark-mode-only)


## These special Tasmota binaries are not official stable releases

## ⚠️ No support/warranty with these binaries! ⚠️

[![Build_special_firmware](https://github.com/Jason2866/Tasmota-build/actions/workflows/Build_special_firmware.yml/badge.svg)](https://github.com/Jason2866/Tasmota-build/actions/workflows/Build_special_firmware.yml)

## The special firmware files are [here](https://github.com/Jason2866/Tasmota-specials/tree/firmware/firmware). [Source code](https://github.com/Jason2866/Tasmota-build)<br>
Official ✨ Tasmota ✨ firmware files are [here](https://github.com/arendst/Tasmota-firmware)

## For easy flashing Tasmota use:
- [Tasmota WebInstaller](https://tasmota.github.io/install/) Only Chrome or Edge Browser needed!
- [ESP_Flasher](https://github.com/Jason2866/ESP_Flasher/releases)

## Build variants:
-  tasmota32c2 - Support for ESP32-C2 2M/4M
 - tasmota32c3 - Support for ESP32-C3 2M no OTA variant (tasmota32c3_2M)
 - tasmota32(c3/s3)-bluetooth - Support for BLE
 - tasmota-battery - extremely cut down build for battery powered Tuya sensors
 - tasmota32(c3/s2/s3/c6)-teleinfo - (ESP32 only) for Teleinfo French metering system, MQTT and TLS included 
 - tasmota32-zigbeebridge - ESP32 based [ZigbeeBridge](https://templates.blakadder.com/ewelink_ZB-GW03.html)
 - tasmota-zigbee - Zigbee for TI based chips (Esp8266 and ESP32)
 - tasmota-minicustom - even smaller minimal build, **only for Updates (warning MQTT only! No Webserver)**
 - tasmota-gps - GPS driver enabled
 - tasmota-mega - big binary, almost every sensor included, OTA possible only with minimal
 - tasmota-allsensors - guess whats in ;-)
 - tasmota-platinum - IT...IS...HUGE!!! nearly everything is enabled (only for devices with >=4Mb flash)
 - tasmota-titanium - as platinum with scripting enabled
 - tasmota-rangeextender - Experimental build where Tasmota acts as AP range extender
 - tasmota-scripting - all scripting features instead of rules + Smart Meter Interface enabled
 - tasmota-thermostat - Thermostat driver and temperature sensors (ESP32 has Bluetooth included)
 - tasmota32solo1-thermostat - ESP32 Single Core Thermostat driver and Bluetooth temperature sensors (used on Shelly Plus 1PM for example)
 - tasmota-teleinfo - For Teleinfo French metering system, MQTT enabled but No TLS due to lack of ressources
 - tasmota-tls - MQTT TLS enabled
 - tasmota32(c3/s3)-mi - Support for MI BLE devices
