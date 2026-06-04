#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include "models.h"

using namespace std;

void createQueue(int nodeIdx);
int isEmpty(int nodeIdx);
void insertQueue(int nodeIdx, Person p);
void deleteQueue(int nodeIdx);
void findQueue(int nodeIdx, string cari);

#endif
