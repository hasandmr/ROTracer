#include "data_for_graphic.h"   //data_for_graphic deki veriler için çagırıyoruz 
using namespace std;


ROTracer::ROTracer() {
	this->Agv = new AgvData();
	this->Net = new NetData();

	this->SGD = new SpeedGraphicData();

	_loginPageVisibility = true;
}


ROTracer::~ROTracer() {
	delete this->Agv;
	delete this->Net;

}


void ROTracer::StartStreamParser() {
	if (!_isRunning)
	{
		this->_zmqLoopFlag = true;
		std::thread lth(&ROTracer::ZMQDataStreamParser, this);
		lth.detach();
	}
}


void ROTracer::StopStreamParser() {
	this->_zmqLoopFlag = false;
}


//-------------------------agv grafik çizme alanı---------------------------------------------------------------

void ROTracer::SpeedPage() {
	_zmqLoopFlag = true;
	if (this->_speedPageVisibility == true) {

		if (this->Agv == NULL) {
			return;
		}

		ImGui::BulletText("Move your speed to change the data!");

		ImGui::Checkbox("Pause", &isPauseSpeed);     // duraklatma seçenegi 

		if (!isPauseSpeed)      // eger checkbox'a tıklanmazsa (false)  güncel zamanı alıyor  
		{

			this->SGD->TimeSpeedagv += ImGui::GetIO().DeltaTime;

			this->SGD->ReadingSpeed.AddPoint(this->SGD->TimeSpeedagv, this->Agv->RSpeed);
			this->SGD->WritingSpeed.AddPoint(this->SGD->TimeSpeedagv, this->Agv->WSpeed);

		}
		else     // eger checkbox'a tıklanırsa en son zamanı alıyor 
			this->SGD->TimeSpeedagv;



		ImGui::SliderFloat("History", &this->SGD->HistorySpeedagv, 1, 300, "% 1.f saniye");  // .1f yaparsak milisaniye olarak ayarlanıyor 


		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

		if (ImPlot::BeginPlot("##Scrolling", ImVec2(800, 350))) {     // grafik ölçeklendirme 
			ImPlot::SetupAxes("Time [s]", "Speed [mm/s]");

			ImPlot::SetupAxisLimits(ImAxis_X1, this->SGD->TimeSpeedagv - this->SGD->HistorySpeedagv, this->SGD->TimeSpeedagv, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, -500, 1900);

			//ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.f);
			ImPlot::PlotLine("rs X", &this->SGD->ReadingSpeed.Data[0].x, &this->SGD->ReadingSpeed.Data[0].y, this->SGD->ReadingSpeed.Data.size(), 0, this->SGD->ReadingSpeed.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("ws Y", &this->SGD->WritingSpeed.Data[0].x, &this->SGD->WritingSpeed.Data[0].y, this->SGD->WritingSpeed.Data.size(), 0, this->SGD->WritingSpeed.Offset, 2 * sizeof(float));
			// plotline düz cizgili grafik <=> PlotShaded gölgeli grafik  
			ImPlot::EndPlot();
		}
	}
}



void ROTracer::WheelPage() {
	_zmqLoopFlag = true;
	if (this->_wheelPageVisibility == true) {

		if (this->Agv == NULL) {
			return;
		}

		ImGui::BulletText("Move your whell to change the data!");

		ImGui::Checkbox("Pause", &isPauseWheel);     // duraklatma seçenegi 

		if (!isPauseWheel)      // eger checkbox'a tıklanmazsa (false)  güncel zamanı alıyor  
		{

			this->SGD->TimeWheel += ImGui::GetIO().DeltaTime;

			this->SGD->ReadingAngel.AddPoint(this->SGD->TimeWheel, this->Agv->RAngle);
			this->SGD->WritingAngel.AddPoint(this->SGD->TimeWheel, this->Agv->WAngle);

		}
		else     // eger checkbox'a tıklanırsa en son zamanı alıyor 
			this->SGD->TimeWheel;



		ImGui::SliderFloat("History", &this->SGD->HistoryWheel, 1, 300, "% 1.f saniye");  // .1f yaparsak milisaniye olarak ayarlanıyor 


		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

		if (ImPlot::BeginPlot("##Scrolling", ImVec2(800, 350))) {     // grafik ölçeklendirme 
			ImPlot::SetupAxes("Time [s]", "Wheel °");

			ImPlot::SetupAxisLimits(ImAxis_X1, this->SGD->TimeWheel - this->SGD->HistoryWheel, this->SGD->TimeWheel, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, -500, 1900);

			//ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.f);
			ImPlot::PlotLine("READ WHEEL ANGLE ", &this->SGD->ReadingAngel.Data[0].x, &this->SGD->ReadingAngel.Data[0].y, this->SGD->ReadingAngel.Data.size(), 0, this->SGD->ReadingAngel.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("WRITE WHEEL ANGLE", &this->SGD->WritingAngel.Data[0].x, &this->SGD->WritingAngel.Data[0].y, this->SGD->WritingAngel.Data.size(), 0, this->SGD->WritingAngel.Offset, 2 * sizeof(float));
			// plotline düz cizgili grafik <=> PlotShaded gölgeli grafik  
			ImPlot::EndPlot();
		}
	}

}


void ROTracer::TotalAngelPage() {
	_zmqLoopFlag = true;
	if (this->_angleTotalPageVisibility == true) {

		if (this->Agv == NULL) {
			return;
		}

		ImGui::BulletText("Move your Angel to change the data!");

		ImGui::Checkbox("Pause", &isPauseTotalAngel);     // duraklatma seçenegi 

		if (!isPauseTotalAngel)      // eger checkbox'a tıklanmazsa (false)  güncel zamanı alıyor  
		{

			this->SGD->TimeTotalAngel += ImGui::GetIO().DeltaTime;

			this->SGD->ReadingAngel.AddPoint(this->SGD->TimeTotalAngel, this->Agv->RAngle);
			this->SGD->WritingAngel.AddPoint(this->SGD->TimeTotalAngel, this->Agv->WAngle);
		}
		else     // eger checkbox'a tıklanırsa en son zamanı alıyor 
			this->SGD->TimeTotalAngel;



		ImGui::SliderFloat("History", &this->SGD->HistoryTotalAngel, 1, 300, "% 1.f saniye");  // .1f yaparsak milisaniye olarak ayarlanıyor 


		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

		if (ImPlot::BeginPlot("##Scrolling", ImVec2(800, 350))) {     // grafik ölçeklendirme 
			ImPlot::SetupAxes("Time [s]", "Angle °");

			ImPlot::SetupAxisLimits(ImAxis_X1, this->SGD->TimeTotalAngel - this->SGD->HistoryTotalAngel, this->SGD->TimeTotalAngel, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, -100, 100);

			//ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.f);
			ImPlot::PlotLine("W ANGLE ", &this->SGD->ReadingAngel.Data[0].x, &this->SGD->ReadingAngel.Data[0].y, this->SGD->ReadingAngel.Data.size(), 0, this->SGD->ReadingAngel.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("R ANGLE ", &this->SGD->WritingAngel.Data[0].x, &this->SGD->WritingAngel.Data[0].y, this->SGD->WritingAngel.Data.size(), 0, this->SGD->WritingAngel.Offset, 2 * sizeof(float));

			// plotline düz cizgili grafik <=> PlotShaded gölgeli grafik  
			ImPlot::EndPlot();
		}
	}

}


void ROTracer::PositionPage() {

	_zmqLoopFlag = true;
	if (this->_positionPageVisibility == true) {

		if (this->Agv == NULL) {
			return;
		}

		ImGui::BulletText("Move your position to change the data!");

		ImGui::Checkbox("Pause", &isPausePosition);     // duraklatma seçenegi 

		if (!isPausePosition)      // eger checkbox'a tıklanmazsa (false)  güncel zamanı alıyor  
		{
			this->SGD->HistoryPosition = 20.0f;
			this->SGD->TimePosition += ImGui::GetIO().DeltaTime;

			this->SGD->StokingPosition.AddPoint(this->Agv->X, this->Agv->Y);


		}
		else     // eger checkbox'a tıklanırsa en son zamanı alıyor 
			this->SGD->TimeReceivedRate;

		ImGui::SliderFloat("History", &this->SGD->HistoryPosition, 1, 300, "% 1.f saniye");  // .1f yaparsak milisaniye olarak ayarlanıyor 


		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;


		if (ImPlot::BeginPlot("Scatter Plot", ImVec2(800, 350))) {
			ImPlot::SetupAxesLimits(10000, 30000, 30000, 80000);
			ImPlot::PlotScatter("POSITION BEFORE ", &this->SGD->StokingPosition.Data[0].x, &this->SGD->StokingPosition.Data[0].y, this->SGD->StokingPosition.Data.size(), 0, 0, 2 * sizeof(float));
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);

			ImPlot::SetNextMarkerStyle(ImPlotMarker_Up, 6, ImPlot::GetColormapColor(1), IMPLOT_AUTO, ImPlot::GetColormapColor(1));
			ImPlot::PlotScatter("POSITION END ", &this->SGD->StokingPosition.Data[this->SGD->StokingPosition.Offset].x, &this->SGD->StokingPosition.Data[this->SGD->StokingPosition.Offset].y, this->SGD->StokingPosition.Data.size(), 0, 0, 2 * sizeof(float));
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}

	}
}


//--------------------------------giriş sayfası-------------------------------------------------------------

void ROTracer::LoginPage() {
	//_zmqLoopFlag = false;
	if (this->_loginPageVisibility == true)
	{
		ImGui::SetNextWindowSize(ImVec2(920, 520));

		// GİRİŞ SAYFA
		if (page1 == true)
		{
			ImGui::Begin("PAGE", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);  //, &no_collapse
			ImGui::StyleColorsDark();
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec4* colors = style.Colors;
			style.WindowRounding = 0.9f; // Pencere köşelerinin yuvarlanma miktarı
			colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.95f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
			colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
			ImGui::Text(" ");
			ImGui::Text(" ");
			ImGui::Text(" ");
			ImGui::Text(" ");
			ImGui::Text(" ");
			// Pencere başlığı ve IP adresi giriş kutusu
			ImGui::Text("                                                       Enter IP Address");
			ImGui::Spacing();

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(" ", NULL, true).x) / (1, 5));
			ImGui::InputText(" ", IpAddress, 32, ImGuiInputTextFlags_CharsNoBlank);

			ImGui::Spacing();

			// IP adresi girişi ve "Login" düğmesi
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Login", NULL, true).x) / 2);

			// IP adresi girişi ve "Login" düğmesi
			if (ImGui::Button("Login")) {
				// IP adresi doğrulama
				std::regex ip_regex("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
				if (!std::regex_match(IpAddress, ip_regex)) {
					ImGui::OpenPopup("Uyari");
				}
				else {
					// Login düğmesine tıklandığında, saklanan IP adresi ile diğer sayfaya geçme
					std::cout << "Entered IP Address:" << IpAddress << std::endl;
					ImGui::OpenPopup("New Page");
					page1 = false;
					this->_loginPageVisibility = false;
					this->StartStreamParser();

				}
			}


			// IP adresi uyarı mesajı
			if (ImGui::BeginPopupModal("Uyari", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
				ImGui::Text("Please enter a valid IP address.");
				if (ImGui::Button("Ok")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::End();
		}

	}

	// Yeni acılacak grafiklerin sayfası penceresi 
	if (!this->_loginPageVisibility) {

		if (!page1) {
			ImGui::Begin("New Page");
			ImGui::SetNextWindowSize(ImVec2(920, 520));


			if (ImGui::CollapsingHeader("NET"))
			{
				ImGui::TableNextColumn();
				ImGui::Text("DeviceMacAddress: %s", this->Net->DeviceMacAddress.c_str());

				ImGui::TableNextColumn();
				ImGui::Text("SSID: %s", this->Net->SSID.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("Status: %s", this->Net->Status.c_str());

				// İlk iki grafiği yan yana sırala
				ImGui::BeginChild("Ping", ImVec2(600, 400), true);
				_signalSpeedPingPageVisibility = true;
				this->SignalPingSpeedPage();
				ImGui::EndChild();
			}


			if (ImGui::CollapsingHeader("AGV"))
			{
				ImGui::Checkbox("Wheel", &wheel);
				ImGui::SameLine();
				ImGui::Checkbox("Speed", &Agvspeed);
				ImGui::SameLine();
				ImGui::Checkbox("Angle", &totalangle);
				ImGui::SameLine();
				ImGui::Checkbox("Position", &position);


				if (Agvspeed) {
					ImGui::BeginChild("speed", ImVec2(900, 500), true);
					_speedPageVisibility = true;
					this->SpeedPage();
					ImGui::EndChild();

				}

				if (wheel) {
					ImGui::BeginChild("Ping", ImVec2(900, 500), true);
					_wheelPageVisibility = true;
					this->WheelPage();
					ImGui::EndChild();

				}

				if (totalangle) {
					ImGui::BeginChild("Total Angle", ImVec2(900, 500), true);
					_angleTotalPageVisibility = true;
					this->TotalAngelPage();
					ImGui::EndChild();

				}

				if (position) {
					ImGui::BeginChild("Postion", ImVec2(900, 500), true);
					_positionPageVisibility = true;
					this->PositionPage();
					ImGui::EndChild();
				}


			}

			ImGui::End();

		}
	}

}
//-------------------------------DATA  PARSER BLOCK----------------------------------------------------------

void ROTracer::ZMQDataStreamParser()
{

	void* context = zmq_ctx_new();
	void* subscriber = zmq_socket(context, ZMQ_SUB);
	std::string take_ip = this->IpAddress;
	std::string  a = "tcp://" + take_ip + ":5556";
	//std::string b = ("tcp://%s:%d", take_ip,take_port);    // opsiyonel kullanım 

	int rc = zmq_connect(subscriber, a.c_str());  //"tcp://192.168.2.125:5556"
	rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

	//const int topic_size = 5;
	//const int msg_size = 128;

	//char topic[topic_size] = { 0 };
	//char msg[msg_size] = { 0 };
	//std::string new_msg(msg);
	size_t sayac = 0;
	char* pch;
	_isRunning = true;

	fprintf(stdout, "start");

	while (this->_zmqLoopFlag)
	{
		zmq_msg_t zmq_topic;
		zmq_msg_t zmq_message;

		zmq_msg_init(&zmq_topic);
		rc = zmq_msg_recv(&zmq_topic, subscriber, 0);

		zmq_msg_init(&zmq_message);
		rc = zmq_msg_recv(&zmq_message, subscriber, 0);

		int zmq_topic_size = zmq_msg_size(&zmq_topic);
		int zmq_message_size = zmq_msg_size(&zmq_message);

		char* zmq_topic_data = (char*)malloc(zmq_topic_size + 1);
		memcpy(zmq_topic_data, zmq_msg_data(&zmq_topic), zmq_topic_size);
		zmq_msg_close(&zmq_topic);
		zmq_topic_data[zmq_topic_size] = 0;

		char* zmq_message_data = (char*)malloc(zmq_message_size + 1);
		memcpy(zmq_message_data, zmq_msg_data(&zmq_message), zmq_message_size);
		zmq_msg_close(&zmq_message);
		zmq_message_data[zmq_message_size] = 0;

		//rc = zmq_recv(subscriber, topic, topic_size, 0);
		if (rc != -1) {
			fprintf(stdout, "TOPIC: %s \n", zmq_topic_data);
			fprintf(stdout, "MSG: %s \n", zmq_message_data);
		}

		//rc = zmq_recv(subscriber, msg, msg_size, 0);
		if (rc != -1) {

			if (std::strcmp("real", zmq_topic_data) == 0) {
				pch = strtok(zmq_message_data, ";");
				while (pch != NULL)
				{
					if (sayac == 0) {
						Agv->X = stoi(pch);
						sayac++;

					}
					else if (sayac == 1) {
						Agv->Y = stoi(pch);
						sayac++;

					}
					else if (sayac == 2) {
						Agv->Angle = std::stof(pch);
						sayac++;

					}
					else if (sayac == 3) {
						Agv->WSpeed = stoi(pch);
						sayac++;

					}
					else if (sayac == 4) {
						Agv->RSpeed = stoi(pch);
						sayac++;

					}
					else if (sayac == 5) {
						Agv->WAngle = std::stof(pch);
						sayac++;

					}
					else if (sayac == 6) {
						Agv->RAngle = std::stof(pch);
						sayac++;

					}

					//printf("%s\n", pch);
					pch = strtok(NULL, ";");
				}
				sayac = 0;
			}
			else if (std::strcmp("net", zmq_topic_data) == 0) {
				pch = strtok(zmq_message_data, ";");
				while (pch != NULL)
				{
					if (sayac == 0) {
						Net->DeviceMacAddress = std::string(pch);
						sayac++;

					}
					else if (sayac == 1) {
						Net->Ping = stoi(pch);
						sayac++;

					}
					else if (sayac == 2) {
						Net->ReceivedRate = stoi(pch);
						sayac++;

					}
					else if (sayac == 3) {
						Net->Signal = stoi(pch);
						sayac++;

					}
					else if (sayac == 4) {
						Net->Speed = stoi(pch);
						sayac++;

					}
					else if (sayac == 5) {
						Net->SSID = std::string(pch);;
						sayac++;

					}
					else if (sayac == 6) {
						Net->Status = std::string(pch);
						sayac++;
					}
					else if (sayac == 7) {
						Net->TransmitededRate = stoi(pch);
						sayac++;

					}

					//	printf("%s\n", pch);
					pch = strtok(NULL, ";");
				}
				sayac = 0;
			}
			/*
			else if (std::strcmp("route", topic) == 0) {
			}
			else if (std::strcmp("sim", topic) == 0) {
			}
			else if (std::strcmp("curve", topic) == 0) {
			}*/


			fprintf(stdout, "----------------------- \n");

		}


		//  serbest bırakma 
		free(zmq_topic_data);
		free(zmq_message_data);
	}

	zmq_close(subscriber);
	zmq_ctx_destroy(context);

	_isRunning = false;

}


//------------------------------ NET grafik çizme alanı ----------------------------------------------------

void ROTracer::ReceivedRatePage() {

	_zmqLoopFlag = true;
	if (this->_receivedRatePageVisibility == true) {

		if (this->Net == NULL) {
			return;
		}

		ImGui::BulletText("Move your Received Rate to change the data!");

		ImGui::Checkbox("Pause", &isPauseReceivedRate);     // duraklatma seçenegi 

		if (!isPauseReceivedRate)      // eger checkbox'a tıklanmazsa (false)  güncel zamanı alıyor  
		{
			this->SGD->HistoryReceivedRate = 20.0f;
			this->SGD->TimeReceivedRate += ImGui::GetIO().DeltaTime;

			this->SGD->StokingReceivedRate.AddPoint(this->SGD->TimeReceivedRate, this->Net->ReceivedRate);


		}
		else     // eger checkbox'a tıklanırsa en son zamanı alıyor 
			this->SGD->TimeReceivedRate;

		ImGui::SliderFloat("History", &this->SGD->HistoryReceivedRate, 1, 300, "% 1.f saniye");  // .1f yaparsak milisaniye olarak ayarlanıyor 


		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

		if (ImPlot::BeginPlot("##Scrolling", ImVec2(800, 350))) {     // grafik ölçeklendirme 
			ImPlot::SetupAxes("Time [s]", "Received rate  [ ]");

			ImPlot::SetupAxisLimits(ImAxis_X1, this->SGD->TimeReceivedRate - this->SGD->HistoryReceivedRate, this->SGD->TimeReceivedRate, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 3000000);

			//ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.f);
			ImPlot::PlotLine("received rate", &this->SGD->StokingReceivedRate.Data[0].x, &this->SGD->StokingReceivedRate.Data[0].y, this->SGD->StokingReceivedRate.Data.size(), 0, this->SGD->StokingReceivedRate.Offset, 2 * sizeof(float));

			ImPlot::EndPlot();
		}
	}



}
void ROTracer::SignalPingSpeedPage() {
	_zmqLoopFlag = true;
	if (this->_signalSpeedPingPageVisibility == true) {

		if (this->Net == NULL) {
			return;
		}

		ImGui::BulletText("Move your signal,Ping, Speed to change the data!");

		ImGui::Checkbox("Pause", &isPauseSignalSpeedPing);     // duraklatma seçenegi 

		if (!isPauseSignalSpeedPing)      // eger checkbox'a týklanmazsa (false)  güncel zamaný alýyor  
		{
			this->SGD->HistorySignal = 20.0f;
			this->SGD->TimeSignalSpeedPing += ImGui::GetIO().DeltaTime;

			this->SGD->StokingSignal.AddPoint(this->SGD->TimeSignalSpeedPing, this->Net->Signal);
			this->SGD->StokingPing.AddPoint(this->SGD->TimeSignalSpeedPing, this->Net->Ping);
			this->SGD->StokingWifiSpeed.AddPoint(this->SGD->TimeSignalSpeedPing, this->Net->Speed);


		}
		else     // eger checkbox'a týklanýrsa en son zamaný alýyor 
			this->SGD->TimeSignalSpeedPing;



		ImGui::SliderFloat("History", &this->SGD->HistorySignal, 1, 300, "% 1.f saniye");  // .1f yaparsak milisaniye olarak ayarlanýyor 


		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

		if (ImPlot::BeginPlot("##Scrolling", ImVec2(500, 250))) {     // grafik ölçeklendirme 
			ImPlot::SetupAxes("Time [s]", "Speed [mm/s]");

			ImPlot::SetupAxisLimits(ImAxis_X1, this->SGD->TimeSignalSpeedPing - this->SGD->HistorySignal, this->SGD->TimeSignalSpeedPing, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, -500, 1900);

			//ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.f);
			ImPlot::PlotLine("Signal", &this->SGD->StokingSignal.Data[0].x, &this->SGD->StokingSignal.Data[0].y, this->SGD->StokingSignal.Data.size(), 0, this->SGD->StokingSignal.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("Ping", &this->SGD->StokingPing.Data[0].x, &this->SGD->StokingPing.Data[0].y, this->SGD->StokingPing.Data.size(), 0, this->SGD->StokingPing.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("Speed", &this->SGD->StokingWifiSpeed.Data[0].x, &this->SGD->StokingWifiSpeed.Data[0].y, this->SGD->StokingWifiSpeed.Data.size(), 0, this->SGD->StokingWifiSpeed.Offset, 2 * sizeof(float));
			// plotline düz cizgili grafik <=> PlotShaded gölgeli grafik  
			ImPlot::EndPlot();
		}
	}





}
void ROTracer::TransmitededRatePage() {
	_zmqLoopFlag = true;
	if (this->_transmitededRatePageVisibility == true) {

		if (this->Net == NULL) {
			return;
		}

		ImGui::BulletText("Move your signal to change the data!");

		ImGui::Checkbox("Pause", &isPauseTransmitededRate);     // duraklatma seçenegi 

		if (!isPauseTransmitededRate)      // eger checkbox'a tıklanmazsa (false)  güncel zamanı alıyor  
		{
			this->SGD->HistoryTransmittededRate = 20.0f;
			this->SGD->TimeTransmittededRate += ImGui::GetIO().DeltaTime;

			this->SGD->StokingTransmitededRate.AddPoint(this->SGD->TimeTransmittededRate, this->Net->TransmitededRate);


		}
		else     // eger checkbox'a tıklanırsa en son zamanı alıyor 
			this->SGD->TimeTransmittededRate;

		ImGui::SliderFloat("History", &this->SGD->HistoryTransmittededRate, 1, 300, "% 1.f saniye");  // .1f yaparsak milisaniye olarak ayarlanıyor 


		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

		if (ImPlot::BeginPlot("##Scrolling", ImVec2(800, 350))) {     // grafik ölçeklendirme 
			ImPlot::SetupAxes("Time [s]", "transmiteded rate  [ ]");

			ImPlot::SetupAxisLimits(ImAxis_X1, this->SGD->TimeTransmittededRate - this->SGD->HistoryTransmittededRate, this->SGD->TimeTransmittededRate, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 30000000);

			//ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.f);
			ImPlot::PlotLine("transmiteded rate ", &this->SGD->StokingTransmitededRate.Data[0].x, &this->SGD->StokingTransmitededRate.Data[0].y, this->SGD->StokingTransmitededRate.Data.size(), 0, this->SGD->StokingTransmitededRate.Offset, 2 * sizeof(float));

			ImPlot::EndPlot();
		}
	}


}

