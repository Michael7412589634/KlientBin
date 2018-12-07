#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Protokol.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib,"ws2_32.lib") 

class Klient
{
public:
	Klient(std::string IP, int PORT);
	bool Polacz();
	bool zakonczPolaczenie();
	u_int64 zwrocID();
	bool wyslijProtokol(Protokol * protocol);
	bool innyKlient;
	bool oczekujaceZaproszenie;
	bool wyslaneZaproszenie;
	bool polaczony;
	SOCKET polaczenie;
private:
	WSAData wsaData;
	WORD DllVersion;
	SOCKADDR_IN adres; //Addres to bind connection socket to
	int dlAdresu;
	u_int64 id;
	Protokol * recvProtocol();
	bool processProtocol(Protokol * protocol);
	static void ClientThread();
};

static Klient * klientWsk;

