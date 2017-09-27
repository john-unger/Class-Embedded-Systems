#include <DHT.h>

// Digital pins
#define LEDPIN 13
#define DHTPIN 12

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

typedef union {
  float floatVal;
  byte bytes[4];
} serialFloat;


void blinkLED(unsigned long durationOn, unsigned long durationOff, unsigned int numBlinks) {
  digitalWrite(LEDPIN, LOW);
  for(int i = 0; i < numBlinks; i++) {
    digitalWrite(LEDPIN, HIGH);
    delay(durationOn);
    digitalWrite(LEDPIN, LOW);
    
    // Only apply the delay if this is not the last blink
    if (i < numBlinks - 1) delay(durationOff);
  }
}

void reportTemp()
{
  serialFloat temp;
  digitalWrite(LEDPIN, HIGH);
  temp.floatVal = dht.readTemperature();
  if(isnan(temp.floatVal)) {
    blinkLED(50, 50, 5);
  } else {
    delay(100);
    digitalWrite(LEDPIN, LOW);
    for(int i = 0; i < 4; i++) Serial.write(temp.bytes[i]);
    //Serial.write(temp.bytes, 4);
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  delay(2000);

}

void loop() {
  int byteRead = Serial.read();
  if (byteRead == 1) reportTemp();
}
