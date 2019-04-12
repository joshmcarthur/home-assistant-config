// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// MYSENSORS LIBRARIES 
// Enable debug prints
#define MY_DEBUG
#define MY_RADIO_NRF24
#include <SPI.h>
#include <MySensors.h>  
// END MYSENSORS LIBRARIES

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Define settings for the data readout
#define I2C_ADDR    0x3F // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define LIGHTING_RELAY_PIN  3
#define LIGHTING_ON true
#define LIGHTING_OFF false

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature dallasTemp(&oneWire);

// Pass pin layout to I2C
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

// Set up sleep interval
// Sleep time between sensor updates (in milliseconds)
static const uint64_t UPDATE_INTERVAL = 10000; // every 10 seconds

// Send the initial relay status once
bool initialRelayStateSent = false;

// Define sensor IDs
#define CHILD_ID_INTAKE_TEMP 0
#define CHILD_ID_OUTLET_TEMP 10
#define CHILD_ID_LIGHT_STATUS 20

#define ONEWIRE_INTAKE_INDEX 0
#define ONEWIRE_OUTLET_INDEX 1

// Define MySensor message types
MyMessage msgIntakeTemp(CHILD_ID_INTAKE_TEMP, V_TEMP);
MyMessage msgOutletTemp(CHILD_ID_OUTLET_TEMP, V_TEMP);
MyMessage msgLightingStatus(CHILD_ID_LIGHT_STATUS, V_STATUS);

/*
 * The presentation function 'presents' all of the sensors to the 
 * gateway
 */
void presentation()  
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("Hot Tub", "1.0");
  
  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_INTAKE_TEMP, S_TEMP);
  present(CHILD_ID_OUTLET_TEMP, S_TEMP);
  present(CHILD_ID_LIGHT_STATUS, S_BINARY);
}

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
  Serial.begin(115200);
  Serial.println("Starting");

  lcd.begin (16,2); 
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home
  lcd.print("Updating...");

  pinMode(LIGHTING_RELAY_PIN, OUTPUT);

  // Start up the library
  dallasTemp.begin();
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
   // Send initial relay state
  if (!initialRelayStateSent) {
    send(msgLightingStatus.set(LIGHTING_OFF));
    request(CHILD_ID_LIGHT_STATUS, V_STATUS);
    wait(2000, C_SET, V_STATUS);
  }
  
  dallasTemp.requestTemperatures(); // Send the command to get temperatures
  processTemperatures(dallasTemp);
  
  // Sleep for a while to save energy
  wait(UPDATE_INTERVAL); 
}


void receive(const MyMessage &message) {
   // We only expect one type of message from controller. 
  if (message.type != V_STATUS) { return; }
  if (!initialRelayStateSent) {
    Serial.println("Received initial value from controller for lighting relay.");
    initialRelayStateSent = true;
  }
  
  // Change relay state
  digitalWrite(LIGHTING_RELAY_PIN, message.getBool()?LIGHTING_ON:LIGHTING_OFF);
   
  // Write some debug info
  Serial.print("Incoming change for sensor:");
  Serial.print(message.sensor);
  Serial.print(", New status: ");
  Serial.println(message.getBool());
}

void processTemperatures(DallasTemperature adapter) {
  float tempAtIntake = adapter.getTempCByIndex(ONEWIRE_INTAKE_INDEX);
  float tempAtOutlet = adapter.getTempCByIndex(ONEWIRE_OUTLET_INDEX);
  transmitTemperatures(tempAtIntake, tempAtOutlet);
  displayTemperatures(tempAtIntake, tempAtOutlet);
}

void transmitTemperatures(float intake, float outlet) {
  msgIntakeTemp.set(intake, 2);
  msgOutletTemp.set(outlet, 2);
  send(msgIntakeTemp);
  send(msgOutletTemp);
}

void displayTemperatures(float intake, float outlet) {
  lcd.clear();
  displayLine("Intake: ", intake);
  lcd.setCursor(0, 1);
  displayLine("Outlet: ", outlet);
}

void displayLine(String label, float temp) {
  Serial.println(label + temp);
  lcd.print(label);
  lcd.print(temp);
  lcd.print((char)223);
}

