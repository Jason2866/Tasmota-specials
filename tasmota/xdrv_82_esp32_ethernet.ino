/*
  xdrv_82_esp32_ethernet.ino - ESP32 (PoE) ethernet support for Tasmota

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

#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32
#ifdef USE_ETHERNET
/*********************************************************************************************\
 * Ethernet support for ESP32
 *
 * Dedicated fixed Phy pins
 * GPIO17 - EMAC_CLK_OUT_180
 * GPIO19 - EMAC_TXD0(RMII)
 * GPIO21 - EMAC_TX_EN(RMII)
 * GPIO22 - EMAC_TXD1(RMII)
 * GPIO25 - EMAC_RXD0(RMII)
 * GPIO26 - EMAC_RXD1(RMII)
 * GPIO27 - EMAC_RX_CRS_DV
 *
 * {"NAME":"Olimex ESP32-PoE","GPIO":[1,1,1,1,1,1,0,0,5536,1,1,1,1,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
 * GPIO12 = ETH POWER
 * GPIO18 = ETH MDIO
 * GPIO23 = ETH MDC
 * #define ETH_TYPE          ETH_PHY_LAN8720
 * #define ETH_CLKMODE       ETH_CLOCK_GPIO17_OUT
 * #define ETH_ADDRESS       0
 *
 * {"NAME":"wESP32","GPIO":[0,0,1,0,1,1,0,0,1,1,1,1,5568,5600,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
 * GPIO16 = ETH MDC
 * GPIO17 = ETH MDIO
 * #define ETH_TYPE          ETH_PHY_LAN8720
 * #define ETH_CLKMODE       ETH_CLOCK_GPIO0_IN
 * #define ETH_ADDRESS       0
 *
 * {"NAME":"WT32-ETH01","GPIO":[1,1,1,1,1,1,0,0,1,0,1,1,3840,576,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1],"FLAG":0,"BASE":1}
 * GPIO16 = Force Hi
 * GPIO18 = ETH MDIO
 * GPIO23 = ETH MDC
 * #define ETH_TYPE          ETH_PHY_LAN8720
 * #define ETH_CLKMODE       ETH_CLOCK_GPIO0_IN
 * #define ETH_ADDRESS       1
 *
\*********************************************************************************************/

#define XDRV_82           82

/*
// Olimex ESP32-PoE
#define ETH_CLKMODE       ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN     12

//********************************************************************************************

#ifndef ETH_ADDRESS
#define ETH_ADDRESS       0                      // ETH.h uint8_t:          0 = PHY0 .. 31 = PHY31
#endif

#ifndef ETH_TYPE
#define ETH_TYPE          ETH_PHY_LAN8720        // ETH.h eth_phy_type_t:   0 = ETH_PHY_LAN8720, 1 = ETH_PHY_TLK110/ETH_PHY_IP101, 2 = ETH_PHY_RTL8201, 3 = ETH_PHY_DP83848, 4 = ETH_PHY_DM9051, 5 = ETH_PHY_KSZ8081

#endif

#ifndef ETH_CLKMODE
#define ETH_CLKMODE       ETH_CLOCK_GPIO0_IN     // ETH.h eth_clock_mode_t: 0 = ETH_CLOCK_GPIO0_IN, 1 = ETH_CLOCK_GPIO0_OUT, 2 = ETH_CLOCK_GPIO16_OUT, 3 = ETH_CLOCK_GPIO17_OUT
#endif
*/

#include <ETH.h>

char eth_hostname[sizeof(TasmotaGlobal.hostname)];

void EthernetEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      AddLog(LOG_LEVEL_DEBUG, PSTR("ETH: " D_ATTEMPTING_CONNECTION));
      ETH.setHostname(eth_hostname);
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      AddLog(LOG_LEVEL_INFO, PSTR("ETH: " D_CONNECTED " at %dMbps%s"),
        ETH.linkSpeed(), (ETH.fullDuplex()) ? " Full Duplex" : "");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      AddLog(LOG_LEVEL_DEBUG, PSTR("ETH: Mac %s, IPAddress %_I, Hostname %s"),
        ETH.macAddress().c_str(), (uint32_t)ETH.localIP(), eth_hostname);
      Settings->ipv4_address[1] = (uint32_t)ETH.gatewayIP();
      Settings->ipv4_address[2] = (uint32_t)ETH.subnetMask();
      Settings->ipv4_address[3] = (uint32_t)ETH.dnsIP();
      Settings->ipv4_address[4] = (uint32_t)ETH.dnsIP(1);
      TasmotaGlobal.global_state.eth_down = 0;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      AddLog(LOG_LEVEL_INFO, PSTR("ETH: Disconnected"));
      TasmotaGlobal.global_state.eth_down = 1;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      AddLog(LOG_LEVEL_DEBUG, PSTR("ETH: Stopped"));
      TasmotaGlobal.global_state.eth_down = 1;
      break;
    default:
      break;
  }
}

void EthernetInit(void) {
  if (!Settings->flag4.network_ethernet) { return; }
  if (!PinUsed(GPIO_ETH_PHY_MDC) && !PinUsed(GPIO_ETH_PHY_MDIO)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("ETH: No ETH MDC and/or ETH MDIO GPIO defined"));
    return;
  }

  if (WT32_ETH01 == TasmotaGlobal.module_type) {
    Settings->eth_address = 1;                    // EthAddress
    Settings->eth_type = ETH_PHY_LAN8720;         // EthType
    Settings->eth_clk_mode = ETH_CLOCK_GPIO0_IN;  // EthClockMode
  }

//  snprintf_P(Eth.hostname, sizeof(Eth.hostname), PSTR("%s_eth"), TasmotaGlobal.hostname);
  strlcpy(eth_hostname, TasmotaGlobal.hostname, sizeof(eth_hostname) -5);  // Make sure there is room for "_eth"
  strcat(eth_hostname, "_eth");

  WiFi.onEvent(EthernetEvent);

  int eth_power = Pin(GPIO_ETH_PHY_POWER);
  int eth_mdc = Pin(GPIO_ETH_PHY_MDC);
  int eth_mdio = Pin(GPIO_ETH_PHY_MDIO);
  if (!ETH.begin(Settings->eth_address, eth_power, eth_mdc, eth_mdio, (eth_phy_type_t)Settings->eth_type, (eth_clock_mode_t)Settings->eth_clk_mode)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("ETH: Bad PHY type or init error"));
  };
}

IPAddress EthernetLocalIP(void) {
  return ETH.localIP();
}

char* EthernetHostname(void) {
  return eth_hostname;
}

String EthernetMacAddress(void) {
  return ETH.macAddress();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMND_ETHADDRESS "EthAddress"
#define D_CMND_ETHTYPE "EthType"
#define D_CMND_ETHCLOCKMODE "EthClockMode"

const char kEthernetCommands[] PROGMEM = "|"  // No prefix
  D_CMND_ETHERNET "|" D_CMND_ETHADDRESS "|" D_CMND_ETHTYPE "|" D_CMND_ETHCLOCKMODE;

void (* const EthernetCommand[])(void) PROGMEM = {
  &CmndEthernet, &CmndEthAddress, &CmndEthType, &CmndEthClockMode };

void CmndEthernet(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->flag4.network_ethernet = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndStateText(Settings->flag4.network_ethernet);
}

void CmndEthAddress(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 31)) {
    Settings->eth_address = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndNumber(Settings->eth_address);
}

void CmndEthType(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
    Settings->eth_type = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndNumber(Settings->eth_type);
}

void CmndEthClockMode(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->eth_clk_mode = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndNumber(Settings->eth_clk_mode);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv82(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kEthernetCommands, EthernetCommand);
      break;
    case FUNC_INIT:
      EthernetInit();
      break;
  }
  return result;
}

#endif  // USE_ETHERNET
#endif  // CONFIG_IDF_TARGET_ESP32
#endif  // ESP32
