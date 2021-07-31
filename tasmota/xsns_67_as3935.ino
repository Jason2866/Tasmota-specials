/*
  xsns_67_as3935.ino - AS3935 Franklin Lightning Sensor support for Tasmota

  Copyright (C) 2021  Martin Wagner

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
#ifdef USE_AS3935
/*********************************************************************************************\
 * AS3935 Lightning Sensor
 *
 * I2C Address: 0x03
\*********************************************************************************************/

#define XSNS_67             67
#define XI2C_48             48  // See I2CDEVICES.md

#define D_NAME_AS3935 "AS3935"
#define AS3935_ADDR   0x03

//  I2C Registers      Reg  mask shift
#define PWR_REG       0x00, 0x01, 0
#define AFE_GB        0x00, 0x3E, 0
#define WDTH          0x01, 0x0F, 0
#define NF_LEVEL      0x01, 0x70, 4
#define SPIKE_REJECT  0x02, 0x0F, 0
#define MIN_NUM_LIGH  0x02, 0x30, 4
#define DISTURBER     0x03, 0x20, 5
#define LCO_FDIV      0x03, 0xC0, 6
#define IRQ_TBL       0x03, 0x0F, 0
#define ENERGY_RAW_1  0x04, 0xFF, 0
#define ENERGY_RAW_2  0x05, 0xFF, 0
#define ENERGY_RAW_3  0x06, 0x1F, 0
#define LGHT_DIST     0x07, 0x3F, 0
#define DISP_TRCO     0x08, 0x20, 5 // should 31.250 kHz with devide 16
#define DISP_SRCO     0x08, 0x40, 6 // 1,1 MHz
#define DISP_LCO      0x08, 0x80, 7 // 32.768 kHz
#define TUNE_CAPS     0x08, 0x0F, 0
#define CAL_TRCO_NOK  0x3A, 0x40, 6 // 1 = NOK
#define CAL_TRCO_DONE 0x3A, 0x80, 7 // 0 = OK
#define CAL_SRCO_NOK  0x3B, 0x40, 6
#define CAL_SRCO_DONE 0x3B, 0x80, 7

// I2C Commands
#define RESET_DEFAULT 0x3C, 0x96
#define CALIBATE_RCO  0x3D, 0x96

// NF-Level
#define INDOORS       0x24
#define OUTDOORS      0x1C

// Load Settings Mask
#define SETREG00MASK     0x3E // For Power On
#define SETREG03MASK     0xF0 // For LCO and Disturber

// Http
const char HTTP_SNS_AS3935_ENERGY[] PROGMEM  = "{s}" D_NAME_AS3935 " " D_AS3935_ENERGY " {m}%d{e}";
const char HTTP_SNS_AS3935_DISTANZ[] PROGMEM = "{s}" D_NAME_AS3935 " " D_AS3935_DISTANCE " {m}%u " D_UNIT_KILOMETER "{e}";
const char HTTP_SNS_AS3935_VRMS[] PROGMEM = "{s}" D_NAME_AS3935 " " D_AS3935_VRMS "{m}%#4u (%d){e}";

const char HTTP_SNS_AS3935_OUTDOORS[] PROGMEM = "{s}%s " D_AS3935_GAIN " {m}" D_AS3935_OUTDOORS " {e}";
const char HTTP_SNS_AS3935_INDOORS[] PROGMEM = "{s}%s " D_AS3935_GAIN " {m}" D_AS3935_INDOORS " {e}";
const char* const HTTP_SNS_AS3935_GAIN[] PROGMEM = {HTTP_SNS_AS3935_INDOORS, HTTP_SNS_AS3935_OUTDOORS};

const char HTTP_SNS_AS3935_DIST_ON[] PROGMEM = "{s}%s " D_AS3935_DISTURBER " {m}" D_AS3935_ON " {e}";
const char HTTP_SNS_AS3935_DIST_OFF[] PROGMEM = "{s}%s " D_AS3935_DISTURBER " {m}" D_AS3935_OFF " {e}";
const char* const HTTP_SNS_AS3935_DISTURBER[] PROGMEM = {HTTP_SNS_AS3935_DIST_OFF, HTTP_SNS_AS3935_DIST_ON};
// http Messages
const char HTTP_SNS_AS3935_EMPTY[] PROGMEM = "{s}%s " D_AS3935_NOMESS "{e}";
const char HTTP_SNS_AS3935_OUT[] PROGMEM = "{s}%s " D_AS3935_OUT "{e}";
const char HTTP_SNS_AS3935_NOT[] PROGMEM = "{s}%s " D_AS3935_NOT "{e}";
const char HTTP_SNS_AS3935_ABOVE[] PROGMEM = "{s}%s " D_AS3935_ABOVE "{e}";
const char HTTP_SNS_AS3935_NOISE[] PROGMEM = "{s}%s " D_AS3935_NOISE "{e}";
const char HTTP_SNS_AS3935_DISTURB[] PROGMEM = "{s}%s " D_AS3935_DISTDET "{e}";
const char HTTP_SNS_AS3935_INTNOEV[] PROGMEM = "{s}%s " D_AS3935_INTNOEV "{e}";
const char HTTP_SNS_AS3935_FLICKER[] PROGMEM = "{s}%s " D_AS3935_FLICKER "{e}";
const char HTTP_SNS_AS3935_POWEROFF[] PROGMEM = "{s}%s " D_AS3935_POWEROFF "{e}";

const char HTTP_SNS_AS3935_MSG[] PROGMEM = "{s}%s " D_AS3935_LIGHT " " D_AS3935_APRX " %d " D_UNIT_KILOMETER " " D_AS3935_AWAY "{e}";
const char* const HTTP_SNS_AS3935_TABLE_1[] PROGMEM = { HTTP_SNS_AS3935_EMPTY, HTTP_SNS_AS3935_MSG, HTTP_SNS_AS3935_OUT, HTTP_SNS_AS3935_NOT, HTTP_SNS_AS3935_ABOVE, HTTP_SNS_AS3935_NOISE, HTTP_SNS_AS3935_DISTURB, HTTP_SNS_AS3935_INTNOEV, HTTP_SNS_AS3935_FLICKER, HTTP_SNS_AS3935_POWEROFF };
// Json
const char JSON_SNS_AS3935_EVENTS[] PROGMEM = ",\"%s\":{\"" D_JSON_EVENT "\":%d,\"" D_JSON_DISTANCE "\":%d,\"" D_JSON_ENERGY "\":%u,\"" D_JSON_STAGE "\":%d}";
// Json Command
const char* const S_JSON_AS3935_COMMAND_ONOFF[] PROGMEM = {"\"" D_AS3935_OFF "\"","\"" D_AS3935_ON"\""};
const char* const S_JSON_AS3935_COMMAND_GAIN[] PROGMEM = {"\"" D_AS3935_INDOORS "\"", "\"" D_AS3935_OUTDOORS "\""};
const char* const S_JSON_AS3935_COMMAND_CAL[] PROGMEM = {"" D_AS3935_CAL_FAIL "",""  D_AS3935_CAL_OK ""};

const char S_JSON_AS3935_COMMAND_STRING[] PROGMEM = "{\"" D_NAME_AS3935 "\":{\"%s\":%s}}";
const char S_JSON_AS3935_COMMAND_NVALUE[] PROGMEM = "{\"" D_NAME_AS3935 "\":{\"%s\":%d}}";
const char S_JSON_AS3935_COMMAND_SETTINGS[] PROGMEM = "{\"AS3935_Settings\":{\"Gain\":%s,\"NFfloor\":%d,\"uVrms\":%d,\"Tunecaps\":%d,\"MinNumLight\":%d,\"Rejection\":%d,\"Wdthreshold\":%d,\"MinNFstage\":%d,\"NFAutoTime\":%d,\"DisturberAutoTime\":%d,\"Disturber\":%s,\"NFauto\":%s,\"Disturberauto\":%s,\"NFautomax\":%s,\"Mqttlightevent\":%s,\"Mqttnoirqevent\":%s}}";

const char kAS3935_Commands[] PROGMEM  = "power|setnf|setminstage|setml|default|setgain|settunecaps|setrej|setwdth|disttime|nftime|disturber|autonf|autodisturber|autonfmax|lightevent|noirqevent|settings|calibrate";

const uint8_t AS3935_VrmsIndoor[] PROGMEM = { 28, 45, 62, 78, 95, 112, 130, 146 };
const uint16_t AS3935_VrmsOutdoor[] PROGMEM = { 390, 630, 860, 1100, 1140, 1570, 1800, 2000 };

enum AS3935_Commands {         // commands for Console
  CMND_AS3935_POWER,           // Power on/off the device (1 Bit)
  CMND_AS3935_SET_NF,          // Noise Floor Level, value from 0-7  (3 Bit)
  CMND_AS3935_SET_MINNF,       // Set Min Noise Floor Level when Autotune is active Value from 0-15
  CMND_AS3935_SET_MINLIGHT,    // Minimum number of lightning 0=1/1=5/2=9/3=16 Lightnings
  CMND_AS3935_SET_DEF,         // set default for Sensor and Settings
  CMND_AS3935_SET_GAIN,        // Set Inddoor/Outdoor
  CMND_AS3935_SET_TUNE,        // Internal Tuning Capacitors (from 0 to 120pF in steps of 8pf)
  CMND_AS3935_SET_REJ,         // Set Spike Rejection
  CMND_AS3935_SET_WDTH,        // Watchdog threshold
  CMND_AS3935_DISTTIME,        // Threshhold Time for Auto Disturber
  CMND_AS3935_NFTIME,          // Threshhold Time for NF-Autotune
  CMND_AS3935_SET_DISTURBER,   // Set Disturber on/off
  CMND_AS3935_NF_AUTOTUNE,     // Autotune the NF Noise
  CMND_AS3935_DIST_AUTOTUNE,   // Autotune Disturber on/off
  CMND_AS3935_NF_ATUNE_BOTH,   // Autotune over both Areas: INDOORS/OUDOORS
  CMND_AS3935_MQTT_LIGHT_EVT,  // mqtt only if lightning Irq
  CMND_AS3935_MQTT_NO_IRQ_EVT, // suppress mqtt "IRQ with no Event"
  CMND_AS3935_SETTINGS,        // Json output of all settings
  CMND_AS3935_CALIBRATE        // caps autocalibrate
};

struct {
  bool active = false;
  bool http_count_start = false;
  bool poweroff = false;
  volatile bool detected = false;
  volatile bool dispLCO = false;
  volatile uint8_t icount = 0;
  uint8_t irq = 0;
  uint8_t mqtt_event = 0;
  uint8_t http_event = 0;
  uint8_t http_time  = 0;
  uint8_t http_count = 0;
  int16_t http_distance = 0;
  int16_t distance = 0;
  uint16_t nftimer = 0;
  uint16_t disttimer = 0;
  uint32_t intensity = 0;
  uint32_t http_intensity = 0;
  volatile uint32_t pulse = 0;
} as3935_sensor;

void IRAM_ATTR AS3935Isr(void) {
  as3935_sensor.detected = true;
  as3935_sensor.icount++;
}

// we have to store 5 Bytes in the eeprom. Register 8 is mapped to Byte 4
uint8_t AS3935ReadRegister(uint8_t reg, uint8_t mask, uint8_t shift) {
  uint8_t data = I2cRead8(AS3935_ADDR, reg);
  if (reg == 0x08) Settings->as3935_sensor_cfg[4] = data;
  if (reg <= 0x03) Settings->as3935_sensor_cfg[reg] = data;
  return ((data & mask) >> shift);
}

void AS3935WriteRegister(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t data) {
  uint8_t currentReg = I2cRead8(AS3935_ADDR, reg);
  currentReg &= (~mask);
  data <<= shift;
  data &= mask;
  data |= currentReg;
  I2cWrite8(AS3935_ADDR, reg, data);
  if (reg == 0x08) Settings->as3935_sensor_cfg[4] = I2cRead8(AS3935_ADDR, reg);
  if (reg <= 0x03) Settings->as3935_sensor_cfg[reg] = I2cRead8(AS3935_ADDR, reg);
}

/********************************************************************************************/
// Autotune Caps
void IRAM_ATTR AS3935CountFreq(void) {
  if (as3935_sensor.dispLCO)
    as3935_sensor.pulse++;
}

bool AS3935AutoTuneCaps(uint8_t irqpin) {
  int32_t maxtune = 17500; // there max 3.5 % tol
  uint8_t besttune;
  uint8_t oldvalue = AS3935GetTuneCaps();
  AS3935WriteRegister(LCO_FDIV, 0); // Fdiv 16
  delay(2);
  for (uint8_t tune = 0; tune < 16; tune++) {
    AS3935WriteRegister(TUNE_CAPS, tune);
    delay(2);
    AS3935WriteRegister(DISP_LCO, 1);
    delay(1);
    as3935_sensor.dispLCO = true;
    as3935_sensor.pulse = 0;
    attachInterrupt(digitalPinToInterrupt(irqpin), AS3935CountFreq, RISING);
    delay(50);
    as3935_sensor.dispLCO = false;
    detachInterrupt(irqpin);
    AS3935WriteRegister(DISP_LCO, 0);
    int32_t currentfreq = 500000 - ((as3935_sensor.pulse * 20) * 16);
    if(currentfreq < 0) currentfreq = -currentfreq;
    if(maxtune > currentfreq) {
      maxtune = currentfreq;
      besttune = tune;
    }
  }
  if (maxtune >= 17500) { // max. 3.5%
    AS3935SetTuneCaps(oldvalue);
    return false;
  }
  AS3935SetTuneCaps(besttune);
  return true;
}

/********************************************************************************************/
// functions
bool AS3935CalRCOResult(void) {
  if(AS3935ReadRegister(CAL_SRCO_NOK) || AS3935ReadRegister(CAL_TRCO_NOK)) {
    AddLog(LOG_LEVEL_INFO, PSTR("I2C: AS3935 Fatal Failure of TRCO or SRCO calibration"));
    return false;
  }
  return true;
}

bool AS3935CalibrateRCO(void) {
  detachInterrupt(Pin(GPIO_AS3935));    // Prevent AS3935Isr from RCO Calibration
  I2cWrite8(AS3935_ADDR, CALIBATE_RCO); // Cal TRCO & SRCO
  AS3935WriteRegister(DISP_TRCO, 1);    // need for Power up
  delay(2);
  AS3935WriteRegister(DISP_TRCO, 0);
  if(!AS3935CalRCOResult())
    return false;
  attachInterrupt(digitalPinToInterrupt(Pin(GPIO_AS3935)), AS3935Isr, RISING);
  return true;
}

void AS3935Reset(void) {
  I2cWrite8(AS3935_ADDR, RESET_DEFAULT);
  delay(2);
}

void AS3935PwrDown(void) {
  AS3935WriteRegister(PWR_REG ,1);
  detachInterrupt(Pin(GPIO_AS3935));
  as3935_sensor.poweroff = true;
  as3935_sensor.mqtt_event = 9;
  as3935_sensor.http_event = 9;
  as3935_sensor.intensity = 0;
  as3935_sensor.distance = 0;
}

void AS3935PwrUp(void) {
  AS3935WriteRegister(PWR_REG ,0);
  AS3935CalibrateRCO();
  as3935_sensor.poweroff = false;
  as3935_sensor.mqtt_event = 0;
  as3935_sensor.http_event = 0;
}

uint8_t AS3935GetPwrStat(void) {
  if (AS3935ReadRegister(PWR_REG))
    return 0;
  return 1;
}

uint8_t AS3935GetIRQ(void) {
  delay(2);
  return AS3935ReadRegister(IRQ_TBL);
}

uint8_t AS3935TranslIrq(uint8_t irq, uint8_t distance) {
  switch(irq) {
    case 0: return 7;  // Interrupt with no IRQ
    case 1: return 5;  // Noise level too high
    case 4: return 6;  // Disturber detected
    case 8:
      if (distance == -1) return 2;  // Lightning out of Distance
      else if (distance == 0) return 3;  // Distance cannot be determined
      else if (distance == 1) return 4;  // Storm is Overhead
      else  return 1; // Lightning with Distance detected
  }
  return 0;  // Fix GCC 10.1 warning
}

uint8_t AS3935GetDistance(void) {
  return AS3935ReadRegister(LGHT_DIST);
}

int16_t AS3935CalcDistance(void) {
  uint8_t dist = AS3935GetDistance();
  switch (dist) {
    case 0x3F:  return -1;  // Out of Range
    case 0x00:  return 0;   // Distance cannot be determined
    case 0x01:  return 1;   // Storm is Overhead
    default:
      if (40 < dist) return 40; // limited because higher is not accurate
    return dist;
  }
}

uint32_t  AS3935GetIntensity(void) {
  uint32_t energy_raw = (AS3935ReadRegister(ENERGY_RAW_3) << 8);
  energy_raw |= AS3935ReadRegister(ENERGY_RAW_2);
  energy_raw <<= 8;
  energy_raw |= AS3935ReadRegister(ENERGY_RAW_1);
  return energy_raw;
}

uint8_t AS3935GetTuneCaps(void) {
  return AS3935ReadRegister(TUNE_CAPS);
}

void AS3935SetTuneCaps(uint8_t tune) {
  AS3935WriteRegister(TUNE_CAPS, tune);
  delay(2);
  AS3935CalibrateRCO();
}

uint8_t AS3935GetDisturber(void) {
  return AS3935ReadRegister(DISTURBER);
}

void AS3935SetDisturber(uint8_t stat) {
  AS3935WriteRegister(DISTURBER, stat);
}

uint8_t AS3935GetMinLights(void) {
  return AS3935ReadRegister(MIN_NUM_LIGH);
}

void AS3935SetMinLights(uint8_t stat) {
  AS3935WriteRegister(MIN_NUM_LIGH, stat);
}

uint8_t AS3935TransMinLights(uint8_t min_lights) {
  if (5 > min_lights) return 0;
   else if (9 > min_lights) return 1;
   else if (16 > min_lights) return 2;
   else return 3;
}

uint8_t AS3935TranslMinLightsInt(uint8_t min_lights) {
  switch (min_lights) {
    case 0:   return 1;
    case 1:   return 5;
    case 2:   return 9;
    case 3:   return 16;
  }
  return 0;  // Fix GCC 10.1 warning
}

uint8_t AS3935GetNoiseFloor(void) {
  return AS3935ReadRegister(NF_LEVEL);
}

void AS3935SetNoiseFloor(uint8_t noise) {
   AS3935WriteRegister(NF_LEVEL , noise);
}

uint8_t AS3935GetGain(void) {
  if (AS3935ReadRegister(AFE_GB) == OUTDOORS)
    return OUTDOORS;
  return INDOORS;
}

void AS3935SetGain(uint8_t room) {
  AS3935WriteRegister(AFE_GB, room);
}

uint8_t AS3935GetGainInt(void) {
  if (AS3935ReadRegister(AFE_GB) == OUTDOORS)
    return 1;
  return 0;
}

void AS3935CalcVrmsLevel(uint16_t &vrms, uint8_t &stage) {
  uint8_t room = AS3935GetGain();
  uint8_t nflev = AS3935GetNoiseFloor();
  if (room == INDOORS) {
    vrms = pgm_read_byte(AS3935_VrmsIndoor + nflev);
    stage = nflev;
  } else {
    vrms = pgm_read_word(AS3935_VrmsOutdoor + nflev);
    stage = nflev + 8;
  }
}

uint8_t AS3935GetSpikeRejection(void) {
  return AS3935ReadRegister(SPIKE_REJECT);
}

void AS3935SetSpikeRejection(uint8_t rej) {
  AS3935WriteRegister(SPIKE_REJECT, rej);
}

uint8_t AS3935GetWdth(void) {
  return AS3935ReadRegister(WDTH);
}

void AS3935SetWdth(uint8_t wdth) {
  AS3935WriteRegister(WDTH, wdth);
}

bool AS3935AutoTune(void) {
  detachInterrupt(Pin(GPIO_AS3935));
  bool result = AS3935AutoTuneCaps(Pin(GPIO_AS3935));
  return result;
}

/********************************************************************************************/
// Noise Floor autofunctions
bool AS3935LowerNoiseFloor(void) {
  uint16_t vrms;
  uint8_t stage;
  AS3935CalcVrmsLevel(vrms, stage);
  if (Settings->as3935_functions.nf_autotune_both) {
    if (stage == 8 && stage > Settings->as3935_parameter.nf_autotune_min) {
      AS3935SetGain(INDOORS);
      AS3935SetNoiseFloor(7);
      return true;
    }
  }
  uint8_t noise = AS3935GetNoiseFloor();
  if (0 < noise && stage > Settings->as3935_parameter.nf_autotune_min) {
    noise--;
    AS3935SetNoiseFloor(noise);
    return true;
  }
  return false;
}

bool AS3935RaiseNoiseFloor(void) {
  uint8_t noise = AS3935GetNoiseFloor();
  uint8_t room = AS3935GetGain();
  if (Settings->as3935_functions.nf_autotune_both) {
    if (7 == noise && room == INDOORS) {
      AS3935SetGain(OUTDOORS);
      AS3935SetNoiseFloor(0);
      return true;
    }
  }
  if (7 > noise) {
    noise++;
    AS3935SetNoiseFloor(noise);
    return true;
  }
  return false;
}

/********************************************************************************************/
// init functions
bool AS3935SetDefault(void) {
  AS3935Reset();
  AS3935SetDisturber(1); // Disturber on by default
  AS3935SetNoiseFloor(7); //  NF High on by default
  Settings->as3935_sensor_cfg[0] = I2cRead8(AS3935_ADDR, 0x00);
  Settings->as3935_sensor_cfg[1] = I2cRead8(AS3935_ADDR, 0x01);
  Settings->as3935_sensor_cfg[2] = I2cRead8(AS3935_ADDR, 0x02);
  Settings->as3935_sensor_cfg[3] = I2cRead8(AS3935_ADDR, 0x03);
  Settings->as3935_sensor_cfg[4] = I2cRead8(AS3935_ADDR, 0x08);
  // set all eeprom functions and values to default
  Settings->as3935_functions.data = 0x00;
  Settings->as3935_parameter.nf_autotune_min = 0x00;
  Settings->as3935_parameter.nf_autotune_time = 4;
  Settings->as3935_parameter.dist_autotune_time = 1;
  return true;
}

void AS3935InitSettings(void) {
  if(Settings->as3935_functions.nf_autotune) {
    if(Settings->as3935_parameter.nf_autotune_min) {
      if (Settings->as3935_parameter.nf_autotune_min > 7) {
        AS3935SetGain(OUTDOORS);
        AS3935SetNoiseFloor(Settings->as3935_parameter.nf_autotune_min - 8);
      } else {
        AS3935SetGain(INDOORS);
        AS3935SetNoiseFloor(Settings->as3935_parameter.nf_autotune_min);
      }
    }
  }
  I2cWrite8(AS3935_ADDR, 0x00, Settings->as3935_sensor_cfg[0] & SETREG00MASK);
  I2cWrite8(AS3935_ADDR, 0x01, Settings->as3935_sensor_cfg[1]);
  I2cWrite8(AS3935_ADDR, 0x02, Settings->as3935_sensor_cfg[2]);
  I2cWrite8(AS3935_ADDR, 0x03, Settings->as3935_sensor_cfg[3] & SETREG03MASK);
  I2cWrite8(AS3935_ADDR, 0x08, Settings->as3935_sensor_cfg[4]);
  delay(2);
}

bool AS3935Setup(void) {
  if (Settings->as3935_sensor_cfg[0] == 0x00) {
    AS3935SetDefault();
  } else {
    AS3935InitSettings();
  }
  return AS3935CalibrateRCO();
}

bool AS3935init(void) {
  AS3935Reset();
  uint8_t afe_gb = I2cRead8(AS3935_ADDR, 0x00) & SETREG00MASK;
  if(INDOORS == afe_gb)
    return true;
  return false;
}

void AS3935Detect(void) {
  if (!I2cSetDevice(AS3935_ADDR)) return;
  if (AS3935init()) {
    I2cSetActiveFound(AS3935_ADDR, D_NAME_AS3935);
    if (PinUsed(GPIO_AS3935)) {
      pinMode(Pin(GPIO_AS3935), INPUT);
        if (!AS3935Setup()) return;
      as3935_sensor.active = true;
    } else {
      AddLog(LOG_LEVEL_INFO, PSTR("I2C: AS3935 GPIO Pin not defined!"));
    }
  }
}

void AS3935EverySecond(void) {
  if (!as3935_sensor.poweroff) { // Power Off
    if (as3935_sensor.detected) {
      as3935_sensor.detected = false;
      as3935_sensor.irq = AS3935GetIRQ(); // 1 = Noise, 4 = Disturber, 8 = storm

      if (10 > as3935_sensor.icount) {
        switch (as3935_sensor.irq) {
        case 1:
          if (Settings->as3935_functions.nf_autotune) {
            if (AS3935RaiseNoiseFloor())
              as3935_sensor.nftimer = 0;
          }
          break;
        case 4:
          if (Settings->as3935_functions.dist_autotune) {
            AS3935SetDisturber(1);
          }
          break;
        case 8:
          as3935_sensor.intensity = AS3935GetIntensity();
          as3935_sensor.distance = AS3935CalcDistance();
          as3935_sensor.http_intensity = as3935_sensor.intensity;
          as3935_sensor.http_distance = as3935_sensor.distance;
          break;
        }
        // http show
        as3935_sensor.http_event = AS3935TranslIrq(as3935_sensor.irq, as3935_sensor.distance);
      } else {
        as3935_sensor.http_event = 8; // flicker detected
      }

      // mqtt publish
      as3935_sensor.mqtt_event = as3935_sensor.http_event;

      switch (as3935_sensor.mqtt_event) {
      case 5:
      case 6:
        if (!Settings->as3935_functions.mqtt_only_Light_Event) {
          MqttPublishSensor();
          as3935_sensor.http_time = 10;
        }
        break;
      case 7:
        if (!Settings->as3935_functions.suppress_irq_no_Event) {
          MqttPublishSensor();
          as3935_sensor.http_time = 10;
        }
        break;
      default:
        as3935_sensor.http_time = 30;
        MqttPublishSensor();
      }

      as3935_sensor.irq = 0;
      // clear mqtt events for Teleperiod
      as3935_sensor.intensity = 0;
      as3935_sensor.distance = 0;
      as3935_sensor.mqtt_event = 0;
      // start http times
      as3935_sensor.http_count_start = true;
      as3935_sensor.http_count = 0;
    }
    as3935_sensor.icount = 0;

    // count http times
    if (as3935_sensor.http_count_start)
      as3935_sensor.http_count++;
    // clear Http Event
    if (as3935_sensor.http_count > as3935_sensor.http_time) {
      as3935_sensor.http_count_start = false;
      as3935_sensor.http_intensity = 0;
      as3935_sensor.http_distance = 0;
      as3935_sensor.http_event = 0;
    }
    // Noise Floor Autotune function
    if (Settings->as3935_functions.nf_autotune) {
      as3935_sensor.nftimer++;
      if (as3935_sensor.nftimer > Settings->as3935_parameter.nf_autotune_time * 60) {
        AS3935LowerNoiseFloor();
        as3935_sensor.nftimer = 0;
      }
    }
    // Disturber auto function
    if (Settings->as3935_functions.dist_autotune) {
      if (AS3935GetDisturber()) {
        as3935_sensor.disttimer++;
      }
      if (as3935_sensor.disttimer >= Settings->as3935_parameter.dist_autotune_time * 60) {
        AS3935SetDisturber(0);
        as3935_sensor.disttimer = 0;
      }
    }
  }
}

bool AS3935Cmd(void) {
  char command[CMDSZ];
  uint8_t name_len = strlen(D_NAME_AS3935);
  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_NAME_AS3935), name_len)) {
    uint32_t command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + name_len, kAS3935_Commands);
    switch (command_code) {
      case CMND_AS3935_POWER:
        if (XdrvMailbox.data_len) {
          if (!XdrvMailbox.payload) {
            AS3935PwrDown();
          } else {
            AS3935PwrUp();
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_STRING, command, S_JSON_AS3935_COMMAND_ONOFF[AS3935GetPwrStat()]);
        break;
      case CMND_AS3935_SET_NF:
        if (XdrvMailbox.data_len) {
          if (15 >= XdrvMailbox.payload) {
            AS3935SetNoiseFloor(XdrvMailbox.payload);
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_NVALUE, command, AS3935GetNoiseFloor());
        break;
      case  CMND_AS3935_SET_MINNF:
        if (XdrvMailbox.data_len) {
          if (15 >= XdrvMailbox.payload) {
            Settings->as3935_parameter.nf_autotune_min = XdrvMailbox.payload;
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_NVALUE, command, Settings->as3935_parameter.nf_autotune_min);
        break;
      case CMND_AS3935_SET_MINLIGHT:
        if (XdrvMailbox.data_len) {
          AS3935SetMinLights(AS3935TransMinLights(XdrvMailbox.payload));
        }
        Response_P(S_JSON_AS3935_COMMAND_NVALUE, command, AS3935TranslMinLightsInt(AS3935GetMinLights()));
        break;
      case CMND_AS3935_SET_DEF:
        if (!XdrvMailbox.data_len) {
          Response_P(S_JSON_AS3935_COMMAND_NVALUE, command, AS3935SetDefault());
        }
        break;
      case CMND_AS3935_SET_GAIN:
        if (XdrvMailbox.data_len > 6) {
          uint8_t data_len = strlen(D_AS3935_OUTDOORS);
          if (!strncasecmp_P(XdrvMailbox.data, PSTR(D_AS3935_OUTDOORS), data_len)) {
            AS3935SetGain(OUTDOORS);
          } else {
            AS3935SetGain(INDOORS);
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_STRING, command, S_JSON_AS3935_COMMAND_GAIN[AS3935GetGainInt()]);
        break;
      case CMND_AS3935_SET_TUNE:
        if (XdrvMailbox.data_len) {
          if (15 >= XdrvMailbox.payload) {
            AS3935SetTuneCaps(XdrvMailbox.payload);
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_NVALUE, command, AS3935GetTuneCaps());
        break;
      case CMND_AS3935_SET_REJ:
        if (XdrvMailbox.data_len) {
          if (15 >= XdrvMailbox.payload) {
            AS3935SetSpikeRejection(XdrvMailbox.payload);
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_NVALUE, command, AS3935GetSpikeRejection());
        break;
      case CMND_AS3935_SET_WDTH:
        if (XdrvMailbox.data_len) {
          if (15 >= XdrvMailbox.payload) {
            AS3935SetWdth(XdrvMailbox.payload);
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_NVALUE, command, AS3935GetWdth());
        break;
      case CMND_AS3935_DISTTIME:
        if (XdrvMailbox.data_len) {
          if (15 >= XdrvMailbox.payload) {
           Settings->as3935_parameter.dist_autotune_time = XdrvMailbox.payload;
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_NVALUE, command, Settings->as3935_parameter.dist_autotune_time);
        break;
      case CMND_AS3935_NFTIME:
        if (XdrvMailbox.data_len) {
          if (15 >= XdrvMailbox.payload) {
           Settings->as3935_parameter.nf_autotune_time = XdrvMailbox.payload;
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_NVALUE, command, Settings->as3935_parameter.nf_autotune_time);
        break;
      case CMND_AS3935_SET_DISTURBER:
        if (XdrvMailbox.data_len) {
          if (2 > XdrvMailbox.payload) {
            AS3935SetDisturber(XdrvMailbox.payload);
            if (!XdrvMailbox.payload) Settings->as3935_functions.dist_autotune = 0;
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_STRING, command, S_JSON_AS3935_COMMAND_ONOFF[AS3935GetDisturber()]);
        break;
      case CMND_AS3935_NF_AUTOTUNE:
        if (XdrvMailbox.data_len) {
          if (2 > XdrvMailbox.payload) {
            Settings->as3935_functions.nf_autotune = XdrvMailbox.payload;
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_STRING, command, S_JSON_AS3935_COMMAND_ONOFF[Settings->as3935_functions.nf_autotune]);
        break;
      case CMND_AS3935_DIST_AUTOTUNE:
        if (XdrvMailbox.data_len) {
          if (2 > XdrvMailbox.payload) {
            Settings->as3935_functions.dist_autotune = XdrvMailbox.payload;
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_STRING, command, S_JSON_AS3935_COMMAND_ONOFF[Settings->as3935_functions.dist_autotune]);
        break;
      case CMND_AS3935_NF_ATUNE_BOTH:
        if (XdrvMailbox.data_len) {
          if (2 > XdrvMailbox.payload) {
            Settings->as3935_functions.nf_autotune_both = XdrvMailbox.payload;
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_STRING, command, S_JSON_AS3935_COMMAND_ONOFF[Settings->as3935_functions.nf_autotune_both]);
        break;
      case CMND_AS3935_MQTT_LIGHT_EVT:
        if (XdrvMailbox.data_len) {
          if (2 > XdrvMailbox.payload) {
            Settings->as3935_functions.mqtt_only_Light_Event = XdrvMailbox.payload;
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_STRING, command, S_JSON_AS3935_COMMAND_ONOFF[Settings->as3935_functions.mqtt_only_Light_Event]);
        break;
      case CMND_AS3935_MQTT_NO_IRQ_EVT:
        if (XdrvMailbox.data_len) {
          if (2 > XdrvMailbox.payload) {
            Settings->as3935_functions.suppress_irq_no_Event = XdrvMailbox.payload;
          }
        }
        Response_P(S_JSON_AS3935_COMMAND_STRING, command, S_JSON_AS3935_COMMAND_ONOFF[Settings->as3935_functions.suppress_irq_no_Event]);
        break;
      case CMND_AS3935_SETTINGS: {
        if (!XdrvMailbox.data_len) {
          uint8_t gain = AS3935GetGainInt();
          uint16_t vrms;
          uint8_t stage;
          AS3935CalcVrmsLevel(vrms, stage);
          uint8_t nf_floor = AS3935GetNoiseFloor();
          uint8_t tunecaps = AS3935GetTuneCaps();
          uint8_t minnumlight = AS3935TranslMinLightsInt(AS3935GetMinLights());
          uint8_t disturber = AS3935GetDisturber();
          uint8_t reinj = AS3935GetSpikeRejection();
          uint8_t wdth = AS3935GetWdth();
          uint8_t min_nf = Settings->as3935_parameter.nf_autotune_min;
          uint8_t nf_time = Settings->as3935_parameter.nf_autotune_time;
          uint8_t nfauto = Settings->as3935_functions.nf_autotune;
          uint8_t nfautomax = Settings->as3935_functions.nf_autotune_both;
          uint8_t distauto = Settings->as3935_functions.dist_autotune;
          uint8_t jsonlight = Settings->as3935_functions.mqtt_only_Light_Event;
          uint8_t jsonirq = Settings->as3935_functions.suppress_irq_no_Event;
          uint8_t dist_time = Settings->as3935_parameter.dist_autotune_time;
          Response_P(S_JSON_AS3935_COMMAND_SETTINGS, S_JSON_AS3935_COMMAND_GAIN[gain], nf_floor, vrms, tunecaps, minnumlight, reinj, wdth, min_nf, nf_time, dist_time, S_JSON_AS3935_COMMAND_ONOFF[disturber], S_JSON_AS3935_COMMAND_ONOFF[nfauto], S_JSON_AS3935_COMMAND_ONOFF[distauto], S_JSON_AS3935_COMMAND_ONOFF[nfautomax], S_JSON_AS3935_COMMAND_ONOFF[jsonlight], S_JSON_AS3935_COMMAND_ONOFF[jsonirq]);
        }
      }
        break;
      case CMND_AS3935_CALIBRATE: {
       bool calreslt;
       if (!XdrvMailbox.data_len) calreslt = AS3935AutoTune();
       Response_P(S_JSON_AS3935_COMMAND_NVALUE, S_JSON_AS3935_COMMAND_CAL[calreslt], AS3935GetTuneCaps());
      }
        break;
      default:
        return false;
    }
    return true;
  } else {
    return false;
  }
}

void AH3935Show(bool json) {
  if (json) {
    uint16_t vrms;
    uint8_t stage;
    AS3935CalcVrmsLevel(vrms, stage);
    ResponseAppend_P(JSON_SNS_AS3935_EVENTS, D_SENSOR_AS3935, as3935_sensor.mqtt_event, as3935_sensor.distance, as3935_sensor.intensity, stage);
#ifdef USE_WEBSERVER
  } else {
    uint8_t gain = AS3935GetGainInt();
    uint8_t disturber = AS3935GetDisturber();
    uint16_t vrms;
    uint8_t stage;
    AS3935CalcVrmsLevel(vrms, stage);

    WSContentSend_PD(HTTP_SNS_AS3935_TABLE_1[as3935_sensor.http_event], D_NAME_AS3935, as3935_sensor.http_distance);
    WSContentSend_PD(HTTP_SNS_AS3935_DISTANZ, as3935_sensor.http_distance);
    WSContentSend_PD(HTTP_SNS_AS3935_ENERGY, as3935_sensor.http_intensity);
    WSContentSend_PD(HTTP_SNS_AS3935_GAIN[gain], D_NAME_AS3935);
    WSContentSend_PD(HTTP_SNS_AS3935_DISTURBER[disturber], D_NAME_AS3935);
    WSContentSend_PD(HTTP_SNS_AS3935_VRMS, vrms, stage);

#endif // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns67(uint8_t function) {
  if (!I2cEnabled(XI2C_48)) { return false; }
  bool result = false;
  if (FUNC_INIT == function) {
    AS3935Detect();
  }
  else if (as3935_sensor.active) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        AS3935EverySecond();
        break;
      case FUNC_COMMAND:
        result = AS3935Cmd();
        break;
      case FUNC_JSON_APPEND:
        AH3935Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        AH3935Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_AS3935
#endif  // USE_I2C
