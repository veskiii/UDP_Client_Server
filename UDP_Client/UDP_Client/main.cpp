#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <time.h>

#pragma comment (lib, "ws2_32.lib")

int idsesji = -1;
bool histSesFlag = 0;
bool histObFlag = 0;


void dodaj_liczby(std::vector<std::string> &ramka) {
	int x, y;
	std::cout << "Podaj liczby: ";
	std::cin >> x >> y;
	ramka.push_back("LX: "+std::to_string(x)+"%");
	ramka.push_back("LY: "+std::to_string(y)+"%");
}

void dodaj_liczbe(std::vector<std::string> &ramka) {
	int x;
	std::cout << "Podaj liczbe: ";
	std::cin >> x;
	ramka.push_back("LX: "+std::to_string(x) + "%");
	ramka.push_back("LY: null%");
}

void dodaj_id_ID(std::vector<std::string> &ramka) {
	int x;
	std::cout << "Podaj ID: ";
	std::cin >> x;
	
	ramka.push_back("LX: " + std::to_string(x) + "%");
	ramka.push_back("LY: null%");
	ramka.push_back("ST: null%");
	ramka.push_back("OB: null%");
}

void dodaj_id_OB(std::vector<std::string> &ramka) {
	int x;
	std::cout << "Podaj ID: ";
	std::cin >> x;

	ramka.push_back("LX: " + std::to_string(x) + "%");
	ramka.push_back("LY: null%");
	ramka.push_back("ST: null%");
	ramka.push_back("OB: null%");
}

void dopisz_reszte_id(std::vector<std::string> &ramka) {
	ramka.push_back("ST: null%");
	//if (idsesji == -1) ramka.push_back("ID: null%");
	//else ramka.push_back("ID: " + std::to_string(idsesji) + "%");
	ramka.push_back("OB: null%");
}

void wybor_dzialania(std::vector<std::string> &ramka) {
	int tmp;
	std::cout << "Wybierz dzialanie:" << std::endl << "[1] Dodawanie" << std::endl << "[2] Odejmowanie"
		<< std::endl << "[3] Mnozenie" << std::endl << "[4] Dzielenie" << std::endl << "[5] Silnia"<< std::endl
		<<"[6] Historia na podstawie ID sesji"<<std::endl<<"[7] Historia na podstawie ID obliczen"<<std::endl;
	std::cin >> tmp;
	switch (tmp) {
	case 1:
		ramka.push_back("OP: dodawanie%");
		dodaj_liczby(ramka);
		dopisz_reszte_id(ramka);
		break;
	case 2:
		ramka.push_back("OP: odejmowanie%");
		dodaj_liczby(ramka);
		dopisz_reszte_id(ramka);
		break;
	case 3:
		ramka.push_back("OP: mnozenie%");
		dodaj_liczby(ramka);
		dopisz_reszte_id(ramka);
		break;
	case 4:
		ramka.push_back("OP: dzielenie%");
		dodaj_liczby(ramka);
		dopisz_reszte_id(ramka);
		break;
	case 5:
		ramka.push_back("OP: silnia%");
		dodaj_liczbe(ramka);
		dopisz_reszte_id(ramka);
		break;
	case 6:
		ramka.push_back("OP: histSesji%");
		dodaj_id_ID(ramka);
		histSesFlag = true;
		break;
	case 7:
		ramka.push_back("OP: histObl%");
		dodaj_id_OB(ramka);
		histObFlag = true;
		break;
	}
}

std::string rozpisz(std::vector<std::string> ramka) {
	std::string data;
	for (auto e : ramka) {
		data += e;
	}
	return data;
}

std::vector<std::string> fragmentuj(std::vector<std::string> ramka) {
	std::vector<std::string> zbior_ramek;
	std::string tmp;
	int tmp2 = ramka.size();
	for (int i = 0; i < tmp2; i++) {
		tmp = "ID: ";
		if (idsesji == -1) tmp += "null%";
		else tmp += std::to_string(idsesji)+"%";
		tmp += "TI: " + std::to_string((time_t)time(NULL))+"%";
		tmp += "NS: " + std::to_string(i) + "%";
		tmp += ramka[0];
		zbior_ramek.push_back(tmp);
		ramka.erase(ramka.begin());
	}
	return zbior_ramek;
}

std::vector<std::vector<std::string>> odczytaj_dane(std::vector<std::string> &odebrane) {
	std::vector<std::vector<std::string>> odczytane; //OP, LX, LY, ST, ID, OB
	odczytane.resize(2);
	odczytane[0].resize(odebrane.size() * 4);
	odczytane[1].resize(odebrane.size() * 4);
	char tmp[1024];
	char tmp2[1024];
	std::string tmp3;
	int i = 0;
	int j = 0;
	std::string delimeter = "%";
	size_t pos = 0;
	for (int k = 0; k < odebrane.size(); k++) {
		while ((pos = odebrane[k].find(delimeter)) != std::string::npos) {
			odczytane[1][i] = odebrane[k].substr(0, pos);
			//std::cout << odczytane[1][i] << std::endl;
			odebrane[k].erase(0, pos + delimeter.length());
			i++;
		}
	}
	delimeter = ": ";
	for (int k = 0; k < odczytane[0].size(); k++) {
		pos = odczytane[1][k].find(delimeter);
		odczytane[0][k] = odczytane[1][k].substr(0, pos);
		odczytane[1][k].erase(0, pos + delimeter.length());
	}
	
	
	return odczytane;
}

std::vector<int> przepisz_flagi(std::vector<std::vector<std::string>> odebrane) {
	std::vector<int> flagi; //Wynik Status ID_sesji ID_obliczen
	flagi.resize(5);
	for (int i = 0; i < odebrane[0].size(); i++) {
		if (odebrane[0][i] == "LX") {
			flagi[0] = std::stoi(odebrane[1][i]);
		}
		
		else if (odebrane[0][i] == "ID") {
			flagi[2] = std::stoi(odebrane[1][i]);
			idsesji = flagi[2];
		}
		else if (odebrane[0][i] == "OB") {
			flagi[3] = std::stoi(odebrane[1][i]);
		}
		else if (odebrane[0][i] == "ST") {
			if (odebrane[1][i] == "obliczyl") {
				flagi[1] = 1;
			}
			else {
				flagi[1] = 0;
			}
		}
	}
	return flagi;
}

void odczytaj_hist_OB(std::vector<std::vector<std::string>> odebrane) {
	std::vector<int> dane; // L1 L2 WY OP 
	dane.resize(4);
	for (int i = 0; i < odebrane[0].size(); i++) {
		//std::cout << odebrane_v2[0][i] << " - " << odebrane_v2[1][i]<<std::endl;
		if (odebrane[0][i] == "MX") dane[2] = std::stoi(odebrane[1][i]);
		if (odebrane[0][i] == "MY") {
			dane[3] = std::stoi(odebrane[1][i]);
			//std::cout << "OTO ZAWARTOSC DANE 3 " << dane[3] << std::endl;
		}
	}
	for (int i = 0; i < odebrane[0].size(); i++) {
		if (odebrane[0][i] == "LX") dane[0] = std::stoi(odebrane[1][i]);
		if (dane[3] != 5 && odebrane[0][i] == "LY") dane[1] = std::stoi(odebrane[1][i]);
	}
	

	std::cout << "Wybrane obliczenie to: " << dane[0];
	
	switch (dane[3]) { // przepisz to na if-y
	case 1:
		std::cout << " + "<<dane[1];
		break;
	case 2:
		std::cout << " - "<<dane[1];
		break;
	case 3:
		std::cout << " * "<<dane[1];
		break;
	case 4:
		std::cout << " / "<<dane[1];
		break;
	case 5:
		std::cout << "!";
		break;
	}
	std::cout << " = " << dane[2] << std::endl;
}

int main(int argc, char* argv[]) {

	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0) {
		std::cout << "Can't start Winsock!" << wsOk;
		return 0;
	}

	sockaddr_in server;
	//server.sin_family = AF_INET;
	//server.sin_port = htons(54000);

	
	std::string address;
	std::cout << "Podaj adres: np. 127.0.0.1 ";
	std::cin >> address;
	//PCSTR adres(address.c_str());

	//inet_pton(AF_INET, adres, &server.sin_addr);

	SOCKET out;
		//= socket(AF_INET, SOCK_DGRAM, 0);

	
	
	int dlugosc;
		//= sizeof(server);

	char bufor[1024];
	char bufor2[1024];
	char bufor3[1024];

	int id_obliczen;

	std::vector<std::string> odebrane;
	std::vector<std::vector<std::string>> odczytane;
	std::vector<int> flagi;

	std::vector<std::string> ramka, wiadomosc;
	
	int sendOk;

	while (true) {
		server.sin_family = AF_INET;
		server.sin_port = htons(54000);
		PCSTR adres(address.c_str());
		inet_pton(AF_INET, adres, &server.sin_addr);
		out = socket(AF_INET, SOCK_DGRAM, 0);



		dlugosc = sizeof(server);
		ramka.clear();
		wiadomosc.clear();
		odebrane.clear();
		odczytane.clear();
		flagi.clear();
		wybor_dzialania(ramka);
		wiadomosc = fragmentuj(ramka);

		for (auto e : wiadomosc) {
			//std::cout << std::endl << e << std::endl;
		}
		
		for (int i = 0; i < wiadomosc.size(); i++) {
			sendOk = sendto(out, wiadomosc[i].c_str(), wiadomosc[i].size() , 0, (sockaddr*)&server, sizeof(server));
			//std::cout << "wyslano se pakiet ";
			if (sendOk == SOCKET_ERROR) {
				std::cout << "Nie wyslano fragmentu "<<i <<"Kod bledu: " << WSAGetLastError() << std::endl;
			}
		}
		
		if (histObFlag) {
			histObFlag = 0;
			//std::cout << "tu1" << std::endl;
			//std::vector<std::string> odebrane_v2;
			//std::vector<std::vector<std::string>> odczytane_v2;
			for (int i = 0; i < 7; i++) {
				//std::cout << "tu "<<i << std::endl;
				ZeroMemory(bufor, 1024);
				int bytesIn = recvfrom(out, bufor, 1024, 0, (sockaddr*)&server, &dlugosc);
				if (bytesIn == SOCKET_ERROR) {
					std::cout << "Odebrano blad transmisji zwrotnej: " << WSAGetLastError() << std::endl;
					continue;
				}
				else {
					//std::cout << "Odebrano: " << bufor << std::endl;
					odebrane.push_back(bufor);
				}
			}
			/*for (int i = 0; i < 7; i++) {
				ZeroMemory(bufor, 1024);
				int bytesIn = recvfrom(out, bufor, 1024, 0, (sockaddr*)&server, &dlugosc);
				if (bytesIn == SOCKET_ERROR) {
					std::cout << "Odebrano blad transmisji zwrotnej: " << WSAGetLastError() << std::endl;
					continue;
				}
				else {
					std::cout << "Odebrano: " << bufor << std::endl;
					odebrane_v2.push_back(bufor);
				}
			}*/
			//std::cout << "p1"<<std::endl;
			odczytane = odczytaj_dane(odebrane);
			//std::cout << "p2" << std::endl;
			for (int i = 0; i < odczytane[0].size(); i++) {
				//std::cout << odczytane[0][i] << " -%^ " << odczytane[1][i] << std::endl;
			}
			//odczytane_v2 = odczytaj_dane(odebrane_v2);
			odczytaj_hist_OB(odczytane);
			odebrane.clear();
			//odebrane_v2.clear();
			odczytane.clear();
			//odczytane_v2.clear();
		}
		else if (histSesFlag) {
			histSesFlag = 0;
			bool flagEnd = 0;
			//std::vector<std::string> odebrane_v2;
			//std::vector<std::vector<std::string>> odczytane_v2;
			while (true) {
				for (int i = 0; i < 7; i++) {
					ZeroMemory(bufor, 1024);
					int bytesIn = recvfrom(out, bufor, 1024, 0, (sockaddr*)&server, &dlugosc);
					if (bytesIn == SOCKET_ERROR) {
						std::cout << "Odebrano blad transmisji zwrotnej: " << WSAGetLastError() << std::endl;
						continue;
					}
					else {
						//std::cout << "Odebrano: " << bufor << std::endl;
						odebrane.push_back(bufor);
					}
				}
				/*for (int i = 0; i < 3; i++) {
					ZeroMemory(bufor, 1024);
					int bytesIn = recvfrom(out, bufor, 1024, 0, (sockaddr*)&server, &dlugosc);
					if (bytesIn == SOCKET_ERROR) {
						std::cout << "Odebrano blad transmisji zwrotnej: " << WSAGetLastError() << std::endl;
						continue;
					}
					else {
						std::cout << "Odebrano: " << bufor << std::endl;
						odebrane_v2.push_back(bufor);
					}
				}*/
				odczytane = odczytaj_dane(odebrane);
				for (int i = 0; i < odczytane[0].size(); i++) {
				//	std::cout << odczytane[0][i] << " -%^ " << odczytane[1][i] << std::endl;
				}
				//odczytane_v2 = odczytaj_dane(odebrane_v2);
				//for (int i = 0; i < odczytane_v2[0].size(); i++) {
					//std::cout << odczytane_v2[0][i] << " -%^ " << odczytane_v2[1][i] << std::endl;
				//}
				odczytaj_hist_OB(odczytane);
				for (int l = 0; l < odczytane[0].size(); l++) {
					if (odczytane[0][l] == "ST" && odczytane[1][l] == "koniec") {
						//std::cout << "flagujseenda" << std::endl;
						flagEnd = true;
					}
				}
				odebrane.clear();
				//odebrane_v2.clear();
				odczytane.clear();
				//odczytane_v2.clear();
				if (flagEnd) break;
			}
		}
		else {
			for (int i = 0; i < 5; i++) {
				ZeroMemory(bufor, 1024);
				int bytesIn = recvfrom(out, bufor, 1024, 0, (sockaddr*)&server, &dlugosc);
				if (bytesIn == SOCKET_ERROR) {
					std::cout << "Odebrano blad transmisji zwrotnej: " << WSAGetLastError() << std::endl;
					continue;
				}
				else {
					//std::cout << "Odebrano: " << bufor << std::endl;
					odebrane.push_back(bufor);
				}
			}

			odczytane = odczytaj_dane(odebrane);
			odebrane.clear();
			flagi = przepisz_flagi(odczytane);
			odczytane.clear();
			if (!flagi[1]) {
				std::cout << "Blad obliczen" << std::endl;
			}
			else {
				std::cout << "Wynik: " << flagi[0] << std::endl;
				std::cout << "ID sesji: " << flagi[2] << std::endl;
				std::cout << "ID obliczen: " << flagi[3] << std::endl;
			}
		}
		std::cout << std::endl << std::endl;
		
	}

	closesocket(out);

	WSACleanup();
	return 0;
}