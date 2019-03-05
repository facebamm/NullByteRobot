#include "ESP8266.h"

ESP8266::ESP8266(SoftwareSerial serial) {
	ESP8266::Serial = serial;
	Serial.begin(115200);
}

bool ESP8266::Available() {
	Serial.println("AT");
	return ResponseOK();
}
void ESP8266::Restart() {
	Serial.println("AT+RST");
}

bool ESP8266::SetWiFiMode(ESP8266::WiFiMode mode) {
	Serial.println("AT+CWMODE=" + (int)mode);
	return ResponseOK();
}
bool ESP8266::ConnectWiFi(String ssid, String passwd) {
	Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + passwd + "\"");
	return !(ResponseERROR());
}
String ESP8266::GetConnection() {
	Serial.println("AT+CWJAP?");
	return Serial.readString();
}

bool ESP8266::SetMultiConnectionMode(ESP8266::ConnectionMode mode) {
	Serial.println("AT+CIPMUX=" + (int)mode);
	return ResponseOK();
}
ESP8266::ConnectionMode ESP8266::GetConnectionMode() {
	Serial.println("AT+CIPMUX?");
	String result = Serial.readString();
	if (result.indexOf("Single Connection") > 0) {
		return ConnectionMode::SingleConnection;
	}
	else if (result.indexOf("Multiple Connections") > 0) {
		return ConnectionMode::MultipleConnections;
	}
	return (ConnectionMode) -1;
}

bool ESP8266::ConnectToTcp(String remoteAddr, String remotePort) {
	Serial.println("AT+CIPSTART=\"TCP\"," + remoteAddr + "\",\"" + remotePort + "\"");
	return ResponseOK();
}
bool ESP8266::ConnectToTcp(ESP8266::ConnectionID id, String remoteAddr, String remotePort) {
	if (GetConnectionMode() == ConnectionMode::SingleConnection) {
		SetMultiConnectionMode(ConnectionMode::MultipleConnections);
	}
	Serial.println("AT+CIPSTART=" + String((int)id) + ",\"TCP\"," + remoteAddr + "\",\"" + remotePort + "\"");
	return ResponseOK();
}

bool ESP8266::ConnectToUdp(String remoteAddr, String remotePort) {
	Serial.println("AT+CIPSTART=\"UDP\"," + remoteAddr + "\",\"" + remotePort + "\"");
	return ResponseOK();
}
bool ESP8266::ConnectToUdp(ESP8266::ConnectionID id, String remoteAddr, String remotePort) {
	if (GetConnectionMode() == ConnectionMode::SingleConnection) {
		SetMultiConnectionMode(ConnectionMode::MultipleConnections);
	}
	Serial.println("AT+CIPSTART=" + String((int)id) + ",\"UDP\"," + remoteAddr + "\",\"" + remotePort + "\"");
	return ResponseOK();
}

bool ESP8266::WriteData(byte data[]) {
	if (GetConnectionMode() == ConnectionMode::SingleConnection) {
		int nElements = ArrLength(data);
		if (nElements < 2049) {
			Serial.println("AT+CIPSEND=" + String(nElements));
			Serial.println(String((char *)data));
			return ResponseOK();
		}
	}
	return false;
}
bool ESP8266::WriteData(ESP8266::ConnectionID id, byte data[]) {
	if (GetConnectionMode() == ConnectionMode::MultipleConnections) {
		int nElements = ArrLength(data);
		if (nElements < 2049) {
			Serial.println("AT+CIPSEND=" + String((int)id) + "," + String(nElements));
			Serial.println(String((char *)data));
			return ResponseOK();
		}
	}
	return false;
}

bool ESP8266::CloseConnection() {
	if (GetConnectionMode() == ConnectionMode::SingleConnection) {
		Serial.println("AT+CIPCLOSE");
		return ResponseOK();
	}
	return false;
}
bool ESP8266::CloseConnection(ConnectionID id) {
	if (GetConnectionMode() == ConnectionMode::MultipleConnections) {
		Serial.println("AT+CIPCLOSE=" + String((int)id));
		return ResponseOK();
	}
	return false;
}
bool ESP8266::CloseAllConnections() {
	if (GetConnectionMode() == ConnectionMode::MultipleConnections) {
		Serial.println("AT+CIPCLOSE=5");
		return ResponseOK();
	}
	return true;
}

String ESP8266::GetLocalIp() {
	Serial.println("AT+CIFSR");
	return Serial.readString();
}

bool ESP8266::SetStationMac(String mac) {
	Serial.println("AT+CIPSTAMAC=" + mac);
	return ResponseOK();
}
String ESP8266::GetStationMac() {
	Serial.println("AT+CIPSTAMAC?");
	return Serial.readString();
}

bool ESP8266::SetStationIp(String ip) {
	Serial.println("AT+CIPSTA=" + ip);
	return ResponseOK();
}
String ESP8266::GetStationIp() {
	Serial.println("AT+CIPSTA?");
	return Serial.readString();
}

bool ESP8266::SetSoftApMac(String mac) {
	Serial.println("AT+CIPAPMAC=" + mac);
}
String ESP8266::GetSoftApMac() {
	Serial.println("AT+CIPAPMA?");
	return Serial.readString();
}

bool ESP8266::SetSoftApIp(String ip) {
	Serial.println("AT+CIPAP=" + ip);
	return ResponseOK();
}
String ESP8266::GetSoftApIp() {
	Serial.println("AT+CIPAP?");
	return Serial.readString();
}

bool ESP8266::CreateServer(int port) {
	if (GetConnectionMode() == ConnectionMode::MultipleConnections) {
		Serial.println("AT+CIPSERVER=1," + String(port));
		return ResponseOK();
	}
	return false;
}
bool ESP8266::DeleteServer(int port) {
	if (GetConnectionMode() == ConnectionMode::MultipleConnections) {
		Serial.println("AT+CIPSERVER=0," + String(port));
		return ResponseOK();
	}
	return false;
}

String ESP8266::AboutConnection() {
	Serial.println("AT+CIPSTATUS");
	return Serial.readString();
}

bool ESP8266::ResponseOK() {
#ifdef SerialDebug
	return Serial.readString().indexOf("OK") > 0;
#else 
	return true;
#endif
}
bool ESP8266::ResponseERROR() {
#ifdef SerialDebug
	return Serial.readString().indexOf("ERROR") < 1;
#else 
	return true;
#endif
}