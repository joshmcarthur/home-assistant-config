const int triggerPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  // Clear the trigger pin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  // Sets the trigger pin on high for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Reads the echo pin to get the duration in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Distance - must half duration due to the time out from object + returning from object
  distance = duration * 0.034 / 2;

  Serial.println(distance);

  delay(1000);
}
