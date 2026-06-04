#include <iostream>
#include "display.h"
#include "globals.h"
#include "graph.h"
#include "person.h"
#include "queue.h"
#include "utils.h"

using namespace std;

// ======================== MANAJEMEN CRUD WARGA CORNER ========================
void addPerson() {
    displayNodes();
    int nodeId;
    cout << "Masukkan ID Lokasi tempat warga berkumpul saat ini: ";
    if (!bacaInt(nodeId)) { cout << "Input dibatalkan.\n"; return; }
    int idx = findNodeIndex(nodeId);
    if (idx == -1) {
        cout << "Lokasi tidak terdaftar.\n";
        return;
    }
    if (nodes[idx].isSafe) {
        cout << "Tidak bisa menambah warga ke Posko Aman. Pilih lokasi bahaya.\n";
        return;
    }
    int count;
    cout << "Jumlah korban warga yang ingin ditambahkan: ";
    if (!bacaInt(count) || count <= 0) { cout << "Jumlah harus angka positif. Input dibatalkan.\n"; return; }
    // FIX: hapus cin.ignore() yang tidak perlu di sini â€” tidak ada getline setelah ini
    for (int i = 0; i < count; i++) {
        Person p;
        p.id = nextPersonId++;
        p.name = "Warga";
        p.startTick = 0; // dicatat saat warga didaftarkan (sebelum simulasi mulai)
        insertQueue(idx, p);
    }
    cout << count << " warga sukses dimasukkan ke dalam antrean evakuasi.\n";
}

void deletePerson() {
    displayNodes();
    int nodeId;
    cout << "Masukkan ID lokasi yang ingin dikosongkan antreannya: ";
    if (!bacaInt(nodeId)) { cout << "Input dibatalkan.\n"; return; }
    int idx = findNodeIndex(nodeId);
    if (idx == -1) {
        cout << "Lokasi tidak valid.\n";
        return;
    }
    createQueue(idx); // reset top = 0 secara instan
    cout << "Seluruh antrean warga di lokasi tersebut dibersihkan.\n";
}
