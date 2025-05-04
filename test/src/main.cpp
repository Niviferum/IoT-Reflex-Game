#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Broches selon ton câblage
const int ledPins[3] = {23, 22, 21};
const int buttonPins[3] = {19, 18, 5};

const int MAX_SUCCES = 5;  // nombre de bonnes réponses pour terminer le jeu
const unsigned long TIMEOUT = 2000;  // en ms

// Variables de jeu
int succesCount = 0;
unsigned long totalReactionTime = 0;

void afficherMessage(const String& ligne1, const String& ligne2 = "") {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(ligne1);
  if (ligne2 != "") {
    display.println(ligne2);
  }
  display.display();
}

void setup() {
  Serial.begin(115200);

  Wire.begin(2, 1); // SDA=2, SCL=1 pour ton OLED

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erreur écran OLED");
    while (true);
  }

  afficherMessage("Jeu de rythme", "Appuie vite !");
  delay(2000);

  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // Réinitialiser les compteurs pour chaque partie
  succesCount = 0;
  totalReactionTime = 0;

  while (succesCount < MAX_SUCCES) {
    afficherMessage("Prépare-toi...");
    delay(random(1000, 3000));

    int activeLed = random(0, 3);
    digitalWrite(ledPins[activeLed], HIGH);
    unsigned long startTime = millis();

    bool boutonPresse = false;
    int boutonAppuye = -1;

    while (millis() - startTime < TIMEOUT) {
      for (int i = 0; i < 3; i++) {
        if (digitalRead(buttonPins[i]) == LOW) {
          boutonPresse = true;
          boutonAppuye = i;
          break;
        }
      }
      if (boutonPresse) break;
    }

    digitalWrite(ledPins[activeLed], LOW);

    if (!boutonPresse) {
      afficherMessage("Trop lent !");
      delay(1500);
      continue;  // ne compte pas cette manche
    }

    if (boutonAppuye == activeLed) {
      unsigned long reactionTime = millis() - startTime;
      totalReactionTime += reactionTime;
      succesCount++;

      afficherMessage("Bien joué !", "Temps: " + String(reactionTime) + " ms");
    } else {
      afficherMessage("Mauvais bouton !");
    }

    delay(2000);
  }

  // Affichage du score à la fin
  unsigned long averageTime = totalReactionTime / MAX_SUCCES;
  afficherMessage("Fin du jeu !", "Moyenne: " + String(averageTime) + " ms");
  delay(5000);

  // Option : recommencer automatiquement
  afficherMessage("Rejouer dans 3s...");
  delay(3000);
}
