#include <iostream>
#include "graph.h"
#include "menu.h"
#include "randomization.h"
#include "simulation.h"
#include "utils.h"

using namespace std;

int main() {
    inisialisasiGraph();
    clearScreen();

    int choice;
    do {
        showMenu();
        if (!bacaInt(choice)) { backToMenu(); continue; }
        clearScreen();
        switch (choice) {       
            case 1: menuLokasi();              break;
            case 2: menuJalur();               break;
            case 3: menuWarga();               break;
            case 4: runSimulation(); backToMenu(); break;
            case 5: caraMenggunakanSimulasi(); backToMenu(); break;
            case 6: randomisasi(); backToMenu(); break;
            case 0: cout << "\n  Program selesai. Terima kasih dan tetap siaga bencana!\n\n"; break;
            default:
                cout << "  Pilihan tidak valid.\n";
                backToMenu();
        }
    } while (choice != 0);
    return 0;
}
