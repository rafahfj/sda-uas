#include "globals.h"

// Pembagian kerja: Anggota 2
// Fokus: definisi state global aplikasi.
AntrianWarga antrian[MAX_LOKASI + 1]; 
Node nodes[MAX_LOKASI + 1];
int nnode = 0; 
int nextPersonId = 1;
int w[MAX_LOKASI + 1][MAX_LOKASI + 1];
int kapasitasJalur[MAX_LOKASI + 1][MAX_LOKASI + 1];
