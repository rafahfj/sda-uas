#include <iostream>
#include "globals.h"
#include "graph.h"
#include "queue.h"

using namespace std;

void inisialisasiGraph() {
    nnode = 0;
    for (int i = 1; i <= MAX_LOKASI; i++) {
        nodes[i].isActive = false;
        createQueue(i);
        for (int j = 1; j <= MAX_LOKASI; j++) {
            w[i][j] = INF;
            kapasitasJalur[i][j] = 0;
        }
    }

    Node dataLokasi[] = {
        // --- DESA-DESA DI KECAMATAN LEMBANG (ZONA BAHAYA/ASAL EVAKUASI) ---
        {1, "Desa Lembang Utama", "Desa", 6, false, true},
        {2, "Desa Pagerwangi", "Desa", 6, false, true},
        {3, "Desa Kayuambon", "Desa", 6, false, true},
        {4, "Desa Gudang Kahuripan", "Desa", 6, false, true},
        {5, "Desa Suntenjaya", "Desa", 6, false, true},
        {6, "Desa Cibodas Lembang", "Desa", 6, false, true},

        // --- TITIK PERANTARA / PERSIMPANGAN JALUR UTAMA & ALTERNATIF ---
        {7, "Pertigaan Maribaya", "Kecamatan", 6, false, true},
        {8, "Jalan Raya Lembang (Pusat)", "Kecamatan", 6, false, true},
        {9, "Jalur Alternatif Citamiang", "Kecamatan", 6, false, true},
        {10, "Jalan Dago Giri (Jalur Pintas)", "Kecamatan", 6, false, true},

        // --- POSKO AMAN (TUJUAN AKHIR) ---
        {11, "Posko Utama Stadion Sporthall Lembang", "Posko", 6, true, true},
        {12, "Posko Sabuga (Sasana Budaya Ganesha)", "Posko", 22, true, true},
        {13, "Posko Lapangan Gasibu", "Posko", 21, true, true}
    };

    int totalDataMasuk = sizeof(dataLokasi) / sizeof(dataLokasi[0]);

    int idTerbesar = 0;
    for (int i = 0; i < totalDataMasuk; i++){
        int targetIndeks = dataLokasi[i].id;
        nodes[targetIndeks] = dataLokasi[i];
        if (targetIndeks > idTerbesar){
            idTerbesar = targetIndeks;
        }
    }

    nnode = idTerbesar;
    
    // =========================================================================
    // PRESET JALUR OTOMATIS (Hubungan Ketetanggan untuk bahan uji coba Dijkstra)
    // =========================================================================
    
    // 1. Jalur Keluar dari Desa Suntenjaya (ID 5) punya 2 Alternatif rute ke Posko Sabuga:
    // Lewat Pertigaan Maribaya (ID 7)
    w[5][7] = 8;   kapasitasJalur[5][7] = 20;
    w[7][12] = 12; kapasitasJalur[7][12] = 25; // Total waktu = 8 + 12 = 20 menit
    
    // Lewat Jalur Alternatif Citamiang (ID 9)
    w[5][9] = 15;  kapasitasJalur[5][9] = 15;
    w[9][12] = 10; kapasitasJalur[9][12] = 15; // Total waktu = 15 + 10 = 25 menit
    
    // 2. Jalur dari Desa Lembang Utama (ID 1) menuju Posko Utama Lembang (ID 11)
    w[1][8] = 4;   kapasitasJalur[1][8] = 30;  // Ke Jalan Raya Lembang Pusat
    w[8][11] = 5;  kapasitasJalur[8][11] = 40; // Dari Pusat ke Stadion Sporthall

    // 3. Jalur dari Desa Pagerwangi (ID 2) menuju Posko Gasibu (ID 13) lewat Dago Giri
    w[2][10] = 6;  kapasitasJalur[2][10] = 15;
    w[10][13] = 14; kapasitasJalur[10][13] = 20;

    cout << "Sukses memuat " << totalDataMasuk << " titik lokasi khusus Kecamatan Lembang (3 Posko Aman).\n";
}

// ===== FUNGSI BANTU INDEKS ====
int findNodeIndex(int id) {
    for (int i = 1; i <= nnode; i++) { // Dioptimasi batasnya sampai nnode saja
        if (nodes[i].isActive && nodes[i].id == id) return i;
    }
    return -1;
}

// ==== ALGORITMA DIJKSTRA ====
int hitungDijkstraManual(int startNode, int targetNode, int ruteHasil[], int &panjangRute) {
    int temp[MAX_LOKASI + 1];   
    int prev[MAX_LOKASI + 1];  
    int visited[MAX_LOKASI + 1];
    int path[MAX_LOKASI + 1]; 

    for (int i = 0; i <= MAX_LOKASI; i++) {
        temp[i] = INF;
        prev[i] = -1;
        visited[i] = 0;
        path[i] = 0;
    }

    int start = startNode;
    temp[start] = 0;
    visited[start] = 1;

    // Loop utama pencarian tetangga terdekat
    while (visited[targetNode] == 0) {
        int minimum = INF;
        int m = -1; 

        for (int i = 1; i <= nnode; i++) {
            if (nodes[i].isActive && w[start][i] != INF) {
                int update = temp[start] + w[start][i];

                if (update < temp[i] && visited[i] == 0) {
                    temp[i] = update;
                    prev[i] = start;
                }
            }

            if (minimum > temp[i] && visited[i] == 0) {
                minimum = temp[i];
                m = i;
            }
        }

        if (m == -1 || minimum == INF) {
            break;
        }

        start = m;
        visited[start] = 1;
    }

    if (temp[targetNode] == INF) {
        panjangRute = 0;
        return INF; 
    }

    int v = targetNode;
    start = v; 
    int j = 0;
    while (start != -1) {
        path[j] = start;
        start = prev[start];
        j++;
    }

    panjangRute = 0;
    for (int i = j - 1; i >= 0; i--) {
        ruteHasil[panjangRute] = path[i];
        panjangRute++;
    }

    return temp[targetNode];
}