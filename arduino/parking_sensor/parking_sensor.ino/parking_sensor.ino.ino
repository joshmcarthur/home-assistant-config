#include <NewPing.h>

#define SONAR_TRIG_PIN 8
#define SONAR_ECHO_PIN 9
#define ALERT_LED_PIN 12
#define OK_LED_PIN 13
#define MAX_DETECT_DISTANCE 200 //cm
#define STATE_NONE 0
#define STATE_ALERT 1
#define STATE_OK 2
#define STATE_OK_MAX_DISTANCE 75
#define STATE_OK_MIN_DISTANCE 50
#define READ_FREQUENCY 250
#define MAX_IDENTICAL_READINGS 100 // 250ms @ 4/s * 4 - if reading is the same for 16 seconds, poweroff.

int lastDistanceRead;
int identicalLastDistanceCount = 0;

NewPing sonar(SONAR_TRIG_PIN, SONAR_ECHO_PIN, MAX_DETECT_DISTANCE);


// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(OK_LED_PIN, OUTPUT);
  pinMode(ALERT_LED_PIN, OUTPUT);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  delay(READ_FREQUENCY);
  
  int distance = sonar.ping_cm();
  Serial.println(distance);
  if (distance == lastDistanceRead) { 
    identicalLastDistanceCount++;
    if (identicalLastDistanceCount >= MAX_IDENTICAL_READINGS) { setLedState(STATE_NONE); }
    return;
  } 
  
  lastDistanceRead = distance;
  identicalLastDistanceCount = 0;
  
  if (distance <= STATE_OK_MAX_DISTANCE && distance >= STATE_OK_MIN_DISTANCE) {
    setLedState(STATE_OK);
  } 
  else {
    setLedState(STATE_ALERT);
  }
}

void setLedState(int state) {
  switch (state) {
    case (STATE_NONE):
      Serial.println("STATE NONE");
      digitalWrite(ALERT_LED_PIN, LOW);
      digitalWrite(OK_LED_PIN, LOW);
      break;
    case (STATE_ALERT):
      Serial.println("STATE ALERT");
      digitalWrite(ALERT_LED_PIN, HIGH);
      digitalWrite(OK_LED_PIN, LOW);
      break;
    case (STATE_OK):
      Serial.println("STATE OK");
      digitalWrite(ALERT_LED_PIN, LOW);
      digitalWrite(OK_LED_PIN, HIGH);
      break;
  }
}

