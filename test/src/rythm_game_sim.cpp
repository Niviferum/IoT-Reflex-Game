/* 
  This code simulates the rhythm game in a console environment. 
  It's mean to use to test the logic of the game without needing hardware.

  The game will prompt the user to press a button corresponding to an LED that lights up, 
  and it will measure the reaction time. After a set number of successful presses, 
  it will display the average reaction time and ask if the user wants to play again.
  
  The game uses random delays and LED activation to simulate the original game behavior. 
  The user input is taken from the console, and the program will wait for the user to press the correct button 
  within a timeout period. 
  If the user presses the wrong button or takes too long, it will notify them accordingly.
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace chrono;

const int MAX_SUCCES = 5;
const int TIMEOUT_MS = 4000;

void afficher(const string& ligne1, const string& ligne2 = "") {
    cout << "=== " << ligne1 << endl;
    if (!ligne2.empty()) cout << ">>> " << ligne2 << endl;
    cout << endl;
}

int demanderBouton(int timeout_ms, int ledActive) {
    auto start = steady_clock::now();

    afficher("LED #" + to_string(ledActive), "Tape le bon chiffre (0/1/2)");

    while (duration_cast<milliseconds>(steady_clock::now() - start).count() < timeout_ms) {
        if (cin.rdbuf()->in_avail() > 0) {
            int val;
            cin >> val;
            return val;
        }
        this_thread::sleep_for(milliseconds(10));
    }

    return -1; // timeout
}

bool demanderRejouer() {
    afficher("Veux-tu rejouer ?", "Tape 3 fois le même chiffre");
    int dernier = -1, compte = 0;
    auto t0 = steady_clock::now();

    while (duration_cast<seconds>(steady_clock::now() - t0).count() < 20) {
        if (cin.rdbuf()->in_avail() > 0) {
            int val;
            cin >> val;
            if (val == dernier) {
                compte++;
            } else {
                compte = 1;
                dernier = val;
            }
            afficher("Confirmation", to_string(compte) + " / 3");
            if (compte >= 3) return true;
            t0 = steady_clock::now();
        }
        this_thread::sleep_for(milliseconds(10));
    }

    afficher("Inactif", "Fin du programme.");
    return false;
}

void jouerUnePartie() {
    int succes = 0;
    long totalReaction = 0;

    while (succes < MAX_SUCCES) {
        afficher("Prépare-toi...");
        this_thread::sleep_for(milliseconds(1000 + rand() % 2000));

        int led = rand() % 3;
        auto start = steady_clock::now();
        int appui = demanderBouton(TIMEOUT_MS, led);

        if (appui == -1) {
            afficher("Trop lent !");
            continue;
        }

        if (appui == led) {
            int temps = duration_cast<milliseconds>(steady_clock::now() - start).count();
            totalReaction += temps;
            succes++;
            afficher("Bien joué !", "Temps: " + to_string(temps) + " ms");
        } else {
            afficher("Mauvais bouton !");
        }

        this_thread::sleep_for(milliseconds(1000));
    }

    long moyenne = totalReaction / MAX_SUCCES;
    afficher("Fin !", "Moyenne: " + to_string(moyenne) + " ms");
}

int main() {
    srand(time(0));
    afficher("Jeu de rythme console", "Sois rapide !");
    while (true) {
        jouerUnePartie();
        if (!demanderRejouer()) break;
    }

    afficher("Fin du jeu !");
    return 0;
}
