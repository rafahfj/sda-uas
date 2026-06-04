#include "globals.h"

AntrianWarga antrian[MAX_LOKASI + 1]; // Array objek antrean untuk tiap node lokasi
Node nodes[MAX_LOKASI + 1];
int nnode = 0; // Total lokasi aktif terpantau komputer
int nextPersonId = 1;
int w[MAX_LOKASI + 1][MAX_LOKASI + 1]; // Matriks Waktu Tempuh Jalur
int kapasitasJalur[MAX_LOKASI + 1][MAX_LOKASI + 1]; // Menyimpan kapasitas aliran orang per tick
