/*
   Simblee code
   Authors: Alex Flores and John Unger
*/

#include <SimbleeForMobile.h>

uint8_t text;
uint8_t ui_button;

unsigned long dely;
float temp;

#define BUTTON_PIN 5
#define RLED 2
#define GLED 3
#define BLED 4

typedef union {
  float floatVal;
  byte bytes[4];
} serialFloat;

bool refreshScreen = false;

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);

  dely = millis(); // Timestamp for delay

  // use a shared cache
  SimbleeForMobile.deviceName = "AJ XBee";
  SimbleeForMobile.advertisementData = "XBee system";
  SimbleeForMobile.domain = "AlexAndJohn.com";
  SimbleeForMobile.begin();
}

// include newlib printf float support (%f used in sprintf below)
asm(".global _printf_float");

void loop()
{
  // a delay is needed to filter significant changes
  // a Delay is going to result the rest to be delayed as well
  if (millis() - dely > 500)
  {
    int buttonPressed = digitalRead(BUTTON_PIN);
    if (buttonPressed)
    {
      digitalWrite(RLED, LOW);
      digitalWrite(GLED, LOW);
      digitalWrite(BLED, HIGH); // Blue light for signal snd/rcv
      Serial.write(1);

      // serialFloat that is being transferred
      serialFloat report;

      for (int i = 0; i < 4; i++)  report.bytes[i] = Serial.read();

      digitalWrite(BLED, LOW);

      temp = report.floatVal;

      // LED indicator of temp range
      if (temp <= 28.00) {
        digitalWrite(GLED, HIGH);
      } else if (temp > 28.00) {
        digitalWrite(RLED, HIGH);
      }
    }
    dely = millis();
  }

  // Change Value on the GUI
  if (refreshScreen)
  {
    // requires newlib printf float support
    char buf[16];
    sprintf(buf, "%.02f", temp);
    // update the text first with the actual temp
    SimbleeForMobile.updateText(text, buf);
    refreshScreen = false;
  }

  SimbleeForMobile.process();
}

void ui()
{
  SimbleeForMobile.beginScreen(WHITE);

  // height and width of the Mobile Screen
  int height = SimbleeForMobile.screenHeight;
  int width = SimbleeForMobile.screenWidth;

  SimbleeForMobile.drawText( (width / 2) - 70 , height / 4, "Press to Update Values");

  // I prefer the button style below, uncomment the line following for the original button style
  ui_button = SimbleeForMobile.drawButton((width / 2) - 40, height / 4 + 40, 80, "Button");
  //SimbleeForMobile.drawButton(120, 180, 80, "Button", BLUE, TEXT_TYPE);

  // we need a button (the default is a push button)
  SimbleeForMobile.setEvents(ui_button, EVENT_PRESS);

  // Numeric value of temp, keep object refrence
  text = SimbleeForMobile.drawText((width / 2) - 45, height / 2, "", RED, 45);

  //
  // \xb0 is the symbol for "degrees"
  // Android requires the unicode version, \xc2\xb0
  //
  if (SimbleeForMobile.remoteDeviceType == REMOTE_DEVICE_OS_IOS) {
    SimbleeForMobile.drawText(width - 50, (height / 2) + 8, "\xb0" "C", RED);
  } else if (SimbleeForMobile.remoteDeviceType == REMOTE_DEVICE_OS_ANDROID) {
    SimbleeForMobile.drawText(width - 50, (height / 2) + 8, "\xc2\xb0" "C", RED);
  } else if (SimbleeForMobile.remoteDeviceType == REMOTE_DEVICE_OS_UNKNOWN) {
    SimbleeForMobile.drawText(width - 50, (height / 2) + 8, "C", RED);
  }

  SimbleeForMobile.endScreen();
}

void ui_event(event_t &event)
{
  if ( event.id == ui_button && event.type == EVENT_PRESS )
  {
    // If button has been pressed you can change in the loop
    refreshScreen = true;
  }
}


