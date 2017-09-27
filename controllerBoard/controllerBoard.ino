#define BUTTON_PIN 13
#define RLED 12
#define GLED 11

typedef union {
  float floatVal;
  byte bytes[4];
} serialFloat;

bool hasPushedButton = false;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
}

void loop() {
  int buttonPressed = !digitalRead(BUTTON_PIN);
  if (buttonPressed && !hasPushedButton) {
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);
    Serial.write(1);
    hasPushedButton = true;
  }

  serialFloat report;
  for(int i = 0; i < 4; i++)
  {
    report.bytes[i] = Serial.read();
  }
  
  if (report.floatVal <= 28.00) {
    digitalWrite(GLED, HIGH);
  } else if (report.floatVal > 28.00) {
    digitalWrite(RLED, HIGH);
  }

  delay(500);
  hasPushedButton = false;
}
