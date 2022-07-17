#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

void meni() {
    std::cout << "\nRazveji in omeji – izbira:";
    std::cout << "\n1 Preberi podatke iz datoteke";
    std::cout << "\n2 Iskanje poti med vozliščema s in g";
    std::cout << "\n3 Konec";
    std::cout << "\n\nVaša izbira: ";
}

struct Pot {
    int dolzina;
    std::vector<int> vozlisca;
};

void RAZVEJI_IN_OMEJI(const int* C, int n, int s, int g, std::vector<Pot> &R, int &cena) {
    Pot pot, p, p1;
    int u;
    bool vPoti;
    pot.vozlisca.push_back(s);
    pot.dolzina = 0;
    R.clear();
    cena = 9999;
    std::vector<Pot> Q;
    Q.push_back(pot);
    while (!Q.empty()) {
        p = Q.front();
        Q.erase(Q.begin());
        u = p.vozlisca.back();
        for (int i = 0; i < n; ++i) {
            vPoti = false;
            if (C[(n*u)+i] != 9999) {
                // v = i
                for (int j = 0; j < p.vozlisca.size(); ++j) {
                    if (p.vozlisca[j] == i) {
                        vPoti = true;
                    }
                }
                if (!vPoti) {
                    p1.vozlisca = p.vozlisca;
                    p1.vozlisca.push_back(i);
                    p1.dolzina = p.dolzina + C[(n*u)+i];
                    if (i == g) {
                        if (p1.dolzina < cena) {
                            R.clear();
                            R.push_back(p1);
                            cena = p1.dolzina;
                        } else if (p1.dolzina == cena) {
                            R.push_back(p1);
                        }
                    } else if (p.dolzina < cena) {
                        Q.push_back(p1);
                    }
                }
            }
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

    int s, g;
    bool branjeSet = false;

    std::vector<Pot> R;
    int cenaResitve;

    do {
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
                        C[i] = 9999;
                    }
                    for (int i = 0; i < stevilo_povezav; ++i) {
                        f >> v1 >> v2 >> cena;
                        C[(stolpcev * (v1 - 1)) + (v2 - 1)] = cena;
                    }
                    branjeSet = true;
                    std::cout << "\nBranje iz datoteke koncano.\n";
                } else {
                    std::cout << "\nBranje iz datoteke ni bilo uspesno.\n";
                }
                break;
            case 2:
                if (branjeSet) {
                    std::cout << "\nVnesite zacetno vozlisce: ";
                    s = vnos(stevilo_vozlisc);
                    std::cout << "\nVnesite koncno vozlisce: ";
                    g = vnos(stevilo_vozlisc);
                    RAZVEJI_IN_OMEJI(C,stevilo_vozlisc,s,g,R,cenaResitve);
                    std::cout << "\nCena resitve: " << cenaResitve << std::endl;
                    std::cout << "Poti: \n";
                    for (int i = 0; i < R.size(); ++i) {
                        for (int j = 0; j < R[i].vozlisca.size(); ++j) {
                            std::cout << R[i].vozlisca[j];
                            if (j != R[i].vozlisca.size()-1) {
                                std::cout << " -> ";
                            }
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                } else {
                    std::cout << "\nPrvo preberite graf iz datoteke(opcija 1)!";
                }
                break;
            case 3:
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