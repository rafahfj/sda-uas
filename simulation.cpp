#include <iostream>
#include "globals.h"
#include "graph.h"
#include "queue.h"
#include "simulation.h"

using namespace std;

// ======================== PROSES LOGIKA SIMULASI EVAKUASI WAKTU ========================
void runSimulation() {
    // Kumpulkan semua posko aman yang ada
    int daftarPosko[MAX_LOKASI + 1];
    int jumlahPosko = 0;
    for (int i = 1; i <= nnode; i++) {
        if (nodes[i].isActive && nodes[i].isSafe) {
            daftarPosko[jumlahPosko++] = i;
        }
    }

    if (jumlahPosko == 0) {
        cout << "Simulasi Gagal: Buat minimal 1 Lokasi berstatus AMAN (Posko) terlebih dahulu!\n";
        return;
    }

    // Hitung total warga awal
    int totalWarga = 0;
    for (int i = 1; i <= nnode; i++) {
        if (nodes[i].isActive && !nodes[i].isSafe) {
            totalWarga += antrian[i].top;
        }
    }

    if (totalWarga == 0) {
        cout << "Tidak ada warga di zona bahaya untuk dievakuasi saat ini.\n";
        return;
    }

    // Gandakan cadangan antrean warga asli sebelum disimulasikan agar data aslinya tidak hilang
    AntrianWarga antrianCadangan[MAX_LOKASI + 1];
    for(int i = 1; i <= nnode; i++) {
        antrianCadangan[i] = antrian[i];
    }

    // Header mulai simulasi
    cout << "\n  SIMULASI EVAKUASI\n";
    cout << "  Total warga: " << totalWarga << "  |  Maks: 500 menit\n\n";

    // FIX: Dijkstra dihitung SEKALI di sini, bukan di dalam loop tick
    // Jalur tidak berubah selama simulasi, jadi tidak perlu dihitung ulang tiap menit
    struct HasilDijkstra {
        int ruteTerbaik[MAX_LOKASI + 1];
        int panjangRute;
        int waktuTerbaik;
    };
    HasilDijkstra hasilPerNode[MAX_LOKASI + 1];
    for (int i = 1; i <= nnode; i++) {
        hasilPerNode[i].panjangRute  = 0;
        hasilPerNode[i].waktuTerbaik = INF;
    }
    for (int i = 1; i <= nnode; i++) {
        if (!nodes[i].isActive || nodes[i].isSafe) continue;
        for (int p = 0; p < jumlahPosko; p++) {
            int ruteCoba[MAX_LOKASI + 1];
            int panjangCoba = 0;
            int waktuCoba = hitungDijkstraManual(i, daftarPosko[p], ruteCoba, panjangCoba);
            if (waktuCoba < hasilPerNode[i].waktuTerbaik && panjangCoba >= 2) {
                hasilPerNode[i].waktuTerbaik = waktuCoba;
                hasilPerNode[i].panjangRute  = panjangCoba;
                for (int x = 0; x < panjangCoba; x++)
                    hasilPerNode[i].ruteTerbaik[x] = ruteCoba[x];
            }
        }
    }

    int tick = 0;
    int selamat = 0;

    // Loop pergerakan waktu evakuasi sampai batas maksimum aman 500 tick
    while (selamat < totalWarga && tick < 500) {
        tick++;
        bool adaPergerakan = false;

        // ---- Header menit ----
        int menunggu = 0;
        for (int i = 1; i <= nnode; i++)
            if (nodes[i].isActive && !nodes[i].isSafe) menunggu += antrian[i].top;

        cout << "\n--- Menit " << tick
             << "  |  menunggu: " << menunggu
             << "  |  selamat: " << selamat << "/" << totalWarga << " ---\n";

        // ---- Satu baris per lokasi berpenghuni ----
        for (int i = 1; i <= nnode; i++) {
            if (!nodes[i].isActive || nodes[i].isSafe || isEmpty(i) == 1) continue;
            int wkt = hasilPerNode[i].waktuTerbaik;
            int pan = hasilPerNode[i].panjangRute;
            if (wkt == INF || pan < 2) {
                cout << "  [!] " << nodes[i].name
                     << " (" << antrian[i].top << " org) -- TERISOLASI\n";
            } else {
                int next = hasilPerNode[i].ruteTerbaik[1];
                cout << "  " << nodes[i].name
                     << " (" << antrian[i].top << " org)"
                     << " -> " << nodes[next].name
                     << "\n";
            }
        }

        // ---- Proses pergerakan ----
        for (int i = 1; i <= nnode; i++) {
            if (!nodes[i].isActive || nodes[i].isSafe || isEmpty(i) == 1) continue;

            int panjangRute = hasilPerNode[i].panjangRute;
            int waktuTerbaik = hasilPerNode[i].waktuTerbaik;
            if (waktuTerbaik == INF || panjangRute < 2) continue;

            int nextNodeIdx = hasilPerNode[i].ruteTerbaik[1];
            int kapasitasPindah = kapasitasJalur[i][nextNodeIdx];
            int jumlahDiAntrian = antrian[i].top;
            int jumlahPindah = (kapasitasPindah < jumlahDiAntrian) ? kapasitasPindah : jumlahDiAntrian;

            for (int k = 0; k < jumlahPindah; k++) {
                Person p = antrian[i].isi[1];
                deleteQueue(i);
                if (nodes[nextNodeIdx].isSafe) {
                    selamat++;
                } else {
                    insertQueue(nextNodeIdx, p);
                }
                adaPergerakan = true;
            }
        }

        // Ringkasan akhir tick
        int sisaMenunggu = 0;
        for (int i = 1; i <= nnode; i++)
            if (nodes[i].isActive && !nodes[i].isSafe) sisaMenunggu += antrian[i].top;
        cout << "    => selamat: " << selamat << "/" << totalWarga
             << "  sisa: " << sisaMenunggu << "\n";

        if (!adaPergerakan && selamat < totalWarga) {
            cout << "\n  [PERINGATAN] Evakuasi macet! Sisa warga terisolasi (rute buntu / kapasitas 0).\n";
            break;
        }
    }

    // Laporan akhir
    cout << "\n  HASIL AKHIR SIMULASI\n\n";
    cout << "  Total korban terdata    : " << totalWarga << " orang\n";
    cout << "  Berhasil diselamatkan   : " << selamat    << " orang\n";
    cout << "  Sisa korban tertinggal  : " << (totalWarga - selamat) << " orang\n";
    cout << "  Total durasi operasi    : " << tick       << " menit\n\n";

    // FIX: pulihkan antrean asli agar simulasi bisa dijalankan ulang
    for (int i = 1; i <= nnode; i++) {
        antrian[i] = antrianCadangan[i];
    }
}

void caraMenggunakanSimulasi() {
    cout << "\n  CARA MENGGUNAKAN SIMULASI\n\n";
    cout << "  1. Tambahkan lokasi (Menu Lokasi)\n";
    cout << "     Minimal 1 lokasi BAHAYA dan 1 POSKO AMAN\n\n";
    cout << "  2. Hubungkan jalur antar lokasi (Menu Jalur)\n";
    cout << "     Tentukan waktu tempuh dan kapasitas jalur\n\n";
    cout << "  3. Tambahkan warga ke lokasi bahaya (Menu Warga)\n\n";
    cout << "  4. Jalankan simulasi (Menu Simulasi)\n";
    cout << "     Sistem otomatis cari rute tercepat (Dijkstra)\n";
    cout << "     Warga berpindah setiap menit\n\n";
}
