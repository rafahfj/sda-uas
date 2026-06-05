#include <cstdlib>
#include <ctime>
#include <iostream>
#include "globals.h"
#include "queue.h"
#include "randomization.h"
#include "utils.h"

using namespace std;

// Pembagian kerja: Anggota 4
// Fokus: randomisasi data warga dan jalur graph untuk kebutuhan simulasi.
// ==== RANDOMISASI WARGA & JALUR ====
void randomisasi() {
    srand((unsigned int)time(0));

    int totalWarga;
    cout << "\nMasukkan total warga yang akan disebar secara acak: ";
    if (!bacaInt(totalWarga) || totalWarga <= 0) {
        cout << "Input tidak valid. Dibatalkan.\n";
        return;
    }

    int densitas;
    cout << "Densitas jalur acak (1-100, misal 30 = ~30% pasangan node terhubung): ";
    if (!bacaInt(densitas) || densitas < 1 || densitas > 100) {
        cout << "Input tidak valid. Dibatalkan.\n";
        return;
    }

    for (int i = 1; i <= MAX_LOKASI; i++) {
        for (int j = 1; j <= MAX_LOKASI; j++) {
            w[i][j] = INF;
            kapasitasJalur[i][j] = 0;
        }
        createQueue(i);
    }
    nextPersonId = 1;

    int nodeBahaya[MAX_LOKASI + 1], jumlahBahaya = 0;
    int jumlahPosko = 0;
    for (int i = 1; i <= nnode; i++) {
        if (!nodes[i].isActive) continue;
        if (nodes[i].isSafe) jumlahPosko++;
        else                  nodeBahaya[jumlahBahaya++] = i;
    }

    if (jumlahBahaya == 0) {
        cout << "Tidak ada node bahaya aktif. Tambahkan lokasi dulu.\n";
        return;
    }
    if (jumlahPosko == 0) {
        cout << "Tidak ada posko aman aktif. Tambahkan posko dulu.\n";
        return;
    }

    for (int k = 0; k < totalWarga; k++) {
        int idx = nodeBahaya[rand() % jumlahBahaya];
        Person p;
        p.id = nextPersonId++;
        p.name = "Warga";
        p.startTick = 0; 
        insertQueue(idx, p);
    }

    int semuaNode[MAX_LOKASI + 1], jumlahSemua = 0;
    for (int i = 1; i <= nnode; i++)
        if (nodes[i].isActive) semuaNode[jumlahSemua++] = i;

    int jalurDibuat = 0;

    for (int b = 0; b < jumlahBahaya; b++) {
        int dari = nodeBahaya[b];
        if (antrian[dari].top == 0) continue;

        int tujuan = -1;
        int coba = 0;
        while (coba < 20) {
            int kandidat = semuaNode[rand() % jumlahSemua];
            if (kandidat != dari) { tujuan = kandidat; break; }
            coba++;
        }
        if (tujuan == -1) continue;

        if (w[dari][tujuan] == INF) {
            w[dari][tujuan]            = 1 + rand() % 10; 
            kapasitasJalur[dari][tujuan] = 1 + rand() % 5;  
            jalurDibuat++;
        }
    }

    int targetJalur = (jumlahSemua * (jumlahSemua - 1) * densitas) / 100;
    int maxCoba = targetJalur * 5;
    for (int c = 0; c < maxCoba && jalurDibuat < targetJalur; c++) {
        int dari  = semuaNode[rand() % jumlahSemua];
        int tujuan = semuaNode[rand() % jumlahSemua];
        if (dari == tujuan) continue;
        if (nodes[dari].isSafe) continue;
        if (w[dari][tujuan] != INF) continue;

        w[dari][tujuan]            = 1 + rand() % 10;
        kapasitasJalur[dari][tujuan] = 1 + rand() % 5;
        jalurDibuat++;
    }

    cout << "\n  Randomisasi selesai:\n";
    cout << "  - " << totalWarga << " warga disebar ke " << jumlahBahaya << " lokasi bahaya\n";
    cout << "  - " << jalurDibuat << " jalur acak dibuat\n";
    cout << "  Sebaran warga:\n";
    for (int b = 0; b < jumlahBahaya; b++) {
        int idx = nodeBahaya[b];
        if (antrian[idx].top > 0)
            cout << "    " << nodes[idx].name << ": " << antrian[idx].top << " org\n";
    }
}
