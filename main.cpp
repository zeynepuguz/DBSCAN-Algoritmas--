#include <iostream>
#include <vector> //Bu k�t�phane, dinamik boyutlu bir vekt�r (dizi) veri yap�s� sunar. DBSCAN algoritmas�nda nokta k�mesini ve yo�un b�lgeleri temsil etmek i�in vekt�rler kullan�l�r.
#include <cmath> //Bu k�t�phane, matematiksel i�lemler ve fonksiyonlar i�in gerekli olan matematik fonksiyonlar�n� i�erir. DBSCAN algoritmas�nda noktalar aras�ndaki mesafeyi hesaplamak i�in k�k alma (sqrt) i�lemi kullan�l�r.
using namespace std;

// Nokta yap�s�
struct Nokta {
    double x;
    double y;

    bool operator==(const Nokta& diger) const {
        return x == diger.x && y == diger.y;
    }
};
// �ki nokta aras�ndaki Euclidean mesafeyi hesaplar.
double mesafeHesapla(const Nokta& n1, const Nokta& n2) {
    double dx = n1.x - n2.x;
    double dy = n1.y - n2.y;
    return sqrt(dx * dx + dy * dy);
}
// Bir noktan�n epsilon mesafesi i�indeki kom�ular�n� d�nd�r�r.
vector<Nokta> komsulariAl(const vector<Nokta>& noktalar, const Nokta& merkez, double epsilon) {
    vector<Nokta> komsular;
    for (const int auto& nokta : noktalar) {
        double mesafe = mesafeHesapla(nokta, merkez);
        if (mesafe <= epsilon) {
            komsular.push_back(nokta);
        }
    }
    return komsular;
}

// DBSCAN algoritmas�
vector<int> dbscan(const vector<Nokta>& noktalar, double epsilon, int minNoktalar) {
    vector<int> kumeEtiketleri(noktalar.size(), -1); // Nokta etiketlerini tutan vekt�r, ba�lang��ta -1 ile ba�lat�l�r
    int kumeId = 0; // K�me kimli�i

    for (size_t i = 0; i < noktalar.size(); ++i) {
        if (kumeEtiketleri[i] != -1) {
            continue;  // �nceden ziyaret edilen noktalar� atla.
        }

        vector<Nokta> komsular = komsulariAl(noktalar, noktalar[i], epsilon); // Noktan�n epsilon mesafesindeki kom�ular�n� al�r.
        if (komsular.size() < minNoktalar) {
            kumeEtiketleri[i] = 0; // G�r�lt� olarak etiketler (etiket = 0)
        } else {
            kumeId++;
            kumeEtiketleri[i] = kumeId; // Noktay� yeni bir k�meye atar ve k�meye kimlik (etiket) verir.

            // BFS (Breadth-First Search) mant���yla kom�ular� geni�letme i�lemi
            for (size_t j = 0; j < komsular.size(); ++j) {
                int komsuIdx = distance(noktalar.begin(), find(noktalar.begin(), noktalar.end(), komsular[j]));
                  // KomsuIdx, komsular[j] noktas�n�n dizideki konumunu bulmak i�in find fonksiyonunu kullan�r.

                if (kumeEtiketleri[komsuIdx] == -1) {
                    kumeEtiketleri[komsuIdx] = kumeId; // Komsu noktay� ayn� k�meyle etiketler.

                    vector<Nokta> komsuKomsular = komsulariAl(noktalar, komsular[j], epsilon);
                    if (komsuKomsular.size() >= minNoktalar) {
                        komsular.insert(komsular.end(), komsuKomsular.begin(), komsuKomsular.end()); // Komsu noktan�n kom�ular�n� da komsular vekt�r�ne ekler.
                    }
                }
            }
        }
    }

    return kumeEtiketleri; // K�me etiketlerini d�nd�r�r.
}

int main() {
    vector<Nokta> noktalar = {
        Nokta{1.0, 1.0},
        Nokta{1.5, 2.0},
        Nokta{2.0, 1.5},
        Nokta{3.0, 4.0},
        Nokta{3.5, 3.5},
        Nokta{4.0, 4.0},
        Nokta{5.0, 5.0},
        Nokta{5.5, 5.5},
        Nokta{6.0, 5.0},
     };

    double epsilon = 1.5; // Epsilon de�eri: Bir noktan�n kom�u olarak kabul edilece�i maksimum mesafe
    int minNoktalar = 3;  // MinNoktalar de�eri: Bir noktan�n �evresindeki en az kom�u say�s�

    vector<int> kumeEtiketleri = dbscan(noktalar, epsilon, minNoktalar);

     // Noktalar�n k�me bilgilerini ekrana yazd�r�r.
    for (size_t i = 0; i < noktalar.size(); ++i) {
        cout << "Nokta (" << noktalar[i].x << ", " << noktalar[i].y << ") - K�me: " << kumeEtiketleri[i] << endl;
    }

    return 0;
}
