#include "Protokol.h"

Protokol::Protokol(PacketHeader naglowek, u_int64 rozmiar, string daneP, u_int64 idP)
{
	pakietNaglowek = (char)naglowek;
	rozmiarDanych = rozmiar;
	dane = daneP;
	id = idP;
	pakiet.wstawBit(pakietNaglowek, 7);
	pakiet.wstawBajt(rozmiarDanych);
	if (rozmiar > 0)
	{
		pakiet.wstawBajt(dane, dane.size());
	}
	pakiet.wstawBajt(id);
}

Protokol::Protokol(char * odebrane, int rozmiar)
{
	pakiet.wstawBajt(odebrane, rozmiar);
	pakietNaglowek = pakiet.zwrocBit(0, 0, 7);
	rozmiarDanych = pakiet.zwrocOktet(0, 8, 7);
	if (rozmiar > 9)
	{
		if (rozmiarDanych.to_ullong() > 0)
		{
			dane = zwrocDane();
		}
		else
		{
			dane = "";
		}
		id = pakiet.zwrocOktet(rozmiar - 9, 8, 7);
	}
}

char * Protokol::doWyslania()
{
	vector<Bajt> temp = pakiet.zwrocBajt();
	int messageLenght = temp.size();
	char * toSend = new char[messageLenght];
	for (int i = 0; i < messageLenght; i++)
	{
		toSend[i] = temp[i].to_ullong();
	}
	return toSend;
}

int Protokol::zwrocRozmiar()
{
	return pakiet.zwrocRozmiar();
}

PacketHeader Protokol::zwrocNaglowek()
{
	return static_cast<PacketHeader>(pakietNaglowek.to_ulong());
}

u_int64 Protokol::zwrocRozmiarDanych()
{
	return rozmiarDanych.to_ullong();
}

string Protokol::zwrocDane()
{
	string result = "";
	vector<Bajt> temp = pakiet.zwrocBajt(8, rozmiarDanych.to_ullong(), 7);
	for (auto a : temp)
	{
		result += static_cast<char>(a.to_ullong());
	}
	return result;
}

u_int64 Protokol::zwrocID()
{
	return id.to_ullong();
}

Protokol::~Protokol()
{

}
