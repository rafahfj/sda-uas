#include <iostream>
#include "globals.h"
#include "graph.h"
#include "queue.h"
#include "simulation.h"

using namespace std;

// Pembagian kerja: Anggota 4
// Fokus: alur simulasi evakuasi, pemilihan rute Dijkstra, dan laporan hasil.
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
            cout << "\n\n===========================================================\n\n";
        }
    }

    cout << "Persiapan analisis selesai. Tekan ENTER untuk memulai pergerakan simulasi...\n";
    cin.get(); 

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

    cout << "\n\n===========================================================\n";
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
    cout << "\n===========================================================\n";
    cout << "            CARA MENGGUNAKAN SIMULASI (ALUR UTAMA)           \n";
    cout << "===========================================================\n\n";

    cout << " [Tahap 1] Membuat Lokasi [Menu 1]\n";
    cout << "-----------------------------------\n";
    cout << "1. Pilih '1. Tambah Lokasi Baru'\n";
    cout << "2. Masukkan nama lokasi\n";
    cout << "3. Pilih status lokasi\n";
    cout << "   - 1 = Posko Aman (Untuk tujuan akhir evakuasi\n";
    cout << "   - 0 = Desa Bahaya (Untuk tujuan awal evakuasi\n";
    cout << "4. Wajib minimal memiliki 1 posko aman dan 1 desa zona bahaya\n\n";

    cout << " [Tahap 2] Membuat Jalur Evakuasi [Menu 2]\n";
    cout << "-------------------------------------------\n";
    cout << "1. Pilih '1. Buat Jalur Baru'\n";
    cout << "2. Masukkan ID Lokasi Asal\n";
    cout << "3. Masukkan ID Lokasi Tujuan\n";
    cout << "4. Masukkan Kapasitas Jalur (orang per-menit)\n";
    cout << "5. Masukkan Waktu Tempuh (dalam menit)\n";
    cout << "6. Proses berulang jika ingin membuat jalur yang terhubung\n\n";

    cout << " [Tahap 3] Menambah Warga [Menu 3]\n";
    cout << "-------------------------------------------\n";
    cout << "1. Pilih '1. Daftarkan warga ke lokasi'\n";
    cout << "2. Pilih ID Lokasi Bahaya\n";
    cout << "3. Masukkan jumlah warga yang ingin dievakuasi\n\n";

    cout << " [TAHAP 4] MENJALANKAN SIMULASI (Menu 4)\n";
    cout << " --------------------------------------\n";
    cout << "1. Pilih '4. Jalankan Simulasi Evakuasi'\n";
    cout << "2. Sistem akan menganalisis rute tercepat menggunakan Dijkstra\n";
    cout << "3. Tekan ENTER untuk memulai pergerakan warga\n";
    cout << "4. Simulasi berjalan otomatis hingga:\n";
    cout << "   - Semua warga selamat, ATAU\n";
    cout << "   - Waktu maksimal 500 menit tercapai, ATAU\n";
    cout << "   - Evakuasi macet karena jalur buntu\n\n";

    cout << " [TAHAP 5] MEMAHAMI HASIL SIMULASI\n";
    cout << " ---------------------------------\n";
    cout << "1. Setiap siklus menampilkan pergerakan warga\n";
    cout << "2. Rekomendasi Dijkstra ditandai dengan [REKOMENDASI DIJKSTRA]\n";
    cout << "3. Sistem akan menampilkan:\n";
    cout << "   - Total warga yang berhasil selamat\n";
    cout << "   - Warga yang masih tertinggal\n";
    cout << "   - Jumlah siklus pergerakan\n\n";

    cout << " [Tahap 6] Randomisasi Warga dan Jalur [Menu 6 - Opsional]\n";
    cout << "-------------------------------------------\n";
    cout << "1. Pilih '6 Randomisasi Warga dan Jalur'\n";
    cout << "2. Masukkan total warga yang ingin disebar acak\n";
    cout << "3. Masukkan densitas jalur (1-100, semakin besar makin padat)\n";
    cout << "4. Sistem akan membuat jalur dan menyebar warga secara otomatis\n";

    cout << "===========================================================\n\n";
}
