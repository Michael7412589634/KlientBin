#pragma once

#include "Transmisja.h"
#include "Pakiet.h"
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib") 

class Protokol
{
private:
	Transmisja pakiet;
	Bajt pakietNaglowek;
	Oktet rozmiarDanych;
	string dane;
	Oktet id;
public:
	Protokol(PacketHeader naglowek, u_int64 rozmiar = 0, string daneP = "", u_int64 idP = 0);
	Protokol(char * odebrane, int rozmiar);
	char *  doWyslania();
	int zwrocRozmiar();
	PacketHeader zwrocNaglowek();
	u_int64 zwrocRozmiarDanych();
	string zwrocDane();
	u_int64 zwrocID();
	~Protokol();
};

