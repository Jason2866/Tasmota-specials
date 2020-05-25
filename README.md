# These [Tasmota](https://github.com/arendst/Tasmota) binaries are not official stable releases and are built hourly from the latest development branch! 

## :warning: No support/warranty with these binaries! :warning:

## The firmware files are [here](https://github.com/Jason2866/Merge/tree/firmware/Tasmota_firmware). Additional needed [ESP32 files here](https://github.com/Jason2866/Merge/tree/master/ESP32_flash_files)

Command syntax for flashing Tasmota32 firmware on ESP32 via Esptool (replace COM Port Number!):<br>
```esptool.py --chip esp32 --port COM5 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dout --flash_freq 40m --flash_size detect 0x1000 bootloader_dout_40m.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 tasmota32.bin``` 

## Additional build variants:

- Scripting included instead of rules
- Tasmota Zigbee gateway build
- Tasmota Rules with all the extras enabled
- Tasmota Battery build for battery powered tuya sensors
- Tasmota Mega at the bleeding edge - Almost every sensor included
- Tasmota Platinum nearly ALL enabled IT.IS.HUGE! <br>
*Only serial flash is possible, there's no OTA possible!*
