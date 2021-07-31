/*
  xsns_09_bmp.ino - BMP pressure, temperature, humidity and gas sensor support for Tasmota

  Copyright (C) 2021  Heiko Krupp and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_I2C
#ifdef USE_BMP
/*********************************************************************************************\
 * BMP085, BMP180, BMP280, BME280, BME680 - Pressure, Temperature, Humidity (BME280/BME680) and gas (BME680)
 *
 * Source: Heiko Krupp and Adafruit Industries
 *
 * I2C Address: 0x76 or 0x77
\*********************************************************************************************/

#define XSNS_09              9
#define XI2C_10              10  // See I2CDEVICES.md

#define BMP_ADDR1            0x76
#define BMP_ADDR2            0x77

#define BMP180_CHIPID        0x55
#define BMP280_CHIPID        0x58
#define BME280_CHIPID        0x60
#define BME680_CHIPID        0x61

#define BMP_REGISTER_CHIPID  0xD0

#define BMP_REGISTER_RESET   0xE0 // Register to reset to power on defaults (used for sleep)

#define BMP_CMND_RESET       0xB6 // I2C Parameter for RESET to put BMP into reset state

#define BMP_MAX_SENSORS      2

const char kBmpTypes[] PROGMEM = "BMP180|BMP280|BME280|BME680";

typedef struct {
  uint8_t bmp_address;    // I2C bus address
  char bmp_name[7];       // Sensor name - "BMPXXX"
  uint8_t bmp_type;
  uint8_t bmp_model;
#ifdef USE_BME680
  uint8_t bme680_state;
  float bmp_gas_resistance;
#endif  // USE_BME680
  float bmp_temperature;
  float bmp_pressure;
  float bmp_humidity;
} bmp_sensors_t;

uint8_t bmp_addresses[] = { BMP_ADDR1, BMP_ADDR2 };
uint8_t bmp_count = 0;
uint8_t bmp_once = 1;

bmp_sensors_t *bmp_sensors = nullptr;

/*********************************************************************************************\
 * BMP085 and BME180
\*********************************************************************************************/

#define BMP180_REG_CONTROL   0xF4
#define BMP180_REG_RESULT    0xF6
#define BMP180_TEMPERATURE   0x2E
#define BMP180_PRESSURE3     0xF4 // Max. oversampling -> OSS = 3

#define BMP180_AC1           0xAA
#define BMP180_AC2           0xAC
#define BMP180_AC3           0xAE
#define BMP180_AC4           0xB0
#define BMP180_AC5           0xB2
#define BMP180_AC6           0xB4
#define BMP180_VB1           0xB6
#define BMP180_VB2           0xB8
#define BMP180_MB            0xBA
#define BMP180_MC            0xBC
#define BMP180_MD            0xBE

#define BMP180_OSS           3

typedef struct {
  int16_t  cal_ac1;
  int16_t  cal_ac2;
  int16_t  cal_ac3;
  int16_t  cal_b1;
  int16_t  cal_b2;
  int16_t  cal_mc;
  int16_t  cal_md;
  uint16_t cal_ac4;
  uint16_t cal_ac5;
  uint16_t cal_ac6;
} bmp180_cal_data_t;

bmp180_cal_data_t *bmp180_cal_data = nullptr;

bool Bmp180Calibration(uint8_t bmp_idx)
{
  if (!bmp180_cal_data) {
    bmp180_cal_data = (bmp180_cal_data_t*)malloc(BMP_MAX_SENSORS * sizeof(bmp180_cal_data_t));
  }
  if (!bmp180_cal_data) { return false; }

  bmp180_cal_data[bmp_idx].cal_ac1 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC1);
  bmp180_cal_data[bmp_idx].cal_ac2 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC2);
  bmp180_cal_data[bmp_idx].cal_ac3 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC3);
  bmp180_cal_data[bmp_idx].cal_ac4 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC4);
  bmp180_cal_data[bmp_idx].cal_ac5 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC5);
  bmp180_cal_data[bmp_idx].cal_ac6 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC6);
  bmp180_cal_data[bmp_idx].cal_b1  = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_VB1);
  bmp180_cal_data[bmp_idx].cal_b2  = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_VB2);
  bmp180_cal_data[bmp_idx].cal_mc  = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_MC);
  bmp180_cal_data[bmp_idx].cal_md  = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_MD);

  // Check for Errors in calibration data. Value never is 0x0000 or 0xFFFF
  if (!bmp180_cal_data[bmp_idx].cal_ac1 |
      !bmp180_cal_data[bmp_idx].cal_ac2 |
      !bmp180_cal_data[bmp_idx].cal_ac3 |
      !bmp180_cal_data[bmp_idx].cal_ac4 |
      !bmp180_cal_data[bmp_idx].cal_ac5 |
      !bmp180_cal_data[bmp_idx].cal_ac6 |
      !bmp180_cal_data[bmp_idx].cal_b1 |
      !bmp180_cal_data[bmp_idx].cal_b2 |
      !bmp180_cal_data[bmp_idx].cal_mc |
      !bmp180_cal_data[bmp_idx].cal_md) {
    return false;
  }

  if ((bmp180_cal_data[bmp_idx].cal_ac1 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac2 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac3 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac4 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac5 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac6 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_b1 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_b2 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_mc == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_md == (int16_t)0xFFFF)) {
    return false;
  }
  return true;
}

void Bmp180Read(uint8_t bmp_idx)
{
  if (!bmp180_cal_data) { return; }

  I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BMP180_REG_CONTROL, BMP180_TEMPERATURE);
  delay(5); // 5ms conversion time
  int ut = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_REG_RESULT);
  int32_t xt1 = (ut - (int32_t)bmp180_cal_data[bmp_idx].cal_ac6) * ((int32_t)bmp180_cal_data[bmp_idx].cal_ac5) >> 15;
  int32_t xt2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_mc << 11) / (xt1 + (int32_t)bmp180_cal_data[bmp_idx].cal_md);
  int32_t bmp180_b5 = xt1 + xt2;
  bmp_sensors[bmp_idx].bmp_temperature = ((bmp180_b5 + 8) >> 4) / 10.0;

  I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BMP180_REG_CONTROL, BMP180_PRESSURE3); // Highest resolution
  delay(2 + (4 << BMP180_OSS));                                 // 26ms conversion time at ultra high resolution
  uint32_t up = I2cRead24(bmp_sensors[bmp_idx].bmp_address, BMP180_REG_RESULT);
  up >>= (8 - BMP180_OSS);

  int32_t b6 = bmp180_b5 - 4000;
  int32_t x1 = ((int32_t)bmp180_cal_data[bmp_idx].cal_b2 * ((b6 * b6) >> 12)) >> 11;
  int32_t x2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_ac2 * b6) >> 11;
  int32_t x3 = x1 + x2;
  int32_t b3 = ((((int32_t)bmp180_cal_data[bmp_idx].cal_ac1 * 4 + x3) << BMP180_OSS) + 2) >> 2;

  x1 = ((int32_t)bmp180_cal_data[bmp_idx].cal_ac3 * b6) >> 13;
  x2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  uint32_t b4 = ((uint32_t)bmp180_cal_data[bmp_idx].cal_ac4 * (uint32_t)(x3 + 32768)) >> 15;
  uint32_t b7 = ((uint32_t)up - b3) * (uint32_t)(50000UL >> BMP180_OSS);

  int32_t p;
  if (b7 < 0x80000000) {
    p = (b7 * 2) / b4;
  }
  else {
    p = (b7 / b4) * 2;
  }
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  p += ((x1 + x2 + (int32_t)3791) >> 4);
  bmp_sensors[bmp_idx].bmp_pressure = (float)p / 100.0;  // convert to mbar
}

/*********************************************************************************************\
 * BMP280 and BME280
 *
 * Programmer : BMP280/BME280 Datasheet and Adafruit with changes by Theo Arends
\*********************************************************************************************/

#define BME280_REGISTER_CONTROLHUMID  0xF2
#define BME280_REGISTER_CONTROL       0xF4
#define BME280_REGISTER_CONFIG        0xF5
#define BME280_REGISTER_PRESSUREDATA  0xF7
#define BME280_REGISTER_TEMPDATA      0xFA
#define BME280_REGISTER_HUMIDDATA     0xFD

#define BME280_REGISTER_DIG_T1        0x88
#define BME280_REGISTER_DIG_T2        0x8A
#define BME280_REGISTER_DIG_T3        0x8C
#define BME280_REGISTER_DIG_P1        0x8E
#define BME280_REGISTER_DIG_P2        0x90
#define BME280_REGISTER_DIG_P3        0x92
#define BME280_REGISTER_DIG_P4        0x94
#define BME280_REGISTER_DIG_P5        0x96
#define BME280_REGISTER_DIG_P6        0x98
#define BME280_REGISTER_DIG_P7        0x9A
#define BME280_REGISTER_DIG_P8        0x9C
#define BME280_REGISTER_DIG_P9        0x9E
#define BME280_REGISTER_DIG_H1        0xA1
#define BME280_REGISTER_DIG_H2        0xE1
#define BME280_REGISTER_DIG_H3        0xE3
#define BME280_REGISTER_DIG_H4        0xE4
#define BME280_REGISTER_DIG_H5        0xE5
#define BME280_REGISTER_DIG_H6        0xE7

typedef struct {
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;
  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;
  int16_t  dig_P6;
  int16_t  dig_P7;
  int16_t  dig_P8;
  int16_t  dig_P9;
  int16_t  dig_H2;
  int16_t  dig_H4;
  int16_t  dig_H5;
  uint8_t  dig_H1;
  uint8_t  dig_H3;
  int8_t   dig_H6;
} Bme280CalibrationData_t;

Bme280CalibrationData_t *Bme280CalibrationData = nullptr;

bool Bmx280Calibrate(uint8_t bmp_idx)
{
  //  if (I2cRead8(bmp_address, BMP_REGISTER_CHIPID) != BME280_CHIPID) return false;

  if (!Bme280CalibrationData) {
    Bme280CalibrationData = (Bme280CalibrationData_t*)malloc(BMP_MAX_SENSORS * sizeof(Bme280CalibrationData_t));
  }
  if (!Bme280CalibrationData) { return false; }

  Bme280CalibrationData[bmp_idx].dig_T1 = I2cRead16LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_T1);
  Bme280CalibrationData[bmp_idx].dig_T2 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_T2);
  Bme280CalibrationData[bmp_idx].dig_T3 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_T3);
  Bme280CalibrationData[bmp_idx].dig_P1 = I2cRead16LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P1);
  Bme280CalibrationData[bmp_idx].dig_P2 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P2);
  Bme280CalibrationData[bmp_idx].dig_P3 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P3);
  Bme280CalibrationData[bmp_idx].dig_P4 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P4);
  Bme280CalibrationData[bmp_idx].dig_P5 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P5);
  Bme280CalibrationData[bmp_idx].dig_P6 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P6);
  Bme280CalibrationData[bmp_idx].dig_P7 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P7);
  Bme280CalibrationData[bmp_idx].dig_P8 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P8);
  Bme280CalibrationData[bmp_idx].dig_P9 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P9);
  if (BME280_CHIPID == bmp_sensors[bmp_idx].bmp_type) {  // #1051
    Bme280CalibrationData[bmp_idx].dig_H1 = I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H1);
    Bme280CalibrationData[bmp_idx].dig_H2 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H2);
    Bme280CalibrationData[bmp_idx].dig_H3 = I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H3);
    Bme280CalibrationData[bmp_idx].dig_H4 = (I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H4) << 4) | (I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H4 + 1) & 0xF);
    Bme280CalibrationData[bmp_idx].dig_H5 = (I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H5 + 1) << 4) | (I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H5) >> 4);
    Bme280CalibrationData[bmp_idx].dig_H6 = (int8_t)I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H6);
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONTROL, 0x00);      // sleep mode since writes to config can be ignored in normal mode (Datasheet 5.4.5/6 page 27)
    // Set before CONTROL_meas (DS 5.4.3)
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONTROLHUMID, 0x01); // 1x oversampling
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONFIG, 0xA0);       // 1sec standby between measurements (to limit self heating), IIR filter off
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONTROL, 0x27);      // 1x oversampling, normal mode
  } else {
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONTROL, 0xB7);      // 16x oversampling, normal mode (Adafruit)
  }

  return true;
}

void Bme280Read(uint8_t bmp_idx)
{
  if (!Bme280CalibrationData) { return; }

  int32_t adc_T = I2cRead24(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_TEMPDATA);
  adc_T >>= 4;

  int32_t vart1 = ((((adc_T >> 3) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1 << 1))) * ((int32_t)Bme280CalibrationData[bmp_idx].dig_T2)) >> 11;
  int32_t vart2 = (((((adc_T >> 4) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1)) * ((adc_T >> 4) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1))) >> 12) *
    ((int32_t)Bme280CalibrationData[bmp_idx].dig_T3)) >> 14;
  int32_t t_fine = vart1 + vart2;
  float T = (t_fine * 5 + 128) >> 8;
  bmp_sensors[bmp_idx].bmp_temperature = T / 100.0;

  int32_t adc_P = I2cRead24(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  int64_t var1 = ((int64_t)t_fine) - 128000;
  int64_t var2 = var1 * var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P6;
  var2 = var2 + ((var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P5) << 17);
  var2 = var2 + (((int64_t)Bme280CalibrationData[bmp_idx].dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P3) >> 8) + ((var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)Bme280CalibrationData[bmp_idx].dig_P1) >> 33;
  if (0 == var1) {
    return; // avoid exception caused by division by zero
  }
  int64_t p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)Bme280CalibrationData[bmp_idx].dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)Bme280CalibrationData[bmp_idx].dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)Bme280CalibrationData[bmp_idx].dig_P7) << 4);
  bmp_sensors[bmp_idx].bmp_pressure = (float)p / 25600.0;

  if (BMP280_CHIPID == bmp_sensors[bmp_idx].bmp_type) { return; }

  int32_t adc_H = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_HUMIDDATA);

  int32_t v_x1_u32r = (t_fine - ((int32_t)76800));
  v_x1_u32r = (((((adc_H << 14) - (((int32_t)Bme280CalibrationData[bmp_idx].dig_H4) << 20) -
    (((int32_t)Bme280CalibrationData[bmp_idx].dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
    (((((((v_x1_u32r * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H6)) >> 10) *
    (((v_x1_u32r * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
    ((int32_t)2097152)) * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
    ((int32_t)Bme280CalibrationData[bmp_idx].dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  float h = (v_x1_u32r >> 12);
  bmp_sensors[bmp_idx].bmp_humidity = h / 1024.0;
}

#ifdef USE_BME680
/*********************************************************************************************\
 * BME680 support by Bosch https://github.com/BoschSensortec/BME680_driver
\*********************************************************************************************/

#include <bme680.h>

struct bme680_dev *gas_sensor = nullptr;

static void BmeDelayMs(uint32_t ms)
{
  delay(ms);
}

bool Bme680Init(uint8_t bmp_idx)
{
  if (!gas_sensor) {
    gas_sensor = (bme680_dev*)malloc(BMP_MAX_SENSORS * sizeof(bme680_dev));
  }
  if (!gas_sensor) { return false; }

  gas_sensor[bmp_idx].dev_id = bmp_sensors[bmp_idx].bmp_address;
  gas_sensor[bmp_idx].intf = BME680_I2C_INTF;
  gas_sensor[bmp_idx].read = &I2cReadBuffer;
  gas_sensor[bmp_idx].write = &I2cWriteBuffer;
  gas_sensor[bmp_idx].delay_ms = BmeDelayMs;
  /* amb_temp can be set to 25 prior to configuring the gas sensor
   * or by performing a few temperature readings without operating the gas sensor.
   */
  gas_sensor[bmp_idx].amb_temp = 25;

  int8_t rslt = BME680_OK;
  rslt = bme680_init(&gas_sensor[bmp_idx]);
  if (rslt != BME680_OK) { return false; }

  /* Set the temperature, pressure and humidity settings */
  gas_sensor[bmp_idx].tph_sett.os_hum = BME680_OS_2X;
  gas_sensor[bmp_idx].tph_sett.os_pres = BME680_OS_4X;
  gas_sensor[bmp_idx].tph_sett.os_temp = BME680_OS_8X;
  gas_sensor[bmp_idx].tph_sett.filter = BME680_FILTER_SIZE_3;

  /* Set the remaining gas sensor settings and link the heating profile */
  gas_sensor[bmp_idx].gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
  /* Create a ramp heat waveform in 3 steps */
  gas_sensor[bmp_idx].gas_sett.heatr_temp = 320; /* degree Celsius */
  gas_sensor[bmp_idx].gas_sett.heatr_dur = 150; /* milliseconds */

  /* Select the power mode */
  /* Must be set before writing the sensor configuration */
  gas_sensor[bmp_idx].power_mode = BME680_FORCED_MODE;

  /* Set the required sensor settings needed */
  uint8_t set_required_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

  /* Set the desired sensor configuration */
  rslt = bme680_set_sensor_settings(set_required_settings,&gas_sensor[bmp_idx]);
  if (rslt != BME680_OK) { return false; }

  bmp_sensors[bmp_idx].bme680_state = 0;

  return true;
}

void Bme680Read(uint8_t bmp_idx)
{
  if (!gas_sensor) { return; }

  int8_t rslt = BME680_OK;

  if (BME680_CHIPID == bmp_sensors[bmp_idx].bmp_type) {
    if (0 == bmp_sensors[bmp_idx].bme680_state) {
      /* Trigger the next measurement if you would like to read data out continuously */
      rslt = bme680_set_sensor_mode(&gas_sensor[bmp_idx]);
      if (rslt != BME680_OK) { return; }

      /* Get the total measurement duration so as to sleep or wait till the
       * measurement is complete */
//      uint16_t meas_period;
//      bme680_get_profile_dur(&meas_period, &gas_sensor[bmp_idx]);
//      delay(meas_period); /* Delay till the measurement is ready */  // 183 mSec - we'll wait a second

      bmp_sensors[bmp_idx].bme680_state = 1;
    } else {
      bmp_sensors[bmp_idx].bme680_state = 0;

      struct bme680_field_data data;
      rslt = bme680_get_sensor_data(&data, &gas_sensor[bmp_idx]);
      if (rslt != BME680_OK) { return; }

      bmp_sensors[bmp_idx].bmp_temperature = data.temperature / 100.0;
      bmp_sensors[bmp_idx].bmp_humidity = data.humidity / 1000.0;
      bmp_sensors[bmp_idx].bmp_pressure = data.pressure / 100.0;
      /* Avoid using measurements from an unstable heating setup */
      if (data.status & BME680_GASM_VALID_MSK) {
        bmp_sensors[bmp_idx].bmp_gas_resistance = data.gas_resistance / 1000.0;
      } else {
        bmp_sensors[bmp_idx].bmp_gas_resistance = 0;
      }
    }
  }
  return;
}

#endif  // USE_BME680

/********************************************************************************************/

void BmpDetect(void)
{
  int bmp_sensor_size = BMP_MAX_SENSORS * sizeof(bmp_sensors_t);
  if (!bmp_sensors) {
    bmp_sensors = (bmp_sensors_t*)malloc(bmp_sensor_size);
  }
  if (!bmp_sensors) { return; }
  memset(bmp_sensors, 0, bmp_sensor_size);  // Init defaults to 0

  for (uint32_t i = 0; i < BMP_MAX_SENSORS; i++) {
    if (I2cActive(bmp_addresses[i])) { continue; }
    uint8_t bmp_type = I2cRead8(bmp_addresses[i], BMP_REGISTER_CHIPID);
    if (bmp_type) {
      bmp_sensors[bmp_count].bmp_address = bmp_addresses[i];
      bmp_sensors[bmp_count].bmp_type = bmp_type;
      bmp_sensors[bmp_count].bmp_model = 0;

      bool success = false;
      switch (bmp_type) {
        case BMP180_CHIPID:
          success = Bmp180Calibration(bmp_count);
          break;
        case BME280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 2
        case BMP280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 1
          success = Bmx280Calibrate(bmp_count);
          break;
#ifdef USE_BME680
        case BME680_CHIPID:
          bmp_sensors[bmp_count].bmp_model = 3;  // 3
          success = Bme680Init(bmp_count);
          break;
#endif  // USE_BME680
      }
      if (success) {
        GetTextIndexed(bmp_sensors[bmp_count].bmp_name, sizeof(bmp_sensors[bmp_count].bmp_name), bmp_sensors[bmp_count].bmp_model, kBmpTypes);
        I2cSetActiveFound(bmp_sensors[bmp_count].bmp_address, bmp_sensors[bmp_count].bmp_name);
        bmp_count++;
      }
    }
  }
}

void BmpRead(void)
{
  for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
    switch (bmp_sensors[bmp_idx].bmp_type) {
      case BMP180_CHIPID:
        Bmp180Read(bmp_idx);
        break;
      case BMP280_CHIPID:
      case BME280_CHIPID:
        Bme280Read(bmp_idx);
        break;
#ifdef USE_BME680
      case BME680_CHIPID:
        Bme680Read(bmp_idx);
        break;
#endif  // USE_BME680
    }
  }
}

void BmpShow(bool json)
{
  for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
    if (bmp_sensors[bmp_idx].bmp_type) {
      float bmp_sealevel = ConvertPressureForSeaLevel(bmp_sensors[bmp_idx].bmp_pressure);
      float bmp_temperature = ConvertTemp(bmp_sensors[bmp_idx].bmp_temperature);
      float bmp_pressure = ConvertPressure(bmp_sensors[bmp_idx].bmp_pressure);

      char name[10];
      strlcpy(name, bmp_sensors[bmp_idx].bmp_name, sizeof(name));
      if (bmp_count > 1) {
        snprintf_P(name, sizeof(name), PSTR("%s%c%02X"), name, IndexSeparator(), bmp_sensors[bmp_idx].bmp_address);  // BMXXXX-XX
      }

      char pressure[33];
      dtostrfd(bmp_pressure, Settings->flag2.pressure_resolution, pressure);
      char sea_pressure[33];
      dtostrfd(bmp_sealevel, Settings->flag2.pressure_resolution, sea_pressure);

      float bmp_humidity = ConvertHumidity(bmp_sensors[bmp_idx].bmp_humidity);
      char humidity[33];
      dtostrfd(bmp_humidity, Settings->flag2.humidity_resolution, humidity);
      float f_dewpoint = CalcTempHumToDew(bmp_temperature, bmp_humidity);
      char dewpoint[33];
      dtostrfd(f_dewpoint, Settings->flag2.temperature_resolution, dewpoint);
#ifdef USE_BME680
      char gas_resistance[33];
      dtostrfd(bmp_sensors[bmp_idx].bmp_gas_resistance, 2, gas_resistance);
#endif  // USE_BME680

      if (json) {
        char json_humidity[80];
        snprintf_P(json_humidity, sizeof(json_humidity), PSTR(",\"" D_JSON_HUMIDITY "\":%s,\"" D_JSON_DEWPOINT "\":%s"), humidity, dewpoint);
        char json_sealevel[40];
        snprintf_P(json_sealevel, sizeof(json_sealevel), PSTR(",\"" D_JSON_PRESSUREATSEALEVEL "\":%s"), sea_pressure);
#ifdef USE_BME680
        char json_gas[40];
        snprintf_P(json_gas, sizeof(json_gas), PSTR(",\"" D_JSON_GAS "\":%s"), gas_resistance);

        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f%s,\"" D_JSON_PRESSURE "\":%s%s%s}"),
          name,
          Settings->flag2.temperature_resolution, &bmp_temperature,
          (bmp_sensors[bmp_idx].bmp_model >= 2) ? json_humidity : "",
          pressure,
          (Settings->altitude != 0) ? json_sealevel : "",
          (bmp_sensors[bmp_idx].bmp_model >= 3) ? json_gas : "");
#else
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f%s,\"" D_JSON_PRESSURE "\":%s%s}"),
          name, Settings->flag2.temperature_resolution, &bmp_temperature, (bmp_sensors[bmp_idx].bmp_model >= 2) ? json_humidity : "", pressure, (Settings->altitude != 0) ? json_sealevel : "");
#endif  // USE_BME680

#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (0 == bmp_idx)) {  // We want the same first sensor to report to Domoticz in case a read is missed
          DomoticzTempHumPressureSensor(bmp_temperature, bmp_humidity, bmp_pressure);
#ifdef USE_BME680
          if (bmp_sensors[bmp_idx].bmp_model >= 3) { DomoticzSensor(DZ_AIRQUALITY, (uint32_t)bmp_sensors[bmp_idx].bmp_gas_resistance); }
#endif  // USE_BME680
        }
#endif  // USE_DOMOTICZ

#ifdef USE_KNX
        if (0 == TasmotaGlobal.tele_period) {
          KnxSensor(KNX_TEMPERATURE, bmp_temperature);
          KnxSensor(KNX_HUMIDITY, bmp_humidity);
        }
#endif  // USE_KNX

#ifdef USE_WEBSERVER
      } else {
        WSContentSend_Temp(name, bmp_temperature);
        if (bmp_sensors[bmp_idx].bmp_model >= 2) {
          WSContentSend_PD(HTTP_SNS_HUM, name, humidity);
          WSContentSend_PD(HTTP_SNS_DEW, name, dewpoint, TempUnit());
        }
        WSContentSend_PD(HTTP_SNS_PRESSURE, name, pressure, PressureUnit().c_str());
        if (Settings->altitude != 0) {
          WSContentSend_PD(HTTP_SNS_SEAPRESSURE, name, sea_pressure, PressureUnit().c_str());
        }
#ifdef USE_BME680
        if (bmp_sensors[bmp_idx].bmp_model >= 3) {
          WSContentSend_PD(PSTR("{s}%s " D_GAS "{m}%s " D_UNIT_KILOOHM "{e}"), name, gas_resistance);
        }
#endif  // USE_BME680

#endif  // USE_WEBSERVER
      }
    }
  }
}

#ifdef USE_DEEPSLEEP

void BMP_EnterSleep(void)
{
  if (DeepSleepEnabled()) {
    for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
      switch (bmp_sensors[bmp_idx].bmp_type) {
        case BMP180_CHIPID:
        case BMP280_CHIPID:
        case BME280_CHIPID:
          I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BMP_REGISTER_RESET, BMP_CMND_RESET);
          break;
        default:
          break;
      }
    }
  }
}

#endif // USE_DEEPSLEEP

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns09(uint8_t function)
{
  if (!I2cEnabled(XI2C_10)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    BmpDetect();
  }
  else if (bmp_count) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        BmpRead();
        break;
      case FUNC_JSON_APPEND:
        BmpShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        BmpShow(0);
        break;
#endif  // USE_WEBSERVER
#ifdef USE_DEEPSLEEP
      case FUNC_SAVE_BEFORE_RESTART:
        BMP_EnterSleep();
        break;
#endif // USE_DEEPSLEEP
    }
  }
  return result;
}

#endif  // USE_BMP
#endif  // USE_I2C
