#ifndef UTILS_H
#define UTILS_H

#include <string>

// Pembagian kerja: Anggota 2
// Fokus: deklarasi utilitas input dan layar.
bool bacaInt(int &nilai);
void clearScreen();
void backToMenu();
bool simbol(const std::string& str);
bool isValid(const std::string& str);

#endif
