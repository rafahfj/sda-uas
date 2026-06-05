#ifndef MODELS_H
#define MODELS_H

#include <string>
#include "constants.h"

using namespace std;

// Pembagian kerja: Anggota 1
// Fokus: struktur data dasar, terutama Person dan AntrianWarga untuk queue.
struct Person {
    int id;
    string name;
    int startTick;
};

struct AntrianWarga {
    int top;
    Person isi[MAX_ANTREAN + 1];
};

struct Node {
    int id;
    string name;
    string tipe; 
    // int parentID;
    bool isSafe;
    bool isActive; 
};

#endif
