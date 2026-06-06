#include <iostream>
#include "display.h"
#include "globals.h"
#include "graph.h"
#include "utils.h"

using namespace std;

void displayPosko() {
    cout << "\n--- DAFTAR LOKASI POSKO ---\n";
    bool posko = false;
    for (int i = 1; i <= MAX_LOKASI; i++) {
        if (nodes[i].isActive && nodes[i].tipe == "Posko") {
            cout << "ID: " << nodes[i].id << " | Nama: " << nodes[i].name
                 << " | Status: " << (nodes[i].isSafe ? "POSKO" : "BAHAYA")
                 << " | Antrean: " << antrian[i].top << " orang\n";
            posko = true;
        }
    }

    if (!posko){
        cout << "Belum ada lokasi yang terdaftar.\n";
        return;
    }
}

void displayNodes() {
    cout << "\n===========================================================\n";
    cout << "        DAFTAR LOKASI EVAKUASI KECAMATAN LEMBANG          \n";
    cout << "===========================================================\n";
    cout << " ID  | Nama Lokasi                     | Status    | Antrean\n";
    cout << "-----------------------------------------------------------\n";

    bool ada = false;
    for (int i = 1; i <= nnode; i++) {
        if (nodes[i].isActive) {
            printf(" %-3d | %-31s | %-9s | %d orang\n", 
                   nodes[i].id, 
                   nodes[i].name.c_str(), 
                   (nodes[i].isSafe ? "POSKO" : "BAHAYA"), 
                   antrian[i].top);
            ada = true;
        }
    }

    if (!ada) {
        cout << " [!] Belum ada data lokasi yang terdaftar.\n";
    }
    cout << "===========================================================\n";
}

void displayEdges() {
    cout << "\n--- DAFTAR JALUR EVAKUASI ANTRAR LOKASI ---\n";
    bool ada = false;
    for (int i = 1; i <= MAX_LOKASI; i++) {
        if (!nodes[i].isActive) continue;
        for (int j = 1; j <= MAX_LOKASI; j++) {
            if (nodes[j].isActive && w[i][j] != INF) {
                cout << "Dari: " << nodes[i].name << " (" << nodes[i].id << ") --> "
                     << "Ke: " << nodes[j].name << " (" << nodes[j].id << ")"
                     << " | Kapasitas: " << kapasitasJalur[i][j] << " org/tick"
                     << " | Waktu: " << w[i][j] << " tick\n";
                ada = true;
            }
        }
    }
    if (!ada) cout << "Belum ada jalur evakuasi yang dibuat.\n";
}
