/*
  xdrv_03_energy.ino - Energy sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifdef USE_ENERGY_SENSOR
/*********************************************************************************************\
 * Energy
\*********************************************************************************************/

#define XDRV_03                3
#define XSNS_03                3

//#define USE_ENERGY_MARGIN_DETECTION
//  #define USE_ENERGY_POWER_LIMIT

#define ENERGY_NONE            0
#define ENERGY_WATCHDOG        4        // Allow up to 4 seconds before deciding no valid data present
#define ENERGY_MAX_PHASES      3

#include <Ticker.h>

#define D_CMND_POWERCAL "PowerCal"
#define D_CMND_VOLTAGECAL "VoltageCal"
#define D_CMND_CURRENTCAL "CurrentCal"
#define D_CMND_FREQUENCYCAL "FrequencyCal"
#define D_CMND_TARIFF "Tariff"
#define D_CMND_MODULEADDRESS "ModuleAddress"

enum EnergyCommands {
  CMND_POWERCAL, CMND_VOLTAGECAL, CMND_CURRENTCAL, CMND_FREQUENCYCAL,
  CMND_POWERSET, CMND_VOLTAGESET, CMND_CURRENTSET, CMND_FREQUENCYSET, CMND_MODULEADDRESS, CMND_ENERGYCONFIG };

const char kEnergyCommands[] PROGMEM = "|"  // No prefix
  D_CMND_POWERCAL "|" D_CMND_VOLTAGECAL "|" D_CMND_CURRENTCAL "|" D_CMND_FREQUENCYCAL "|"
  D_CMND_POWERSET "|" D_CMND_VOLTAGESET "|" D_CMND_CURRENTSET "|" D_CMND_FREQUENCYSET "|" D_CMND_MODULEADDRESS "|" D_CMND_ENERGYCONFIG "|"
#ifdef USE_ENERGY_MARGIN_DETECTION
  D_CMND_POWERDELTA "|" D_CMND_POWERLOW "|" D_CMND_POWERHIGH "|" D_CMND_VOLTAGELOW "|" D_CMND_VOLTAGEHIGH "|" D_CMND_CURRENTLOW "|" D_CMND_CURRENTHIGH "|"
#ifdef USE_ENERGY_POWER_LIMIT
  D_CMND_MAXENERGY "|" D_CMND_MAXENERGYSTART "|"
  D_CMND_MAXPOWER "|" D_CMND_MAXPOWERHOLD "|" D_CMND_MAXPOWERWINDOW "|"
  D_CMND_SAFEPOWER "|" D_CMND_SAFEPOWERHOLD "|"  D_CMND_SAFEPOWERWINDOW "|"
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION
  D_CMND_ENERGYRESET "|" D_CMND_TARIFF;

void (* const EnergyCommand[])(void) PROGMEM = {
  &CmndPowerCal, &CmndVoltageCal, &CmndCurrentCal, &CmndFrequencyCal,
  &CmndPowerSet, &CmndVoltageSet, &CmndCurrentSet, &CmndFrequencySet, &CmndModuleAddress, &CmndEnergyConfig,
#ifdef USE_ENERGY_MARGIN_DETECTION
  &CmndPowerDelta, &CmndPowerLow, &CmndPowerHigh, &CmndVoltageLow, &CmndVoltageHigh, &CmndCurrentLow, &CmndCurrentHigh,
#ifdef USE_ENERGY_POWER_LIMIT
  &CmndMaxEnergy, &CmndMaxEnergyStart,
  &CmndMaxPower, &CmndMaxPowerHold, &CmndMaxPowerWindow,
  &CmndSafePower, &CmndSafePowerHold, &CmndSafePowerWindow,
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION
  &CmndEnergyReset, &CmndTariff};

const char kEnergyPhases[] PROGMEM = "|%s / %s|%s / %s / %s||[%s,%s]|[%s,%s,%s]";

struct ENERGY {
  float voltage[ENERGY_MAX_PHASES];             // 123.1 V
  float current[ENERGY_MAX_PHASES];             // 123.123 A
  float active_power[ENERGY_MAX_PHASES];        // 123.1 W
  float apparent_power[ENERGY_MAX_PHASES];      // 123.1 VA
  float reactive_power[ENERGY_MAX_PHASES];      // 123.1 VAr
  float power_factor[ENERGY_MAX_PHASES];        // 0.12
  float frequency[ENERGY_MAX_PHASES];           // 123.1 Hz
#if defined(SDM630_IMPORT) || defined(SDM72_IMPEXP)
  float import_active[ENERGY_MAX_PHASES];       // 123.123 kWh
#endif  // SDM630_IMPORT || SDM72_IMPEXP
  float export_active[ENERGY_MAX_PHASES];       // 123.123 kWh

  float start_energy;                           // 12345.12345 kWh total previous
  float daily;                                  // 123.123 kWh
  float total;                                  // 12345.12345 kWh total energy

  unsigned long kWhtoday_delta;                 // 1212312345 Wh 10^-5 (deca micro Watt hours) - Overflows to Energy.kWhtoday (HLW and CSE only)
  unsigned long kWhtoday_offset;                // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
  unsigned long kWhtoday;                       // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
  unsigned long period;                         // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily

  uint8_t fifth_second;
  uint8_t command_code;
  uint8_t data_valid[ENERGY_MAX_PHASES];

  uint8_t phase_count;                          // Number of phases active
  bool voltage_common;                          // Use single voltage
  bool frequency_common;                        // Use single frequency
  bool use_overtemp;                            // Use global temperature as overtemp trigger on internal energy monitor hardware
  bool kWhtoday_offset_init;

  bool voltage_available;                       // Enable if voltage is measured
  bool current_available;                       // Enable if current is measured

  bool type_dc;
  bool power_on;

#ifdef USE_ENERGY_MARGIN_DETECTION
  uint16_t power_history[ENERGY_MAX_PHASES][3];
  uint8_t power_steady_counter;                 // Allow for power on stabilization
  bool min_power_flag;
  bool max_power_flag;
  bool min_voltage_flag;
  bool max_voltage_flag;
  bool min_current_flag;
  bool max_current_flag;

#ifdef USE_ENERGY_POWER_LIMIT
  uint16_t mplh_counter;
  uint16_t mplw_counter;
  uint8_t mplr_counter;
  uint8_t max_energy_state;
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION
} Energy;

Ticker ticker_energy;

/********************************************************************************************/

char* EnergyFormatIndex(char* result, char* input, bool json, uint32_t index, bool single = false)
{
  char layout[16];
  GetTextIndexed(layout, sizeof(layout), (index -1) + (ENERGY_MAX_PHASES * json), kEnergyPhases);
  switch (index) {
    case 2:
      snprintf_P(result, FLOATSZ * ENERGY_MAX_PHASES, layout, input, input + FLOATSZ);  // Dirty
      break;
    case 3:
      snprintf_P(result, FLOATSZ * ENERGY_MAX_PHASES, layout, input, input + FLOATSZ, input + FLOATSZ + FLOATSZ);  // Even dirtier
      break;
    default:
      snprintf_P(result, FLOATSZ * ENERGY_MAX_PHASES, input);
  }
  return result;
}

char* EnergyFormat(char* result, char* input, bool json, bool single = false)
{
  uint8_t index = (single) ? 1 : Energy.phase_count;  // 1,2,3
  return EnergyFormatIndex(result, input, json, index, single);
}

/********************************************************************************************/

bool EnergyTariff1Active()  // Off-Peak hours
{
  uint8_t dst = 0;
  if (IsDst() && (Settings->tariff[0][1] != Settings->tariff[1][1])) {
    dst = 1;
  }
  if (Settings->tariff[0][dst] != Settings->tariff[1][dst]) {
    if (Settings->flag3.energy_weekend && ((RtcTime.day_of_week == 1) ||   // CMND_TARIFF
                                          (RtcTime.day_of_week == 7))) {
      return true;
    }
    uint32_t minutes = MinutesPastMidnight();
    if (Settings->tariff[0][dst] > Settings->tariff[1][dst]) {
      // {"Tariff":{"Off-Peak":{"STD":"22:00","DST":"23:00"},"Standard":{"STD":"06:00","DST":"07:00"},"Weekend":"OFF"}}
      return ((minutes >= Settings->tariff[0][dst]) || (minutes < Settings->tariff[1][dst]));
    } else {
      // {"Tariff":{"Off-Peak":{"STD":"00:29","DST":"01:29"},"Standard":{"STD":"07:29","DST":"08:29"},"Weekend":"OFF"}}
      return ((minutes >= Settings->tariff[0][dst]) && (minutes < Settings->tariff[1][dst]));
    }
  } else {
    return false;
  }
}

void EnergyUpdateToday(void)
{
  if (Energy.kWhtoday_delta > 1000) {
    unsigned long delta = Energy.kWhtoday_delta / 1000;
    Energy.kWhtoday_delta -= (delta * 1000);
    Energy.kWhtoday += delta;
  }

  RtcSettings.energy_kWhtoday = Energy.kWhtoday_offset + Energy.kWhtoday;
  Energy.daily = (float)(RtcSettings.energy_kWhtoday) / 100000;
  Energy.total = (float)(RtcSettings.energy_kWhtotal + RtcSettings.energy_kWhtoday) / 100000;

  if (RtcTime.valid){ // We calc the difference only if we have a valid RTC time.

    uint32_t energy_diff = (uint32_t)(Energy.total * 100000) - RtcSettings.energy_usage.last_usage_kWhtotal;
    RtcSettings.energy_usage.last_usage_kWhtotal = (uint32_t)(Energy.total * 100000);

    uint32_t return_diff = 0;
    if (!isnan(Energy.export_active[0])) {
//      return_diff = (uint32_t)(Energy.export_active * 100000) - RtcSettings.energy_usage.last_return_kWhtotal;
//      RtcSettings.energy_usage.last_return_kWhtotal = (uint32_t)(Energy.export_active * 100000);

      float export_active = 0.0;
      for (uint32_t i = 0; i < Energy.phase_count; i++) {
        if (!isnan(Energy.export_active[i])) {
          export_active += Energy.export_active[i];
        }
      }
      return_diff = (uint32_t)(export_active * 100000) - RtcSettings.energy_usage.last_return_kWhtotal;
      RtcSettings.energy_usage.last_return_kWhtotal = (uint32_t)(export_active * 100000);
    }

    if (EnergyTariff1Active()) {  // Tarrif1 = Off-Peak
      RtcSettings.energy_usage.usage1_kWhtotal += energy_diff;
      RtcSettings.energy_usage.return1_kWhtotal += return_diff;
    } else {
      RtcSettings.energy_usage.usage2_kWhtotal += energy_diff;
      RtcSettings.energy_usage.return2_kWhtotal += return_diff;
    }
  }
}

void EnergyUpdateTotal(float value, bool kwh)
{
//  AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Energy Total %4_f %sWh"), &value, (kwh) ? "k" : "");

  uint32_t multiplier = (kwh) ? 100000 : 100;  // kWh or Wh to deca milli Wh

  if (0 == Energy.start_energy || (value < Energy.start_energy)) {
    Energy.start_energy = value;  // Init after restart and handle roll-over if any
  }
  else if (value != Energy.start_energy) {
    Energy.kWhtoday = (unsigned long)((value - Energy.start_energy) * multiplier);
  }

  if ((Energy.total < (value - 0.01)) &&       // We subtract a little offset to avoid continuous updates
      Settings->flag3.hardware_energy_total) {  // SetOption72 - Enable hardware energy total counter as reference (#6561)
    RtcSettings.energy_kWhtotal = (unsigned long)((value * multiplier) - Energy.kWhtoday_offset - Energy.kWhtoday);
    Settings->energy_kWhtotal = RtcSettings.energy_kWhtotal;
    Energy.total = (float)(RtcSettings.energy_kWhtotal + Energy.kWhtoday_offset + Energy.kWhtoday) / 100000;
    Settings->energy_kWhtotal_time = (!Energy.kWhtoday_offset) ? LocalTime() : Midnight();
//    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Energy Total updated with hardware value"));
  }
  EnergyUpdateToday();
}

/*********************************************************************************************/

void Energy200ms(void)
{
  Energy.power_on = (TasmotaGlobal.power != 0) | Settings->flag.no_power_on_check;  // SetOption21 - Show voltage even if powered off

  Energy.fifth_second++;
  if (5 == Energy.fifth_second) {
    Energy.fifth_second = 0;

    XnrgCall(FUNC_ENERGY_EVERY_SECOND);

    if (RtcTime.valid) {

      if (!Energy.kWhtoday_offset_init && (RtcTime.day_of_year == Settings->energy_kWhdoy)) {
        Energy.kWhtoday_offset = Settings->energy_kWhtoday;
        Energy.kWhtoday_offset_init = true;
      }

      if (LocalTime() == Midnight()) {
        Settings->energy_kWhyesterday = RtcSettings.energy_kWhtoday;

        RtcSettings.energy_kWhtotal += RtcSettings.energy_kWhtoday;
        Settings->energy_kWhtotal = RtcSettings.energy_kWhtotal;

        Energy.period -= RtcSettings.energy_kWhtoday;                // this becomes a large unsigned, effectively a negative for EnergyShow calculation
        Energy.kWhtoday = 0;
        Energy.kWhtoday_offset = 0;
        RtcSettings.energy_kWhtoday = 0;
        Energy.start_energy = 0;
//        Energy.kWhtoday_delta = 0;                                 // dont zero this, we need to carry the remainder over to tomorrow

        EnergyUpdateToday();
#if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
        Energy.max_energy_state  = 3;
#endif  // USE_ENERGY_POWER_LIMIT
      }
#if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
      if ((RtcTime.hour == Settings->energy_max_energy_start) && (3 == Energy.max_energy_state )) {
        Energy.max_energy_state  = 0;
      }
#endif  // USE_ENERGY_POWER_LIMIT

    }
  }

  XnrgCall(FUNC_EVERY_200_MSECOND);
}

void EnergySaveState(void)
{
  Settings->energy_kWhdoy = (RtcTime.valid) ? RtcTime.day_of_year : 0;

  Settings->energy_kWhtoday = RtcSettings.energy_kWhtoday;
  Settings->energy_kWhtotal = RtcSettings.energy_kWhtotal;

  Settings->energy_usage = RtcSettings.energy_usage;
}

#ifdef USE_ENERGY_MARGIN_DETECTION
bool EnergyMargin(bool type, uint16_t margin, uint16_t value, bool &flag, bool &save_flag)
{
  bool change;

  if (!margin) return false;
  change = save_flag;
  if (type) {
    flag = (value > margin);
  } else {
    flag = (value < margin);
  }
  save_flag = flag;
  return (change != save_flag);
}

void EnergyMarginCheck(void)
{
  if (Energy.power_steady_counter) {
    Energy.power_steady_counter--;
    return;
  }

  bool jsonflg = false;
  Response_P(PSTR("{\"" D_RSLT_MARGINS "\":{"));

  int16_t power_diff[ENERGY_MAX_PHASES] = { 0 };
  for (uint32_t phase = 0; phase < Energy.phase_count; phase++) {
    uint16_t active_power = (uint16_t)(Energy.active_power[phase]);

//    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: APower %d, HPower0 %d, HPower1 %d, HPower2 %d"), active_power, Energy.power_history[phase][0], Energy.power_history[phase][1], Energy.power_history[phase][2]);

    if (Settings->energy_power_delta[phase]) {
      power_diff[phase] = active_power - Energy.power_history[phase][0];
      uint16_t delta = abs(power_diff[phase]);
      bool threshold_met = false;
      if (delta > 0) {
        if (Settings->energy_power_delta[phase] < 101) {  // 1..100 = Percentage
          uint16_t min_power = (Energy.power_history[phase][0] > active_power) ? active_power : Energy.power_history[phase][0];
          if (0 == min_power) { min_power++; }    // Fix divide by 0 exception (#6741)
          delta = (delta * 100) / min_power;
          if (delta > Settings->energy_power_delta[phase]) {
            threshold_met = true;
          }
        } else {                                  // 101..32000 = Absolute
          if (delta > (Settings->energy_power_delta[phase] -100)) {
            threshold_met = true;
          }
        }
      }
      if (threshold_met) {
        Energy.power_history[phase][1] = active_power;  // We only want one report so reset history
        Energy.power_history[phase][2] = active_power;
        jsonflg = true;
      } else {
        power_diff[phase] = 0;
      }
    }
    Energy.power_history[phase][0] = Energy.power_history[phase][1];  // Shift in history every second allowing power changes to settle for up to three seconds
    Energy.power_history[phase][1] = Energy.power_history[phase][2];
    Energy.power_history[phase][2] = active_power;
  }
  if (jsonflg) {
    char power_diff_chr[Energy.phase_count][FLOATSZ];
    for (uint32_t phase = 0; phase < Energy.phase_count; phase++) {
      dtostrfd(power_diff[phase], 0, power_diff_chr[phase]);
    }
    char value_chr[FLOATSZ * ENERGY_MAX_PHASES];
    ResponseAppend_P(PSTR("\"" D_CMND_POWERDELTA "\":%s"), EnergyFormat(value_chr, power_diff_chr[0], 1));
  }

  uint16_t energy_power_u = (uint16_t)(Energy.active_power[0]);

  if (Energy.power_on && (Settings->energy_min_power || Settings->energy_max_power || Settings->energy_min_voltage || Settings->energy_max_voltage || Settings->energy_min_current || Settings->energy_max_current)) {
    uint16_t energy_voltage_u = (uint16_t)(Energy.voltage[0]);
    uint16_t energy_current_u = (uint16_t)(Energy.current[0] * 1000);

    DEBUG_DRIVER_LOG(PSTR("NRG: W %d, U %d, I %d"), energy_power_u, energy_voltage_u, energy_current_u);

    bool flag;
    if (EnergyMargin(false, Settings->energy_min_power, energy_power_u, flag, Energy.min_power_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_POWERLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(true, Settings->energy_max_power, energy_power_u, flag, Energy.max_power_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_POWERHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(false, Settings->energy_min_voltage, energy_voltage_u, flag, Energy.min_voltage_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_VOLTAGELOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(true, Settings->energy_max_voltage, energy_voltage_u, flag, Energy.max_voltage_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_VOLTAGEHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(false, Settings->energy_min_current, energy_current_u, flag, Energy.min_current_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_CURRENTLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(true, Settings->energy_max_current, energy_current_u, flag, Energy.max_current_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_CURRENTHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
  }
  if (jsonflg) {
    ResponseJsonEndEnd();
    MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_MARGINS), MQTT_TELE_RETAIN);
    EnergyMqttShow();
  }

#ifdef USE_ENERGY_POWER_LIMIT
  // Max Power
  if (Settings->energy_max_power_limit) {
    if (Energy.active_power[0] > Settings->energy_max_power_limit) {
      if (!Energy.mplh_counter) {
        Energy.mplh_counter = Settings->energy_max_power_limit_hold;
      } else {
        Energy.mplh_counter--;
        if (!Energy.mplh_counter) {
          ResponseTime_P(PSTR(",\"" D_JSON_MAXPOWERREACHED "\":%d}"), energy_power_u);
          MqttPublishPrefixTopicRulesProcess_P(STAT, S_RSLT_WARNING);
          EnergyMqttShow();
          SetAllPower(POWER_ALL_OFF, SRC_MAXPOWER);
          if (!Energy.mplr_counter) {
            Energy.mplr_counter = Settings->param[P_MAX_POWER_RETRY] +1;  // SetOption33 - Max Power Retry count
          }
          Energy.mplw_counter = Settings->energy_max_power_limit_window;
        }
      }
    }
    else if (TasmotaGlobal.power && (energy_power_u <= Settings->energy_max_power_limit)) {
      Energy.mplh_counter = 0;
      Energy.mplr_counter = 0;
      Energy.mplw_counter = 0;
    }
    if (!TasmotaGlobal.power) {
      if (Energy.mplw_counter) {
        Energy.mplw_counter--;
      } else {
        if (Energy.mplr_counter) {
          Energy.mplr_counter--;
          if (Energy.mplr_counter) {
            ResponseTime_P(PSTR(",\"" D_JSON_POWERMONITOR "\":\"%s\"}"), GetStateText(1));
            MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_JSON_POWERMONITOR));
            RestorePower(true, SRC_MAXPOWER);
          } else {
            ResponseTime_P(PSTR(",\"" D_JSON_MAXPOWERREACHEDRETRY "\":\"%s\"}"), GetStateText(0));
            MqttPublishPrefixTopicRulesProcess_P(STAT, S_RSLT_WARNING);
            EnergyMqttShow();
            SetAllPower(POWER_ALL_OFF, SRC_MAXPOWER);
          }
        }
      }
    }
  }

  // Max Energy
  if (Settings->energy_max_energy) {
    uint16_t energy_daily_u = (uint16_t)(Energy.daily * 1000);
    if (!Energy.max_energy_state  && (RtcTime.hour == Settings->energy_max_energy_start)) {
      Energy.max_energy_state  = 1;
      ResponseTime_P(PSTR(",\"" D_JSON_ENERGYMONITOR "\":\"%s\"}"), GetStateText(1));
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_JSON_ENERGYMONITOR));
      RestorePower(true, SRC_MAXENERGY);
    }
    else if ((1 == Energy.max_energy_state ) && (energy_daily_u >= Settings->energy_max_energy)) {
      Energy.max_energy_state  = 2;
      ResponseTime_P(PSTR(",\"" D_JSON_MAXENERGYREACHED "\":%3_f}"), &Energy.daily);
      MqttPublishPrefixTopicRulesProcess_P(STAT, S_RSLT_WARNING);
      EnergyMqttShow();
      SetAllPower(POWER_ALL_OFF, SRC_MAXENERGY);
    }
  }
#endif  // USE_ENERGY_POWER_LIMIT
}

void EnergyMqttShow(void)
{
// {"Time":"2017-12-16T11:48:55","ENERGY":{"Total":0.212,"Yesterday":0.000,"Today":0.014,"Period":2.0,"Power":22.0,"Factor":1.00,"Voltage":213.6,"Current":0.100}}
  int tele_period_save = TasmotaGlobal.tele_period;
  TasmotaGlobal.tele_period = 2;
  ResponseClear();
  ResponseAppendTime();
  EnergyShow(true);
  TasmotaGlobal.tele_period = tele_period_save;
  ResponseJsonEnd();
  MqttPublishTeleSensor();
}
#endif  // USE_ENERGY_MARGIN_DETECTION

void EnergyEverySecond(void)
{
  // Overtemp check
  if (Energy.use_overtemp && TasmotaGlobal.global_update) {
    if (TasmotaGlobal.power && !isnan(TasmotaGlobal.temperature_celsius) && (TasmotaGlobal.temperature_celsius > (float)Settings->param[P_OVER_TEMP])) {  // SetOption42 Device overtemp, turn off relays

      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Temperature %1_f"), &TasmotaGlobal.temperature_celsius);

      SetAllPower(POWER_ALL_OFF, SRC_OVERTEMP);
    }
  }

  // Invalid data reset
  if (TasmotaGlobal.uptime > ENERGY_WATCHDOG) {
    uint32_t data_valid = Energy.phase_count;
    for (uint32_t i = 0; i < Energy.phase_count; i++) {
      if (Energy.data_valid[i] <= ENERGY_WATCHDOG) {
        Energy.data_valid[i]++;
        if (Energy.data_valid[i] > ENERGY_WATCHDOG) {
          // Reset energy registers
          Energy.voltage[i] = 0;
          Energy.current[i] = 0;
          Energy.active_power[i] = 0;
          if (!isnan(Energy.apparent_power[i])) { Energy.apparent_power[i] = 0; }
          if (!isnan(Energy.reactive_power[i])) { Energy.reactive_power[i] = 0; }
          if (!isnan(Energy.frequency[i])) { Energy.frequency[i] = 0; }
          if (!isnan(Energy.power_factor[i])) { Energy.power_factor[i] = 0; }
          if (!isnan(Energy.export_active[i])) { Energy.export_active[i] = 0; }

          data_valid--;
        }
      }
    }
    if (!data_valid) {
      //Energy.start_energy = 0;
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Energy reset by invalid data"));

      XnrgCall(FUNC_ENERGY_RESET);
    }
  }

#ifdef USE_ENERGY_MARGIN_DETECTION
  EnergyMarginCheck();
#endif  // USE_ENERGY_MARGIN_DETECTION
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void EnergyCommandCalResponse(uint32_t nvalue) {
  snprintf_P(XdrvMailbox.command, CMDSZ, PSTR("%sCal"), XdrvMailbox.command);
  ResponseCmndNumber(nvalue);
}

void CmndEnergyReset(void) {
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);
  values[0] *= 100;

  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 5)) {
    if (params > 0) {
      switch (XdrvMailbox.index) {
      case 1:
        // Reset Energy Today
        Energy.kWhtoday_offset = values[0];
        Energy.kWhtoday = 0;
        Energy.kWhtoday_delta = 0;
        Energy.start_energy = 0;
        Energy.period = Energy.kWhtoday_offset;
        Settings->energy_kWhtoday = Energy.kWhtoday_offset;
        RtcSettings.energy_kWhtoday = Energy.kWhtoday_offset;
        Energy.daily = (float)Energy.kWhtoday_offset / 100000;
        if( params > 1) {
          Settings->energy_kWhtotal_time = values[1];
        }
        else {
          if (!RtcSettings.energy_kWhtotal && !Energy.kWhtoday_offset) {
            Settings->energy_kWhtotal_time = LocalTime();
          }
        }
        break;
      case 2:
        // Reset Energy Yesterday
        Settings->energy_kWhyesterday = values[0];
        if( params > 1) {
          Settings->energy_kWhtotal_time = values[1];
        }
        break;
      case 3:
        // Reset Energy Total
        RtcSettings.energy_kWhtotal = values[0];
        Settings->energy_kWhtotal = RtcSettings.energy_kWhtotal;
//        Energy.total = (float)(RtcSettings.energy_kWhtotal + Energy.kWhtoday_offset + Energy.kWhtoday) / 100000;
        if( params > 1) {
          Settings->energy_kWhtotal_time = values[1];
        }
        else {
          Settings->energy_kWhtotal_time = (!Energy.kWhtoday_offset) ? LocalTime() : Midnight();
        }
        RtcSettings.energy_usage.last_usage_kWhtotal = (uint32_t)(Energy.total * 1000);
        break;
      case 4:
        // Reset energy_usage.usage totals
        RtcSettings.energy_usage.usage1_kWhtotal = values[0];
        if (params > 1) {
          RtcSettings.energy_usage.usage2_kWhtotal = values[1] * 100;
        }
        Settings->energy_usage.usage1_kWhtotal = RtcSettings.energy_usage.usage1_kWhtotal;
        Settings->energy_usage.usage2_kWhtotal = RtcSettings.energy_usage.usage2_kWhtotal;
        break;
      case 5:
        // Reset energy_usage.return totals
        RtcSettings.energy_usage.return1_kWhtotal = values[0];
        if (params > 1) {
          RtcSettings.energy_usage.return2_kWhtotal = values[1] * 100;
        }
        Settings->energy_usage.return1_kWhtotal = RtcSettings.energy_usage.return1_kWhtotal;
        Settings->energy_usage.return2_kWhtotal = RtcSettings.energy_usage.return2_kWhtotal;
        break;
      }
    }
  }

  Energy.total = (float)(RtcSettings.energy_kWhtotal + Energy.kWhtoday_offset + Energy.kWhtoday) / 100000;
  float energy_kWhyesterday = (float)Settings->energy_kWhyesterday / 100000;
  float usage1_kWhtotal = (float)Settings->energy_usage.usage1_kWhtotal / 100000;
  float usage2_kWhtotal = (float)Settings->energy_usage.usage2_kWhtotal / 100000;
  float return1_kWhtotal = (float)Settings->energy_usage.return1_kWhtotal / 100000;
  float return2_kWhtotal = (float)Settings->energy_usage.return2_kWhtotal / 100000;

  Response_P(PSTR("{\"%s\":{\"" D_JSON_TOTAL "\":%*_f,\"" D_JSON_YESTERDAY "\":%*_f,\"" D_JSON_TODAY "\":%*_f,\"" D_JSON_USAGE "\":[%*_f,%*_f],\"" D_JSON_EXPORT "\":[%*_f,%*_f]}}"),
    XdrvMailbox.command,
    Settings->flag2.energy_resolution, &Energy.total,
    Settings->flag2.energy_resolution, &energy_kWhyesterday,
    Settings->flag2.energy_resolution, &Energy.daily,
    Settings->flag2.energy_resolution, &usage1_kWhtotal,
    Settings->flag2.energy_resolution, &usage2_kWhtotal,
    Settings->flag2.energy_resolution, &return1_kWhtotal,
    Settings->flag2.energy_resolution, &return2_kWhtotal);
}

void CmndTariff(void) {
  // Tariff1 22:00,23:00 - Tariff1 start hour for Standard Time and Daylight Savings Time
  // Tariff2 6:00,7:00   - Tariff2 start hour for Standard Time and Daylight Savings Time
  // Tariffx 1320, 1380  = minutes and also 22:00, 23:00
  // Tariffx 22, 23      = hours and also 22:00, 23:00
  // Tariff9 0/1

  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    uint32_t tariff = XdrvMailbox.index -1;
    uint32_t time_type = 0;
    char *p;
    char *str = strtok_r(XdrvMailbox.data, ", ", &p);  // 23:15, 22:30
    while ((str != nullptr) && (time_type < 2)) {
      char *q;
      uint32_t value = strtol(str, &q, 10);            // 23 or 22
      Settings->tariff[tariff][time_type] = value;
      if (value < 24) {                                // Below 24 is hours
        Settings->tariff[tariff][time_type] *= 60;      // Multiply hours by 60 minutes
        char *minute = strtok_r(nullptr, ":", &q);
        if (minute) {
          value = strtol(minute, nullptr, 10);         // 15 or 30
          if (value > 59) {
            value = 59;
          }
          Settings->tariff[tariff][time_type] += value;
        }
      }
      if (Settings->tariff[tariff][time_type] > 1439) {
        Settings->tariff[tariff][time_type] = 1439;     // Max is 23:59
      }
      str = strtok_r(nullptr, ", ", &p);
      time_type++;
    }
  }
  else if (XdrvMailbox.index == 9) {
    Settings->flag3.energy_weekend = XdrvMailbox.payload & 1;  // CMND_TARIFF
  }
  Response_P(PSTR("{\"%s\":{\"Off-Peak\":{\"STD\":\"%s\",\"DST\":\"%s\"},\"Standard\":{\"STD\":\"%s\",\"DST\":\"%s\"},\"Weekend\":\"%s\"}}"),
    XdrvMailbox.command,
    GetMinuteTime(Settings->tariff[0][0]).c_str(),GetMinuteTime(Settings->tariff[0][1]).c_str(),
    GetMinuteTime(Settings->tariff[1][0]).c_str(),GetMinuteTime(Settings->tariff[1][1]).c_str(),
    GetStateText(Settings->flag3.energy_weekend));             // CMND_TARIFF
}

void CmndPowerCal(void) {
  Energy.command_code = CMND_POWERCAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if (XdrvMailbox.payload > 999) {
      Settings->energy_power_calibration = XdrvMailbox.payload;
    }
    ResponseCmndNumber(Settings->energy_power_calibration);
  }
}

void CmndVoltageCal(void) {
  Energy.command_code = CMND_VOLTAGECAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if (XdrvMailbox.payload > 999) {
      Settings->energy_voltage_calibration = XdrvMailbox.payload;
    }
    ResponseCmndNumber(Settings->energy_voltage_calibration);
  }
}

void CmndCurrentCal(void) {
  Energy.command_code = CMND_CURRENTCAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if (XdrvMailbox.payload > 999) {
      Settings->energy_current_calibration = XdrvMailbox.payload;
    }
    ResponseCmndNumber(Settings->energy_current_calibration);
  }
}

void CmndFrequencyCal(void) {
  Energy.command_code = CMND_FREQUENCYCAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if (XdrvMailbox.payload > 999) {
      Settings->energy_frequency_calibration = XdrvMailbox.payload;
    }
    ResponseCmndNumber(Settings->energy_frequency_calibration);
  }
}

void CmndPowerSet(void) {
  Energy.command_code = CMND_POWERSET;
  if (XnrgCall(FUNC_COMMAND)) {  // Watt
    EnergyCommandCalResponse(Settings->energy_power_calibration);
  }
}

void CmndVoltageSet(void) {
  Energy.command_code = CMND_VOLTAGESET;
  if (XnrgCall(FUNC_COMMAND)) {  // Volt
    EnergyCommandCalResponse(Settings->energy_voltage_calibration);
  }
}

void CmndCurrentSet(void) {
  Energy.command_code = CMND_CURRENTSET;
  if (XnrgCall(FUNC_COMMAND)) {  // milliAmpere
    EnergyCommandCalResponse(Settings->energy_current_calibration);
  }
}

void CmndFrequencySet(void) {
  Energy.command_code = CMND_FREQUENCYSET;
  if (XnrgCall(FUNC_COMMAND)) {  // Hz
    EnergyCommandCalResponse(Settings->energy_frequency_calibration);
  }
}

void CmndModuleAddress(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 4) && (1 == Energy.phase_count)) {
    Energy.command_code = CMND_MODULEADDRESS;
    if (XnrgCall(FUNC_COMMAND)) {  // Module address
      ResponseCmndDone();
    }
  }
}

void CmndEnergyConfig(void) {
  Energy.command_code = CMND_ENERGYCONFIG;
  if (XnrgCall(FUNC_COMMAND)) {
    ResponseCmndDone();
  }
}

#ifdef USE_ENERGY_MARGIN_DETECTION
void CmndPowerDelta(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= ENERGY_MAX_PHASES)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32000)) {
      Settings->energy_power_delta[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings->energy_power_delta[XdrvMailbox.index -1]);
  }
}

void CmndPowerLow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_min_power = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_min_power);
}

void CmndPowerHigh(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power);
}

void CmndVoltageLow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
    Settings->energy_min_voltage = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_min_voltage);
}

void CmndVoltageHigh(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
    Settings->energy_max_voltage = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_voltage);
}

void CmndCurrentLow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
    Settings->energy_min_current = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_min_current);
}

void CmndCurrentHigh(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
    Settings->energy_max_current = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_current);
}

#ifdef USE_ENERGY_POWER_LIMIT
void CmndMaxPower(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_limit = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_limit);
}

void CmndMaxPowerHold(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_limit_hold = (1 == XdrvMailbox.payload) ? MAX_POWER_HOLD : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_limit_hold);
}

void CmndMaxPowerWindow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_limit_window = (1 == XdrvMailbox.payload) ? MAX_POWER_WINDOW : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_limit_window);
}

void CmndSafePower(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_safe_limit = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_safe_limit);
}

void CmndSafePowerHold(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_safe_limit_hold = (1 == XdrvMailbox.payload) ? SAFE_POWER_HOLD : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_safe_limit_hold);
}

void CmndSafePowerWindow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 1440)) {
    Settings->energy_max_power_safe_limit_window = (1 == XdrvMailbox.payload) ? SAFE_POWER_WINDOW : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_safe_limit_window);
}

void CmndMaxEnergy(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_energy = XdrvMailbox.payload;
    Energy.max_energy_state  = 3;
  }
  ResponseCmndNumber(Settings->energy_max_energy);
}

void CmndMaxEnergyStart(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 24)) {
    Settings->energy_max_energy_start = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_energy_start);
}
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION

void EnergyDrvInit(void) {
  memset(&Energy, 0, sizeof(Energy));  // Reset all to 0 and false;
  for (uint32_t phase = 0; phase < ENERGY_MAX_PHASES; phase++) {
    Energy.apparent_power[phase] = NAN;
    Energy.reactive_power[phase] = NAN;
    Energy.power_factor[phase] = NAN;
    Energy.frequency[phase] = NAN;
#if defined(SDM630_IMPORT) || defined(SDM72_IMPEXP)
    Energy.import_active[phase] = NAN;
#endif  // SDM630_IMPORT || SDM72_IMPEXP
    Energy.export_active[phase] = NAN;
  }
  Energy.phase_count = 1;              // Number of phases active
  Energy.voltage_available = true;     // Enable if voltage is measured
  Energy.current_available = true;     // Enable if current is measured
  Energy.power_on = true;
#ifdef USE_ENERGY_MARGIN_DETECTION
  Energy.power_steady_counter = 8;     // Allow for power on stabilization
#endif  // USE_ENERGY_MARGIN_DETECTION

  TasmotaGlobal.energy_driver = ENERGY_NONE;
  XnrgCall(FUNC_PRE_INIT);  // Find first energy driver
}

void EnergySnsInit(void)
{
  XnrgCall(FUNC_INIT);

  if (TasmotaGlobal.energy_driver) {
//    Energy.kWhtoday_offset = 0;
    // Do not use at Power On as Rtc was invalid (but has been restored from Settings already)
    if ((ResetReason() != REASON_DEFAULT_RST) && RtcSettingsValid()) {
      Energy.kWhtoday_offset = RtcSettings.energy_kWhtoday;
      Energy.kWhtoday_offset_init = true;
    }
//    Energy.kWhtoday = 0;
//    Energy.kWhtoday_delta = 0;
    Energy.period = Energy.kWhtoday_offset;
    EnergyUpdateToday();
    ticker_energy.attach_ms(200, Energy200ms);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS1[] PROGMEM =
  "{s}" D_POWERUSAGE_APPARENT "{m}%s " D_UNIT_VA "{e}"
  "{s}" D_POWERUSAGE_REACTIVE "{m}%s " D_UNIT_VAR "{e}"
  "{s}" D_POWER_FACTOR "{m}%s{e}";

const char HTTP_ENERGY_SNS2[] PROGMEM =
  "{s}" D_ENERGY_TODAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

const char HTTP_ENERGY_SNS3[] PROGMEM =
  "{s}" D_EXPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}";

#if defined(SDM630_IMPORT) || defined(SDM72_IMPEXP)
const char HTTP_ENERGY_SNS4[] PROGMEM =
  "{s}" D_IMPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}";
#endif  // SDM630_IMPORT || SDM72_IMPEXP
#endif  // USE_WEBSERVER

void EnergyShow(bool json)
{
  for (uint32_t i = 0; i < Energy.phase_count; i++) {
    if (Energy.voltage_common) {
      Energy.voltage[i] = Energy.voltage[0];
    }
  }

  float power_factor_knx = Energy.power_factor[0];

  char apparent_power_chr[Energy.phase_count][FLOATSZ];
  char reactive_power_chr[Energy.phase_count][FLOATSZ];
  char power_factor_chr[Energy.phase_count][FLOATSZ];
  char frequency_chr[Energy.phase_count][FLOATSZ];
  if (!Energy.type_dc) {
    if (Energy.current_available && Energy.voltage_available) {
      for (uint32_t i = 0; i < Energy.phase_count; i++) {
        float apparent_power = Energy.apparent_power[i];
        if (isnan(apparent_power)) {
          apparent_power = Energy.voltage[i] * Energy.current[i];
        }
        if (apparent_power < Energy.active_power[i]) {  // Should be impossible
          Energy.active_power[i] = apparent_power;
        }

        float power_factor = Energy.power_factor[i];
        if (isnan(power_factor)) {
          power_factor = (Energy.active_power[i] && apparent_power) ? Energy.active_power[i] / apparent_power : 0;
          if (power_factor > 1) {
            power_factor = 1;
          }
        }
        if (0 == i) { power_factor_knx = power_factor; }

        float reactive_power = Energy.reactive_power[i];
        if (isnan(reactive_power)) {
          reactive_power = 0;
          uint32_t difference = ((uint32_t)(apparent_power * 100) - (uint32_t)(Energy.active_power[i] * 100)) / 10;
          if ((Energy.current[i] > 0.005) && ((difference > 15) || (difference > (uint32_t)(apparent_power * 100 / 1000)))) {
            // calculating reactive power only if current is greater than 0.005A and
            // difference between active and apparent power is greater than 1.5W or 1%
            //reactive_power = (float)(RoundSqrtInt((uint64_t)(apparent_power * apparent_power * 100) - (uint64_t)(Energy.active_power[i] * Energy.active_power[i] * 100))) / 10;
            float power_diff = apparent_power * apparent_power - Energy.active_power[i] * Energy.active_power[i];
            if (power_diff < 10737418) // 2^30 / 100 (RoundSqrtInt is limited to 2^30-1)
              reactive_power = (float)(RoundSqrtInt((uint32_t)(power_diff * 100.0))) / 10.0;
            else
              reactive_power = (float)(SqrtInt((uint32_t)(power_diff)));
          }
        }

        dtostrfd(apparent_power, Settings->flag2.wattage_resolution, apparent_power_chr[i]);
        dtostrfd(reactive_power, Settings->flag2.wattage_resolution, reactive_power_chr[i]);
        dtostrfd(power_factor, 2, power_factor_chr[i]);
      }
    }
    for (uint32_t i = 0; i < Energy.phase_count; i++) {
      float frequency = Energy.frequency[i];
      if (isnan(Energy.frequency[i])) {
        frequency = 0;
      }
      dtostrfd(frequency, Settings->flag2.frequency_resolution, frequency_chr[i]);
    }
  }

  char voltage_chr[Energy.phase_count][FLOATSZ];
  char current_chr[Energy.phase_count][FLOATSZ];
  char active_power_chr[Energy.phase_count][FLOATSZ];
#if defined(SDM630_IMPORT) || defined(SDM72_IMPEXP)
  char import_active_chr[Energy.phase_count][FLOATSZ];
#endif  // SDM630_IMPORT || SDM72_IMPEXP
  char export_active_chr[Energy.phase_count][FLOATSZ];
  for (uint32_t i = 0; i < Energy.phase_count; i++) {
    dtostrfd(Energy.voltage[i], Settings->flag2.voltage_resolution, voltage_chr[i]);
    dtostrfd(Energy.current[i], Settings->flag2.current_resolution, current_chr[i]);
    dtostrfd(Energy.active_power[i], Settings->flag2.wattage_resolution, active_power_chr[i]);
#if defined(SDM630_IMPORT) || defined(SDM72_IMPEXP)
    dtostrfd(Energy.import_active[i], Settings->flag2.energy_resolution, import_active_chr[i]);
#endif  // SDM630_IMPORT || SDM72_IMPEXP
    dtostrfd(Energy.export_active[i], Settings->flag2.energy_resolution, export_active_chr[i]);
  }

  char energy_total_chr[FLOATSZ];
  dtostrfd(Energy.total, Settings->flag2.energy_resolution, energy_total_chr);
  char energy_daily_chr[FLOATSZ];
  dtostrfd(Energy.daily, Settings->flag2.energy_resolution, energy_daily_chr);
  char energy_yesterday_chr[FLOATSZ];
  dtostrfd((float)Settings->energy_kWhyesterday / 100000, Settings->flag2.energy_resolution, energy_yesterday_chr);


  bool energy_tariff = false;
  char energy_usage_chr[2][FLOATSZ];
  char energy_return_chr[2][FLOATSZ];
  if (Settings->tariff[0][0] != Settings->tariff[1][0]) {
    dtostrfd((float)RtcSettings.energy_usage.usage1_kWhtotal / 100000, Settings->flag2.energy_resolution, energy_usage_chr[0]);  // Tariff1
    dtostrfd((float)RtcSettings.energy_usage.usage2_kWhtotal / 100000, Settings->flag2.energy_resolution, energy_usage_chr[1]);  // Tariff2
    dtostrfd((float)RtcSettings.energy_usage.return1_kWhtotal / 100000, Settings->flag2.energy_resolution, energy_return_chr[0]);  // Tariff1
    dtostrfd((float)RtcSettings.energy_usage.return2_kWhtotal / 100000, Settings->flag2.energy_resolution, energy_return_chr[1]);  // Tariff2
    energy_tariff = true;
  }

  char value_chr[FLOATSZ * ENERGY_MAX_PHASES];   // Used by EnergyFormatIndex
  char value2_chr[FLOATSZ * ENERGY_MAX_PHASES];
  char value3_chr[FLOATSZ * ENERGY_MAX_PHASES];

  if (json) {
    bool show_energy_period = (0 == TasmotaGlobal.tele_period);

    ResponseAppend_P(PSTR(",\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL_START_TIME "\":\"%s\",\"" D_JSON_TOTAL "\":%s"),
      GetDateAndTime(DT_ENERGY).c_str(),
      energy_total_chr);

    if (energy_tariff) {
      ResponseAppend_P(PSTR(",\"" D_JSON_TOTAL D_CMND_TARIFF "\":%s"),
        EnergyFormatIndex(value_chr, energy_usage_chr[0], json, 2));
    }

    ResponseAppend_P(PSTR(",\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s"),
      energy_yesterday_chr,
      energy_daily_chr);

 #if defined(SDM630_IMPORT) || defined(SDM72_IMPEXP)
    if (!isnan(Energy.import_active[0])) {
      ResponseAppend_P(PSTR(",\"" D_JSON_IMPORT_ACTIVE "\":%s"),
        EnergyFormat(value_chr, import_active_chr[0], json));
      if (energy_tariff) {
        ResponseAppend_P(PSTR(",\"" D_JSON_IMPORT D_CMND_TARIFF "\":%s"),
          EnergyFormatIndex(value_chr, energy_return_chr[0], json, 2));
      }
    }
#endif  // SDM630_IMPORT || SDM72_IMPEXP

    if (!isnan(Energy.export_active[0])) {
      ResponseAppend_P(PSTR(",\"" D_JSON_EXPORT_ACTIVE "\":%s"),
        EnergyFormat(value_chr, export_active_chr[0], json));
      if (energy_tariff) {
        ResponseAppend_P(PSTR(",\"" D_JSON_EXPORT D_CMND_TARIFF "\":%s"),
          EnergyFormatIndex(value_chr, energy_return_chr[0], json, 2));
      }
    }

    if (show_energy_period) {
      float energy = (float)(RtcSettings.energy_kWhtoday - Energy.period) / 100;
      Energy.period = RtcSettings.energy_kWhtoday;
      char energy_period_chr[FLOATSZ];
      dtostrfd(energy, Settings->flag2.wattage_resolution, energy_period_chr);
      ResponseAppend_P(PSTR(",\"" D_JSON_PERIOD "\":%s"), energy_period_chr);
    }
    ResponseAppend_P(PSTR(",\"" D_JSON_POWERUSAGE "\":%s"),
      EnergyFormat(value_chr, active_power_chr[0], json));
    if (!Energy.type_dc) {
      if (Energy.current_available && Energy.voltage_available) {
        ResponseAppend_P(PSTR(",\"" D_JSON_APPARENT_POWERUSAGE "\":%s,\"" D_JSON_REACTIVE_POWERUSAGE "\":%s,\"" D_JSON_POWERFACTOR "\":%s"),
          EnergyFormat(value_chr, apparent_power_chr[0], json),
          EnergyFormat(value2_chr, reactive_power_chr[0], json),
          EnergyFormat(value3_chr, power_factor_chr[0], json));
      }
      if (!isnan(Energy.frequency[0])) {
        ResponseAppend_P(PSTR(",\"" D_JSON_FREQUENCY "\":%s"),
          EnergyFormat(value_chr, frequency_chr[0], json, Energy.frequency_common));
      }
    }
    if (Energy.voltage_available) {
      ResponseAppend_P(PSTR(",\"" D_JSON_VOLTAGE "\":%s"),
        EnergyFormat(value_chr, voltage_chr[0], json, Energy.voltage_common));
    }
    if (Energy.current_available) {
      ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT "\":%s"),
        EnergyFormat(value_chr, current_chr[0], json));
    }
    XnrgCall(FUNC_JSON_APPEND);
    ResponseJsonEnd();

#ifdef USE_DOMOTICZ
    if (show_energy_period) {  // Only send if telemetry
      dtostrfd(Energy.total * 1000, 1, energy_total_chr);
      DomoticzSensorPowerEnergy((int)Energy.active_power[0], energy_total_chr);  // PowerUsage, EnergyToday

      dtostrfd((float)RtcSettings.energy_usage.usage1_kWhtotal / 100, 1, energy_usage_chr[0]);  // Tariff1
      dtostrfd((float)RtcSettings.energy_usage.usage2_kWhtotal / 100, 1, energy_usage_chr[1]);  // Tariff2
      dtostrfd((float)RtcSettings.energy_usage.return1_kWhtotal / 100, 1, energy_return_chr[0]);
      dtostrfd((float)RtcSettings.energy_usage.return2_kWhtotal / 100, 1, energy_return_chr[1]);
      DomoticzSensorP1SmartMeter(energy_usage_chr[0], energy_usage_chr[1], energy_return_chr[0], energy_return_chr[1], (int)Energy.active_power[0]);

      if (Energy.voltage_available) {
        DomoticzSensor(DZ_VOLTAGE, voltage_chr[0]);  // Voltage
      }
      if (Energy.current_available) {
        DomoticzSensor(DZ_CURRENT, current_chr[0]);  // Current
      }
    }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    if (show_energy_period) {
      if (Energy.voltage_available) {
        KnxSensor(KNX_ENERGY_VOLTAGE, Energy.voltage[0]);
      }
      if (Energy.current_available) {
        KnxSensor(KNX_ENERGY_CURRENT, Energy.current[0]);
      }
      KnxSensor(KNX_ENERGY_POWER, Energy.active_power[0]);
      if (!Energy.type_dc) {
        KnxSensor(KNX_ENERGY_POWERFACTOR, power_factor_knx);
      }
      KnxSensor(KNX_ENERGY_DAILY, Energy.daily);
      KnxSensor(KNX_ENERGY_TOTAL, Energy.total);
      KnxSensor(KNX_ENERGY_YESTERDAY, (float)Settings->energy_kWhyesterday / 100000);
    }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
  } else {
    if (Energy.voltage_available) {
      WSContentSend_PD(HTTP_SNS_VOLTAGE, EnergyFormat(value_chr, voltage_chr[0], json, Energy.voltage_common));
    }
    if (Energy.current_available) {
      WSContentSend_PD(HTTP_SNS_CURRENT, EnergyFormat(value_chr, current_chr[0], json));
    }
    WSContentSend_PD(HTTP_SNS_POWER, EnergyFormat(value_chr, active_power_chr[0], json));
    if (!Energy.type_dc) {
      if (Energy.current_available && Energy.voltage_available) {
        WSContentSend_PD(HTTP_ENERGY_SNS1, EnergyFormat(value_chr, apparent_power_chr[0], json),
                                           EnergyFormat(value2_chr, reactive_power_chr[0], json),
                                           EnergyFormat(value3_chr, power_factor_chr[0], json));
      }
      if (!isnan(Energy.frequency[0])) {
        WSContentSend_PD(PSTR("{s}" D_FREQUENCY "{m}%s " D_UNIT_HERTZ "{e}"),
          EnergyFormat(value_chr, frequency_chr[0], json, Energy.frequency_common));
      }
    }
    WSContentSend_PD(HTTP_ENERGY_SNS2, energy_daily_chr, energy_yesterday_chr, energy_total_chr);
    if (!isnan(Energy.export_active[0])) {
      WSContentSend_PD(HTTP_ENERGY_SNS3, EnergyFormat(value_chr, export_active_chr[0], json));
    }
#if defined(SDM630_IMPORT) || defined(SDM72_IMPEXP)
    if (!isnan(Energy.import_active[0])) {
      WSContentSend_PD(HTTP_ENERGY_SNS4, EnergyFormat(value_chr, import_active_chr[0], json));
    }
#endif  // SDM630_IMPORT || SDM72_IMPEXP

    XnrgCall(FUNC_WEB_SENSOR);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv03(uint8_t function)
{
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    EnergyDrvInit();
  }
  else if (TasmotaGlobal.energy_driver) {
    switch (function) {
      case FUNC_LOOP:
        XnrgCall(FUNC_LOOP);
        break;
      case FUNC_EVERY_250_MSECOND:
        if (TasmotaGlobal.uptime > 4) {
          XnrgCall(FUNC_EVERY_250_MSECOND);
        }
        break;
      case FUNC_EVERY_SECOND:
        XnrgCall(FUNC_EVERY_SECOND);
        break;
      case FUNC_SERIAL:
        result = XnrgCall(FUNC_SERIAL);
        break;
#ifdef USE_ENERGY_MARGIN_DETECTION
      case FUNC_SET_POWER:
        Energy.power_steady_counter = 2;
        break;
#endif  // USE_ENERGY_MARGIN_DETECTION
      case FUNC_COMMAND:
        result = DecodeCommand(kEnergyCommands, EnergyCommand);
        break;
    }
  }
  return result;
}

bool Xsns03(uint8_t function)
{
  bool result = false;

  if (TasmotaGlobal.energy_driver) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        EnergyEverySecond();
        break;
      case FUNC_JSON_APPEND:
        EnergyShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        EnergyShow(false);
        break;
#endif  // USE_WEBSERVER
      case FUNC_SAVE_BEFORE_RESTART:
        EnergySaveState();
        break;
      case FUNC_INIT:
        EnergySnsInit();
        break;
    }
  }
  return result;
}

#endif  // USE_ENERGY_SENSOR
