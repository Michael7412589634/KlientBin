#include "Transmisja.h"


Transmisja::Transmisja()
{
	obecnyBit = 7;
	obecnyBajt = 0;
	rozmiarCalosci = 0;
}

Transmisja::~Transmisja() {}

void Transmisja::wstawBajt(string wyraz, size_t rozmiar)
{
	for (int i = 0; i < rozmiar; i++) {
		Bajt wynik = static_cast<Bajt>(wyraz[i]);
		wstawBit(wynik, 8);
	}
}

void Transmisja::wstawBajt(char * wyraz, size_t rozmiar)
{
	for (int i = 0; i < rozmiar; i++) {
		Bajt wynik = static_cast<Bajt>(wyraz[i]);
		wstawBit(wynik, 8);
	}
}

void Transmisja::wstawBit(Bajt wyraz, size_t rozmiar)
{
	int test = obecnyBit;
	if (rozmiar > test || rozmiarCalosci == 0)
	{
		wektor.push_back(0);
		rozmiarCalosci++;
	}
	for (int i = 0, j = 7; i < rozmiar && j >= 0; i++, j--) {
		wektor[obecnyBajt][obecnyBit] = wyraz[j];
		obecnyBit--;
		if (obecnyBit < 0)
		{
			obecnyBit = 7;
			obecnyBajt++;
		}
	}
}

void Transmisja::wstawBajt(Oktet wyraz, size_t dlBitowa)
{
	int byteSize = 8;
	for (int i = 7; i >= 0; i--)
	{
		Bajt wynik;
		for (int j = 7 + i*byteSize, k = 7; j >= 0 + i*byteSize; j--, k--) {
			wynik[k] = wyraz[j];
		}
		wstawBit(wynik, 8);
	}
}

Bajt Transmisja::zwrocBit(size_t poczatek, size_t indeks, size_t bit)
{
	Bajt wynik;
	int obBit = 7 - poczatek;
	for (int i = 0, j = 7; i < bit && j >= 0; i++, j--)
	{
		wynik[j] = wektor[indeks][obBit];
		obBit--;
		if (obBit < 0)
		{
			obBit = 7;
			indeks++;
		}
	}
	return wynik;
}

Oktet Transmisja::zwrocOktet(size_t bajtPoczatek, size_t ilBajtow, size_t bitPoczatek)
{
	Oktet wynik;
	Bajt nwOktet;
	int byteSize = 8;
	for (int i = 0; i < 8; i++)
	{
		nwOktet = zwrocBit(bitPoczatek, bajtPoczatek + ilBajtow - 1 - i);
		for (int j = 7 + i*byteSize, k = 7; j >= 0 + i*byteSize; j--, k--) {
			wynik[j] = nwOktet[k];
		}
	}
	return wynik;
}

vector<Bajt> Transmisja::zwrocBajt(size_t bajtPoczatek, size_t ilBajtow, size_t bitPoczatek)
{
	vector<Bajt> wynik;
	Bajt bajt;
	if (ilBajtow<0 || ilBajtow > rozmiarCalosci)
	{
		ilBajtow = rozmiarCalosci;
	}
	for (int i = 0; i < ilBajtow; i++)
	{
		bajt = zwrocBit(bitPoczatek, bajtPoczatek + i);
		wynik.push_back(bajt);
	}
	return wynik;
}

string Transmisja::toString()
{
	string result = "", byteString;
	for (auto a : wektor) {
		byteString = a.to_string();
		result += byteString + " ";
	}
	return result;
}

size_t Transmisja::zwrocRozmiar()
{
	return rozmiarCalosci;
}
