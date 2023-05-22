/****************************************************************************************************************************
  MQTTClient_SSL_Auth.ino - Dead simple SSL MQTT Client for Ethernet shields

  For Ethernet shields using WT32_ETH01 (ESP32 + LAN8720)

  WebServer_WT32_ETH01 is a library for the Ethernet LAN8720 in WT32_ETH01 to run WebServer

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/WebServer_WT32_ETH01
  Licensed under MIT license
 *****************************************************************************************************************************/

/*
  Basic MQTT example (with SSL!) with Authentication
  This sketch demonstrates the basic capabilities of the library.
  It connects to an MQTT server then:
  - providing username and password ("testuser" and "testpass")
  - publishes "Hello from MQTTClient_SSL_Auth on WT32-ETH01" to the topic "MQTT_Pub"
  - subscribes to the topic "MQTT_Sub", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.
*/

#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_       3

// Uncomment to use ESP32 core v1.0.6-
//#define USING_CORE_ESP32_CORE_V200_PLUS     false

#include <WebServer_WT32_ETH01.h>

#include <WiFiClientSecure.h>

// Select the IP address according to your local network
IPAddress myIP(192, 168, 2, 232);
IPAddress myGW(192, 168, 2, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);

#include "certificates.h"
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* mqttServer = "broker.emqx.io";        // Broker address

const char *TOPIC     = "MQTT_Pub";               // Topic to subcribe to
const char *subTopic  = "MQTT_Sub";               // Topic to subcribe to

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (unsigned int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  
  Serial.println();
}

WiFiClientSecure ethClientSSL;
  
PubSubClient client(mqttServer, 8883, callback, ethClientSSL);

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTTS connection to ");
    Serial.print(mqttServer);
    
    // Attempt to connect
    if (client.connect("arduinoClient", "testuser", "testpass"))
    {
      Serial.println("...connected");
      
      // Once connected, publish an announcement...
      client.publish(TOPIC, "Hello World from WT32_ETH01");
      // ... and resubscribe
      client.subscribe(subTopic);
      // for loopback testing
      client.subscribe(TOPIC);
    } 
    else 
    {
      Serial.print(" => failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Not sure if WiFiClientSecure checks the validity date of the certificate.
// Setting clock just to be sure...
void setClock()
{
  configTime(0, 0, "pool.ntp.org");

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);

  while (nowSecs < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  // Using this if Serial debugging is not necessary or not using Serial port
  //while (!Serial && (millis() < 3000));

  Serial.print("\nStarting MQTTClient_SSL_Auth on " + String(ARDUINO_BOARD));
  Serial.println(" with " + String(SHIELD_TYPE));
  Serial.println(WEBSERVER_WT32_ETH01_VERSION);

  //bool begin(uint8_t phy_addr=ETH_PHY_ADDR, int power=ETH_PHY_POWER, int mdc=ETH_PHY_MDC, int mdio=ETH_PHY_MDIO,
  //           eth_phy_type_t type=ETH_PHY_TYPE, eth_clock_mode_t clk_mode=ETH_CLK_MODE);
  //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);

  // Static IP, leave without this line to get IP via DHCP
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  ETH.config(myIP, myGW, mySN, myDNS);

  WT32_ETH01_onEvent();

  WT32_ETH01_waitForConnect();

  setClock();

  ethClientSSL.setCACert(rootCACertificate);
  
  // Note - the default maximum packet size is 128 bytes. If the
  // combined length of clientId, username and password exceed this use the
  // following to increase the buffer size:
   client.setBufferSize(255);
}

#define MQTT_PUBLISH_INTERVAL_MS       10000L

String data = "Hello from MQTTClient_SSL_Auth on " + String(BOARD_NAME);
const char *pubData = data.c_str();

unsigned long lastMsg = 0;

void loop() 
{
  static unsigned long now;
  
  if (!client.connected()) 
  {
    reconnect();
  }

  // Sending Data
  now = millis();
  
  if (now - lastMsg > MQTT_PUBLISH_INTERVAL_MS)
  {
    lastMsg = now;

    if (!client.publish(TOPIC, pubData))
    {
      Serial.println("Message failed to send.");
    }

    Serial.print("Message Send : " + String(TOPIC) + " => ");
    Serial.println(data);
  }
  
  client.loop();
}