#ifndef ROTRACER_H   // bu k�t�phaneye birden fazla yerden �ag�rmak i�in  �simlendirmenini k�saltmas� �eklinde yazd�k burdaki ismi
#define ROTRACER_H    
// ----------------------------B�T�N TANIMLAMA ��LEMLER�N� BURDA YAPIYORUZ --------------------------
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4996)

#include <iostream>
#include <imgui.h>               // imgui bile�enlerini kullnmak i�in 
#include <implot/implot.h>       // implot bile�enleri i�in 
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
    float Angle;                 // a��
    int WSpeed;                  // yaz�lan h�z
    int RSpeed;                  // okunan h�z 
    float WAngle;                // yaz�lan teker a��s� 
    float RAngle;                // okunan teker a��s� 
};

class NetData {
public:
	std::string DeviceMacAddress;     //mac adresi    
	int Ping;                         // ping degeri 
	int ReceivedRate;                 // al�nma oran�
	int Signal;                       // wifi sinyal kalitesi
	int Speed;                        // wifi h�z� 
	std ::string SSID;                // wifi ismi 
	std::string Status;               // wifi durumu 
	int TransmitededRate;             // iletim oran�
	

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
	ScrollingBuffer ReadingSpeed;        // data depolamak i�in olu�turdu�umuz de�i�ken 
	ScrollingBuffer WritingSpeed;        // data depolamak i�in olu�turdu�umuz de�i�ken 
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

	 float Time;                          // ge�en zaman� depolamak i�in olu�turdugumuz de�i�ken 
	 float History;                       // ge�mi� zaman� depolamak i�in 
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
	void WifiSpeedPage();    // net fonksiyonlar� 
	void PingPage();
	void ReceivedRatePage();
	void SignalPingSpeedPage();
	void TransmitededRatePage();


	void StartStreamParser();
	void StopStreamParser();


	AgvData* Agv;          //agv class�na ait  nesne olu�turduk  
	NetData* Net;
	SpeedGraphicData* SGD;    // SpeedGraphicData class'�n�n nesnesini olu�turduk 

	char IpAddress[16] = "192.168.2.125";
	bool _isRunning;
	bool page1 = true;
	// agv checkbox durum deki�kenleri
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
	bool _loginPageVisibility ;        //giri� sayfa g�r�n�rl���
	bool _speedPageVisibility = false;        //h�z grafik sayfas�n�n g�r�n�rl���
	bool _positionPageVisibility = false;
	bool _wheelPageVisibility = false;
	bool _anglePageVisibility=false;
	bool _angleTotalPageVisibility = false;

	
	bool _receivedRatePageVisibility;
	bool _signalSpeedPingPageVisibility;
	bool _transmitededRatePageVisibility;


	bool _zmqLoopFlag;               //zmq data parse i�lemini yap�p yapmama  

	void ZMQDataStreamParser();       // parse i�lemi private onun i�in burda yoksa yukar� da yazabilirdik
	
};


#endif