/**
   StreamHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;



void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();
  
  
  
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SSID", "PASSWORD");
  Serial.printf("[SETUP] Connected!!!\n");
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    
    
  
  //char* callsign = "TA4APS";
  //char* pass = "17329";
  //char* latitude = "3751.58N";
  //char* longitude = "02716.16E";
  //char* comment = "Op. Ali 433.5000Mhz QRV 73!!";
  //char* symbol1 = "/";
  //char* symbol2 = "-";
  
  send2APRS("TA4APS","17329","3751.58N","02716.16E","Op. Ali 433.5000Mhz QRV","/","-");
  
  send2APRS("TA4APT","17334","3751.59N","02716.17E","Op. Istemihan","/","-");

  send2APRS("TA4IDR","22456","3751.52N","02715.98E","433.5000 QRV","/","-");

  
  }
  //Wait for 15min
  delay(1000*60*15);
}

void send2APRS(char* callsign, char* pass, char* latitude, char* longitude, char* comment, char* symbol1, char* symbol2){

  WiFiClient client;
  
  char login[60];
  char sentence[150];
  char* server = "rotate.aprs.net";
  int port = 14580;

  sprintf(login, "user %s pass %s vers VERSION ESP32", callsign, pass); // user must be "yourcallsign-13" if you are an hamradio operator, otherwise request and use a CWOP callsign....

  


  Serial.print(F("Connecting to APRS server..."));
  int retr = 20;
  while (!client.connect(server, port) && (retr > 0)) {
    delay(50);
    --retr;
  }

  if (!client.connected()) {
    Serial.println(F("connection failed"));
    client.stop();
    return;
  }
  else
  {
    Serial.println(F("done"));

    client.println(login);
    Serial.println(login);

    delay(3000); //as reccomended, 3" between login and sends packet
  }

  //print server reply
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

 
  Serial.println(F("** POSITION BEACON **"));
  sprintf(sentence, "%s>APRS,TCPIP*:=%s%s%s%s%s", callsign,
          latitude,
          symbol1,
          longitude,
          symbol2,
          comment);
  client.println(sentence);
  Serial.println(sentence);


  Serial.println();
  Serial.print(F("closing connection..."));
  client.stop();
  Serial.println(F("closed!"));
  //wait for 10sec.
  delay (10000);  
  
  }
