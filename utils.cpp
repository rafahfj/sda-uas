#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <cctype>
#include "utils.h"

using namespace std;

// Pembagian kerja: Anggota 2
// Fokus: utilitas input aman, clear screen, dan pause menu.
// ==== FUNGSI BANTU INPUT AMAN ====
bool bacaInt(int &nilai) {
    string input;
    cin >> input;

    bool valid = true;
    for (size_t i = 0; i < input.length(); i++) {
        if (i == 0 && input[i] == '-') continue;
        if (!isdigit(input[i])) {
            valid = false;
            break;
        }
    }

    if (!valid) {
        cout << "  [!] Input tidak valid. Harap masukkan angka.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return false;
    }

    nilai = stoi(input); //utk konversi 
    
    cin.ignore(10000, '\n');
    
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
    // cin.ignore(10000, '\n');
    cin.get();
    clearScreen();
}

bool simbol(const string& str) {
    for (char c : str) {
        if (!isalnum(c) && c != ' ') {
            return true;
        }
    }
    return false;
}

bool isValid(const string& str) {
    if (str.empty() || str.find_first_not_of(' ') == string::npos) {
        cout << " [!] Nama tidak boleh kosong!\n";
        return false;
    }

    if (simbol(str)) {
        cout << " [!] Nama tidak boleh mengandung simbol (!@#$%^&* dll.)\n";
        return false;
    }

    return true;
}

bool hurufSpasi(const string& str) {
    for (char c : str) {
        if (!isalpha(c) && c != ' ') {
            cout << "  [!] Nama hanya boleh mengandung huruf dan spasi!\n";
            return false;
        }
    }
    return true;
}
