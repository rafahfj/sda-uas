# sda-uas

Program simulasi evakuasi bencana Sesar Lembang berbasis Queue, Graph, dan Dijkstra.

## Struktur Modul

- `draftIndra_fixed2_1.cpp`: entry point / `main`
- `constants.h`, `models.h`, `globals.*`: konstanta, struktur data, dan state global
- `utils.*`: input aman dan layar
- `queue.*`: operasi antrean warga
- `graph.*`: inisialisasi graph, pencarian node, dan Dijkstra
- `display.*`: tampilan data lokasi, posko, dan jalur
- `location.*`, `edge.*`, `person.*`: CRUD lokasi, jalur, dan warga
- `simulation.*`: proses simulasi evakuasi
- `randomization.*`: randomisasi warga dan jalur
- `menu.*`: menu dan sub-menu aplikasi

## Compile

```bash
g++ -std=c++17 *.cpp -o sda-uas
```
