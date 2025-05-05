#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Broches selon ton câblage
const int ledPins[3] = {13, 14, 26};
const int buttonPins[3] = {12, 27, 25};

const int MAX_SUCCES = 5;
const unsigned long TIMEOUT = 4000;

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
  Wire.begin(2, 1); // SDA=2, SCL=1

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error OLED screen");
    while (true);
  }

  afficherMessage("Reflex Game", "Be quick !");
  delay(4000);

  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void jouerUnePartie() {
  succesCount = 0;
  totalReactionTime = 0;

  while (succesCount < MAX_SUCCES) {
    afficherMessage("Wait...");
    delay(random(1000, 5000));

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
      afficherMessage("Too slow !");
      delay(1500);
      continue;
    }

    if (boutonAppuye == activeLed) {
      unsigned long reactionTime = millis() - startTime;
      totalReactionTime += reactionTime;
      succesCount++;

      afficherMessage("Well Played !", "Time: " + String(reactionTime) + " ms");
    } else {
      afficherMessage("Wrong button !");
    }

    delay(2000);
  }

  // Score final
  unsigned long averageTime = totalReactionTime / MAX_SUCCES;
  afficherMessage("END !", "average: " + String(averageTime) + " ms");
  delay(4000);
}

void attendreRejouer() {
  afficherMessage("press 3x", "the same button");

  int dernierBouton = -1;
  int compte = 0;
  unsigned long t0 = millis();
  const unsigned long TIMEOUT_REJOUER = 10000; // 10 secondes

  while (millis() - t0 < TIMEOUT_REJOUER) {
    for (int i = 0; i < 3; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        delay(30); // debounce

        while (digitalRead(buttonPins[i]) == LOW); // attendre relâchement

        if (i == dernierBouton) {
          compte++;
        } else {
          compte = 1;
          dernierBouton = i;
        }

        afficherMessage("Press...", String(compte) + " / 3");

        if (compte >= 3) {
          afficherMessage("Again !");
          delay(1000);
          return;
        }

        t0 = millis(); // reset le timer à chaque interaction
        delay(300);
      }
    }
  }

  // Timeout dépassé → mise en "veille"
  afficherMessage("Inactive", "Extinction...");
  delay(2000);

  display.ssd1306_command(SSD1306_DISPLAYOFF); // éteindre l’écran OLED

  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[i], LOW); // éteindre toutes les LEDs
  }

  esp_sleep_enable_timer_wakeup(0); // désactive tout réveil
  esp_deep_sleep_start();           // met l'ESP32 en sommeil profond
}


void loop() {
  jouerUnePartie();
  attendreRejouer(); // Boucle tant que le joueur ne confirme pas
}
