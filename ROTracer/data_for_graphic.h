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
	static ScrollingBuffer totalX;
	static ScrollingBuffer totalY;

	float Time;                          // ge�en zaman� depolamak i�in olu�turdugumuz de�i�ken 
	float History;                       // ge�mi� zaman� depolamak i�in 
};

class ROTracer
{   
public:                   //constructor 
	ROTracer();
	~ROTracer();

	void SpeedPage();      //fonksiyonlar 
	void LoginPage();


	void StartStreamParser();
	void StopStreamParser();

	AgvData* Agv;          //agv class�na ait  nesne olu�turduk  
	NetData* Net;
	char IpAddress[16] = "192.168.2.125";
	bool _isRunning;
	bool page1 = true;
	bool position = false;
	bool angle = false;
	bool speed = false;
	bool wheel = false;

	bool isPause=false;       // grafik durdurma durumunu tutma       
	SpeedGraphicData* SGD;    // SpeedGraphicData class'�n�n nesnesini olu�turduk  


private:
	bool _loginPageVisibility;        //giri� sayfa g�r�n�rl���
	bool _speedPageVisibility;        //h�z grafik sayfas�n�n g�r�n�rl���
	bool _positionPageVisibility;
	bool _wheelPageVisibility;
	bool _anglePageVisibility;


	bool _zmqLoopFlag;               //zmq data parse i�lemini yap�p yapmama  

	void ZMQDataStreamParser();       // parse i�lemi private onun i�in burda yoksa yukar� da yazabilirdik
	void PositionPage();
	void WheelPage();
	void AngelPage();
};


#endif