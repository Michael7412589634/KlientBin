#pragma once

#include <bitset>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

typedef bitset<8> Bajt;
typedef bitset <64> Oktet;

class Transmisja
{
private:
	vector<Bajt> wektor;
	int obecnyBit;
	int obecnyBajt;
	int rozmiarCalosci;
public:
	Transmisja();
	~Transmisja();
	void wstawBajt(string wyraz, size_t rozmiar);
	void wstawBajt(char * wyraz, size_t rozmiar);
	void wstawBajt(Oktet wyraz, size_t dlBitowa = 64);
	void wstawBit(Bajt wyraz, size_t rozmiar);
	Bajt zwrocBit(size_t poczatek, size_t indeks, size_t bit = 8);
	Oktet zwrocOktet(size_t bajtPoczatek, size_t ilBajtow, size_t bitPoczatek = 0);
	vector<Bajt> zwrocBajt(size_t bajtPoczatek = 0, size_t ilBajtow = -1, size_t bitPoczatek = 0);
	string toString();
	size_t zwrocRozmiar();
};

