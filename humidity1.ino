// Displays humidity using a bar graph/increment style (like the battery charge bars), displayed on the 2510sr-1

#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// 10 segment bar graph pins (10% -> 100%)
const int barPins[10] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

// Timing vars (non-blocking so doesn't flicker)
unsigned long lastReadTime = 0;
const unsigned long readInterval = 2000;

void setup() {
  dht.begin();

  // Initialize bar pins
  for (int i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
    digitalWrite(barPins[i], LOW);
  }
}

void loop() {
  if (millis() - lastReadTime >= readInterval) {
    lastReadTime = millis();
    float humidity = dht.readHumidity();

    if (!isnan(humidity)) {
      int barsToLight = map(constrain((int)humidity, 0, 100), 0, 100, 0, 10);
      for (int i = 0; i < 10; i++) {
        if (i < barsToLight) {
          digitalWrite(barPins[i], HIGH);
        } else {
          digitalWrite(barPins[i], LOW);
        }
      }
    }
  }
}
