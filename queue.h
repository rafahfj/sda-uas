#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include "models.h"

using namespace std;

// Pembagian kerja: Anggota 1
// Fokus: deklarasi operasi queue warga.
void createQueue(int nodeIdx);
int isEmpty(int nodeIdx);
void insertQueue(int nodeIdx, Person p);
void deleteQueue(int nodeIdx);
void findQueue(int nodeIdx, string cari);

#endif
