#include <iostream>
#include "display.h"
#include "globals.h"
#include "graph.h"
#include "location.h"
#include "queue.h"
#include "utils.h"

using namespace std;

// ======================== MANAJEMEN CRUD LOKASI (NODE) ========================
void createNode() {
    int slot = -1;
    for (int i = 1; i <= MAX_LOKASI; i++) {
        if (!nodes[i].isActive) {
            slot = i;
            break;
        }
    }
    if (slot == -1) {
        cout << "Gagal: Kapasitas penyimpanan memori Lokasi penuh!\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "       TAMBAH DATA LOKASI BARU          \n";
    cout << "========================================\n";

    cout << "Pilih Tingkatan/Tipe Lokasi:\n";
    cout << "1. Kota / Kabupaten\n";
    cout << "2. Kecamatan\n";
    cout << "3. Desa / Kelurahan / Posko\n";
    cout << "Masukkan pilihan (1-3): ";
    int pilihanTipe;
    if (!bacaInt(pilihanTipe)) { cout << "Input dibatalkan.\n"; return; }
    cin.ignore();

    if (pilihanTipe < 1 || pilihanTipe > 3) {
        cout << "Pilihan tidak valid! Input dibatalkan\n";
        return;
    }

    cout << "Masukkan nama lokasi baru: "; getline(cin, nodes[slot].name);

    if (pilihanTipe == 1) {
        // FIX: tambah pilihan Kabupaten agar konsisten dengan data preset
        cout << "Pilih jenis wilayah (1=Kota, 2=Kabupaten): ";
        int jenisWilayah;
        if (!bacaInt(jenisWilayah)) { cout << "Input dibatalkan.\n"; return; }
        nodes[slot].tipe = (jenisWilayah == 2) ? "Kabupaten" : "Kota";
        nodes[slot].parentID = 0;
        nodes[slot].isSafe = false;

    } else if (pilihanTipe == 2) {
        nodes[slot].tipe = "Kecamatan";

        // FIX: tampilkan Kota DAN Kabupaten sebagai pilihan induk kecamatan
        cout << "\n--- PILIH KOTA/KABUPATEN INDUK UNTUK KECAMATAN INI ---\n";
        for (int i = 1; i <= nnode; i++){
            if (nodes[i].isActive && (nodes[i].tipe == "Kota" || nodes[i].tipe == "Kabupaten")) {
                cout << "[" << nodes[i].id << "] " << nodes[i].name << " (" << nodes[i].tipe << ")" << endl;
            }
        }
        cout << "Masukkan ID Kota/Kabupaten Induk: ";
        if (!bacaInt(nodes[slot].parentID)) { cout << "Input dibatalkan.\n"; return; }

    } else if (pilihanTipe == 3) {
        cout << "\n--- PILIH KECAMATAN INDUK UNTUK DESA INI ---\n";
        for (int i = 1; i <= nnode; i++){
            if (nodes[i].isActive && nodes[i].tipe == "Kecamatan") {
                cout << "[" << nodes[i].id << "] " << nodes[i].name << endl;
            }
        }

        cout << "Masukkan ID Kecamatan Induk: ";
        if (!bacaInt(nodes[slot].parentID)) { cout << "Input dibatalkan.\n"; return; }

        cout << "Apakah lokasi ini Posko Aman? (1=Ya, 0=Tidak): ";
        int safe;
        if (!bacaInt(safe)) { cout << "Input dibatalkan.\n"; return; }
        nodes[slot].isSafe = (safe == 1);
        nodes[slot].tipe = (safe == 1) ? "Posko" : "Desa";
    }

    nodes[slot].id = slot;
    nodes[slot].isActive = true;
    createQueue(slot);

    if (slot > nnode) {
        nnode = slot;
    }

    cout << "\nLokasi '" << nodes[slot].name << "' sukses ditambahkan dengan ID sistem: " << nodes[slot].id << endl;
}

void updateNode() {
    displayNodes();
    int id;
    cout << "Masukkan ID lokasi yang akan diupdate: ";
    if (!bacaInt(id)) { cout << "Input dibatalkan.\n"; return; }
    int idx = findNodeIndex(id);
    if (idx == -1) {
        cout << "Lokasi tidak ditemukan.\n";
        return;
    }
    cout << "Nama baru lokasi: ";
    cin.ignore();
    getline(cin, nodes[idx].name);
    cout << "Status titik aman baru? (1=Ya, 0=Tidak): ";
    int safe;
    if (!bacaInt(safe)) { cout << "Input dibatalkan.\n"; return; }
    nodes[idx].isSafe = (safe == 1);
    // Sesuaikan tipe jika posko/bukan posko
    if (safe == 1 && nodes[idx].tipe != "Posko") nodes[idx].tipe = "Posko";
    else if (safe == 0 && nodes[idx].tipe == "Posko") nodes[idx].tipe = "Desa";
    cout << "Data lokasi sukses diperbarui.\n";
}

void deleteNode() {
    displayNodes();
    int id;
    cout << "Masukkan ID lokasi yang akan dihapus: ";
    if (!bacaInt(id)) { cout << "Input dibatalkan.\n"; return; }
    int idx = findNodeIndex(id);
    if (idx == -1) {
        cout << "Lokasi tidak ditemukan.\n";
        return;
    }

    // Hapus total relasi rute di matriks
    for (int i = 1; i <= MAX_LOKASI; i++) {
        w[idx][i] = INF;
        w[i][idx] = INF;
        kapasitasJalur[idx][i] = 0;
        kapasitasJalur[i][idx] = 0;
    }

    // Kosongkan antrean warga di dalamnya
    createQueue(idx);
    nodes[idx].isActive = false;
    cout << "Lokasi beserta jalur hubungannya sukses dihapus dari peta simulasi.\n";
}
