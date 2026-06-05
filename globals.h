#ifndef GLOBALS_H
#define GLOBALS_H

#include "models.h"

// Pembagian kerja: Anggota 2
// Fokus: deklarasi state global yang dipakai lintas modul.
extern AntrianWarga antrian[MAX_LOKASI + 1];
extern Node nodes[MAX_LOKASI + 1];
extern int nnode;
extern int nextPersonId;
extern int w[MAX_LOKASI + 1][MAX_LOKASI + 1];
extern int kapasitasJalur[MAX_LOKASI + 1][MAX_LOKASI + 1];

#endif
