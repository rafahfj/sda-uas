#ifndef MODELS_H
#define MODELS_H

#include <string>
#include "constants.h"

using namespace std;

// ======================== STRUKTUR DATA WARGA ========================
struct Person {
    int id;
    string name;
    int startTick;
};

// ======================== STRUKTUR DATA QUEUE ========================
struct AntrianWarga {
    int top;
    Person isi[MAX_ANTREAN + 1];
};

// ======================== STRUKTUR DATA NODE/GRAPH LOKASI ========================
struct Node {
    int id;
    string name;
    string tipe; //ini buat taruh tipenya kab-kota/kecamatan/desa
    int parentID; //buat naruh id parent, kalo misal user ngisi tipe nya "kec/desa". kalo misal user ga ngisi desa/kec, ini valuenya 0
    bool isSafe;
    bool isActive; // Flag untuk melacak data yang aktif (pengganti vector::erase)
};

#endif
