#include <iostream>
#include <string>
#include "display.h"
#include "globals.h"
#include "graph.h"
#include "location.h"
#include "queue.h"
#include "utils.h"

using namespace std;

bool isPilihanBinerValid(int nilai) {
    return nilai == 0 || nilai == 1;
}

// ===== MANAJEMEN CRUD LOKASI (NODE) ====
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

    // cin.ignore(10000, '\n');

    cout << "Masukkan nama lokasi baru: "; getline(cin, nodes[slot].name);
    if (nodes[slot].name == "") {
        cout << "Nama lokasi tidak boleh kosong. Input dibatalkan.\n";
        return;
    }

    if (!isValid(nodes[slot].name)) {
        return;
    }

    cout << "Apakah lokasi ini Posko Aman? (1=Ya, 0=Tidak): ";
    int safe;
    if (!bacaInt(safe)) { cout << "Input dibatalkan.\n"; return; }
    if (!isPilihanBinerValid(safe)) {
        cout << "Pilihan status aman harus 1 atau 0. Input dibatalkan.\n";
        return;
    }
    nodes[slot].isSafe = (safe == 1);
    nodes[slot].tipe = (safe == 1) ? "Posko" : "Desa";

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
    // cin.ignore();
    string namaBaru;
    getline(cin, namaBaru);
    if (namaBaru == "") {
        cout << "Nama lokasi tidak boleh kosong. Update dibatalkan.\n";
        return;
    }
    if (!isValid(namaBaru)) {
        return;
    }
    if (nodes[idx].tipe != "Desa" && nodes[idx].tipe != "Kelurahan" && nodes[idx].tipe != "Posko") {
        nodes[idx].name = namaBaru;
        cout << "Nama lokasi sukses diperbarui. Status aman tidak diubah untuk Jalur.\n";
        return;
    }
    cout << "Status titik aman baru? (1=Ya, 0=Tidak): ";
    int safe;
    if (!bacaInt(safe)) { cout << "Input dibatalkan.\n"; return; }
    if (!isPilihanBinerValid(safe)) {
        cout << "Pilihan status aman harus 1 atau 0. Update dibatalkan.\n";
        return;
    }
    nodes[idx].name = namaBaru;
    nodes[idx].isSafe = (safe == 1);
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
    
    cout << "\n========================================\n";
    cout << " Lokasi yang akan dihapus:\n";
    cout << " ID: " << nodes[idx].id << " | Nama: " << nodes[idx].name;
    cout << " | Tipe: " << nodes[idx].tipe << "\n";
    cout << "========================================\n";
    
    char konfirmasi;
    cout << " [!] PERINGATAN: Aksi ini akan menghapus lokasi beserta semua jalurnya!\n";
    cout << " Apakah Anda yakin ingin menghapus lokasi ini? (y/n): "; 
    cin >> konfirmasi;
    cin.ignore(10000, '\n');
    
    if (konfirmasi != 'y' && konfirmasi != 'Y') {
        cout << " [-] Aksi dibatalkan. Penghapusan data lokasi tidak dilakukan.\n";
        return;
    }
    
    for (int i = 1; i <= MAX_LOKASI; i++) {
        w[idx][i] = INF;
        w[i][idx] = INF;
        kapasitasJalur[idx][i] = 0;
        kapasitasJalur[i][idx] = 0;
    }
    
    createQueue(idx);
    
    nodes[idx].isActive = false;
    
    cout << " [+] Lokasi '" << nodes[idx].name << "' beserta jalur hubungannya sukses dihapus dari peta simulasi.\n";
}
