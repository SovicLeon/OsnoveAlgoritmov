#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

struct Povezava {
    Povezava(int p, int q, int cena) : p(p), q(q), cena(cena){
    }
    int p;
    int q;
    int cena;
};

void meni() {
    std::cout << "\nKruskalov algoritem – izbira:\n";
    std::cout << "\n1 Preberi graf iz datoteke";
    std::cout << "\n2 Generiraj naključni graf";
    std::cout << "\n3 Poženi";
    std::cout << "\n4 Izpis sprejetih povezav";
    std::cout << "\n5 Konec";
    std::cout << "\n\nVaša izbira: ";
}

bool myCmp(const Povezava& x, const Povezava& y) {
    return (x.cena < y.cena);
}

int deli(std::vector<Povezava>& polje, int dno, int vrh) {
    int l=dno, d=vrh, pe, m;
    m = dno+vrh;
    m /= 2;
    std::swap(polje[dno],polje[m]);
    pe=polje[dno].cena;
    while (l<d) {
        while (polje[l].cena<=pe&&l<vrh) {
            l++;
        }

        while (polje[d].cena>=pe&&d>dno) {
            d--;
        }

        if (l<d) {
            std::swap(polje[l],polje[d]);
        }
    }
    std::swap(polje[dno],polje[d]);
    return d;
}

void hitroUredi(std::vector<Povezava>& polje, int dno, int vrh) {
    if (dno < vrh) {
        int j = deli(polje, dno, vrh);
        hitroUredi(polje, dno, j-1);
        hitroUredi(polje, j+1, vrh);
    }
}

void KRUSKAL(std::vector<Povezava> P, std::vector<Povezava>& R, int n) {
    std::sort(P.begin(),P.end(), myCmp);
    //hitroUredi(P,0,P.size());
    int *S = new int[n];
    for (int i = 0; i < n; ++i) {
        S[i] = 0;
    }
    int sCount = 1;
    int sSet;
    bool koncano = false;
    int i = 0;
    int stSprejetihPov = 0;
    while (!koncano) {
        if (!((S[P[i].p-1] != 0) && (S[P[i].q-1] == S[P[i].p-1]))) {
            if ((S[P[i].p-1] != 0) && (S[P[i].q-1] != 0) && (S[P[i].q-1] != S[P[i].p-1])) {
                sSet = S[P[i].q-1];
                for (int j = 0; j < n; ++j) {
                    if (S[j] == sSet) {
                        S[j] = S[P[i].p-1];
                    }
                }
                stSprejetihPov++;
                R.push_back(P[i]);
            } else if ((S[P[i].p-1] != 0 && S[P[i].q-1] == 0) || (S[P[i].p-1] == 0 && S[P[i].q-1] != 0)) {
                if (S[P[i].q-1] == 0) {
                    S[P[i].q-1] = S[P[i].p-1];
                } else {
                    S[P[i].p-1] = S[P[i].q-1];
                }
                stSprejetihPov++;
                R.push_back(P[i]);
            } else if (S[P[i].p-1] == 0 && S[P[i].q-1] == 0) {
                S[P[i].p-1] = sCount;
                S[P[i].q-1] = sCount;
                sCount++;
                stSprejetihPov++;
                R.push_back(P[i]);
            }
        }
        if (stSprejetihPov == n-1) {
            koncano = true;
        } else {
            i++;
        }
    }
    delete[] S;
}

void IZPIS_SPREJETIH_POVEZAV(std::vector<Povezava> izpis) {
    for (int i = 0; i < izpis.size(); ++i) {
        std::cout << izpis[i].p << " " << izpis[i].q << " " << izpis[i].cena << std::endl;
    }
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
    std::chrono::time_point<std::chrono::system_clock> start, end;
    srand(time(NULL));
    bool deluje = true, uredi = false;
    int opcija;

    // branje grafa
    int stevilo_vozlisc, stevilo_povezav, v1, v2, cena;
    std::ifstream f("graf.txt");
    int vrstic, stolpcev;
    //int *C = new int[vrstic*stolpcev];
    int *C = nullptr;
    std::vector<Povezava> P;
    std::vector<Povezava> R;

    bool pozeniSet = false, branjeSet = false;
    do {
        if (uredi) {
            std::chrono::duration<double> elapsed_seconds = end - start;
            std::cout << "Čas delovanja: " << elapsed_seconds.count() << "s\n";
            uredi = false;
        }
        meni();
        std::cin >> opcija;
        switch (opcija) {
            case 1:
                f.clear();
                f.close();
                f.open("graf.txt");
                if (f.good()) {
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
                    P.clear();
                    for (int i = 0; i < stevilo_povezav; ++i) {
                        f >> v1 >> v2 >> cena;
                        if (v1<v2) {
                            C[(stolpcev * (v1 - 1)) + (v2 - 1)] = cena;
                        } else {
                            C[(stolpcev * (v2 - 1)) + (v1 - 1)] = cena;
                        }
                    }
                    for (int p = 0; p < stevilo_vozlisc; ++p) {
                        for (int q=p+1; q < stevilo_vozlisc; ++q) {
                            if (C[(stolpcev*p)+q] != 0) {
                                cena = C[(stolpcev*p)+q];
                                P.emplace_back(p+1,q+1,C[(stolpcev*p)+q]);
                            }
                        }
                    }
                    branjeSet = true;
                    std::cout << "\nBranje iz datoteke končano.\n";
                } else {
                    std::cout << "\nBranje iz datoteke ni bilo uspešno.\n";
                }
                break;
            case 2:
                stevilo_povezav = 0;
                std::cout << "Vnesite število vozlišč(največ 1500): ";
                stevilo_vozlisc = vnos(1500);
                if (!P.empty()) {
                    P.clear();
                }
                for (int i = 0; i < stevilo_vozlisc; ++i) {
                    for (int j = i+1; j < stevilo_vozlisc; ++j) {
                        cena = rand() % 250 + 1;
                        P.emplace_back(i+1,j+1,cena);
                        stevilo_povezav++;
                    }
                }
                branjeSet = true;
                std::cout << "Povezave polnega grafa generirane!\n";
                break;
            case 3:
                if (branjeSet) {
                    if (!R.empty()) {
                        R.clear();
                    }
                    start = std::chrono::system_clock::now();
                    KRUSKAL(P,R,stevilo_vozlisc);
                    end = std::chrono::system_clock::now();
                    uredi = true;
                    std::cout << "Povezav v izhodiščnem grafu " << stevilo_povezav << std::endl;
                    std::cout << "Vozlišč v izhodiščnem grafu " << stevilo_vozlisc << std::endl;
                    std::cout << "Povezav v sprejetih s strani algoritma: " << R.size();
                    pozeniSet = true;
                } else {
                    std::cout << "Prvo izvedite branje grafa(opcija 1)!\n";
                }
                break;
            case 4:
                if (branjeSet) {
                    if (pozeniSet) {
                        IZPIS_SPREJETIH_POVEZAV(R);
                    } else {
                        std::cout << "Prvo poženite kruskalov algoritem(opcija 3)!\n";
                    }
                } else {
                    std::cout << "Prvo izvedite branje grafa(opcija 1)!\n";
                }
                break;
            case 5:
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

    return 0;
}