#ifndef ROTRACER_H   // bu kütüphaneye birden fazla yerden çagýrmak için  Ýsimlendirmenini kýsaltmasý þeklinde yazdýk burdaki ismi
#define ROTRACER_H    
// ----------------------------BÜTÜN TANIMLAMA ÝÞLEMLERÝNÝ BURDA YAPIYORUZ --------------------------
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4996)

#include <iostream>
#include <imgui.h>               // imgui bileþenlerini kullnmak için 
#include <implot/implot.h>       // implot bileþenleri için 
#include <regex>
#include <string.h>
#include <zmq.h>
#include <thread>
#include <stdio.h>

// AgvStreamData
class AgvData {
public: 
    int X;                       //x konum
    int Y;                       // y konum
    float Angle;                 // açý
    int WSpeed;                  // yazýlan hýz
    int RSpeed;                  // okunan hýz 
    float WAngle;                // yazýlan teker açýsý 
    float RAngle;                // okunan teker açýsý 
};

class NetData {
public:
	std::string DeviceMacAddress;     //mac adresi    
	int Ping;                         // ping degeri 
	int ReceivedRate;                 // alýnma oraný
	int Signal;                       // wifi sinyal kalitesi
	int Speed;                        // wifi hýzý 
	std ::string SSID;                // wifi ismi 
	std::string Status;               // wifi durumu 
	int TransmitededRate;             // iletim oraný
	

};

struct ScrollingBuffer {
	int MaxSize;
	int Offset;
	ImVector<ImVec2> Data;
	ScrollingBuffer(int max_size = 2000000) {
		MaxSize = max_size;
		Offset = 0;
		Data.reserve(MaxSize);
	}
	void AddPoint(float x, float y) {
		if (Data.size() < MaxSize)
			Data.push_back(ImVec2(x, y));
		else {
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}
	void Erase() {
		if (Data.size() > 0) {
			Data.shrink(0);
			Offset = 0;
		}
	}
};

class SpeedGraphicData
{
public:
	ScrollingBuffer ReadingSpeed;        // data depolamak için oluþturduðumuz deðiþken 
	ScrollingBuffer WritingSpeed;        // data depolamak için oluþturduðumuz deðiþken 
	ScrollingBuffer ReadingAngel;
	ScrollingBuffer WritingAngel;
	ScrollingBuffer totalAngel;
	ScrollingBuffer StokingPosition;
	static ScrollingBuffer totalX;
	static ScrollingBuffer totalY;

	ScrollingBuffer StokingWifiSpeed;
	ScrollingBuffer StokingPing;
	ScrollingBuffer StokingReceivedRate;
	ScrollingBuffer StokingSignal;
	ScrollingBuffer StokingTransmitededRate;

	 float Time;                          // geçen zamaný depolamak için oluþturdugumuz deðiþken 
	 float History;                       // geçmiþ zamaný depolamak için 
	 float TimeSpeedagv;
	 float TimePosition;
	 float TimeWheel;
	 float TimeAngel;
	 float TimeTotalAngel;
	 float TimeReceivedRate;
	 float TimeSignalSpeedPing;
	 float TimeTransmittededRate;
	 float HistorySpeedagv= 20.f;
	 float HistoryPosition;
	 float HistoryWheel = 20.f;
	 float HistoryAngel;
	 float HistoryTotalAngel=20.f;
	 float HistoryWifiSpeed;
	 float HistoryPing;
	 float HistoryReceivedRate;
	 float HistorySignal;
	 float HistoryTransmittededRate;
	 

};

class ROTracer
{   
public:                   //constructor 
	ROTracer();
	~ROTracer();

	void SpeedPage();      // agv fonksiyonlar 
	void LoginPage();
	void PositionPage();
	void WheelPage();
	void TotalAngelPage();
	//----------------
	void WifiSpeedPage();    // net fonksiyonlarý 
	void PingPage();
	void ReceivedRatePage();
	void SignalPingSpeedPage();
	void TransmitededRatePage();


	void StartStreamParser();
	void StopStreamParser();


	AgvData* Agv;          //agv classýna ait  nesne oluþturduk  
	NetData* Net;
	SpeedGraphicData* SGD;    // SpeedGraphicData class'ýnýn nesnesini oluþturduk 

	char IpAddress[16] = "192.168.2.125";
	bool _isRunning;
	bool page1 = true;
	// agv checkbox durum dekiþkenleri
	bool position = false;
	bool angle = false;
	bool totalangle = false;
	bool Agvspeed = false;
	bool wheel = false;
	bool wifispeed = false;
	bool ping = false;
	bool receivedrate = false;
	bool signal = false;
	bool transmitededrate = false;
    
	bool isPauseWheel = false;      	// grafik durdurma durumunu tutma   
	bool isPauseAngel = false;
	bool isPauseTotalAngel = false;
	bool isPauseReceivedRate = false;
	bool isPauseSignalSpeedPing = false;
	bool isPauseTransmitededRate = false;
	bool isPauseSpeed = false;
	bool isPausePosition = false;

private:
	bool _loginPageVisibility ;        //giriþ sayfa görünürlüðü
	bool _speedPageVisibility = false;        //hýz grafik sayfasýnýn görünürlüðü
	bool _positionPageVisibility = false;
	bool _wheelPageVisibility = false;
	bool _anglePageVisibility=false;
	bool _angleTotalPageVisibility = false;

	
	bool _receivedRatePageVisibility;
	bool _signalSpeedPingPageVisibility;
	bool _transmitededRatePageVisibility;


	bool _zmqLoopFlag;               //zmq data parse iþlemini yapýp yapmama  

	void ZMQDataStreamParser();       // parse iþlemi private onun için burda yoksa yukarý da yazabilirdik
	
};


#endif