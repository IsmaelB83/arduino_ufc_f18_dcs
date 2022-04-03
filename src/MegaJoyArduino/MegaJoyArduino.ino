
#include "MegaJoy.h"
#include <Encoder.h>

// Joystick
megaJoyControllerData_t controllerData;

// Encoders
int comm1_CLK = 20;
int comm1_DT = 21;
Encoder comm1(comm1_DT, comm1_CLK);
int comm2_CLK = 19;
int comm2_DT = 18;
Encoder comm2(comm2_DT, comm2_CLK);

// Setup arduino
void setup() {
  // All input pins with pull-up except first two lines used to transmit
  for (int i = 2; i <= 36; i++) {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }  
  // Setup mega joy
  setupMegaJoy();
  // Log
  Serial.println("Listo...");
}

// Loop
void loop() {
  // Always be getting fresh data
  controllerData = getControllerData();
  setControllerData(controllerData);
}

// Get controller data
megaJoyControllerData_t getControllerData(void) {

  // Clear datta
  controllerData = getBlankDataForMegaController();

  // Analogs inputs
  controllerData.analogAxisArray[2] = analogRead(A0);
  controllerData.analogAxisArray[3] = analogRead(A1);
  controllerData.analogAxisArray[4] = analogRead(A2);
  // Digital inputs
  for (int i = 2; i < 18; i++) {
    setButtonState(i-2, !digitalRead(i));
  }
  for (int i = 22; i < 36; i++) {
    setButtonState(i-2, !digitalRead(i));
  }
  // Encoders
  boolean auxCLK = digitalRead(comm1_CLK);
  boolean auxDT = digitalRead(comm1_DT);
  if (!auxCLK && auxDT) setButtonState(20-2, HIGH); 
  if (auxCLK && !auxDT) setButtonState(21-2, HIGH); 
  // Encoders
  auxCLK = digitalRead(comm2_CLK);
  auxDT = digitalRead(comm2_DT);
  if (!auxCLK && auxDT) setButtonState(18-2, HIGH); 
  if (auxCLK && !auxDT) setButtonState(19-2, HIGH); 
  // Return the data
  return controllerData;
}

// Set button state
void setButtonState(unsigned int index, boolean value) {
    controllerData.buttonArray[index/8] |= value << (index%8);
}
