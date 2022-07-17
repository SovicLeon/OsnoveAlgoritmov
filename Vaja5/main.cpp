#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

int imax = std::numeric_limits<int>::max()/2;

void meni() {
    std::cout << "\nFloyd-Warshallov algoritem – izbira:";
    std::cout << "\n1 Preberi graf iz datoteke";
    std::cout << "\n2 Zagon algoritma";
    std::cout << "\n3 Izpis najkrajše poti med vozliščema s in g";
    std::cout << "\n4 Konec";
    std::cout << "\n\nVaša izbira: ";
}


void FLOYD_WARSHALL(const int* C, int n, int* &D, int* &Pi) {
    if (D != NULL) {
        delete[] D;
    }
    if (Pi != NULL) {
        delete[] Pi;
    }
    D = new int[n * n];
    Pi = new int[n * n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            D[n*i+j] = C[n*i+j];
            if (i != j && C[n*i+j] != imax) {
                Pi[n*i+j] = i;
            } else {
                Pi[n*i+j] = -1;
            }
        }
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    if (D[n*i+j] > D[n*i+k] + D[n*k+j]) {
                        D[n*i+j] = D[n*i+k] + D[n*k+j];
                        Pi[n*i+j] = Pi[n*k+j];
                    }
                }
            }
        }
    }
}

bool IZPIS_POTI(const int* Pi, int n, int s, int g) {
    if (s == g) {
        std::cout << s << " ";
        return true;
    } else {
        if (Pi[n*s+g]==-1) {
            std::cout << "\nMed " << s << " in " << g << " ni poti!";
            return false;
        } else {
            if (IZPIS_POTI(Pi,n,s,Pi[n*s+g])) {
                std::cout << g << " ";
            }
            return true;
        }
    }
}

int vnos(int stV){
    int n = 0;
    do {
        if (n<0 || n>stV-1) {
            std::cout << "\nVneseno število mora biti večje ali enako 0 ter manjše ali enako od " << stV-1 << "\nZnova vnesite število: ";
        }
        std::cin >> n;
    } while (n<0 || n>stV-1);
    return n;
}

int main() {
    bool deluje = true;
    int opcija;

    // branje grafa
    int stevilo_vozlisc, stevilo_povezav, v1, v2, cena;
    std::ifstream f("graf.txt");
    int vrstic, stolpcev;
    //int *C = new int[vrstic*stolpcev];
    int *C = nullptr;
    int *D = nullptr;
    int *Pi = nullptr;

    int s, g;
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
                        C[i] = imax;
                    }
                    for (int i = 0; i < vrstic * stolpcev; ++i) {
                        C[stevilo_vozlisc*i+i] = 0;
                    }
                    for (int i = 0; i < stevilo_povezav; ++i) {
                        f >> v1 >> v2 >> cena;
                        C[(stolpcev * (v1 - 1)) + (v2 - 1)] = cena;
                    }
                    branjeSet = true;
                    std::cout << "\nBranje iz datoteke končano.\n";
                } else {
                    std::cout << "\nBranje iz datoteke ni bilo uspešno.\n";
                }
                break;
            case 2:
                if (branjeSet) {
                    FLOYD_WARSHALL(C,stevilo_vozlisc,D,Pi);
                    sSet = true;
                } else {
                    std::cout << "\nPrvo preberite graf iz datoteke(opcija 1)!";
                }
                break;
            case 3:
                if (branjeSet) {
                    if (sSet) {
                        std::cout << "\nVnesite zacetno vozlišče: ";
                        s = vnos(stevilo_vozlisc);
                        std::cout << "\nVnesite koncno vozlišče: ";
                        g = vnos(stevilo_vozlisc);
                        std::cout << "\nCena poti: " << D[stevilo_vozlisc*s+g] << std::endl;
                        IZPIS_POTI(Pi,stevilo_vozlisc,s,g);
                    } else {
                        std::cout << "\nPrvo poženite algoritem(opcija 2)!";
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

    if (D != NULL) {
        delete[] D;
    }

    if (Pi != NULL) {
        delete[] Pi;
    }

    return 0;
}