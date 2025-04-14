#include <iostream>  // Gerekli kütüphanelerin eklenmesi
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <string>
#include <locale.h>



using namespace std;
const int MAX_TAVUK_SAYISI = 500;   //Kurucu fonksiyon için gerekli olan değişkenler main fonksiyonu içinde kullanıbilmesi için global değişken olarak atama.
double baslangicBakiye;
double tavukAlimFiyati;
double yemFiyat;
double yumurtaFiyati;
int gunSayisi;
int tavukSayisi;

class Tavuk {    //Tavuk sınıfı oluşturma
private:
    int yumurtlamamaGunSayisi; // Private değişkenler
    int yumurtaSayisi;
    int toplamYumurtaSayisi;
    bool yeniTavuk;     //Tavuğun yeni olup olmadığını kontrol etmek için

public:
    Tavuk() : yumurtlamamaGunSayisi(0), yumurtaSayisi(0), toplamYumurtaSayisi(0), yeniTavuk(true) {} // Tavuk sınıfı içinde bazı değişkenleri sıfırlayan fonskiyon tanımlanması

    void yumurtlamafonksiyonu() {  //Yumurtlama fonksiyonu
        if (yeniTavuk) {      // Eğer tabuk çiftliğe yeni geldiyse. ilk 3 gün yumurtlamamamsı önemsenmez.
            yumurtlamamaGunSayisi++;
            if (yumurtlamamaGunSayisi >= 3) {
                yeniTavuk = false;  //Yeni gelen tavuk 3 gün sonunda yenitavuk özelliğini kaybeder.
                yumurtlamamaGunSayisi = 0; //Yumurtlama gün sayısı resetlenir
            }
            yumurtaSayisi = 0; // Yeni tavuk ilk 3 gün yumurtlamaz
        }
        else {       //Çiftlikte yeni olmayan tavuklar için yumurtlama fonksiyonu
            yumurtaSayisi = rand() % 3; // 0, 1 veya 2 yumurta 
            if (yumurtaSayisi == 0) yumurtlamamaGunSayisi++;  //Yumurtlamazsa yumurtalamam gün değerini 1 arttır. 
            else {
                toplamYumurtaSayisi += yumurtaSayisi;         //Yumurtlarsa yumurtlamama gün değerini sıfırla ve yumurta sayısını arttır.
                yumurtlamamaGunSayisi = 0;
            }
        }
    }

    int yumurtaSayisiAtama() const {
        return yumurtaSayisi;
    }

    bool kesimKontrol() const { //Tavukların kesiliğ kesilmemesini kontrol eden fonksiyon
        return (!yeniTavuk && yumurtlamamaGunSayisi >= 3);
    }

    void reset() {   //Kesilen tavuk yerine alınan tavuk
        yumurtaSayisi = 0;
        yumurtlamamaGunSayisi = 0;
        toplamYumurtaSayisi = 0;
        yeniTavuk = true;
    }
};

double sayialma(bool ondalikIzin = true) {  //İnputları getch fonksiyonu aracılığı ile string dizisi şeklinde alma fonksiyonu
    char a;
    string stringdizisi = "";
    bool noktaKullanildi = false;

    do {



        a = _getch(); // Karakteri oku ve ekrana yaz

        if (isdigit(a)) { // Eğer rakam ise ekrana yazdır
            stringdizisi += a;
            cout << a;
        }
        else if (a == '.' && ondalikIzin && !noktaKullanildi) { // Double değeri girilecekse ve henuz nokta konulmadıysa
            stringdizisi += a;
            cout << a;
            noktaKullanildi = true;
        }
        else if (a == '\b' && !stringdizisi.empty()) { // Backspace tuşuna basılmışsa ve string dizisi boş değilse 
            cout << "\b \b"; // Karakteri ekrandan sil
            if (stringdizisi.back() == '.') { // Eğer son karakter nokta ise
                noktaKullanildi = false; // Nokta kullanımını serbest bırak
            }
            stringdizisi.pop_back(); // Diziden son karakteri sil
        }
    } while (a != '\r'); // Enter tuşuna basılana kadar döngü devam eder

    return stringdizisi.empty() ? sayialma() : stod(stringdizisi);
}

void veriIsteme() {    //Başlangıç verilerini isteme fonksiyonu
    cout << "Baslangic Sermayesi: ";
    baslangicBakiye = sayialma(true);
    cout << endl << "Tavuk sayisi: ";

    do {
        tavukSayisi = sayialma(false);
        if (tavukSayisi > 500 || tavukSayisi <= 0) cout << endl << "\033[1;31m" << "Lutfen (1-500) arasinda bir deger girin: " << "\033[0m";
    } while (tavukSayisi > 500 || tavukSayisi <= 0);
    cout << endl << "Yem kilogram fiyati: ";
    yemFiyat = sayialma(true);
    cout << endl << "Yumurta fiyati: ";
    yumurtaFiyati = sayialma(true);
    cout << endl << "Tavuk alim fiyati: ";
    tavukAlimFiyati = sayialma(true);
    cout << endl << "Simulasyon gun sayisi: ";
    gunSayisi = sayialma(false);
}

class Ciftlik {   //Çiftlik sınıfı tanımlama
private:
    double baslangicBakiye;   //Private değişkenler
    double bakiye;
    double yemFiyati;
    double yumurtaFiyati;
    double tavukAlimFiyati;
    int tavukSayisi;
    int gunSayisi;
    double kalanYemMiktari;
    Tavuk tavuklar[MAX_TAVUK_SAYISI];

public:
    Ciftlik(double baslangicBakiye, int baslangicTavukSayisi, double yemFiyati, double yumurtaFiyati, double tavukFiyati) //Çiftlik kurucu fonksiyonu.
        : bakiye(baslangicBakiye), tavukSayisi(baslangicTavukSayisi), yemFiyati(yemFiyati), yumurtaFiyati(yumurtaFiyati), tavukAlimFiyati(tavukFiyati), kalanYemMiktari(700) {
    }

    void simulasyon(int simulasyongunsayisi) { //Similasyonu yapan fonksiyon
        srand(time(0));

        // Tablo yazdırma
        cout << endl << "\033[1;33;5m" << string(120, '~') << "\033[0m" << endl;
        cout << "\033[1;36m";
        cout << right << setw(5) << "Gun" << setw(19) << "Yem Tuketimi(kg)" << setw(10) << "Yumurta"
            << setw(17) << "Kesilen Tavuk" << setw(18) << "Gunluk Gelir" << setw(18) << "Gunluk Gider"
            << setw(16) << "Kalan Yem(kg)" << setw(15) << "    Bakiye (TL)" << endl;
        cout << "\033[1;33;5m" << string(120, '~') << "\033[0m" << endl;

        for (gunSayisi = 1; gunSayisi <= simulasyongunsayisi; gunSayisi++) {  //Kullanıcının girdiği gün kadar yap.
            double gunlukYemTuketimi = 0;
            int toplamYumurta = 0;
            int kesilenTavukSayisi = 0;
            double gunlukGelir = 0;
            double gunlukGider = 0;

            for (int tavukNumarasi = 0; tavukNumarasi < tavukSayisi; tavukNumarasi++) {   //Her tavuk için tavuk fonksiyonlarını yap.
                tavuklar[tavukNumarasi].yumurtlamafonksiyonu();
                toplamYumurta += tavuklar[tavukNumarasi].yumurtaSayisiAtama();

                double tavukGunlukTukettigiYem = (100 + rand() % 21) / 1000.0; // 100-120 gram yem
                gunlukYemTuketimi += tavukGunlukTukettigiYem;
            }

            kalanYemMiktari -= gunlukYemTuketimi;

            if (kalanYemMiktari < 70 || gunSayisi == 1) {  //Yem miktarı uygun değilse yem al, giderlere ekle, bakiyeden düş
                gunlukGider += 700 * yemFiyati;
                bakiye -= 700 * yemFiyati;
                gunlukGider += tavukAlimFiyati * tavukSayisi;
                bakiye -= tavukAlimFiyati * tavukSayisi;
                if (kalanYemMiktari < 70)
                    kalanYemMiktari += 700;
            }

            gunlukGelir += toplamYumurta * yumurtaFiyati;   //Gelire satılan yumurtaları ekle
            bakiye += toplamYumurta * yumurtaFiyati;

            for (int tavukNumarasi = 0; tavukNumarasi < tavukSayisi; tavukNumarasi++) {   //Her tavuğu kesim kontrolüne gönder.
                if (tavuklar[tavukNumarasi].kesimKontrol()) {
                    gunlukGelir += tavukAlimFiyati / 2; // Kesim geliri
                    gunlukGider += tavukAlimFiyati;     // Yeni tavuk alımı
                    bakiye += tavukAlimFiyati / 2;
                    bakiye -= tavukAlimFiyati;
                    tavuklar[tavukNumarasi].reset();
                    kesilenTavukSayisi++;
                }
            }

            // Tablo satırı yazdırma
            cout << fixed << setprecision(3) << right << setw(5) << gunSayisi << setw(16) << gunlukYemTuketimi;
            cout << setprecision(0) << setw(12) << toplamYumurta;
            cout << setprecision(2) << setw(17) << kesilenTavukSayisi << setw(18) << gunlukGelir << setw(18) << gunlukGider;
            cout << setprecision(3) << right << setw(15) << kalanYemMiktari;
            cout << setprecision(2) << setw(16) << bakiye << endl;

            if (bakiye < 0) {  //Her günün sonunda bakiyeye bak negatifse output ver.
                cout << "\033[1;31;5m" << "\nSimülasyon Sona Erdi, İflas ettiniz!\n" << "\033[0m" << endl;
                break;
            }
        }

        if (bakiye >= 0) {
            cout << "\033[1;32;5m" << "\nSimülasyon Başarıyla Tamamlandı!\n" << "\033[0m\n";
        }

    }
};

int main() {   //MAİN FONKSİYON

    setlocale(LC_ALL, "Turkish");
    veriIsteme();
    Ciftlik ciftlik(baslangicBakiye, tavukSayisi, yemFiyat, yumurtaFiyati, tavukAlimFiyati);
    ciftlik.simulasyon(gunSayisi); // Similasyonu yap.

    return 0;
}