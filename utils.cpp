#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "utils.h"

using namespace std;

// ==== FUNGSI BANTU INPUT AMAN ====
bool bacaInt(int &nilai) {
    cin >> nilai;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "  [!] Input tidak valid. Harap masukkan angka.\n";
        return false;
    }

    while (cin.peek() == ' ' || cin.peek() == '\t') {
        cin.get();
    }

    int karakterBerikutnya = cin.peek();
    if (karakterBerikutnya != '\n' && karakterBerikutnya != '\r' && karakterBerikutnya != EOF) {
        // cin.ignore(10000, '\n');
        cout << "  [!] Input tidak valid. Harap masukkan angka tanpa karakter tambahan.\n";
        return false;
    }

    return true;
}

void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void backToMenu() {
    cout << "\nTekan enter untuk kembali ke menu utama..";
    cin.ignore(10000, '\n');
    cin.get();
    clearScreen();
}
