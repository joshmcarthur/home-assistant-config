int soilHumidityPin = A0;
int thresholdValue = 800;

void setup() {
  pinMode(soilHumidityPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(soilHumidityPin);
  Serial.println(sensorValue);

  delay(500);
}
