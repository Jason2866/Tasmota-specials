## These special Tasmota binaries are not official stable releases

## :warning: No support/warranty with these binaries! :warning:

[![Build_special_firmware](https://github.com/Jason2866/Tasmota-specials/actions/workflows/Build_special_firmware.yml/badge.svg)](https://github.com/Jason2866/Tasmota-specials/actions/workflows/Build_special_firmware.yml)

## The special firmware files are [here](https://github.com/Jason2866/Tasmota-specials/tree/firmware/firmware).<br>
Official ✨ Tasmota ✨ firmware files are [here](https://github.com/arendst/Tasmota-firmware)

## For easy flashing Tasmota use:
- [Tasmota WebInstaller](https://jason2866.github.io/Tasmota-specials/) Only Chrome or Edge Browser needed!
- [ESP_Flasher](https://github.com/Jason2866/ESP_Flasher/releases) (Windows and Mac executables, no ESP32-C3 support)

## Build variants:
 - tasmota32c3 - Support for ESP32-C3 2M no OTA variant (tasmota32c3_2M)
 - tasmota32c3-bluetooth - Support for BLE
 - tasmota32s2 - Beta support for ESP32-S2 chip
 - tasmota-battery - extremely cut down build for battery powered Tuya sensors
 - tasmota32-ethernet - (ESP32 only) for wired LAN, works only on devices with Ethernet hardware!
 - tasmota32-zigbeebridge - ESP32 based [ZigbeeBridge](https://templates.blakadder.com/ewelink_ZB-GW03.html)
 - tasmota-fullrules - Rules with all the features (expressions, If statements, MQTT subscription)
 - tasmota-minicustom - even smaller minimal build (warning: it has **NO** Webserver, you have to use MQTT for control)
 - tasmota-gps - GPS driver enabled
 - tasmota-mega - big binary, almost every sensor included, OTA possible only with minimal
 - tasmota-allsensors - guess whats in ;-)
 - tasmota-platinum - IT...IS...HUGE!!! nearly everything is enabled (only for devices with >=4Mb flash)
 - tasmota-rangeextender - Experimental build where Tasmota acts as AP range extender
 - tasmota-scripting - all scripting features instead of rules + Smart Meter Interface enabled
 - tasmota-thermostat - Thermostat, PID driver and temperature sensors
 - tasmota-tls - MQTT TLS enabled
 - tasmota32-udisplay - ESP32 only, uses Universal Display Driver
