#include <iostream>
#include "globals.h"
#include "queue.h"

using namespace std;

void createQueue(int nodeIdx) {
    antrian[nodeIdx].top = 0;
}

int isEmpty(int nodeIdx) {
    if (antrian[nodeIdx].top == 0) {
        return 1;
    } else {
        return 0;
    }
}

// Fungsi Enqueue (Tambah data warga ke antrean)
void insertQueue(int nodeIdx, Person p) {
    if (antrian[nodeIdx].top >= MAX_ANTREAN - 1) {
        cout << "Maaf, antrean di lokasi ini sudah penuh!\n";
        return;
    }
    antrian[nodeIdx].top++;
    antrian[nodeIdx].isi[antrian[nodeIdx].top] = p;
}

// Fungsi Dequeue
void deleteQueue(int nodeIdx) {
    if (isEmpty(nodeIdx) == 1) {
        // Jika kosong, tidak ada tindakan atau cetak pesan kosong
        return;
    } else {
        // Menggeser antrean ke depan (indeks dimulai dari 1 agar konsisten dengan loop geser dosen)
        for (int i = 2; i <= antrian[nodeIdx].top; i++) {
            antrian[nodeIdx].isi[i - 1] = antrian[nodeIdx].isi[i];
        }
        antrian[nodeIdx].top--;
    }
}

// Fungsi cari antrean manual (Hanya contoh pelengkap sesuai fungsi 'findQueue' materi dosen)
void findQueue(int nodeIdx, string cari) {
    string temp = "";
    int itemp = 0;

    if (isEmpty(nodeIdx) == 1) {
        cout << "Maaf pencarian data tidak dapat dilakukan karena antrian kosong\n";
    } else {
        // Loop pencarian berdasarkan penanda antrian.top dari dosen
        for (int i = 1; i <= antrian[nodeIdx].top; i++) {
            if (cari == antrian[nodeIdx].isi[i].name) {
                temp = cari;
                itemp = i;
            }
        }
        if (cari == temp) {
            cout << "Data " << cari << " ditemukan pada posisi ke-" << itemp << " di antrian\n";
        } else {
            cout << "Tidak ada data " << cari << " yang ditemukan pada antrian\n";
        }
    }
}
