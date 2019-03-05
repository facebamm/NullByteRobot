
#include <SoftwareSerial.h>
#include <ESP8266.h>

#ifndef Serial2Debug
//#define Serial2Debug
#endif
#define ArrLength(x)  (sizeof(x) / sizeof((x)[0]))

const String SSID = "FRITZ!Box 7490";
const String PASSWD = "90800622054156093910";

ESP8266* Wlan;


void setup() {
  Serial.begin(115200);
  int val = ArrLength(new int[100]);
  Serial.println(val);
  Serial.println("start");

  Wlan = new ESP8266(*((SoftwareSerial*)&Serial2));
  SetWifi();
}

void loop() {
  if (!Wlan->Available()) {
    Serial.println("ESP8266 isn't available -> restart try");
    Wlan->Restart();
  }
}

void SetWifi() {
  Wlan->SetWiFiMode(ESP8266::WiFiMode::Client);
  if (Wlan->Available()) {
    if (Wlan->GetConnection().indexOf(SSID) < 0) { //no connection
      if (Wlan->ConnectWiFi(SSID, PASSWD)) {
        Serial.println(Wlan->GetConnection());
      }
      else {
        Serial.println("Error by the Connection");
      }
    }
    else {
      Serial.println("ESP8266 is already connected");
    }
  }
  else {
    Serial.println("Error in WLan-Modul");
  }
}


//Mega
//SERIAL  PINS -> 0(RX), 1(TX)
//SERIAL1 PINS -> 19(RX), 18(TX)
//SERIAL2 PINS -> 16(RX), 17(TX)
//SERIAL3 PINS -> 15(RX), 14(TX)
//SoftwareSerial(rxPin, txPin, inverse_logic, buffer size);
