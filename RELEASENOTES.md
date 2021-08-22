<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information

**This version removes support for direct migration from versions before v8.1.0 (Doris)**

See [migration path](https://tasmota.github.io/docs/Upgrading#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.7.1** (http://ota.tasmota.com/tasmota/release-6.7.1/)
5. Migrate to **Tasmota 7.2.0** (http://ota.tasmota.com/tasmota/release-7.2.0/)

--- Major change in parameter storage layout ---

6. Migrate to **Tasmota 8.5.1** (http://ota.tasmota.com/tasmota/release-8.5.1/)

--- Major change in internal GPIO function representation ---

7. Migrate to **Tasmota 9.1** (http://ota.tasmota.com/tasmota/release-9.1.0/)

While fallback or downgrading is common practice it was never supported due to Settings additions or changes in newer releases. Starting with release **v9.1.0 Imogen** the internal GPIO function representation has changed in such a way that fallback is only possible to the latest GPIO configuration before installing **v9.1.0**.

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.7.4.9** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

Support of Core versions before 2.7.1 has been removed.

## Support of TLS

In addition to TLS using fingerprints now also user supplied CA certs, AWS IoT and Azure IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Initial installation

Easy initial installation of Tasmota can be performed using the [Tasmota WebInstaller](https://arendst.github.io/Tasmota-firmware/).

## Provided Binary Downloads

### ESP8266 or ESP8285 based
The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.4.9**.

- **tasmota.bin** = The Tasmota version with most drivers for 1M+ flash. **RECOMMENDED RELEASE BINARY**
- **tasmota-AF.bin** to **tasmota-VN.bin** = The Tasmota version in different languages for 1M+ flash.
- **tasmota-lite.bin** = The Lite version without most drivers and sensors for 1M+ flash.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support for 1M+ flash.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors for 1M+ flash.
- **tasmota-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 1M+ flash.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support for 1M+ flash.
- **tasmota-zbbridge.bin** = The dedicated Sonoff Zigbee Bridge version for 2M+ flash.
- **tasmota-zigbee.bin** = The dedicated cc25xx Zigbee Bridge version for 4M+ flash.

Above binaries are also available as gzipped version allowing faster uploads.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- http://ota.tasmota.com/tasmota/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota/release-9.5.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32 based
The following binary downloads have been compiled with ESP32/Arduino library core version **1.0.7.3**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32_8M.bin** = The Tasmota version with most drivers including additional sensors and KNX for 8M+ flash.
- **tasmota32_16M.bin** = The Tasmota version with most drivers including additional sensors and KNX for 16M+ flash.
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32-AF.bin** to **tasmota32-VN.bin** = The Tasmota version in different languages for 4M+ flash.
- **tasmota32-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 4M+ flash.
- **tasmota32-display.bin** = The Display version without Energy Monitoring but adds display support for 4M+ flash.
- **tasmota32-webcam.bin** = The Webcam version adds webcam support for 4M+ flash.
- **tasmota32-bluetooth.bin** = The Bluetooth version adds BLE support for 4M+ flash.
- **tasmota32-odroidgo.bin** = The Odroid-Go version is specifically tailored to Odroid_go hardware for 8M+ flash.
- **tasmota32-core2.bin** = The Core2 version is specifically tailored to M5Stack Core2 hardware for 8M+ flash.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- http://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota32/release-9.5.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v9.5.0.7
### Added
- Release of [Tasmota WebInstaller](https://arendst.github.io/Tasmota-firmware/)
- Command ``SetOption127 1`` to force Wi-Fi in no-sleep mode even if ``Sleep 0`` is not enabled
- Command ``SetOption128 0|1`` web referer check disabling HTTP API commands if set to 0. Default set to 1 for backward compatibility [#12828](https://github.com/arendst/Tasmota/issues/12828)
- Command ``SetSensor1..127 0|1`` to globally disable individual sensor driver
- Neopool commands ``NPPHRes``, ``NPCLRes`` and ``NPIonRes`` [#12813](https://github.com/arendst/Tasmota/issues/12813)
- Support for second DNS server
- Support for (Yeelight) Mi Desk Pro using binary tasmota32solo1.bin
- Initial support for influxdb using ``#define USE_INFLUXDB`` and several ``Ifx`` commands
- Initial support for Tasmota Mesh (TasMesh) providing node/broker communication using ESP-NOW [#11939](https://github.com/arendst/Tasmota/issues/11939)
- Berry ESP32 partition manager [#12465](https://github.com/arendst/Tasmota/issues/12465)
- Berry ESP32 support for I2S audio mp3 playback
- Support for AM2320 Temperature and Humidity Sensor by Lars Wessels [#12485](https://github.com/arendst/Tasmota/issues/12485)
- Rule event support as JSON payload [#12496](https://github.com/arendst/Tasmota/issues/12496)
- MQTT minimum password length restriction in GUI [#12553](https://github.com/arendst/Tasmota/issues/12553)
- Support for Technoline WS2300-15 Anemometer [#12573](https://github.com/arendst/Tasmota/issues/12573)
- Support for Telaire T6700 Series CO2 sensor by Alexander Savchenko [#12618](https://github.com/arendst/Tasmota/issues/12618)
- Support for CAN bus and Freedom Won Battery Management System by Marius Bezuidenhout [#12651](https://github.com/arendst/Tasmota/issues/12651)
- Optional IP filter to command ``TCPStart`` [#12806](https://github.com/arendst/Tasmota/issues/12806)
- Inital support for Wi-Fi extender [#12784](https://github.com/arendst/Tasmota/issues/12784)
- Support for IEM3155 Wattmeter [#12940](https://github.com/arendst/Tasmota/issues/12940)

### Changed
- Move firmware binaries to https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- ESP32 core library from v1.0.6 to v1.0.7.3
- IRremoteESP8266 library from v2.7.18 to v2.7.19
- NeoPixelBus library from v2.6.3 to v2.6.7
- Message ``Upload buffer miscompare`` into ``Not enough space``
- ESP32 Ethernet Phy Type information to IDF v3+
- Speed up initial GUI console refresh
- Enable UFILESYS, GUI_TRASH_FILE and GUI_EDIT_FILE for any device compiled with more than 1M flash size
- ESP32 internal sensor driver id moved from 87 to 127
- Extended supported sensor driver range to 128
- Disable PSRAM on unsupported hardware
- ESP32 remove GPIO initialization to INPUT from not used GPIOs to allow JTAG support
- Relax NTP poll if no ntpserver can be resolved by DNS
- Make Sonoff L1 MusicSync persistent [#12008](https://github.com/arendst/Tasmota/issues/12008)
- Simplified configuration for ir-full and removal of tasmota-ircustom [#12428](https://github.com/arendst/Tasmota/issues/12428)
- Refactor platformio [#12442](https://github.com/arendst/Tasmota/issues/12442)
- Allow buttons to work in AP normal mode [#12518](https://github.com/arendst/Tasmota/issues/12518)
- Enable Ping and rule features for any device compiled with more than 1M flash size [#12539](https://github.com/arendst/Tasmota/issues/12539)
- Replace spaces by hyphens in final hostname [#12710](https://github.com/arendst/Tasmota/issues/12710)
- Default disable CORS for enhanced security and provide user compile option ``#define USE_CORS`` [#12827](https://github.com/arendst/Tasmota/issues/12827)
- Prometheus: All metrics are prefixed with ``tasmota_`` [#12842](https://github.com/arendst/Tasmota/issues/12842)
    Memory metrics have been cleaned up to work consistently between ESP8266 and ESP32
    The device name is reported as an info metric

### Fixed
- ESP32 core v2.0.0 setting hostname
- ESP32-C3 settings layout for configuration backup and restore
- ESP32-Solo OTA upgrade
- DDS238-2 wrong reactive power value [#12283](https://github.com/arendst/Tasmota/issues/12283)
- ESP32 Webcam add boundary marker before sending mjpeg image [#12376](https://github.com/arendst/Tasmota/issues/12376)
- NO VALID JSON regression from may 4th [#12440](https://github.com/arendst/Tasmota/issues/12440)
- Telegram response decoding stopped working after 20210621 and exception on long result message [#12451](https://github.com/arendst/Tasmota/issues/12451)
- Neopool compile error on DEBUG_TASMOTA_SENSOR [#12464](https://github.com/arendst/Tasmota/issues/12464)
- Berry button handlers and error messages [#12521](https://github.com/arendst/Tasmota/issues/12521)
- Scripter and Display MQTT errors due to MQTT_DATA move to String [#12525](https://github.com/arendst/Tasmota/issues/12525)
- Scripter moving average and sml input validation [#12541](https://github.com/arendst/Tasmota/issues/12541)
- Zigbee Hue angle encoding [#12545](https://github.com/arendst/Tasmota/issues/12545)
- Exception 28 when unable to send MQTT message and a topic name without a slash '/' [#12555](https://github.com/arendst/Tasmota/issues/12555)
- Wi-Fi initial setup workaround for 11n only routers [#12566](https://github.com/arendst/Tasmota/issues/12566)
- ESP32 do not use chip temperature sensor as global temperature if external temperature sensor is used [#12630](https://github.com/arendst/Tasmota/issues/12630)
- Discovery fails when using ``%hostname%`` in a topic [#12710](https://github.com/arendst/Tasmota/issues/12710)
- ESP32 buzzer in PWM mode exception (#12717)[#12717](https://github.com/arendst/Tasmota/issues/12717)
- Neopool communication error [#12813](https://github.com/arendst/Tasmota/issues/12813)
- WDT reset on shutters with stepper motors during deceleration [#12849](https://github.com/arendst/Tasmota/issues/12849)
- Negative power values for ADE7953 based devices like Shelly EM [#12874](https://github.com/arendst/Tasmota/issues/12874)
