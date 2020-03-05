#include <iostream>
#include<cstdlib>
#include<fstream>
#include<math.h>
#include<Windows.h>
#include<conio.h>
#include<sstream>

using namespace std;
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

int minIndexX;
int minIndexY;
int maxIndexX;
int maxIndexY;
const int genislik = 61;
const int yukseklik = 61;
float araliksabiti = 1000 / 60;
char grafik[genislik][yukseklik];
struct agirlikMerkezi {
	int x;
	int y;
	
};
struct kose {
	int x, y;
};
struct gruplar {

	string GrupAdi;
	int x;
	int y;
	kose solUstKose;
	kose solAltKose;
	kose sagUstKose;
	kose sagAltKose;
	agirlikMerkezi agirlik;
	float uzaklik;
};
void grafikTemizle();
void randomGrupUretici(int xMin, int xMax, int yMin, int yMax, gruplar grup[], string GrupAd);
void maxBul(gruplar grup[]);
void minBul(gruplar grup[]);
void Siniflandir(gruplar yeniEleman, gruplar grup1[], gruplar grup2[], gruplar grup3[]);
gruplar* uzaklikBul(gruplar yeniEleman, gruplar grup[]);
gruplar* uzaklikSirala(gruplar yeniEleman, gruplar grup[]);
void knnSiniflandirmasi(gruplar yeniEleman, gruplar grup1[], gruplar grup2[], gruplar grup3[], int karsilastirmaAdedi);
void Color(int color_code);
void AgirlikMerkeziBul(gruplar grup[]);
void grafikCiz();
void eksenleriCiz();
void grafigeYerlestir(gruplar grup[]);
void fontsize(int, int);




int main()
{
	
	gruplar aGrubu[100];
	gruplar bGrubu[100];
	gruplar cGrubu[100];
	int aMinSinirX = 100, bMinSinirX = 250, cMinSinirX = 400, aMinSinirY = 400, bMinSinirY = 100, cMinSinirY = 400;
	int aMaxSinirX = 300, bMaxSinirX = 450, cMaxSinirX = 800, aMaxSinirY = 700, bMaxSinirY = 450, cMaxSinirY = 800;


	randomGrupUretici(aMinSinirX, aMaxSinirX, aMinSinirY, aMaxSinirY, aGrubu, "A Grubu");
	randomGrupUretici(bMinSinirX, bMaxSinirX, bMinSinirY, bMaxSinirY, bGrubu, "B Grubu");
	randomGrupUretici(cMinSinirX, cMaxSinirX, cMinSinirY, cMaxSinirY, cGrubu, "C Grubu");

	
	gruplar yeniEleman;
	cout << "Yeni elemanin x ve y degerlerini giriniz:(0-950 arasinda bir deger giriniz..)" << endl;
	cin >> yeniEleman.x >> yeniEleman.y;

	
	fontsize(18, 18);
	
	cout << endl << endl;
	cout << "0,0" << endl;
	grafikTemizle();
	grafigeYerlestir(aGrubu);
	grafigeYerlestir(bGrubu);
	grafigeYerlestir(cGrubu);
	eksenleriCiz();
	int xDegeri = ceil(yeniEleman.x / araliksabiti);
	int yDegeri = ceil(yeniEleman.y / araliksabiti);
	
	grafik[xDegeri][yDegeri] = 'X';
	
	grafikCiz();

	Color(6);
	Siniflandir(yeniEleman, aGrubu, bGrubu, cGrubu);
	knnSiniflandirmasi(yeniEleman, aGrubu, bGrubu, cGrubu, 5);

	Color(7);

/*	ofstream dosyaYaz("siniflandirma.xls");
	for (int i = 0; i < 100; i++) {
		if (i == 0) {
			dosyaYaz << "A Grubu \t\tB Grubu \t\tC Grubu" << endl;
		}
		dosyaYaz << aGrubu[i].x << "\t" << aGrubu[i].y << "\t" << bGrubu[i].x << "\t" << bGrubu[i].y << "\t" << cGrubu[i].x << "\t" << cGrubu[i].y << endl;

	}
	dosyaYaz.close();*/
	return 0;
}
void randomGrupUretici(int xMin, int xMax, int yMin, int yMax, gruplar grup[], string GrupAd) {
	
	for (int i = 0; i < 100; i++) {
		grup[i].sagAltKose.x = xMax;
		grup[i].sagAltKose.y = yMin;
		grup[i].solUstKose.x = xMin;
		grup[i].solUstKose.y = yMax;
		grup[i].solAltKose.x = xMin;
		grup[i].solAltKose.y = yMin;
		grup[i].sagUstKose.x = xMax;	
		grup[i].sagUstKose.y = yMax;
		
		grup[i].GrupAdi = GrupAd;
		grup[i].x = (rand() % (xMax - xMin)) + xMin;
		grup[i].y = (rand() % (yMax - yMin)) + yMin;
	}
}



void maxBul(gruplar grup[]) {
	int xMax = grup[0].x;
	int yMax = grup[0].y;


	for (int i = 0; i < 100; i++) {
		if (grup[i].x > xMax) {
			xMax = grup[i].x;
			maxIndexX = i;

		}
		if (grup[i].y > yMax) {
			yMax = grup[i].y;
			maxIndexY = i;
		}
	}
}
void minBul(gruplar grup[]) {
	int xMin = grup[0].x;
	int yMin = grup[0].y;

	for (int i = 0; i < 100; i++) {
		if (grup[i].x < xMin) {
			xMin = grup[i].x;
			minIndexX = i;

		}
		if (grup[i].y < yMin) {
			yMin = grup[i].y;
			minIndexY = i;
		}
	}

}
void AgirlikMerkeziBul(gruplar grup[]) {
	 
	grup[0].agirlik.x =grup[0].solAltKose.x+ ceil(abs(grup[0].sagAltKose.x - grup[0].solAltKose.x) / 2);
	grup[0].agirlik.y =grup[0].solAltKose.y + ceil(abs(grup[0].sagUstKose.y - grup[0].sagAltKose.y) / 2);


}
void Siniflandir(gruplar yeniEleman, gruplar grup1[], gruplar grup2[], gruplar grup3[]) {
	float agirlikUzaklik1, agirlikUzaklik2, agirlikUzaklik3;
	AgirlikMerkeziBul(grup1);
	AgirlikMerkeziBul(grup2);
	AgirlikMerkeziBul(grup3);

	agirlikUzaklik1 = sqrt((grup1[0].agirlik.x - yeniEleman.x) * (grup1[0].agirlik.x - yeniEleman.x) + (grup1[0].agirlik.y - yeniEleman.y) * (grup1[0].agirlik.y - yeniEleman.y));
	agirlikUzaklik2 = sqrt((grup2[0].agirlik.x - yeniEleman.x) * (grup2[0].agirlik.x - yeniEleman.x) + (grup2[0].agirlik.y - yeniEleman.y) * (grup2[0].agirlik.y - yeniEleman.y));
	agirlikUzaklik3 = sqrt((grup3[0].agirlik.x - yeniEleman.x) * (grup3[0].agirlik.x - yeniEleman.x) + (grup3[0].agirlik.y - yeniEleman.y) * (grup3[0].agirlik.y - yeniEleman.y));
	if (agirlikUzaklik1 < agirlikUzaklik2 && agirlikUzaklik1 < agirlikUzaklik3) {
		cout << "Agirlik merkezine olan uzakliklarina gore : " << grup1[0].GrupAdi << " uyedir.." << endl;
	}
	if (agirlikUzaklik2 < agirlikUzaklik1 && agirlikUzaklik2 < agirlikUzaklik3) {
		cout << "Agirlik merkezine olan uzakliklarina gore : " << grup2[0].GrupAdi << " uyedir.." << endl;
	}
	if (agirlikUzaklik3 < agirlikUzaklik1 && agirlikUzaklik3 < agirlikUzaklik2) {
		cout << "Agirlik merkezine olan uzakliklarina gore : " << grup3[0].GrupAdi << " uyedir.." << endl;
	}
}
gruplar* uzaklikBul(gruplar yeniEleman, gruplar grup[]) {
	for (int i = 0; i < 100; i++) {
		grup[i].uzaklik = sqrt((grup[i].x - yeniEleman.x) * (grup[i].x - yeniEleman.x) + (grup[i].y - yeniEleman.y) * (grup[i].y - yeniEleman.y));
	}
	return grup;
}
gruplar* uzaklikSirala(gruplar yeniEleman, gruplar grup[]) {
	grup = uzaklikBul(yeniEleman, grup);

	gruplar bosKutu;
	int min;
	for (int i = 0; i < 100; i++) {
		min = i;
		for (int j = i; j < 100; j++) {
			if (grup[j].uzaklik < grup[min].uzaklik) {
				min = j;

			}

		}
		bosKutu = grup[i];
		grup[i] = grup[min];
		grup[min] = bosKutu;
	}

	return grup;
}

void knnSiniflandirmasi(gruplar yeniEleman, gruplar grup1[], gruplar grup2[], gruplar grup3[], int karsilastirmaAdedi) {
	uzaklikSirala(yeniEleman, grup1);
	uzaklikSirala(yeniEleman, grup2);
	uzaklikSirala(yeniEleman, grup3);
	int indis1 = 0, indis2 = 0, indis3 = 0, grup1puan = 0, grup2puan = 0, grup3puan = 0;
	for (int i = 0; i < karsilastirmaAdedi; i++) {
		if (grup1[indis1].uzaklik < grup2[indis2].uzaklik && grup1[indis1].uzaklik < grup3[indis3].uzaklik) {
			indis1++;
			grup1puan++;
		}
		if (grup2[indis2].uzaklik < grup1[indis1].uzaklik && grup2[indis2].uzaklik < grup3[indis3].uzaklik) {
			indis2++;
			grup2puan++;
		}
		if (grup3[indis3].uzaklik < grup1[indis1].uzaklik && grup3[indis3].uzaklik < grup2[indis2].uzaklik) {
			indis3++;
			grup3puan++;
		}
	}
	if (grup1puan > grup2puan&& grup1puan > grup2puan) {
		cout << "Knn algoritmasina gore : " << grup1[0].GrupAdi << " uyedir.." << endl;

	}
	if (grup2puan > grup1puan&& grup2puan > grup3puan) {
		cout << "Knn algoritmasina gore : " << grup2[0].GrupAdi << " uyedir.." << endl;

	}
	if (grup3puan > grup1puan&& grup3puan > grup2puan) {
		cout << "Knn algoritmasina gore : " << grup3[0].GrupAdi << " uyedir.." << endl;

	}
}

void grafigeYerlestir(gruplar grup[]) {
	for (int i = 0; i < 100; i++) {
		float xKonumi, yKonumi;
		int xDeger, yDeger;
		xKonumi = grup[i].x / araliksabiti;
		yKonumi = grup[i].y / araliksabiti;
		xDeger = xKonumi;
		yDeger = yKonumi;
		stringstream sstream;
		sstream << grup[i].GrupAdi;
		char deger;
		sstream >> deger;
		
		if (0.5 > (xKonumi - xDeger) && 0.5 > (yKonumi - yDeger)) {
			int x = ceil(xKonumi);
			int y = ceil(yKonumi);
			grafik[x][y] = deger;

		}
		else if (0.5 > (xKonumi - xDeger) && 0.5 < (xKonumi - xDeger)) {
			int x, y;
			x = ceil(xKonumi);
			y = floor(yKonumi);
			grafik[x][y] = deger;
		}
		else if (0.5 < (xKonumi - xDeger) && 0.5 < (xKonumi - xDeger)) {
			int x = floor(xKonumi);
			int y = floor(yKonumi);
			grafik[x][y] = deger;
		}
		else if (0.5 < (xKonumi - xDeger) && 0.5 > (xKonumi - xDeger)) {
			int x = floor(xKonumi);
			int y = ceil(yKonumi);
			grafik[x][y] = deger;
		}
		else {
			
		}

	}
}
void eksenleriCiz() {
	for (int x = 0; x < genislik; x++) {
		grafik[x][0] = '-';
	}
	for (int y = 0; y < yukseklik; y++) {
		grafik[0][y] = '|';
	}

}
void grafikCiz() {

	for (int y = 0; y < yukseklik; y++) {
		for (int x = 0; x < genislik; x++) {
			if (grafik[x][y] == 'A') {
				Color(1);
			}
			else if (grafik[x][y] == 'B') {
				Color(2);
			}
			else if (grafik[x][y] == 'C') {
				Color(3);
			}
			else {
				Color(7);
			}
			Sleep(1);
			cout << grafik[x][y];
		}
		cout << endl;
	}
	Color(7);
}
void Color(int color_code) {
	SetConsoleTextAttribute(out, color_code);
}
void fontsize(int a, int b) {
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
	lpConsoleCurrentFontEx->dwFontSize.X = a;
	lpConsoleCurrentFontEx->dwFontSize.Y = b;
	SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}
void grafikTemizle(){
	for(int x=0;x<genislik;x++){
		for(int y =0;y<yukseklik;y++){
			grafik[x][y]=' ';
		}
	}
	
}
