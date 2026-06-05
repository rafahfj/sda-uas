#include <iostream>
#include "display.h"
#include "edge.h"
#include "globals.h"
#include "graph.h"
#include "utils.h"

using namespace std;

// ======================== MANAJEMEN CRUD JALUR (EDGE) ========================
void createEdge() {
    displayNodes();
    int fromId, toId;
    cout << "Masukkan ID Lokasi Asal: ";
    if (!bacaInt(fromId)) { cout << "Input dibatalkan.\n"; return; }
    cout << "Masukkan ID Lokasi Tujuan: ";
    if (!bacaInt(toId)) { cout << "Input dibatalkan.\n"; return; }

    int fromIdx = findNodeIndex(fromId);
    int toIdx = findNodeIndex(toId);

    if (fromIdx == -1 || toIdx == -1) {
        cout << "ID Lokasi Asal atau Tujuan tidak valid!\n";
        return;
    }

    if (fromIdx == toIdx) {
        cout << "Lokasi asal dan tujuan tidak boleh sama!\n";
        return;
    }

    int kap, waktu;
    cout << "Kapasitas Jalur Lintasan (orang per tick): ";
    if (!bacaInt(kap) || kap <= 0) { cout << "Kapasitas harus angka positif. Input dibatalkan.\n"; return; }
    cout << "Waktu tempuh jarak lintasan (bobot menit): ";
    if (!bacaInt(waktu) || waktu <= 0) { cout << "Waktu tempuh harus angka positif. Input dibatalkan.\n"; return; }
    kapasitasJalur[fromIdx][toIdx] = kap;
    w[fromIdx][toIdx] = waktu;
    cout << "Rute evakuasi sukses dibuat.\n";
}

void updateEdge() {
    displayEdges();
    int fromId, toId;
    cout << "ID lokasi asal rute: ";
    if (!bacaInt(fromId)) { cout << "Input dibatalkan.\n"; return; }
    cout << "ID lokasi tujuan rute: ";
    if (!bacaInt(toId)) { cout << "Input dibatalkan.\n"; return; }

    int fromIdx = findNodeIndex(fromId);
    int toIdx = findNodeIndex(toId);

    if (fromIdx == -1 || toIdx == -1 || w[fromIdx][toIdx] == INF) {
        cout << "Rute jalur tersebut tidak ditemukan.\n";
        return;
    }
    int kap, waktu;
    cout << "Kapasitas Jalur Baru: ";
    if (!bacaInt(kap) || kap <= 0) { cout << "Kapasitas harus angka positif. Input dibatalkan.\n"; return; }
    cout << "Waktu Tempuh (Bobot) Baru: ";
    if (!bacaInt(waktu) || waktu <= 0) { cout << "Waktu tempuh harus angka positif. Input dibatalkan.\n"; return; }
    kapasitasJalur[fromIdx][toIdx] = kap;
    w[fromIdx][toIdx] = waktu;
    cout << "Data rute evakuasi berhasil diperbarui.\n";
}

void deleteEdge() {
    displayEdges();
    int fromId, toId;
    cout << "Masukkan ID Lokasi Asal: ";
    if (!bacaInt(fromId)) { cout << "Input dibatalkan.\n"; return; }
    cout << "Masukkan ID Lokasi Tujuan: ";
    if (!bacaInt(toId)) { cout << "Input dibatalkan.\n"; return; }

    int fromIdx = findNodeIndex(fromId);
    int toIdx = findNodeIndex(toId);

    if (fromIdx == -1 || toIdx == -1 || w[fromIdx][toIdx] == INF) {
        cout << "Rute tidak ditemukan.\n";
        return;
    }
    w[fromIdx][toIdx] = INF;
    kapasitasJalur[fromIdx][toIdx] = 0;
    cout << "Rute evakuasi sukses diputus.\n";
}
