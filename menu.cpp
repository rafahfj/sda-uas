#include <iostream>
#include "display.h"
#include "edge.h"
#include "location.h"
#include "menu.h"
#include "person.h"
#include "utils.h"

using namespace std;

// ======================== SUB-MENU ========================
void menuLokasi() {
    int choice;
    do {
        clearScreen();
        cout << "\n  MANAJEMEN LOKASI\n\n";
        cout << "  1. Tambah Lokasi Baru\n";
        cout << "  2. Lihat Semua Lokasi\n";
        cout << "  3. Lihat Semua Posko Aman\n";
        cout << "  4. Perbarui Info Lokasi\n";
        cout << "  5. Hapus Lokasi\n";
        cout << "  0. Kembali ke Menu Utama\n\n";
        cout << "  Pilihan: ";
        if (!bacaInt(choice)) { backToMenu(); continue; }
        clearScreen();
        switch (choice) {
            case 1: createNode();   backToMenu(); break;
            case 2: displayNodes(); backToMenu(); break;
            case 3: displayPosko(); backToMenu(); break;
            case 4: updateNode();   backToMenu(); break;
            case 5: deleteNode();   backToMenu(); break;
            case 0: break;
            default: cout << "  Pilihan tidak valid.\n"; backToMenu();
        }
    } while (choice != 0);
}

void menuJalur() {
    int choice;
    do {
        clearScreen();
        cout << "\n  MANAJEMEN JALUR\n\n";
        cout << "  1. Buat Jalur Baru\n";
        cout << "  2. Lihat Semua Jalur\n";
        cout << "  3. Perbarui Bobot Jalur\n";
        cout << "  4. Putus Jalur\n";
        cout << "  0. Kembali ke Menu Utama\n\n";
        cout << "  Pilihan: ";
        if (!bacaInt(choice)) { backToMenu(); continue; }
        clearScreen();
        switch (choice) {
            case 1: createEdge();   backToMenu(); break;
            case 2: displayEdges(); backToMenu(); break;
            case 3: updateEdge();   backToMenu(); break;
            case 4: deleteEdge();   backToMenu(); break;
            case 0: break;
            default: cout << "  Pilihan tidak valid.\n"; backToMenu();
        }
    } while (choice != 0);
}

void menuWarga() {
    int choice;
    do {
        clearScreen();
        cout << "\n  MANAJEMEN WARGA\n\n";
        cout << "  1. Daftarkan Warga\n";
        cout << "  2. Bersihkan Antrean\n";
        cout << "  0. Kembali ke Menu Utama\n\n";
        cout << "  Pilihan: ";
        if (!bacaInt(choice)) { backToMenu(); continue; }
        clearScreen();
        switch (choice) {
            case 1: addPerson();    backToMenu(); break;
            case 2: deletePerson(); backToMenu(); break;
            case 0: break;
            default: cout << "  Pilihan tidak valid.\n"; backToMenu();
        }
    } while (choice != 0);
}

// ======================== MENU UTAMA ========================
void showMenu() {
    cout << "\n  SIMULASI EVAKUASI BENCANA SESAR LEMBANG\n";
    cout << "  (Queue & Graph - Dijkstra)\n\n";
    cout << "  1. Manajemen Lokasi\n";
    cout << "  2. Manajemen Jalur\n";
    cout << "  3. Manajemen Warga\n";
    cout << "  4. Jalankan Simulasi Evakuasi\n";
    cout << "  5. Cara Menggunakan Simulasi\n";
    cout << "  6. Randomisasi Warga & Jalur\n\n";
    cout << "  0. Keluar Aplikasi\n\n";
    cout << "  Pilihan: ";
}
