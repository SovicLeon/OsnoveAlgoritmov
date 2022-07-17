#include <iostream>
#include <fstream>
#include <stack>

struct Vozlisce {
    int predhodnik;
    int dolzina;
    int status;
    int indeks;
    std::string ime;
};

void meni() {
    std::cout << "\nIskanje v globino – izbira:";
    std::cout << "\n1) Preberi graf iz datoteke";
    std::cout << "\n2) Poženi iskanje iz vozlišča s";
    std::cout << "\n3) Izpis seznama vozlišč in njihovih podatkov";
    std::cout << "\n4) Izpis poti med vozliščema s in d";
    std::cout << "\n5) Konec";
    std::cout << "\nVaša izbira: ";
}

void ISKANJE_V_GLOBINO(Vozlisce* V, int n, const int* C, int s) {
    for (int i = 0; i < n; ++i) {
        V[i].status = 0; // NEPREGLEDANO
        V[i].dolzina = -1; // neskončno
        V[i].predhodnik = -1;
    }
    V[s].status = 1; // V_OBDELAVI
    V[s].dolzina = 0;
    V[s].predhodnik = -1;
    std::stack<Vozlisce> sklad;
    sklad.push(V[s]);
    Vozlisce v;
    int vInd;
    while (!sklad.empty()) {
        v = sklad.top();
        vInd = v.indeks;
        sklad.pop();
        for (int i = 0; i < n; ++i) {
            if (C[(n*v.indeks)+i]==1) {
                if (V[i].status==0) {
                    V[i].status = 1;
                    V[i].dolzina = v.dolzina+1;
                    V[i].predhodnik = v.indeks;
                    sklad.push(V[i]);
                }
            }
            V[vInd].status = 2; // RAZVITO
        }
    }
}

bool IZPIS_POTI(Vozlisce* V,int s, int v, int vO) {
    if (V[v].indeks==V[s].indeks) {
        std::cout << "\nPot je: " << V[v].ime << " ";
        return true;
    } else {
        if (V[v].predhodnik==-1) {
            std::cout << "\nMed " << V[s].ime << " in " << V[vO].ime << " ni poti!";
            return false;
        } else {
            if (IZPIS_POTI(V,s,V[v].predhodnik,vO)) {
                std::cout << V[v].ime << " ";
                return true;
            }
        }
    }
    return false;
}

void IZPIS_SEZNAMA(Vozlisce* V, int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << "\n\nIme: " << V[i].ime;
        std::cout << "\nIndeks: " << V[i].indeks;
        std::cout << "\nPredhodnik: " << V[i].predhodnik;
        std::cout << "\nStatus: " << V[i].status;
        std::cout << "\nDolzina: " << V[i].dolzina;
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


    //a[stolpcev*4+5]=55;
    //4-vrstica
    //5-stolpec

    int s, d, sNov;
    bool sSet = false, branjeSet = false;

    do {
        meni();
        std::cin >> opcija;
        switch (opcija) {
            case 1:
                f.clear();
                f.close();
                f.open("../primeri_grafov/graf/graf.txt");
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
                        f >> v1 >> v2 >> cena;  // v vsaki vrstici beremo 3 podatke (stevilke)
                        C[(stolpcev * (v1 - 1)) + (v2 - 1)] = 1;
                        C[(stolpcev * (v2 - 1)) + (v1 - 1)] = 1;
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
                    ISKANJE_V_GLOBINO(V,stevilo_vozlisc,C,s-1);
                } else {
                    std::cout << "\nPrvo preberite graf iz datoteke(opcija 1)!";
                }
                break;
            case 3:
                if (branjeSet) {
                    IZPIS_SEZNAMA(V,stevilo_vozlisc);
                } else {
                    std::cout << "\nPrvo preberite graf iz datoteke(opcija 1)!";
                }
                break;
            case 4:
                if (branjeSet) {
                    if (sSet) {
                        std::cout << "\nVnesite izhodiščno vozlišče(ime): ";
                        sNov = vnos(stevilo_vozlisc);
                        if (sNov!=s) {
                            std::cout << "\nIzhodiščno vozlišče v iskani poti(sedaj vneseno) se razlikuje od tistega,\nuporabljenega pri generiranju iskalnega drevesa.\n";
                        }
                        std::cout << "\nVnesite ciljno vozlišče(ime): ";
                        d = vnos(stevilo_vozlisc);
                        IZPIS_POTI(V,sNov-1,d-1,d-1);
                    } else {
                        std::cout << "\nPrvo poženite iskanje iz vozlišča s(opcija 2)!";
                    }
                } else {
                    std::cout << "\nPrvo preberite graf iz datoteke(opcija 1)!";
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
    if (V != NULL) {
        delete[] V;
    }

    return 0;
}