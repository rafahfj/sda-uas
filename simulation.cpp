#include <iostream>
#include "globals.h"
#include "graph.h"
#include "queue.h"
#include "simulation.h"

using namespace std;

// ==== PROSES LOGIKA SIMULASI EVAKUASI WAKTU ====
void runSimulation() {
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

    AntrianWarga antrianCadangan[MAX_LOKASI + 1];
    for(int i = 1; i <= nnode; i++) {
        antrianCadangan[i] = antrian[i];
    }

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

    cout << "\n===========================================================\n";
    cout << "          PROSES ANALISIS JALUR KECAMATAN LEMBANG          \n";
    cout << "===========================================================\n";
    
    for (int i = 1; i <= nnode; i++) {
        if (nodes[i].isActive && !nodes[i].isSafe && antrian[i].top > 0) {
            cout << " [SCANNING] Alternatif Jalur Keluar dari " << nodes[i].name << ":\n";
            cout << "-----------------------------------------------------------\n";
            
            bool adaTetangga = false;
            for (int j = 1; j <= nnode; j++) {
                if (w[i][j] != INF) {
                    printf("  -> Ke %-28s | Jarak/Waktu: %2d mnt | Kapasitas: %2d org", 
                           nodes[j].name.c_str(), w[i][j], kapasitasJalur[i][j]);
                    
                    if (hasilPerNode[i].panjangRute > 1 && j == hasilPerNode[i].ruteTerbaik[1]) {
                        cout << " [REKOMENDASI DIJKSTRA]";
                    }
                    cout << "\n";
                    adaTetangga = true;
                }
            }
            if (!adaTetangga) {
                cout << "  [!] Jalur buntu total! Tidak ada akses jalan keluar.\n";
            }
            
            cout << "-----------------------------------------------------------\n";
            cout << " [INFO DIJKSTRA] Keputusan Akhir untuk " << nodes[i].name << ":\n";
            if (hasilPerNode[i].waktuTerbaik != INF) {
                cout << "  Rute Tercepat : ";
                for (int x = 0; x < hasilPerNode[i].panjangRute; x++) {
                    cout << nodes[hasilPerNode[i].ruteTerbaik[x]].name;
                    if (x < hasilPerNode[i].panjangRute - 1) cout << " -> ";
                }
                cout << "\n  Total Estimasi: " << hasilPerNode[i].waktuTerbaik << " menit\n";
            } else {
                cout << "  [!] Jalur Terisolasi! Tidak terhubung ke posko aman manapun.\n";
            }
            cout << "===========================================================\n\n";
        }
    }

    cout << "Persiapan analisis selesai. Tekan ENTER untuk memulai pergerakan simulasi...\n";
    cin.get(); cin.get(); 

    cout << "\n  SIMULASI EVAKUASI BERJALAN\n";
    cout << "  Total warga: " << totalWarga << "  |  Maks: 500 menit\n";

    int tick = 0;
    int selamat = 0;

    while (selamat < totalWarga && tick < 500) {
        tick++;
        bool adaPergerakan = false;

        int menunggu = 0;
        for (int i = 1; i <= nnode; i++)
            if (nodes[i].isActive && !nodes[i].isSafe) menunggu += antrian[i].top;

        cout << "\n--- SIKLUS PERGERAKAN KE-" << tick << " ---\n";
        cout << "  Status Warga -> Belum Evakuasi: " << menunggu << " org | Berhasil Selamat: " << selamat << "/" << totalWarga << " org\n";

        for (int i = 1; i <= nnode; i++) {
            if (!nodes[i].isActive || nodes[i].isSafe || isEmpty(i) == 1) continue;
            
            int wkt = hasilPerNode[i].waktuTerbaik;
            int pan = hasilPerNode[i].panjangRute;
            
            if (wkt == INF || pan < 2) {
                cout << "  [!] " << nodes[i].name << " (" << antrian[i].top << " org) -- TERISOLASI\n";
            } else {
                int next = hasilPerNode[i].ruteTerbaik[1];
                int kap = kapasitasJalur[i][next];
                int jlh = antrian[i].top;

                if (jlh > kap) {
                    cout << "  [Jalur Utama] " << kap << " org dari " << nodes[i].name << " -> " << nodes[next].name << " (Rekomendasi Dijkstra)\n";
                    
                    int altNode = -1;
                    for (int j = 1; j <= nnode; j++) {
                        if (w[i][j] != INF && j != next && kapasitasJalur[i][j] > 0) {
                            altNode = j;
                            break;
                        }
                    }
                    if (altNode != -1) {
                        int sisaDipindahkan = (jlh - kap > kapasitasJalur[i][altNode]) ? kapasitasJalur[i][altNode] : (jlh - kap);
                        cout << "  [Jalur Pengalihan] " << sisaDipindahkan << " org dialihkan lewat -> " << nodes[altNode].name << " karena jalur utama penuh!\n";
                    } else {
                        cout << "  [!] Jalur Alternatif tidak ada! " << (jlh - kap) << " org terpaksa mengantre kembali.\n";
                    }
                } else {
                    cout << "  " << nodes[i].name << " (" << jlh << " org) -> " << nodes[next].name << " (Lancar)\n";
                }
            }
        }

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

            int sisaWarga = antrian[i].top;
            if (sisaWarga > 0) {
                int altNodeIdx = -1;
                for (int j = 1; j <= nnode; j++) {
                    if (w[i][j] != INF && j != nextNodeIdx && kapasitasJalur[i][j] > 0) {
                        altNodeIdx = j;
                        break;
                    }
                }
                if (altNodeIdx != -1) {
                    int kapasitasAlt = kapasitasJalur[i][altNodeIdx];
                    int jumlahPindahAlt = (kapasitasAlt < sisaWarga) ? kapasitasAlt : sisaWarga;
                    for (int k = 0; k < jumlahPindahAlt; k++) {
                        Person p = antrian[i].isi[1];
                        deleteQueue(i);
                        if (nodes[altNodeIdx].isSafe) {
                            selamat++;
                        } else {
                            insertQueue(altNodeIdx, p);
                        }
                        adaPergerakan = true;
                    }
                }
            }
        }

        int sisaMenunggu = 0;
        for (int i = 1; i <= nnode; i++)
            if (nodes[i].isActive && !nodes[i].isSafe) sisaMenunggu += antrian[i].top;
        cout << "    => selamat: " << selamat << "/" << totalWarga << "  sisa: " << sisaMenunggu << "\n";

        if (!adaPergerakan && selamat < totalWarga) {
            cout << "\n  [PERINGATAN] Evakuasi macet! Sisa warga terisolasi (rute buntu / kapasitas 0).\n";
            break;
        }
    }

    cout << "\n===========================================================\n";
    cout << "                    HASIL AKHIR SIMULASI                   \n";
    cout << "===========================================================\n";
    cout << "  Total Warga Terdata     : " << totalWarga << " orang\n";
    cout << "  Total Berhasil Evakuasi : " << selamat    << " orang\n";
    cout << "  Sisa Warga Tertinggal   : " << (totalWarga - selamat) << " orang\n";
    cout << "  Total Siklus Mobilisasi : " << tick << " kali pergerakan jalur\n";
    cout << "===========================================================\n\n";

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
