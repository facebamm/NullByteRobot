#include <SoftwareSerial.h>
#ifndef Serial2Debug 
//#define Serial2Debug
#endif


const String SSID =  "FRITZ!Box 7490";
const String PASSWD = "90800622054156093910";

class WiFi{ 
  private: 
    enum ServerType { 
      TCP = 0, UDP = 1
    };
  public: 
    enum WiFiMode { 
       Client = 1, Host = 2, HostClient = 3
    };
    enum ConnectionID {
        ID00 = 0, ID01 = 1, ID02 = 2, ID03 = 3, ID04 = 4 
    };
    enum ConnectionMode { 
      SingleConnection = 0,
      MultipleConnections = 1
    };
    WiFi();
    WiFi(int baud);
    bool SetWiFiMode(WiFiMode mode);
    bool Available();
    void Restart();
    bool ConnectWiFi(String ssid, String passwd);
    String GetConnection();
    bool SetMultiConnectionMode(ConnectionMode mode);
    ConnectionMode GetConnectionMode();
    bool ConnectToTcp(String remoteAddr, String remotePort);
    bool ConnectToTcp(ConnectionID id, String remoteAddr, String remotePort);
    bool ConnectToUdp(String remoteAddr, String remotePort);
    bool ConnectToUdp(ConnectionID id, String remoteAddr, String remotePort);
    static bool ResponseOK();
    static bool ResponseERROR();
};

WiFi* ESP8266;


void setup() {
  Serial.begin(115200);
  int val = WiFi::ConnectionID::ID00;
  Serial.println(val);
  Serial.println("start");
  
  ESP8266 = new WiFi();
  SetWifi();
}

void loop() {
  if(!ESP8266->Available()){
    Serial.println("ESP8266 isn't available -> restart try");
    ESP8266->Restart();
  }
}

void SetWifi() { 
  ESP8266->SetWiFiMode(WiFi::WiFiMode::Client);
  if(ESP8266->Available()){
    if(ESP8266->GetConnection().indexOf(SSID) < 0){ //no connection
      if(ESP8266->ConnectWiFi(SSID, PASSWD)){ 
        Serial.println(ESP8266->GetConnection());
      } else {
        Serial.println("Error by the Connection");
      }
    } else { 
      Serial.println("ESP8266 is already connected");
    }
  } else { 
    Serial.println("Error in WLan-Modul");
  }
}

WiFi::WiFi() { 
  Serial2.begin(115200);
}
WiFi::WiFi(int baud) { 
  Serial2.begin(baud);
}
bool WiFi::SetWiFiMode(WiFi::WiFiMode mode){ 
  Serial2.println("AT+CWMODE=" + (int) mode);
  return WiFi::ResponseOK();
}
bool WiFi::Available(){ 
  Serial2.println("AT");
  return WiFi::ResponseOK();
}
void WiFi::Restart() { 
  Serial2.println("AT+RST");
}
bool WiFi::ConnectWiFi(String ssid, String passwd){ 
  Serial2.println("AT+CWJAP=\"" + ssid + "\",\"" + passwd + "\"");
  return !(WiFi::ResponseERROR());
}
String WiFi::GetConnection(){ 
  Serial2.println("AT+CWJAP?");
  return Serial2.readString();
}
bool WiFi::SetMultiConnectionMode(WiFi::ConnectionMode mode){ 
  Serial2.println("AT+CIPMUX=" + (int) mode);
  return WiFi::ResponseOK();
}

WiFi::ConnectionMode WiFi::GetConnectionMode(){ 
  Serial2.println("AT+CIPMUX?");
  String result = Serial2.readString();
  if(result.indexOf("Single Connection") > 0){
    return WiFi::ConnectionMode::SingleConnection;
  } else if(result.indexOf("Multiple Connections") > 0 ) { 
    return WiFi::ConnectionMode::MultipleConnections;
  }
  return -1;
}
bool WiFi::ConnectToTcp(String remoteAddr, String remotePort){
  Serial2.println("AT+CIPSTART=\"TCP\"," + remoteAddr + "\",\"" + remotePort + "\"");
  return ResponseOK();
}
bool WiFi::ConnectToTcp(WiFi::ConnectionID id, String remoteAddr, String remotePort){ 
  if(GetConnectionMode() ==  ConnectionMode::SingleConnection){ 
    SetMultiConnectionMode(ConnectionMode::MultipleConnections);
  }  
  Serial2.println("AT+CIPSTART=" + String((int) id) + ",\"TCP\"," + remoteAddr + "\",\"" + remotePort + "\"");
  return ResponseOK();
}
bool WiFi::ConnectToUdp(String remoteAddr, String remotePort) { 
   Serial2.println("AT+CIPSTART=\"UDP\"," + remoteAddr + "\",\"" + remotePort + "\"");
  return ResponseOK();
}
bool WiFi::ConnectToUdp(WiFi::ConnectionID id, String remoteAddr, String remotePort){ 
  if(GetConnectionMode() ==  ConnectionMode::SingleConnection){ 
    SetMultiConnectionMode(ConnectionMode::MultipleConnections);
  }  
  Serial2.println("AT+CIPSTART=" + String((int) id) + ",\"UDP\"," + remoteAddr + "\",\"" + remotePort + "\"");
  return ResponseOK();
}
bool WiFi::ResponseOK(){ 
#ifdef Serial2Debug
  return Serial2.readString().indexOf("OK") > 0;
#else 
  return true;
#endif
  
};
bool WiFi::ResponseERROR(){  
#ifdef Serial2Debug
  return Serial2.readString().indexOf("ERROR") < 1;
#else 
  return true;
#endif
};

//Mega
//SERIAL  PINS -> 0(RX), 1(TX)
//SERIAL1 PINS -> 19(RX), 18(TX)
//SERIAL2 PINS -> 16(RX), 17(TX)
//SERIAL3 PINS -> 15(RX), 14(TX)
//SoftwareSerial(rxPin, txPin, inverse_logic, buffer size);