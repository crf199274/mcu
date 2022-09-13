#include "Arduino.h"
//#include <SPI.h>
#include "WiFi.h"

//define a static variable to store the value of SSID
char * const WiFi_SSID = "Mi 10";
//define a static variable to store the value of passphrase
char * const WiFi_passphrase = "ch1994360101";
//define a variable to store the value of the returned localIP
IPAddress WiFi_localIP;
//define a variable to store the value of the returned macAddress
byte WiFi_macAddress[6] = {0};
//define a variable to store the value of the returned gatewayIP
IPAddress WiFi_gatewayIP;
//define a variable to store the value of the returned subnetMask
IPAddress WiFi_subnetMask;
//define a variable to store the value of the status
uint8_t WiFi_status = 0;

void setup() {
  //initialize serial communication
  Serial.begin(9600);
  if(!WiFi.getAutoReconnect()){
    WiFi.setAutoReconnect(true);
  }
  //Start Wifi connection with a WPA2 Enterprise AP
  WiFi.begin(WiFi_SSID,WiFi_passphrase);
  while(!(WiFi.waitForConnectResult(5000)==WL_CONNECTED)){  
    WiFi.begin(WiFi_SSID,WiFi_passphrase);
  }
  Serial.printf("Connected…………\r\n");
}

void loop() {
  //print communiaction of WiFi
  WiFi_status = WiFi.status();
  if(WiFi_status == WL_IDLE_STATUS){
    Serial.printf("WL_IDLE_STATUS\r\n");
  }else if(WiFi_status == WL_NO_SSID_AVAIL){
    Serial.printf("WL_NO_SSID_AVAIL\r\n");
  }else if(WiFi_status == WL_SCAN_COMPLETED){
    Serial.printf("WL_SCAN_COMPLETED\r\n");
  }else if(WiFi_status == WL_CONNECTED){
    Serial.printf("WL_CONNECTED\r\n");
  }else if(WiFi_status == WL_CONNECT_FAILED){
    Serial.printf("WL_CONNECT_FAILED\r\n");
  }else if(WiFi_status == WL_CONNECTION_LOST){
    Serial.printf("WL_CONNECTION_LOST\r\n");
  }else{
    Serial.printf("WL_DISCONNECTED\r\n");
  }
  if(WiFi_status == WL_CONNECTED){
    Serial.printf("SSID:%s\r\n",WiFi.SSID());
    WiFi_localIP = WiFi.localIP();
//    Serial.printf("localIP:%s\r\n",WiFi_localIP);
    Serial.print("localIP:");
    Serial.println(WiFi_localIP);
//    Serial.printf("macAddress:%s\r\n",WiFi.macAddress());
    Serial.print("macAddress:");
    Serial.println(WiFi.macAddress());
    WiFi_gatewayIP = WiFi.gatewayIP();
//    Serial.printf("gatewayIP:%s\r\n",WiFi_gatewayIP);
    Serial.print("gatewayIP:");
    Serial.println(WiFi_gatewayIP);
    WiFi_subnetMask = WiFi.subnetMask();
//    Serial.printf("subnetMask:%s\r\n",WiFi_subnetMask);
    Serial.print("subnetMask:");
    Serial.println(WiFi_subnetMask);
  if(WiFi_status == WL_CONNECTED){
    int i = WiFi.scanNetworks();
    if(i){
      Serial.println("Scanned :");
      for(int j = 0;j <= i;j++){
        Serial.printf("SSID:%s\t",WiFi.SSID(i));
        Serial.printf("RSSI:%d\r\n",WiFi.RSSI(i));
      }
    }
  }
  }
  //A moderate amount of time delay
  delay(5000);
}
