#include "data_for_graphic.h"   //data_for_graphic deki veriler i�in �ag�r�yoruz 
using namespace std;

//--------------------------------grafik �izme alan�-------------
ROTracer::ROTracer() {
	this->Agv = new AgvData();
	_loginPageVisibility = true;
}

ROTracer::~ROTracer() {
	delete this->Agv;
}

void ROTracer::StartStreamParser() {
	if (!_isRunning)
	{
		this->_zmqLoopFlag = true;
		std::thread lth(&ROTracer::ZMQDataStreamParser,this);
		lth.detach();
	}
}

void ROTracer::StopStreamParser() {
	this->_zmqLoopFlag = false;
}

void ROTracer::SpeedPage() {
	_zmqLoopFlag == true;
	if (this->_speedPageVisibility == true) {

		if (this->Agv == NULL) {
			return;
		}

		ImGui::BulletText("Move your speed to change the data!");

		static ScrollingBuffer sdata1, sdata2;
		static RollingBuffer   rdata1, rdata2;


		static float t = 0;
		t += ImGui::GetIO().DeltaTime;
		sdata1.AddPoint(t, this->Agv->rs);
		sdata2.AddPoint(t, this->Agv->ws);

		static float history = 20.0f;   // ba�lang��ta ka� saniyelik k�sm� g�sterecek 
		ImGui::SliderFloat("History", &history, 1, 300, "% 1.f saniye");  // .1f yaparsak milisaniye olarak ayarlan�yor 
		rdata1.Span = history;
		rdata2.Span = history;

		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;


		if (ImPlot::BeginPlot("##Scrolling", ImVec2(800, 350))) {     // grafik �l�eklendirme 
			ImPlot::SetupAxes("Time [s]", "Speed [mm/s]");

			ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, -500, 1900);

			//ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.f);
			ImPlot::PlotLine("rs X", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), 0, sdata1.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("ws Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
			// plotline d�z cizgili grafik <=> PlotShaded g�lgeli grafik  
			ImPlot::EndPlot();
		}

	}
}


//--------------------------------giri� sayfas�-------------------------------------------------------

void ROTracer::LoginPage() {
	//_zmqLoopFlag = false;
	if (this->_loginPageVisibility == true)
	{
		ImGui::SetNextWindowSize(ImVec2(920, 520));
		ImGui::Begin("PAGE", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;
		style.WindowRounding = 0.9f; // Pencere k��elerinin yuvarlanma miktar�
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
		// Pencere ba�l��� ve IP adresi giri� kutusu
		ImGui::Text("                                                       Enter IP Address");
		ImGui::Spacing();

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(" ", NULL, true).x) / (1, 5));
		ImGui::InputText(" ", this->IpAddress, 32, ImGuiInputTextFlags_CharsNoBlank);

		ImGui::Spacing();

		// IP adresi giri�i ve "Login" d��mesi
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Login", NULL, true).x) / 2);

		// IP adresi giri�i ve "Login" d��mesi
		if (ImGui::Button("Login")) {
			// IP adresi do�rulama
			std::regex ip_regex("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
			if (!std::regex_match(this->IpAddress, ip_regex)) {
				ImGui::OpenPopup("Warning");
			}
			else {

				// Login d��mesine t�kland���nda, saklanan IP adresi ile di�er sayfaya ge�me
				std::cout << "Entered IP Address:" << this->IpAddress << std::endl;
				ImGui::OpenPopup("New Page");
				this->_loginPageVisibility = false;

				this->StartStreamParser();
			}
		}

		// IP adresi uyar� mesaj�
		if (ImGui::BeginPopupModal("Uyari", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
			ImGui::Text("Please enter a valid IP address.");
			if (ImGui::Button("Ok")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	// Yeni sayfa penceresi
	if (!this->_loginPageVisibility) {

		// ImGui::BeginPopupModal("Yeni Sayfa", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::Begin("New Page");
		ImGui::SetNextWindowSize(ImVec2(920, 520));
		// Yeni sayfa i�eri�i ve IP adresi bilgisi
	   //--------------------------------------------------
		_speedPageVisibility = true;
		this->SpeedPage();
		//------------------------------------------------------------
		ImGui::Text("Entered IP Address: %s", this->IpAddress); // Girilen IP adresini g�stermek i�in metin kutusu olu�turma

		//// Yeni sayfa butonlar�
		//if (ImGui::Button("Ok")) {
		//    ImGui::CloseCurrentPopup(); // Pencereyi kapat�n
		//    
		//}

		ImGui::End();
	}

}
//-------------------------------DATA PARSER BLOCK-----------------------------------------------------------

void ROTracer::ZMQDataStreamParser()
{

	void* context = zmq_ctx_new();
	void* subscriber = zmq_socket(context, ZMQ_SUB);
	std::string take_ip = this->IpAddress;
	std::string  a = "tcp://" + take_ip + ":5556";
	//std::string b = ("tcp://%s:%d", take_ip,take_port);

	int rc = zmq_connect(subscriber, a.c_str());  //"tcp://192.168.2.125:5556"

	rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);


	const int topic_size = 5;
	const int msg_size = 128;

	char topic[topic_size] = { 0 };
	char msg[msg_size] = { 0 };
	std::string new_msg(msg);
	size_t sayac = 0;
	char* pch;
	_isRunning = true;

	printf("start");

	while (this->_zmqLoopFlag)
	{
		rc = zmq_recv(subscriber, topic, topic_size, 0);
		if (rc != -1)
			fprintf(stdout, "TOPIC: %s \n", topic);

		rc = zmq_recv(subscriber, msg, msg_size, 0);
		if (rc != -1) {

			if (std::strcmp("real", topic) == 0) {
				pch = strtok(msg, ";");
				while (pch != NULL)
				{
					if (sayac == 0) {
						Agv->x = stoi(pch);
						sayac++;

					}
					else if (sayac == 1) {
						Agv->y = stoi(pch);
						sayac++;

					}
					else if (sayac == 2) {
						Agv->a = std::stof(pch);
						sayac++;

					}
					else if (sayac == 3) {
						Agv->ws = stoi(pch);
						sayac++;

					}
					else if (sayac == 4) {
						Agv->rs = stoi(pch);
						sayac++;

					}
					else if (sayac == 5) {
						Agv->wwa = std::stof(pch);
						sayac++;

					}
					else if (sayac == 6) {
						Agv->rwa = std::stof(pch);
						sayac++;

					}



					//printf("%s\n", pch);
					pch = strtok(NULL, ";");
				}
				sayac = 0;
			}
			//printf("-----------------------");
			//fprintf(stdout, "MSG: %s \n", msg);
		}
	}

	zmq_close(subscriber);
	zmq_ctx_destroy(context);

	_isRunning = false;

}

//----------------------------------------------------------------------------------------------------



















/*
// plotline d�z cizgili grafik <=> PlotShaded g�lgeli grafik
if (ImPlot::BeginPlot("##Rolling", ImVec2(1000, 250))) {    // kortinat sistemi 1.b�lge
		ImPlot::SetupAxes("Time [s]", "Speed [mm/s]");            // x ve y bile�enleri
		//ImPlot::SetupAxesLimits(0, 30, -500, 5000);
		//ImPlot::SetupAxes(NULL, NULL, flags, flags);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1500);
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		ImPlot::PlotLine("rs X", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, 2 * sizeof(float));
		ImPlot::PlotLine("ws Y", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 0, 2 * sizeof(float));
		ImPlot::EndPlot();
	}  */




	/*
	void Demo_plots(){
		constexpr int N = 10000;

		static double x[N];
		static double y[N];
		static double t[N];


		// plot waveforms
		if (ImPlot::BeginPlot("##Filter", ImVec2(-1, -1))) {
			ImPlot::SetupAxes("Time [s]", "Speed [mm/s]");            // x ve y bile�enleri
			ImPlot::SetupAxesLimits(0, 30, -500, 5000);           // (ba�lang�� x ,biti� x, ba�lang�s y ,biti� y )

			ImPlot::SetupLegend(ImPlotLocation_NorthEast);
			//ImPlot::PlotLine("x(t)", t, d1->ws, N);              //(grafik ad�,   ------   ,x bile�eni yani zaman ,saya� (tur))
			//ImPlot::PlotLine("y(t)", t, d1->rs, N);              //(bile�en ad� ,    -----  ,y bile�eni yani h�z ,sayac (tur ))
			ImPlot::EndPlot();
		}

	}
	*/