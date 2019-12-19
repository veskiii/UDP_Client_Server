#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include <string>
#include <time.h>

#pragma comment (lib, "ws2_32.lib")


/*double dodawanko(double x, double y,std::vector<std::vector<double>> *& pamiec,int id_obl,int id_sesj) {
	std::vector<double> pom;
	pom.push_back(x);
	pom.push_back(y);
	pom.push_back(id_obl);
	pom.push_back(id_sesj);
	pom.push_back(x + y);
	//(*pamiec).push_back(pom);
	return x + y;
}*/
int dodawanko(int x, int y) {
	std::wcout << x << " " << y << " " << x + y;
	return x + y;
}

std::vector<int> dodaw(int x, int y, int id_obl, int id_sesj) {
	std::vector<int> pom;
	pom.push_back(x);
	pom.push_back(y);
	pom.push_back(x + y);
	pom.push_back(id_obl);
	pom.push_back(id_sesj);
	pom.push_back(1);
	return pom;
}
int odejmowanko(int x, int y) {
	return x - y;
}
std::vector<int> odej(int x, int y, int id_obl, int id_sesj) {
	std::vector<int> pom;
	pom.push_back(x);
	pom.push_back(y);
	pom.push_back(x - y);
	pom.push_back(id_obl);
	pom.push_back(id_sesj);
	pom.push_back(2);
	return pom;
}
int mnozonko(int x, int y) {
	return x * y;
}
std::vector<int> mno(int x, int y, int id_obl, int id_sesj) {
	std::vector<int> pom;
	pom.push_back(x);
	pom.push_back(y);
	pom.push_back(x * y);
	pom.push_back(id_obl);
	pom.push_back(id_sesj);
	pom.push_back(3);
	return pom;
}
int dzielonko(int x, int y) {
	return x / y;
}
std::vector<int> dziel(int x, int y, int id_obl, int id_sesj) {
	std::vector<int> pom;
	pom.push_back(x);
	pom.push_back(y);
	pom.push_back(x / y);
	pom.push_back(id_obl);
	pom.push_back(id_sesj);

	pom.push_back(4);
	return pom;
}
int silnya(int x) {
	int wynik = x;
	for (int i = x - 1; i > 0; i--) {
		wynik *= i;
	}

	return wynik;
}
std::vector<int> sil(int x, int id_obl, int id_sesj) {
	std::vector<int> pom;
	pom.push_back(x);
	pom.push_back(0);
	pom.push_back(silnya(x));
	pom.push_back(id_obl);
	pom.push_back(id_sesj);
	pom.push_back(5);
	return pom;
}

int main()
{


	std::vector<std::vector<int>> histeria;
	std::vector<int> histpom;
	std::string pomwincyj = "koniec";
	int wynik = 0;

	int OP = 0; int LX = 0; int LY = 0; int NS = 0; int ST = 0; int ID = 0; int OB = 0; //flagi czy otrzymalismy juz wszystko
	int x = 0;
	int y = 0;
	int id_obl = 0;
	int id_sesj = 0;
	int typ = 0; //jakie obliczenia wykonac 0 - dod 1 odej 2 mnoz 3 dziel 4 siln

	//tutaj czesc laczaca sie i wgl
	int p = silnya(7);
	std::cout << p << std::endl;
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0) {
		std::cout << "Nie można Winsocka odpalić kurde" << wsOK;
	}

	//dowolne informacje które przychodzą na serwerowy adres idą do tego socketa
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000); // little endian na big endian

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
		std::cout << "Nie mozna socketa polaczyc" << WSAGetLastError() << std::endl;
		return 0;
	}

	sockaddr_in klient; //klienta ades
	int dlugoscKlienta = sizeof(klient);
	ZeroMemory(&klient, dlugoscKlienta);
	int pomids = 0;
	std::string ramka1;
	std::string ramka2;
	std::string ramka3;
	std::string ramka4;
	std::string ramka5;
	std::string cozatenten;
	char bufor[1024];
	char bufor1[1024];
	char bufor2[1024];
	char bufor3[1024];
	char bufor4[1024];
	int i = 0;
	int j = 0;
	int pomid = 0;
	int procent = 0;
	int bytesIn = 0;
	int hist_se = 0;
	int hist_ob = 0;
	while (true) {

		std::cout << "2";
		//typ = -1;
		ZeroMemory(bufor, 1024);
		std::cout << "3";
		bytesIn = recvfrom(in, bufor, 1024, 0, (sockaddr*)&klient, &dlugoscKlienta);
		std::cout << "odebrano se pakiecik mordo ";
		if (bytesIn == SOCKET_ERROR) {
			std::cout << "blad od klienta odebran" << WSAGetLastError() << std::endl;
			continue;
		}
		char klientIP[256];
		ZeroMemory(klientIP, 256);

		inet_ntop(AF_INET, &klient.sin_addr, klientIP, 256);
		std::cout << "Moj panie, wiesci przybyly od" << klientIP << " : " << bufor << std::endl;

		//dekrypcya sygnalu od klienta
		procent = 0;
		i = 0;
		j = 0;
		ZeroMemory(bufor1, 1024); //bufory pomocnicze
		ZeroMemory(bufor2, 1024);
		ZeroMemory(bufor3, 1024);
		ZeroMemory(bufor4, 1024);

		while (procent < 4) {
			if (procent == 0) {
				while (bufor[i] != '%') {
					bufor1[i] = bufor[i];
					i++;
				}
				bufor1[i] = ' ';
				i++;

				procent++;
			}
			if (procent == 1) {
				while (bufor[i] != '%') {

					bufor2[j] = bufor[i];
					i++;
					j++;
				}
				bufor2[j] = ' ';
				i++;
				j = 0;
				procent++;
			}
			if (procent == 2) {
				while (bufor[i] != '%') {
					bufor3[j] = bufor[i];
					i++;
					j++;
				}
				bufor3[j] = ' ';
				i++;
				j = 0;
				procent++;
			}
			if (procent == 3) {
				while (bufor[i] != '%' and i<1024) {
					bufor4[j] = bufor[i];
					i++;
					j++;
				}
				bufor4[j] = ' ';
				i++;
				j = 0;
				procent++;
			}

			if (i >= 1024)procent = 4;
		}
		std::cout << "Moj panie,  bufor:" << bufor1 << " : " << std::endl << bufor2 << " : " << std::endl << bufor3 << " : " << std::endl;


		if (bufor4[0] == 'O' and bufor4[1] == 'P') {
			if (bufor4[4] == 'd' and bufor4[5] == 'o')typ = 0;
			if (bufor4[4] == 'd' and bufor4[5] == 'z')typ = 3;
			if (bufor4[4] == 'o' and bufor4[5] == 'd')typ = 1;
			if (bufor4[4] == 'm' and bufor4[5] == 'n')typ = 2;
			if (bufor4[4] == 's' and bufor4[5] == 'i')typ = 4;
			if (bufor4[4] == 'h' and bufor4[5] == 'i') {
				if (bufor4[8] == 'S' and bufor4[9] == 'e')typ = 5;
				if (bufor4[8] == 'O' and bufor4[9] == 'b')typ = 6;
			}

			OP = 1;
			std::cout << OP << " " << "TYP: " << typ << "nn";
		}

		char pomfor[1024];
		ZeroMemory(pomfor, 1024);
		if (bufor4[0] == 'L' and bufor4[1] == 'X') {
			i = 3;
			while (bufor4[i] != ':') {
				pomfor[i - 3] = bufor4[i];
				i++;
			}
			x = atoi(pomfor);
			LX = 1;
			std::cout << LX << " x: " << x << " ";
		}
		ZeroMemory(pomfor, 1024);
		if (bufor4[0] == 'L' and bufor4[1] == 'Y') {
			i = 3;
			while (bufor4[i] != ':') {
				pomfor[i - 3] = bufor4[i];
				i++;
			}
			y = atoi(pomfor);
			LY = 1;
			std::cout << LY << " y: " << y << " ";
		}


		/*	if (bufor2[0] == 'O' and bufor2[1] == 'P') {
				if (bufor2[4] == 'd' and bufor2[5] == 'o')typ = 0;
				if (bufor2[4] == 'd' and bufor2[5] == 'z')typ = 3;
				if (bufor2[4] == 'o' and bufor2[5] == 'd')typ = 1;
				if (bufor2[4] == 'm' and bufor2[5] == 'n')typ = 2;
				if (bufor2[4] == 's' and bufor2[5] == 'i')typ = 4;
			}*/


		ZeroMemory(pomfor, 1024);
		if (bufor4[0] == 'L' and bufor4[1] == 'X') {
			i = 3;
			while (bufor4[i] != ':') {
				pomfor[i - 3] = bufor4[i];
				i++;
			}
			x = atoi(pomfor);
			LX = 1;
			std::cout << LX << " x: " << x << " ";
		}
		ZeroMemory(pomfor, 1024);
		if (bufor2[0] == 'L' and bufor2[1] == 'Y') {
			i = 3;
			while (bufor2[i] != ':') {
				pomfor[i - 3] = bufor2[i];
				i++;
			}
			y = atoi(pomfor);
			LY = 1;
			std::cout << LY << " y: " << y << " ";
		}
		if (bufor3[0] == 'N' and bufor3[1] == 'S') {
			NS++;
			std::cout << "NS: " << NS << " ";
		}
		if (bufor4[0] == 'S' and bufor4[1] == 'T') {
			ST = 1;
			std::cout << "ST: " << ST << " ";
		}

		ZeroMemory(pomfor, 1024);
		if (bufor4[0] == 'O' and bufor4[1] == 'B') {
			i = 4;
			if (bufor4[i] != 'n') {
				while (bufor4[i] != ':') {
					pomfor[i - 3] = bufor4[i];
					i++;
				}
				id_obl = atoi(pomfor);
			}
			else {
				//
			}
			OB = 1;
			std::cout << "OB:" << OB << " idobl: " << id_obl << " ";
		}
		
		ZeroMemory(pomfor, 1024);
		if (bufor1[0] == 'I' and bufor1[1] == 'D') {
			i = 4;
			if (bufor1[i] != 'n') {
				while (bufor1[i] != ':') {
					pomfor[i - 4] = bufor1[i];
					i++;
				}
				id_sesj = atoi(pomfor);
			}
			else {
				if (pomids == 0) {
					id_sesj++;
					pomids = 1;
				}

			}
			ID = 1;
			std::cout << "ID:" << ID << " idobl: " << id_sesj << " ";
		}


		std::cout << "OP: " << OP << "LX " << LX << "LY " << LY << "NS " << NS << "ST " << ST << "ID" << ID << "OB" << OB << std::endl;
		//wykonanie obliczen zaleznie od otrzymanych danych
		if (OP == 1 and LX == 1 and LY == 1 and NS > 4 and ST == 1 and ID == 1 and OB == 1) {
			std::cout << "witamy w sliczu " << typ;
			switch (typ) {
			case -1: {
				break;
			}
			case 0: {
				wynik = dodawanko(x, y);
				histeria.push_back(dodaw(x, y, id_obl, id_sesj));
				id_obl++;
				cozatenten = "dodawanie";
				break;
			}
			case 1: {
				wynik = odejmowanko(x, y);
				histeria.push_back(odej(x, y, id_obl, id_sesj));
				id_obl++;
				cozatenten = "odejmowanie";
				break;
			}
			case 2: {
				wynik = mnozonko(x, y);
				histeria.push_back(mno(x, y, id_obl, id_sesj));
				id_obl++;
				cozatenten = "mnozenie";
				break;
			}
			case 3: {
				wynik = dzielonko(x, y);
				histeria.push_back(dziel(x, y, id_obl, id_sesj));
				id_obl++;
				cozatenten = "dzielenie";
				break;
			}
			case 4: { // silnya
				wynik = silnya(x);
				histeria.push_back(sil(x, id_obl, id_sesj));
				id_obl++;
				cozatenten = "silnia";
				break;
			}
			case 5: { //wyswietlanie historii na bazie id sesji
				hist_se = 1;
				for (int z = 0; z < histeria.size(); z++) {

					if (histeria[z][4] == x) {
						histpom.push_back(z);
					}

				}
				break;
			}
			case 6: {//historia na bazie id obl
				hist_ob = 1;
				for (int z = 0; z < histeria.size(); z++) {

					if (histeria[z][3] == x - 1) {
						histpom.push_back(z);
					}

				}
				break;
			}
			}
			OP = 0; LX = 0; LY = 0; NS = 0; ST = 0; ID = 0; OB = 0; //wyzerowanie flag
			typ = -1;
			std::cout << std::endl << "WYNIK: " << wynik << std::endl;
			//odeslanie odpowiedzi

			/*char odpowiedz[1024];
			ZeroMemory(odpowiedz, 1024);
			_itoa_s(wynik, odpowiedz,10);*/
			if (hist_se == 1) {
				for (int z = 0; z < histpom.size(); z++) {
					if (z + 1 >= histpom.size()) {
						pomwincyj = "koniec";
					}
					else pomwincyj = "wincyj";
					ramka1 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 0%OB: " + std::to_string(x) + "%";
					
					std::cout << ramka1 << std::endl;
					int sendOk = sendto(out, ramka1.c_str(), ramka1.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka2 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 1%LX: " + std::to_string(histeria[histpom[z]][0])+"%";
					
					std::cout << ramka2 << std::endl;
					int sendOk2 = sendto(out, ramka2.c_str(), ramka2.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk2 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka3 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 2%LY: " + std::to_string(histeria[histpom[z]][1]) + "%";
					
					std::cout << ramka3 << std::endl;
					int sendOk3 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk3 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}

					ramka4 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 3%ST: " + pomwincyj + "%";
					
					std::cout << ramka4 << std::endl;
					int sendOk4 = sendto(out, ramka4.c_str(), ramka4.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk4 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka4 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 4%OP: histObl%";
					
					std::cout << ramka4 << std::endl;
					int sendOk5 = sendto(out, ramka4.c_str(), ramka4.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk5 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka3 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 5%MX: " + std::to_string(histeria[histpom[z]][2]) + "%";
					
					std::cout << ramka3 << std::endl;
					int sendOk6 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk6 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka3 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 6%MY: " + std::to_string(histeria[histpom[z]][5]) + "%";
		
					std::cout << ramka3 << std::endl;
					int sendOk7 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk7 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
				}
			}
			else if (hist_ob == 1) {
				for (int z = 0; z < histpom.size(); z++) {
					if (z + 1 >= histpom.size()) {
						pomwincyj = "koniec";
					}
					else pomwincyj = "wincyj";
					ramka1 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 0%OB: " + std::to_string(x) + "%";

					std::cout << ramka1 << std::endl;
					int sendOk = sendto(out, ramka1.c_str(), ramka1.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka2 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 1%LX: " + std::to_string(histeria[histpom[z]][0]) + "%";

					std::cout << ramka2 << std::endl;
					int sendOk2 = sendto(out, ramka2.c_str(), ramka2.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk2 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka3 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 2%LY: " + std::to_string(histeria[histpom[z]][1]) + "%";

					std::cout << ramka3 << std::endl;
					int sendOk3 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk3 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}

					ramka4 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 3%ST: " + pomwincyj + "%";

					std::cout << ramka4 << std::endl;
					int sendOk4 = sendto(out, ramka4.c_str(), ramka4.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk4 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka4 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 4%OP: histObl%";

					std::cout << ramka4 << std::endl;
					int sendOk5 = sendto(out, ramka4.c_str(), ramka4.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk5 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka3 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 5%MX: " + std::to_string(histeria[histpom[z]][2]) + "%";

					std::cout << ramka3 << std::endl;
					int sendOk6 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk6 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka3 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 6%MY: " + std::to_string(histeria[histpom[z]][5]) + "%";

					std::cout << ramka3 << std::endl;
					int sendOk7 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk7 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					/*ramka1 = "OP: histObl%LX: " + std::to_string(histeria[histpom[z]][0]) + "%NS: 0%";
					std::cout << ramka1 << std::endl;
					int sendOk = sendto(out, ramka1.c_str(), ramka1.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka2 = "LY: " + std::to_string(histeria[histpom[z]][1]) + "%ST: " + pomwincyj + "%NS: 1%";
					std::cout << ramka2 << std::endl;
					int sendOk2 = sendto(out, ramka2.c_str(), ramka2.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk2 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka3 = "ID: " + std::to_string(id_sesj) + "%OB: " + std::to_string(x) + "%NS: 2%";
					std::cout << ramka3 << std::endl;
					int sendOk3 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk3 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}

					ramka1 = "OP: histObl%LX: " + std::to_string(histeria[histpom[z]][2]) + "%NS: 0%";
					std::cout << ramka1 << std::endl;
					int sendOk4 = sendto(out, ramka1.c_str(), ramka1.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk4 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka2 = "LY: " + std::to_string(histeria[histpom[z]][5]) + "%ST: " + pomwincyj + "%NS: 1%";
					std::cout << ramka2 << std::endl;
					int sendOk5 = sendto(out, ramka2.c_str(), ramka2.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk5 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}
					ramka3 = "ID: " + std::to_string(id_sesj) + "%OB: " + std::to_string(x) + "%NS: 2%";
					std::cout << ramka3 << std::endl;
					int sendOk6 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
					if (sendOk6 == SOCKET_ERROR) {
						std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
					}*/
				}

			}
			else {
				/*ramka1 = "OP: " + cozatenten + "%" + "LX: " + std::to_string(wynik) + "%" + "NS: " + std::to_string(0) + "%";
				std::cout << ramka1 << std::endl;
				int sendOk = sendto(out, ramka1.c_str(), ramka1.size(), 0, (sockaddr*)&klient, sizeof(klient));
				if (sendOk == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}


				ramka2 = "LY: null%ST: obliczyl%NS: " + std::to_string(1) + "%";
				std::cout << ramka2 << std::endl;
				int sendOk2 = sendto(out, ramka2.c_str(), ramka2.size(), 0, (sockaddr*)&klient, sizeof(klient));

				if (sendOk2 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}
				ramka3 = "ID: " + std::to_string(id_sesj) + "%" + "OB: " + std::to_string(id_obl) + "%" + "NS: " + std::to_string(2) + "%";
				std::cout << ramka3 << std::endl;
				int sendOk3 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));

				if (sendOk3 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}*/
				//else pomwincyj = "wincyj";
				ramka1 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 0%OB: " + std::to_string(id_obl) + "%";

				std::cout << ramka1 << std::endl;
				int sendOk = sendto(out, ramka1.c_str(), ramka1.size(), 0, (sockaddr*)&klient, sizeof(klient));
				if (sendOk == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}
				ramka2 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 1%LX: " + std::to_string(wynik) + "%";

				std::cout << ramka2 << std::endl;
				int sendOk2 = sendto(out, ramka2.c_str(), ramka2.size(), 0, (sockaddr*)&klient, sizeof(klient));
				if (sendOk2 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}
				ramka3 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 2%LY: null%";

				std::cout << ramka3 << std::endl;
				int sendOk3 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
				if (sendOk3 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}

				ramka4 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 3%ST: obliczyl%";

				std::cout << ramka4 << std::endl;
				int sendOk4 = sendto(out, ramka4.c_str(), ramka4.size(), 0, (sockaddr*)&klient, sizeof(klient));
				if (sendOk4 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}
				ramka4 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 4%OP: " + cozatenten + "%";

				std::cout << ramka4 << std::endl;
				int sendOk5 = sendto(out, ramka4.c_str(), ramka4.size(), 0, (sockaddr*)&klient, sizeof(klient));
				if (sendOk5 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}
				/*ramka3 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 1%MX: " + std::to_string(histeria[histpom[z]][2]) + "%";

				std::cout << ramka3 << std::endl;
				int sendOk6 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
				if (sendOk6 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}
				ramka3 = "ID: " + std::to_string(id_sesj) + "%TI: " + std::to_string((time_t)time(NULL)) + "%NS: 1%MY: " + std::to_string(histeria[histpom[z]][5]) + "%";

				std::cout << ramka3 << std::endl;
				int sendOk7 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));
				if (sendOk7 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}*/
				/*ramka1 = "OB: " + std::to_string(id_obl) + "%" + "ID: " + std::to_string(id_sesj) + "%" + "NS: " + std::to_string(0) + "%";
				std::cout << ramka1 << std::endl;
				int sendOk = sendto(out, ramka1.c_str(), ramka1.size(), 0, (sockaddr*)&klient, sizeof(klient));
				if (sendOk == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}


				ramka2 = "OP: " + cozatenten + "%" + "ST: " + "obliczyl" + "%" + "NS: " + std::to_string(1) + "%";
				std::cout << ramka2 << std::endl;
				int sendOk2 = sendto(out, ramka2.c_str(), ramka2.size(), 0, (sockaddr*)&klient, sizeof(klient));

				if (sendOk2 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}
				ramka3 = "LX: " + std::to_string(wynik) + "%" + "LY: " + "null" + "%" + "NS: " + std::to_string(2) + "%";
				std::cout << ramka3 << std::endl;
				int sendOk3 = sendto(out, ramka3.c_str(), ramka3.size(), 0, (sockaddr*)&klient, sizeof(klient));

				if (sendOk3 == SOCKET_ERROR) {
					std::cout << "Nie wyslano fragmentu " << i << "Kod bledu: " << WSAGetLastError() << std::endl;
				}*/
			}
			hist_se = 0;
			hist_ob = 0;
			pomids = 0;
			histpom.clear();
		}
	}

	closesocket(in);
	WSACleanup();

	return 0;
}