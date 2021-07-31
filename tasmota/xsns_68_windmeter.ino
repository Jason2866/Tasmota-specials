/*
  xsns_68_windmeter.ino - Analog wind sensor support for Tasmota

  Copyright (C) 2021  Matteo Albinola
  (inspired by great works of Thomas Eckerstorfer, Norbert Richter, Maarten Damen and Theo Arends)

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

#ifdef USE_WINDMETER
/*********************************************************************************************\
 * WindMeter sensor (speed)
\*********************************************************************************************/

#define XSNS_68             68

#define D_WINDMETER_NAME "WindMeter"

#define WINDMETER_DEF_RADIUS          61    // Radius in millimeters (calculated by measuring the distance from the centre to the edge of one of the cups)
#define WINDMETER_DEF_PULSES_X_ROT    1     // Number of pulses for a complete rotation
#define WINDMETER_DEF_PULSE_DEBOUNCE  10    // Pulse counter debounce time (milliseconds)
#define WINDMETER_DEF_COMP_FACTOR     1.18  // Compensation factor
#define WINDMETER_DEF_TELE_PCHANGE    255   // Minimum percentage change between current and last reported speed in order to trigger a new tele message (0...100, 255 means off)
#define WINDMETER_WEIGHT_AVG_SAMPLE   150   // No of samples to take

#ifdef USE_WEBSERVER
#define D_WINDMETER_WIND_AVG "&empty;"
#define D_WINDMETER_WIND_ANGLE "&ang;"
#define D_WINDMETER_WIND_DEGREE "&deg;"
const char HTTP_SNS_WINDMETER[] PROGMEM =
   "{s}" D_WINDMETER_NAME " " D_TX20_WIND_SPEED "{m}%s %s{e}"
#ifndef USE_WINDMETER_NOSTATISTICS
   "{s}" D_WINDMETER_NAME " " D_TX20_WIND_SPEED " " D_WINDMETER_WIND_AVG "{m}%s %s{e}"
   "{s}" D_WINDMETER_NAME " " D_TX20_WIND_SPEED_MIN "{m}%s %s{e}"
   "{s}" D_WINDMETER_NAME " " D_TX20_WIND_SPEED_MAX "{m}%s %s{e}"
#endif  // USE_WINDMETER_NOSTATISTICS
//   "{s}WindMeter " D_TX20_WIND_DIRECTION "{m}%s %s" D_WINDMETER_WIND_DEGREE "{e}"
//#ifndef USE_WINDMETER_NOSTATISTICS
//   "{s}WindMeter " D_TX20_WIND_DIRECTION " " D_WINDMETER_WIND_AVG "{m}%s %s" D_WINDMETER_WIND_DEGREE "{e}"
//   "{s}WindMeter " D_TX20_WIND_DIRECTION " " D_WINDMETER_WIND_ANGLE "{m}%s" D_WINDMETER_WIND_DEGREE " (%s,%s)" D_WINDMETER_WIND_DEGREE;
//#endif  // USE_WINDMETER_NOSTATISTICS
   ;
#endif  // USE_WEBSERVER

// float saves 48 byte
float const windmeter_pi = 3.1415926535897932384626433;  // Pi
float const windmeter_2pi = windmeter_pi * 2;

struct WINDMETER {
  volatile uint32_t counter_time;
  volatile unsigned long counter = 0;
  //uint32_t speed_time;
  float speed = 0;
  float last_tele_speed = 0;
#ifndef USE_WINDMETER_NOSTATISTICS
  float speed_min = 0;
  float speed_max = 0;
  float speed_avg = 0;
  uint32_t samples_count = 0;
  uint32_t avg_samples_no;
#endif  // USE_WINDMETER_NOSTATISTICS
} WindMeter;

void IRAM_ATTR WindMeterUpdateSpeed(void)
{
  uint32_t time = micros();
  uint32_t time_diff = time - WindMeter.counter_time;
  if (time_diff > Settings->windmeter_pulse_debounce * 1000) {
    WindMeter.counter_time = time;
    WindMeter.counter++;
//    AddLog(LOG_LEVEL_DEBUG, PSTR("WMET: Counter %d"), WindMeter.counter);
  }
}

/********************************************************************************************/

void WindMeterInit(void)
{
  if (!Settings->flag2.speed_conversion) {
    Settings->flag2.speed_conversion = 2;  // 0 = none, 1 = m/s, 2 = km/h, 3 = kn, 4 = mph, 5 = ft/s, 6 = yd/s
  }
  if (!Settings->windmeter_radius) {
    Settings->windmeter_radius = WINDMETER_DEF_RADIUS;
  }
  if (!Settings->windmeter_pulses_x_rot) {
    Settings->windmeter_pulses_x_rot = WINDMETER_DEF_PULSES_X_ROT;
  }
  if (!Settings->windmeter_pulse_debounce) {
    Settings->windmeter_pulse_debounce = WINDMETER_DEF_PULSE_DEBOUNCE;
  }
  if (!Settings->windmeter_speed_factor) {
    Settings->windmeter_speed_factor = (int16_t)(WINDMETER_DEF_COMP_FACTOR * 1000);
  }
  if (!Settings->windmeter_tele_pchange) {
    Settings->windmeter_tele_pchange = WINDMETER_DEF_TELE_PCHANGE;
  }

#ifndef USE_WINDMETER_NOSTATISTICS
  WindMeterResetStatData();
  WindMeterCheckSampleCount();
#endif  // USE_WINDMETER_NOSTATISTICS

  pinMode(Pin(GPIO_WINDMETER_SPEED), INPUT_PULLUP);
  attachInterrupt(Pin(GPIO_WINDMETER_SPEED), WindMeterUpdateSpeed, FALLING);
}

void WindMeterEverySecond(void)
{
  //uint32_t time = micros();
  //uint32_t delta_time = time - WindMeter.speed_time;
  //AddLog(LOG_LEVEL_INFO, PSTR("delta_time: %d"), delta_time);

  // speed = ( (pulses / pulses_per_rotation) * (2 * pi * radius) ) / delta_time
  WindMeter.speed = ((WindMeter.counter / Settings->windmeter_pulses_x_rot) * (windmeter_2pi * ((float)Settings->windmeter_radius / 1000))) * ((float)Settings->windmeter_speed_factor / 1000);
  //WindMeter.speed = (((WindMeter.counter / Settings->windmeter_pulses_x_rot) * (windmeter_2pi * ((float)Settings->windmeter_radius / 1000))) / ((float)delta_time / 1000000)) * ((float)Settings->windmeter_speed_factor / 1000);
  WindMeter.counter = 0;
  //WindMeter.speed_time = time;

  //char speed_string[FLOATSZ];
  //dtostrfd(WindMeter.speed, 2, speed_string);
  //char uspeed_string[FLOATSZ];
  //dtostrfd(ConvertSpeed(WindMeter.speed), 2, uspeed_string);
  //AddLog(LOG_LEVEL_DEBUG, PSTR("WMET: Speed %s [m/s] - %s [unit]"), speed_string, uspeed_string);

#ifndef USE_WINDMETER_NOSTATISTICS
  if (WindMeter.speed < WindMeter.speed_min) {
    WindMeter.speed_min = WindMeter.speed;
  }
  if (WindMeter.speed > WindMeter.speed_max) {
    WindMeter.speed_max = WindMeter.speed;
  }

  // exponentially weighted average is not quite as smooth as the arithmetic average
  // but close enough to the moving average and does not require the regular reset
  // of the divider with the associated jump in avg values after period is over
  if (WindMeter.samples_count <= WindMeter.avg_samples_no) {
    WindMeter.samples_count++;
  }
  WindMeter.speed_avg -= WindMeter.speed_avg / WindMeter.samples_count;
  WindMeter.speed_avg += float(WindMeter.speed) / WindMeter.samples_count;

  WindMeterCheckSampleCount();
  if (0==Settings->tele_period) {
    WindMeterResetStatData();
  }
#endif  // USE_WINDMETER_NOSTATISTICS

  if (WindMeterShouldTriggerTele()) {
    MqttPublishTeleperiodSensor();
  }
}

bool WindMeterShouldTriggerTele()
{
  if (Settings->windmeter_tele_pchange > 100) {
    return false;
  } else if (WindMeter.last_tele_speed == 0) {
    return WindMeter.speed > 0;
  } else {
    float perc_change = (WindMeter.speed / WindMeter.last_tele_speed) -1;
    return (perc_change * ((perc_change < 0) ? -100 : 100)) >= Settings->windmeter_tele_pchange;
  }
}

void WindMeterResetStatData(void)
{
  WindMeter.speed_min = WindMeter.speed;
  WindMeter.speed_max = WindMeter.speed;
  //WindMeter.direction_min = WindMeter.direction;
  //WindMeter.direction_max = WindMeter.direction;
}

void WindMeterCheckSampleCount(void)
{
  uint32_t prev_avg_samples_no = WindMeter.avg_samples_no;
  if (Settings->tele_period) {
    // number for avg samples = teleperiod value if set
    WindMeter.avg_samples_no = Settings->tele_period;
  } else {
    // otherwise use default number of samples for this driver
    WindMeter.avg_samples_no = WINDMETER_WEIGHT_AVG_SAMPLE;
  }
  if (prev_avg_samples_no != WindMeter.avg_samples_no) {
    WindMeter.speed_avg = WindMeter.speed;
    WindMeter.samples_count = 0;
  }
}

void WindMeterShow(bool json)
{
  char speed_string[FLOATSZ];
  dtostrfd(ConvertSpeed(WindMeter.speed), 2, speed_string);
#ifndef USE_WINDMETER_NOSTATISTICS
  char speed_min_string[FLOATSZ];
  dtostrfd(ConvertSpeed(WindMeter.speed_min), 2, speed_min_string);
  char speed_max_string[FLOATSZ];
  dtostrfd(ConvertSpeed(WindMeter.speed_max), 2, speed_max_string);
  char speed_avg_string[FLOATSZ];
  dtostrfd(ConvertSpeed(WindMeter.speed_avg), 2, speed_avg_string);
  //char direction_avg_string[FLOATSZ];
  //dtostrfd(WindMeter.direction_avg, 1, direction_avg_string);
  //char direction_avg_cardinal_string[4];
  //GetTextIndexed(direction_avg_cardinal_string, sizeof(direction_avg_cardinal_string), int((WindMeter.direction_avg/22.5f)+0.5f) % 16, kWindMeterDirections);
  //char direction_range_string[FLOATSZ];
  //dtostrfd(Tx2xNormalize(WindMeter.direction_max-WindMeter.direction_min)*22.5, 1, direction_range_string);
  //char direction_min_string[FLOATSZ];
  //dtostrfd(Tx2xNormalize(WindMeter.direction_min)*22.5, 1, direction_min_string);
  //char direction_max_string[FLOATSZ];
  //dtostrfd(Tx2xNormalize(WindMeter.direction_max)*22.5, 1, direction_max_string);
#endif  // USE_WINDMETER_NOSTATISTICS

  if (json) {
    WindMeter.last_tele_speed = WindMeter.speed;
#ifndef USE_WINDMETER_NOSTATISTICS
    //ResponseAppend_P(PSTR(",\"" D_WINDMETER_NAME "\":{\"" D_JSON_SPEED "\":{\"Act\":%s,\"Avg\":%s,\"Min\":%s,\"Max\":%s},\"Dir\":{\"Card\":\"%s\",\"Deg\":%s,\"Avg\":%s,\"AvgCard\":\"%s\",\"Min\":%s,\"Max\":%s,\"Range\":%s}}"),
    ResponseAppend_P(PSTR(",\"" D_WINDMETER_NAME "\":{\"" D_JSON_SPEED "\":{\"Act\":%s,\"Avg\":%s,\"Min\":%s,\"Max\":%s}}"),
      speed_string,
      speed_avg_string,
      speed_min_string,
      speed_max_string
      //direction_cardinal_string,
      //direction_string,
      //direction_avg_string,
      //direction_avg_cardinal_string,
      //direction_min_string,
      //direction_max_string,
      //direction_range_string
    );
#else  // USE_WINDMETER_NOSTATISTICS
  //ResponseAppend_P(PSTR(",\"" D_WINDMETER_NAME "\":{\"" D_JSON_SPEED "\":{\"Act\":%s},\"Dir\":{\"Card\":\"%s\",\"Deg\":%s}}"),
  ResponseAppend_P(PSTR(",\"" D_WINDMETER_NAME "\":{\"" D_JSON_SPEED "\":{\"Act\":%s}}"),
    speed_string
    //wind_direction_cardinal_string,
    //wind_direction_string
  );
#endif  // USE_WINDMETER_NOSTATISTICS
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_WINDMETER,
      speed_string,
      SpeedUnit().c_str(),
#ifndef USE_WINDMETER_NOSTATISTICS
      speed_avg_string,
      SpeedUnit().c_str(),
      speed_min_string,
      SpeedUnit().c_str(),
      speed_max_string,
      SpeedUnit().c_str(),
#endif  // USE_WINDMETER_NOSTATISTICS
      "n/a", //wind_direction_cardinal_string,
      "n/a" //wind_direction_string
#ifndef USE_WINDMETER_NOSTATISTICS
      ,"n/a", //,wind_direction_avg_cardinal_string,
      "n/a", //wind_direction_avg_string,
      "n/a", //wind_direction_range_string,
      "n/a", //wind_direction_min_string,
      "n/a" //wind_direction_max_string
#endif  // USE_WINDMETER_NOSTATISTICS
    );
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool Xsns68Cmnd(void)
{
  if (ArgC() > 1) {
    char argument[XdrvMailbox.data_len];
    switch (XdrvMailbox.payload) {
      case 1:
        Settings->windmeter_radius = (uint16_t)strtol(ArgV(argument, 2), nullptr, 10);
        break;
      case 2:
        Settings->windmeter_pulses_x_rot = (uint8_t)strtol(ArgV(argument, 2), nullptr, 10);
        break;
      case 3:
        Settings->windmeter_pulse_debounce = (uint16_t)strtol(ArgV(argument, 2), nullptr, 10);
        break;
      case 4:
        Settings->windmeter_speed_factor = (int16_t)(CharToFloat(ArgV(argument, 2)) * 1000);
        break;
      case 5:
        Settings->windmeter_tele_pchange = (uint8_t)strtol(ArgV(argument, 2), nullptr, 10);
        break;
    }
  }

  float speed_factor = (float)Settings->windmeter_speed_factor / 1000;
  char tele_pchange_string[4] = "off";
  if (Settings->windmeter_tele_pchange <= 100) {
    itoa(Settings->windmeter_tele_pchange, tele_pchange_string, 10);
  }
  Response_P(PSTR("{\"" D_WINDMETER_NAME "\":{\"Radius\":%d,\"PulsesPerRot\":%d,\"PulseDebounce\":%d,\"SpeedFactor\":%3_f,\"TeleTriggerMin%Change\":%s}}"),
        Settings->windmeter_radius, Settings->windmeter_pulses_x_rot, Settings->windmeter_pulse_debounce, &speed_factor, tele_pchange_string);

  return true;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns68(uint8_t function)
{
  bool result = false;
  if (PinUsed(GPIO_WINDMETER_SPEED)) {
    switch (function) {
      case FUNC_INIT:
        WindMeterInit();
        break;
      case FUNC_EVERY_SECOND:
        WindMeterEverySecond();
        break;
#ifndef USE_WINDMETER_NOSTATISTICS
      case FUNC_AFTER_TELEPERIOD:
        WindMeterResetStatData();
        break;
#endif  // USE_WINDMETER_NOSTATISTICS
      case FUNC_JSON_APPEND:
        WindMeterShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        WindMeterShow(false);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND_SENSOR:
        if (XSNS_68 == XdrvMailbox.index) {
          result = Xsns68Cmnd();
        }
    }
  }
  return result;
}

#endif  // USE_WINDMETER
