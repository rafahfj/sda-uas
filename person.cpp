#include <iostream>
#include "display.h"
#include "globals.h"
#include "graph.h"
#include "person.h"
#include "queue.h"
#include "utils.h"

using namespace std;

// Pembagian kerja: Anggota 2
// Fokus: fitur tambah dan hapus warga pada antrean lokasi.
// ==== MANAJEMEN CRUD WARGA CORNER ====
void addPerson() {
    displayNodes(); 
    
    int nodeId;
    cout << "\nMasukkan ID Lokasi tempat warga berkumpul saat ini: ";
    if (!bacaInt(nodeId)) { cout << "Input dibatalkan.\n"; return; }
  
    if (nodeId < 1 || nodeId > nnode || !nodes[nodeId].isActive) {
        cout << " [!] ID Lokasi tidak terdaftar di wilayah Lembang.\n";
        return;
    }
    
    if (nodes[nodeId].isSafe) {
        cout << " [!] Tidak bisa menambah warga ke Posko Aman. Pilih lokasi bahaya.\n";
        return;
    }
    
    int count;
    cout << "Jumlah korban warga yang ingin ditambahkan: ";
    if (!bacaInt(count) || count <= 0) { 
        cout << " [!] Jumlah harus angka positif. Input dibatalkan.\n"; 
        return; 
    }
   
    for (int i = 0; i < count; i++) {
        Person p;
        p.id = nextPersonId++;
        p.name = "Warga";
        p.startTick = 0; 
  
        insertQueue(nodeId, p);
    }
    
    cout << " [+] " << count << " warga sukses dimasukkan ke dalam antrean evakuasi " << nodes[nodeId].name << ".\n";
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
    createQueue(idx); 
    cout << "Seluruh antrean warga di lokasi tersebut dibersihkan.\n";
}
