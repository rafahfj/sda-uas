#ifndef GRAPH_H
#define GRAPH_H

// Pembagian kerja: Anggota 3
// Fokus: deklarasi graph, pencarian node, dan Dijkstra.
void inisialisasiGraph();
int findNodeIndex(int id);
int hitungDijkstraManual(int startNode, int targetNode, int ruteHasil[], int &panjangRute);

#endif
