#pragma once
class ESP8266
{
	private:
		SoftwareSerial Serial;
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
		ESP8266();
		ESP8266(SoftwareSerial serial);
		bool Available();
		void Restart();
		bool SetWiFiMode(WiFiMode mode);
		bool ConnectWiFi(String ssid, String passwd);
		String GetConnection();

		bool SetMultiConnectionMode(ConnectionMode mode);
		ConnectionMode GetConnectionMode();
	
		bool ConnectToTcp(String remoteAddr, String remotePort);
		bool ConnectToTcp(ConnectionID id, String remoteAddr, String remotePort);
	
		bool ConnectToUdp(String remoteAddr, String remotePort);
		bool ConnectToUdp(ConnectionID id, String remoteAddr, String remotePort);
	
		bool WriteData(byte data[]);
		bool WriteData(ConnectionID id, byte data[]);
	
		bool CloseConnection();
		bool CloseConnection(ConnectionID id);
		bool CloseAllConnections();

		String GetLocalIp();
	
		bool SetStationMac(String mac);
		String GetStationMac();
	
		bool SetStationIp(String ip);
		String GetStationIp();
	
		bool SetSoftApMac(String mac);
		String GetSoftApMac();
	
		bool SetSoftApIp(String ip);
		String GetSoftApIp();
	
		bool CreateServer(int port);
		bool DeleteServer(int port);
	
		String AboutConnection();
	
		bool ResponseOK();
		bool ResponseERROR();
};

