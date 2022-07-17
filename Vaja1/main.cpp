#include <iostream>
#include <random>
#include <chrono>

void meni() {
    std::cout << "\nHitro uredi - izbira:\n";
    std::cout << "\n1 Generiraj naključno zaporedje";
    std::cout << "\n2 Generiraj urejeno naraščajoče zaporedje";
    std::cout << "\n3 Generiraj urejeno padajoče zaporedje";
    std::cout << "\n4 Izpis zaporedja";
    std::cout << "\n5 Uredi";
    std::cout << "\n6 Konec";
    std::cout << "\n\nVaša izbira: ";
}

void izpis(int *polje, int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << polje[i] << " ";
    }
}

int deli(int *polje, int dno, int vrh) {
    int l=dno, d=vrh, pe, m;
    m = dno+vrh;
    m /= 2;
    std::swap(polje[dno],polje[m]);
    pe=polje[dno];
    while (l<d) {
        while (polje[l]<=pe && l<vrh) {
            l++;
        }

        while (polje[d]>=pe && d>dno) {
            d--;
        }

        if (l<d) {
            std::swap(polje[l],polje[d]);
        }

    }
    std::swap(polje[dno],polje[d]);
    return d;
}

void hitroUredi(int *polje, int dno, int vrh) {
    if (dno < vrh) {
        int j = deli(polje, dno, vrh);
        hitroUredi(polje, dno, j-1);
        hitroUredi(polje, j+1, vrh);
    }
}

void generirajNakljucno(int *polje, int n) {
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(-500000, 500000);
    for (int i = 0; i < n; ++i) {
        polje[i] = uniform_dist(e1);
    }
}

void generirajNarascajoce(int *polje, int n) {
    for (int i = 0; i < n; ++i) {
        polje[i] = i+1;
    }
}

void generirajPadajoce(int *polje, int n) {
    for (int i = 0; i < n; ++i) {
        polje[i] = n-i;
    }
}

void preveri(int *polje, int dno, int vrh) {
    for (int i = dno; i < vrh-1; ++i) {
        if (polje[i+1]<polje[i]) {
            std::cout << "Zaporedje je napačno urejeno!" << i << "\n";
            return;
        }
    }
    std::cout << "Zaporedje je pravilno urejeno!\n";
}

int vnos(){
    int n = 0;
    std::cout << "Število elementov: ";
    do {
        if (n<0||n>1000000) {
            std::cout << "\nVneseno število mora biti večje ali enako 0 ter manjše od 1.000.000\nZnova vnesite število elementov: ";
        }
        std::cin >> n;
    } while (n<0||n>1000000);
    return n;
}

int main() {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    bool deluje = true, uredi = false;
    int opcija;
    int *polje = nullptr;
    int n;

    do {
        if (uredi) {
            std::chrono::duration<double> elapsed_seconds = end - start;
            std::cout << "Čas urejanja: " << elapsed_seconds.count() << "s\n";
            uredi = false;
            preveri(polje,0,n);
        }
        meni();
        std::cin >> opcija;
        switch (opcija) {
            case 1:
                n = vnos();
                if (polje!=NULL) {
                    delete[] polje;
                }
                polje = new int[n];
                generirajNakljucno(polje,n);
                break;
            case 2:
                n = vnos();
                if (polje!=NULL) {
                    delete[] polje;
                }
                polje = new int[n];
                generirajNarascajoce(polje,n);
                break;
            case 3:
                n = vnos();
                if (polje!=NULL) {
                    delete[] polje;
                }
                polje = new int[n];
                generirajPadajoce(polje,n);
                break;
            case 4:
                if (!n) break;
                std::cout << std::endl;
                izpis(polje,n);
                std::cout << std::endl;
                break;
            case 5:
                if (!n) break;
                start = std::chrono::system_clock::now();
                hitroUredi(polje,0,n-1);
                end = std::chrono::system_clock::now();
                uredi = true;
                break;
            case 6:
                deluje = false;
                break;
            default:
                std::cout << "Neveljavna izbira!" << std::endl;
                break;
        }
        std::cout << std::endl;
    } while (deluje);

    if (polje!=NULL) {
        delete[] polje;
    }

    return 0;
}
