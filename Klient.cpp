#include "Klient.h"

Klient::Klient(std::string IP, int PORT)
{
	DllVersion = MAKEWORD(2, 2);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}
	
	dlAdresu = sizeof(adres);
	inet_pton(AF_INET, IP.c_str(), &(adres.sin_addr));
	adres.sin_port = htons(PORT);
	adres.sin_family = AF_INET;
	innyKlient = false;
	id = 0ll;
	oczekujaceZaproszenie = false;
	wyslaneZaproszenie = false;
	klientWsk = this;
	polaczony = false;
}

bool Klient::Polacz()
{
	polaczenie = socket(AF_INET, SOCK_STREAM, NULL);
	int kod = connect(polaczenie, (SOCKADDR*)&adres, sizeof(adres));
	if (kod != 0)
	{
		kod = WSAGetLastError();
		std::cout << "Nie mozna polaczyc z serwerem, kod: " << kod;
		return false;
	}

	std::cout << "Polaczono z serwerem!" << std::endl;
	polaczony = true;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	send(polaczenie, "Zmienna=Test&Druga=Trzy", 64, NULL);
	//return wyslijProtokol(new Protokol(PacketHeader::ServerRequestID));
}

bool Klient::zakonczPolaczenie()
{
	wyslijProtokol(new Protokol(PacketHeader::Quit, 0, "", id));
	if (closesocket(polaczenie) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK){
			polaczony = false;
			return true;
		}
		cout << "Wystapil blad: " << WSAGetLastError() << ". #\n";
		return false;
	}
	polaczony = false;
	return true;
}

u_int64 Klient::zwrocID()
{
	return id;
}

Protokol * Klient::recvProtocol()
{
	Protokol * pr;
	int rozmiarBufora = 1500;
	char * bufor = new char[rozmiarBufora];
	int check = recv(polaczenie, bufor, rozmiarBufora, NULL);
	if (check == SOCKET_ERROR)
		return nullptr;

	pr = new Protokol(bufor, 9);
	int recvSize = pr->zwrocRozmiarDanych();
	recvSize += 17;
	delete pr;

	pr = new Protokol(bufor, recvSize);
	delete[] bufor;
	return pr;
}

bool Klient::wyslijProtokol(Protokol * pr)
{
	char * wiadomosc = pr->doWyslania();
	int dlWiad = pr->zwrocRozmiar();
	int check = send(polaczenie, wiadomosc, dlWiad, NULL);
	if (check == SOCKET_ERROR)
	{
		delete pr;
		return false;
	}
	delete pr;
	return true;
}

bool Klient::processProtocol(Protokol * pr)
{
	if (pr == nullptr)
		return false;

	PacketHeader packetHeader = pr->zwrocNaglowek();
	u_int64 rozmDanych = pr->zwrocRozmiarDanych();
	string data = "";
	if (rozmDanych > 0)
	{
		data = pr->zwrocDane();
	}
	switch (packetHeader)
	{
	case PacketHeader::ServerSendID:
	{
		id = pr->zwrocID();
		Sleep(100);
		cout << "Identyfikator sesji: " << id << " \n";
		break;
	}
	case PacketHeader::Invite:
	{
		cout << "Otrzymano zaproszenie\n";
		oczekujaceZaproszenie = true;
		break;
	}
	case PacketHeader::InviteAccpet:
	{
		cout << "Zaproszenie zostalo zaakceptowane\n";
		innyKlient = true;
		break;
	}
	case PacketHeader::InviteDecline:
	{
		cout << "Zaproszenie zostalo odrzucone\n";
		wyslaneZaproszenie = false;
		innyKlient = false;
		break;
	}
	case PacketHeader::Bye:
	{
		cout << "Uzytkownik zakonczyl z Toba polaczenie\n";
		innyKlient = false;
		wyslaneZaproszenie = false;
		break;
	}
	case PacketHeader::Message:
	{
		cout << "Odebrano: " << data << "\n";
		break;
	}
	case PacketHeader::ServerAlone:
	{
		cout << "Uzytkownik odlaczyl sie od serwera #\n";
		innyKlient = false;
		wyslaneZaproszenie = false;
		break;
	}
	}
	return true;
}

void Klient::ClientThread()
{
	Protokol * protocol;
	while (true)
	{
		protocol = klientWsk->recvProtocol();
		if (protocol == nullptr)
			break;
		if (!klientWsk->processProtocol(protocol))
			break;
		delete protocol;
	}
	klientWsk->zakonczPolaczenie();
	klientWsk->polaczony = false;
}
