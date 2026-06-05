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
    cout << "\n--- DAFTAR LOKASI BENCANA ---\n";
    bool ada = false;
    for (int i = 1; i <= MAX_LOKASI; i++) {
        if (nodes[i].isActive && ((nodes[i].tipe == "Kota") || (nodes[i].tipe == "Kabupaten"))) {
            cout << "ID: " << nodes[i].id << " | Nama: " << nodes[i].name
                 << " | Status: " << (nodes[i].isSafe ? "AMAN (POSKO)" : "BAHAYA")
                 << " | Antrean: " << antrian[i].top << " orang\n";
            ada = true;
        }
    }

    if (!ada){
        cout << "Belum ada lokasi yang terdaftar.\n";
        return;
    }

    int kotaID;
    cout << "\nPilih ID Kota/Kabupaten untuk melihat data Kecamatan: ";
    if (!bacaInt(kotaID)) { cout << "Input dibatalkan.\n"; return; }

    int kotaIdx = findNodeIndex(kotaID);
    if (kotaIdx == -1 || ((nodes[kotaIdx].tipe != "Kota") && (nodes[kotaIdx].tipe != "Kabupaten"))){
        cout << "ID Kota/Kabupaten tidak valid!\n";
        return;
    }

    clearScreen();
    cout << "\n========================================\n";
    cout << " KOTA/KABUPATEN: " << nodes[kotaIdx].name << "\n";
    cout << "========================================\n";
    cout << "--- DAFTAR KECAMATAN TERSEDIA ---\n\n";

    bool adaKecamatan = false;
    for (int i = 1; i <= nnode; i++) {
        if (nodes[i].isActive && nodes[i].tipe == "Kecamatan" && nodes[i].parentID == kotaID){
            cout << "ID: " << nodes[i].id << " | Nama: " << nodes[i].name
                 << " | Status: " << (nodes[i].isSafe ? "AMAN (POSKO)" : "BAHAYA")
                 << " | Antrean: " << antrian[i].top << " orang\n";
            ada = true;
            adaKecamatan = true;
        }
    }

    if (!adaKecamatan) {
        cout << "Tidak ada data kecamatan di dalam wilayah ini.\n";
        return;
    }

    int kecID;
    cout << "\nPilih ID Kecamatan untuk melihat data Desa/Posko: ";
    if (!bacaInt(kecID)) { cout << "Input dibatalkan.\n"; return; }

    int kecIdx = findNodeIndex(kecID);
    // FIX: cukup cek tipenya Kecamatan, kondisi && sebelumnya terlalu ketat
    if (kecIdx == -1 || nodes[kecIdx].tipe != "Kecamatan" || nodes[kecIdx].parentID != kotaID) {
        cout << "ID Kecamatan tidak valid!\n";
        return;
    }

    clearScreen();
    cout << "\n========================================\n";
    cout << " KECAMATAN: " << nodes[kecIdx].name << "\n";
    cout << "========================================\n";
    cout << "--- DAFTAR DESA / KELURAHAN TERSEDIA ---\n\n";

    bool adaDesa = false;
    for (int i = 1; i <= nnode; i++) {
        if (nodes[i].isActive &&
           ((nodes[i].tipe == "Desa") || (nodes[i].tipe == "Kelurahan") || (nodes[i].tipe == "Posko")) &&
            nodes[i].parentID == kecID){
            // cout << "[" << nodes[i].id << "] " << nodes[i].name << endl;
            cout << "ID: " << nodes[i].id << " | Nama: " << nodes[i].name
                 << " | Status: " << (nodes[i].isSafe ? "AMAN (POSKO)" : "BAHAYA")
                 << " | Antrean: " << antrian[i].top << " orang\n";
            ada = true;
            adaDesa = true;
        }
    }

    if (!adaDesa) {
        cout << "Tidak ada data desa atau posko aman di dalam Kecamatan ini.\n";
        return;
    }
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
