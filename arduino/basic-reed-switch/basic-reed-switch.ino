const int REED_PIN = 4; // Pin connected to reed switch
const int LED_PIN = 13; // LED pin - active-high

void setup() 
{
  Serial.begin(9600);
  // Since the other end of the reed switch is connected to ground, we need
  // to pull-up the reed switch pin internally.
  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

void loop() 
{
  int proximity = digitalRead(REED_PIN); // Read the state of the switch
  if (proximity == LOW) // If the pin reads low, the switch is closed.
  {
    Serial.println("Switch closed");
    digitalWrite(LED_PIN, HIGH); // Turn the LED on
  }
  else
  {
    Serial.println("Switch open");
    digitalWrite(LED_PIN, LOW); // Turn the LED off
  }
}
