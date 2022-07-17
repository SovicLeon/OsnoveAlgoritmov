#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>

int imax = std::numeric_limits<int>::max();

struct Vozlisce {
    int predhodnik;
    int dolzina;
    int indeks;
    std::string ime;
};

void meni() {
    std::cout << "\nDijkstrov algoritem – izbira:";
    std::cout << "\n1 Naloži graf";
    std::cout << "\n2 Zagon algoritma";
    std::cout << "\n3 Izpis najkrajše poti";
    std::cout << "\n4 Konec";
    std::cout << "\n\nVaša izbira: ";
}

bool myCmp(const Vozlisce& x, const Vozlisce& y) {
    return (x.dolzina < y.dolzina);
}

void DIJKSTROV_ALGORITEM(Vozlisce* V, int n, const int* C, int s) {
    for (int i = 0; i < n; ++i) {
        V[i].dolzina = imax; // neskončno
        V[i].predhodnik = -1;
    }
    V[s].dolzina = 0;
    std::vector<Vozlisce> Q;
    for (int i = 0; i < n; ++i) {
        Q.push_back(V[i]);
    }
    Vozlisce u;
    int uInd;
    while (!Q.empty()) {
        std::sort(Q.begin(),Q.end(), myCmp);
        u = Q.front();
        uInd = u.indeks;
        Q.erase(Q.begin());
        for (int i = 0; i < n; ++i) {
            if (C[(n*uInd)+i]!=0) {
                if (V[i].dolzina > V[uInd].dolzina + C[(n*uInd)+i]) {
                    for (int j = 0; j < Q.size(); ++j) {
                        if (Q[j].indeks==V[i].indeks) {
                            Q[j].dolzina = V[uInd].dolzina + C[(n*uInd)+i];
                        }
                    }
                    V[i].dolzina = V[uInd].dolzina + C[(n*uInd)+i];
                    V[i].predhodnik = uInd;
                }
            }
        }
    }
}

bool IZPIS_POTI(Vozlisce* V,int s, int v, int c) {
    if (V[v].indeks==V[s].indeks) {
        std::cout << "\n" << V[s].ime << ": dolzina: " << V[s].dolzina << ", ";
        return true;
    } else {
        if (V[v].predhodnik==-1) {
            std::cout << "\nMed " << V[s].ime << " in " << V[v].ime << " ni poti!";
            return false;
        } else {
            if (IZPIS_POTI(V,s,V[v].predhodnik,c)) {
                if (v == c) {
                    std::cout << V[v].ime << ": dolzina: " << V[v].dolzina << ".";
                } else {
                    std::cout << V[v].ime << ": dolzina: " << V[v].dolzina << ", ";
                }
                return true;
            }
        }
    }
    return false;
}

int vnos(int stV){
    int n = 1;
    do {
        if (n<1 || n>stV) {
            std::cout << "\nVneseno število mora biti večje ali enako 1 ter manjše ali enako od " << stV << "\nZnova vnesite število: ";
        }
        std::cin >> n;
    } while (n<1 || n>stV);
    return n;
}

int main() {
    bool deluje = true;
    int opcija;

    // branje grafa
    int stevilo_vozlisc, stevilo_povezav, v1, v2, cena;
    std::ifstream f("../primeri_grafov/graf/graf.txt");
    int vrstic, stolpcev;
    //int *C = new int[vrstic*stolpcev];
    int *C = nullptr;
    //Vozlisce *V = new Vozlisce[stevilo_vozlisc];
    Vozlisce *V = nullptr;

    int s, d, sNov;
    bool sSet = false, branjeSet = false;

    do {
        meni();
        std::cin >> opcija;
        switch (opcija) {
            case 1:
                f.clear();
                f.close();
                f.open("graf.txt");
                if (f.good()) {
                    sSet = false;
                    f >> stevilo_vozlisc;
                    f >> stevilo_povezav;
                    vrstic = stolpcev = stevilo_vozlisc;
                    if (C != NULL) {
                        delete[] C;
                    }
                    C = new int[vrstic * stolpcev];
                    for (int i = 0; i < vrstic * stolpcev; ++i) {
                        C[i] = 0;
                    }
                    if (V != NULL) {
                        delete[] V;
                    }
                    V = new Vozlisce[stevilo_vozlisc];
                    for (int i = 0; i < stevilo_povezav; ++i) {
                        f >> v1 >> v2 >> cena;
                        C[(stolpcev * (v1 - 1)) + (v2 - 1)] = cena;
                        C[(stolpcev * (v2 - 1)) + (v1 - 1)] = cena;
                    }

                    for (int i = 0; i < stevilo_vozlisc; ++i) {
                        V[i].indeks = i;
                        V[i].ime = std::to_string(i + 1);
                    }
                    branjeSet = true;
                    std::cout << "\nBranje iz datoteke končano.\n";
                } else {
                    std::cout << "\nBranje iz datoteke ni bilo uspešno.\n";
                }
                break;
            case 2:
                if (branjeSet) {
                    std::cout << "\nVnesite izhodiščno vozlišče(ime): ";
                    s = vnos(stevilo_vozlisc);
                    sSet = true;
                    DIJKSTROV_ALGORITEM(V,stevilo_vozlisc,C,s-1);
                } else {
                    std::cout << "\nPrvo preberite graf iz datoteke(opcija 1)!";
                }
                break;
            case 3:
                if (branjeSet) {
                    if (sSet) {
                        std::cout << "\nVnesite ciljno vozlišče(ime): ";
                        d = vnos(stevilo_vozlisc);
                        IZPIS_POTI(V,s-1,d-1,d-1);
                    } else {
                        std::cout << "\nPrvo poženite iskanje iz izhodiščnega vozlišča(opcija 2)!";
                    }
                } else {
                    std::cout << "\nPrvo preberite graf iz datoteke(opcija 1)!";
                }
                break;
            case 4:
                deluje = false;
                break;
            default:
                std::cout << "Neveljavna izbira!" << std::endl;
                break;
        }
        std::cout << std::endl;
    } while (deluje);

    if (C != NULL) {
        delete[] C;
    }
    if (V != NULL) {
        delete[] V;
    }

    return 0;
}