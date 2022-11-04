#include "Arduino.h"
#include "WiFi.h"
#include "String.h"
#include "stdlib.h"

//define a static variable to store the value of SSID
char * const WiFi_SSID = "Mi 10";
//define a static variable to store the value of passphrase
char * const WiFi_passphrase = "ch1994360101";
//define a string to store the value of new SSID
char * WiFi_SSID_new = "";
//define a static variable to store the value of new passphrase
char * WiFi_passphrase_new = NULL;
//define an array to store the value of serial received
char serialBuf[64]={0};
//define a variable to store the value of WiFi number
int num = 0;
//define a variable to store the vlaue of counter
int counter = 0;

uint8_t mystrlen(const char* buf)
{
  uint8_t len = strlen(buf);

  return len;
}

void setup() {
  //initialize serial communication
  Serial.begin(9600);
  // Set WiFi to station mode and set the automatic reconnect mode
  WiFi.mode(WIFI_STA);
  if(!WiFi.getAutoReconnect()){
    WiFi.setAutoReconnect(true);
  }
}

void loop() {
  //print communiaction of WiFi
  if(counter==500){
    counter=0;
    if(WiFi.status()!=WL_CONNECTED){
      WiFi.disconnect();
      int n = WiFi.scanNetworks();
      Serial.println("WiFi info list:");
      for(int i = 0;i < n;i++){
        Serial.print("SSID:");
        Serial.print(WiFi.SSID(i));
        Serial.print("\t");
        //Serial.print(strlen(WiFi.SSID(i).c_str()));
        Serial.print(mystrlen(WiFi.SSID(i).c_str()));
        Serial.print("\t");
        Serial.print("RSSI:");
        Serial.print(WiFi.RSSI(i));
        Serial.printf("\t %d\r\n",i);
      }
      Serial.println("You can seed the following operation characters:");
      Serial.println("c [tip:connect the network]");
      Serial.println("Done.");
    }else{
      Serial.println("Now:");
      Serial.printf("SSID:%s",WiFi.SSID());
      Serial.printf("RSSI:%d\r\n",WiFi.RSSI());
      Serial.println("You can seed the following operation characters:");
      Serial.println("d [tip:disconnect the network]");
      Serial.println("Done.");
    }
  }
  int bufSize = Serial.available();
  if(bufSize==1){
    bufSize = 0;
    //Serial.printf("bufSize=%d\r\n",bufSize);
    int ch = Serial.read();
    if(ch == 'd'){
      WiFi.disconnect();
    }else if(ch == 'c'){
      Serial.println("Input the WiFi number you want to connect to:");
      while(1){
        int bufSize = Serial.available();
        if(bufSize){
          for(int i = 0;i < bufSize;i++){
            serialBuf[i] = Serial.read();
          }
          bufSize = 0;
          num = atoi(serialBuf);
          //Serial.printf("num:%d\r\n",num);
          Serial.println("Input the passphrase of WiFi.");
          while(1){
            int n = Serial.available();
            if(n){
              for(int i = 0;i < n;i++){
                serialBuf[i] = Serial.read();
              }
              n = 0;
              WiFi_passphrase_new = serialBuf;
              //Serial.printf("WiFi_passphrase_new:%s\r\n",WiFi_passphrase_new);
              break;
            }
          }
          WiFi.begin(WiFi.SSID(num).c_str(),WiFi_passphrase_new);
          if(WiFi.waitForConnectResult(2000)!=WL_CONNECTED){
            Serial.println("SSID or passphrase is error.");
            Serial.println("Input the WiFi number you want to connect to:");
          }else{
            Serial.println("WiFi is connected.");
            break;
          }
        }
      }
    }
  }
  //A moderate amount of time delay
  delay(10);
  counter++;
}
