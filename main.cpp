#include <iostream>
#include <vector> //Bu kütüphane, dinamik boyutlu bir vektör (dizi) veri yapýsý sunar. DBSCAN algoritmasýnda nokta kümesini ve yoðun bölgeleri temsil etmek için vektörler kullanýlýr.
#include <cmath> //Bu kütüphane, matematiksel iþlemler ve fonksiyonlar için gerekli olan matematik fonksiyonlarýný içerir. DBSCAN algoritmasýnda noktalar arasýndaki mesafeyi hesaplamak için kök alma (sqrt) iþlemi kullanýlýr.
using namespace std;

// Nokta yapýsý
struct Nokta {
    double x;
    double y;

    bool operator==(const Nokta& diger) const {
        return x == diger.x && y == diger.y;
    }
};
// Ýki nokta arasýndaki Euclidean mesafeyi hesaplar.
double mesafeHesapla(const Nokta& n1, const Nokta& n2) {
    double dx = n1.x - n2.x;
    double dy = n1.y - n2.y;
    return sqrt(dx * dx + dy * dy);
}
// Bir noktanýn epsilon mesafesi içindeki komþularýný döndürür.
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

// DBSCAN algoritmasý
vector<int> dbscan(const vector<Nokta>& noktalar, double epsilon, int minNoktalar) {
    vector<int> kumeEtiketleri(noktalar.size(), -1); // Nokta etiketlerini tutan vektör, baþlangýçta -1 ile baþlatýlýr
    int kumeId = 0; // Küme kimliði

    for (size_t i = 0; i < noktalar.size(); ++i) {
        if (kumeEtiketleri[i] != -1) {
            continue;  // Önceden ziyaret edilen noktalarý atla.
        }

        vector<Nokta> komsular = komsulariAl(noktalar, noktalar[i], epsilon); // Noktanýn epsilon mesafesindeki komþularýný alýr.
        if (komsular.size() < minNoktalar) {
            kumeEtiketleri[i] = 0; // Gürültü olarak etiketler (etiket = 0)
        } else {
            kumeId++;
            kumeEtiketleri[i] = kumeId; // Noktayý yeni bir kümeye atar ve kümeye kimlik (etiket) verir.

            // BFS (Breadth-First Search) mantýðýyla komþularý geniþletme iþlemi
            for (size_t j = 0; j < komsular.size(); ++j) {
                int komsuIdx = distance(noktalar.begin(), find(noktalar.begin(), noktalar.end(), komsular[j]));
                  // KomsuIdx, komsular[j] noktasýnýn dizideki konumunu bulmak için find fonksiyonunu kullanýr.

                if (kumeEtiketleri[komsuIdx] == -1) {
                    kumeEtiketleri[komsuIdx] = kumeId; // Komsu noktayý ayný kümeyle etiketler.

                    vector<Nokta> komsuKomsular = komsulariAl(noktalar, komsular[j], epsilon);
                    if (komsuKomsular.size() >= minNoktalar) {
                        komsular.insert(komsular.end(), komsuKomsular.begin(), komsuKomsular.end()); // Komsu noktanýn komþularýný da komsular vektörüne ekler.
                    }
                }
            }
        }
    }

    return kumeEtiketleri; // Küme etiketlerini döndürür.
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

    double epsilon = 1.5; // Epsilon deðeri: Bir noktanýn komþu olarak kabul edileceði maksimum mesafe
    int minNoktalar = 3;  // MinNoktalar deðeri: Bir noktanýn çevresindeki en az komþu sayýsý

    vector<int> kumeEtiketleri = dbscan(noktalar, epsilon, minNoktalar);

     // Noktalarýn küme bilgilerini ekrana yazdýrýr.
    for (size_t i = 0; i < noktalar.size(); ++i) {
        cout << "Nokta (" << noktalar[i].x << ", " << noktalar[i].y << ") - Küme: " << kumeEtiketleri[i] << endl;
    }

    return 0;
}
