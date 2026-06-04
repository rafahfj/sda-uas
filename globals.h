#ifndef GLOBALS_H
#define GLOBALS_H

#include "models.h"

extern AntrianWarga antrian[MAX_LOKASI + 1];
extern Node nodes[MAX_LOKASI + 1];
extern int nnode;
extern int nextPersonId;
extern int w[MAX_LOKASI + 1][MAX_LOKASI + 1];
extern int kapasitasJalur[MAX_LOKASI + 1][MAX_LOKASI + 1];

#endif
