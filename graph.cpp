#include <iostream>
#include "globals.h"
#include "graph.h"
#include "queue.h"

using namespace std;

// Inisialisasi Graph di Awal Program
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
        //Data kota/kab yang ada di jalur Sesar Lembang
            {1, "Kab. Bandung Barat", "Kabupaten", 0, false, true},
            {2, "Kota Bandung", "Kota", 0, false, true},
            {3, "Kota Cimahi", "Kota", 0, false, true},
            {4, "Kab. Bandung", "Kabupaten", 0, false, true},
            {5, "Kab. Sumedang", "Kabupaten", 0, false, true},

        //kecamatan kbb
            {6, "Kec. Lembang", "Kecamatan", 1, false, true},
            {7, "Kec. Parongpong", "Kecamatan", 1, false, true},
            {8, "Kec. Cisarua", "Kecamatan", 1, false, true},
            {9, "Kec. Ngamprah", "Kecamatan", 1, false, true},
            {10, "Kec. Padalarang", "Kecamatan", 1, false, true},
            {11, "Kec. Cikalong Wetan", "Kecamatan", 1, false, true},
            {12, "Kec. Cipeundeuy", "Kecamatan", 1, false, true},

        //kota bdg
            {13, "Kec. Cibiru", "Kecamatan", 2, false, true},
            {14, "Kec. Ujungberung", "Kecamatan", 2, false, true},
            {15, "Kec. Gedebage", "Kecamatan", 2, false, true},
            {16, "Kec. Arcamanik", "Kecamatan", 2, false, true},
            {17, "Kec. Regol", "Kecamatan", 2, false, true},
            {18, "Kec. Buah Batu", "Kecamatan", 2, false, true},
            {19, "Kec. Kiaracondong", "Kecamatan", 2, false, true},
            {20, "Kec. Lengkong", "Kecamatan", 2, false, true},
            {21, "Kec. Bandung Wetan", "Kecamatan", 2, false, true},
            {22, "Kec. Coblong", "Kecamatan", 2, false, true},

        //kab.bdg
            {23, "Kec. Cimenyan", "Kecamatan", 4, false, true},
            {24, "Kec. Cilengkrang", "Kecamatan", 4, false, true},

        //kab.smd
            {25, "Kec. Tanjungsari", "Kecamatan", 5, false, true},

        //desa di kec. Lembang
            {26, "Desa Lembang", "Desa", 6, false, true},
            {27, "Desa Pagerwangi", "Desa", 6, false, true},
            {28, "Desa Kayuambon", "Desa", 6, false, true},
            {29, "Desa Gudang Kahuripan", "Desa", 6, false, true},
            {30, "Desa Suntenjaya", "Desa", 6, false, true},

        //desa di kec.parongpong
            {31, "Desa Cihideung", "Desa", 7, false, true},
            {32, "Desa Cigugur Girang", "Desa", 7, false, true},
            {33, "Desa Karyawangi", "Desa", 7, false, true},
            {34, "Desa Cihanjuang", "Desa", 7, false, true},

        //desa di kec.cisarua
            {35, "Desa Jambudipa", "Desa", 8, false, true},
            {36, "Desa Tugumukti", "Desa", 8, false, true},
            {37, "Desa Pasirlangu", "Desa", 8, false, true},
            {38, "Desa Pasirhalang", "Desa", 8, false, true},
            {39, "Desa Padaasih", "Desa", 8, false, true},

        //desa di kec.ngamprah
            {40, "Desa Bojongkoneng", "Desa", 9, false, true},
            {41, "Desa Cilame", "Desa", 9, false, true},
            {42, "Desa Cimareme", "Desa", 9, false, true},
            {43, "Desa Sukatani", "Desa", 9, false, true},

        //kel di kota bdg
            {44, "Kel. Turangga", "Kelurahan", 21, false, true},
            {45, "Kel. Babakan Surabaya", "Kelurahan", 21, false, true},
            {46, "Kel. Cipadung", "Kelurahan", 15, false, true},
            {47, "Desa Cisaranten Kulon", "Kelurahan", 14, false, true},

        //posko aman
            {48, "Taman Tegalega", "Posko", 17, true, true},
            {49, "Stadion GBLA", "Posko", 15, true, true},
            {50, "Gasibu", "Posko", 21, true, true},
            {51, "Alun-Alun Kota Bandung", "Posko", 17, true, true},
            {52, "Sabuga (Sasana Budaya Ganesha)", "Posko", 22, true, true},
            {53, "Lap. Olahraga Arcamanik", "Posko", 16, true, true},
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
    cout << "Sukses memuat " << totalDataMasuk << " titik lokasi (termasuk 6 Posko Aman).\n";
}

// ======================== FUNGSI BANTU INDEKS ========================
int findNodeIndex(int id) {
    for (int i = 1; i <= MAX_LOKASI; i++) {
        if (nodes[i].isActive && nodes[i].id == id) return i;
    }
    return -1;
}

// ========================ALGORITMA DIJKSTRA========================
// Fungsi mencari jalur evakuasi terbaik dari parameter start (Asal) menuju posko aman terdekat
int hitungDijkstraManual(int startNode, int targetNode, int ruteHasil[], int &panjangRute) {
    int temp[MAX_LOKASI + 1];   // Menyimpan nilai bobot kumulatif terkecil sementara
    int prev[MAX_LOKASI + 1];   // Jalur simpanan rute balik penelusuran balik dosen
    int visited[MAX_LOKASI + 1];
    int path[MAX_LOKASI + 1];   // Array pembantu cetak rute terbalik dosen

    // 1. Inisialisasi Graph internal Dijkstra
    // FIX: inisialisasi sampai MAX_LOKASI agar node dengan ID > nnode tetap aman diakses
    for (int i = 0; i <= MAX_LOKASI; i++) {
        temp[i] = INF;
        prev[i] = -1;
        visited[i] = 0;
        path[i] = 0;
    }

    int start = startNode;
    temp[start] = 0;
    visited[start] = 1;

    // 2. Loop Utama Penelusuran Dijkstra
    while (visited[targetNode] == 0) {
        int minimum = INF;
        int m = -1; // FIX: gunakan -1 sebagai sentinel, bukan 0 (0 bukan indeks node valid)

        // Mengecek satu persatu hubungan antar vertex berdasarkan arahnya
        for (int i = 1; i <= nnode; i++) {
            if (nodes[i].isActive && w[start][i] != INF) {
                int update = temp[start] + w[start][i];

                // Bandingkan apakah bobot rute baru melintas di jalur ini lebih kecil
                if (update < temp[i] && visited[i] == 0) {
                    temp[i] = update;
                    prev[i] = start;
                }
            }

            // Membandingkan pencarian titik lompatan vertex selanjutnya yang terkecil nilainya
            if (minimum > temp[i] && visited[i] == 0) {
                minimum = temp[i];
                m = i;
            }
        }

        if (m == -1 || minimum == INF) {
            break; // Jika sudah tidak ada jalur tersambung lagi, hentikan pencarian
        }

        start = m;
        visited[start] = 1;
    }

    // 3. Pencetakan Path/Rute Balik
    if (temp[targetNode] == INF) {
        panjangRute = 0;
        return INF; // Rute buntu
    }

    int v = targetNode;
    start = v; // start -> vertex terakhir
    int j = 0;
    while (start != -1) {
        path[j] = start;
        start = prev[start];
        j++;
    }

    // Balik jalurnya agar urutan keluar dari asal -> tujuan
    // FIX: hapus filter != 0, cukup loop j kali agar tidak ada node valid yang terlewat
    panjangRute = 0;
    for (int i = j - 1; i >= 0; i--) {
        ruteHasil[panjangRute] = path[i];
        panjangRute++;
    }

    return temp[targetNode]; // Mengembalikan total bobot nilai waktu tercepat
}
