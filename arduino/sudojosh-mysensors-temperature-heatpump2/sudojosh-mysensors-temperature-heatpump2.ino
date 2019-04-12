/*
* Documentation: http://www.mysensors.org
* Support Forum: http://forum.mysensors.org
*/

#define MY_RADIO_NRF24
#define CHILD_ID_HVAC 10
#define MY_DEBUG
#define COMMAND_TYPE_REQUEST 2
#define COMMAND_TYPE_RECEIVE 1
#define LOOP_DELAY_MS 300000
#include <MySensors.h>
#include <IRremote2.h>

//Some global variables to hold the states
int POWER_STATE;
int TEMP_STATE;
int FAN_STATE;
int MODE_STATE;
int VDIR_STATE;
int HDIR_STATE;
String MODE_STATE_STRING;



IRsend irsend;


MyMessage msgHVACSetPointC(CHILD_ID_HVAC, V_HVAC_SETPOINT_COOL);
MyMessage msgHVACSetPointH(CHILD_ID_HVAC, V_HVAC_SETPOINT_HEAT);
MyMessage msgHVACSpeed(CHILD_ID_HVAC, V_HVAC_SPEED);
MyMessage msgHVACFlowState(CHILD_ID_HVAC, V_HVAC_FLOW_STATE);

bool initialValueSent = false;

void presentation() {
  sendSketchInfo("Heatpump", "2.2");
  present(CHILD_ID_HVAC, S_HVAC, "Thermostat");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!initialValueSent) {
    send(msgHVACSetPointC.set(21));
    send(msgHVACSpeed.set("auto"));
    send(msgHVACFlowState.set("off"));

    initialValueSent = true;
  }

  requestUpdatedStateAndWait();
  waitForStateToUpdate(COMMAND_TYPE_RECEIVE);

  sleep(LOOP_DELAY_MS);
}

void requestUpdatedStateAndWait() {
  request(CHILD_ID_HVAC, V_HVAC_SPEED);
  request(CHILD_ID_HVAC, V_HVAC_SETPOINT_COOL);
  request(CHILD_ID_HVAC, V_HVAC_SETPOINT_HEAT);
  request(CHILD_ID_HVAC, V_HVAC_FLOW_STATE);
  waitForStateToUpdate(COMMAND_TYPE_REQUEST);
}

void waitForStateToUpdate(int command) {
  wait(5000, command, V_HVAC_SPEED);
  wait(5000, command, V_HVAC_SETPOINT_COOL);
  wait(5000, command, V_HVAC_SETPOINT_HEAT);
  wait(5000, command, V_HVAC_FLOW_STATE);
}

void receive(const MyMessage &message) {
  if (message.isAck()) {
     Serial.println("This is an ack from gateway");
     return;
  }

  bool changed = false;

  Serial.print("Incoming message for: ");
  Serial.print(message.sensor);

  String recvData = message.data;
  recvData.trim();

  Serial.print(", New status: ");
  Serial.println(recvData);
  switch (message.type) {
    case V_HVAC_SPEED:
      Serial.println("V_HVAC_SPEED");

      if(recvData.equalsIgnoreCase("auto")) FAN_STATE = FAN_SPEED_AUTO;
      else if(recvData.equalsIgnoreCase("min")) FAN_STATE = FAN_SPEED_1;
      else if(recvData.equalsIgnoreCase("normal")) FAN_STATE = FAN_SPEED_3;
      else if(recvData.equalsIgnoreCase("max")) FAN_STATE = FAN_SPEED_5;
    break;

    case V_HVAC_SETPOINT_COOL:
      Serial.println("V_HVAC_SETPOINT_COOL");
      if (message.getFloat() != TEMP_STATE) { changed = true; }
      TEMP_STATE = message.getFloat();
      Serial.println(TEMP_STATE);
    break;
    case V_HVAC_SETPOINT_HEAT:
      Serial.println("V_HVAC_SETPOINT_HEAT");
      if (message.getFloat() != TEMP_STATE) { changed = true; }
      TEMP_STATE = message.getFloat();
      Serial.println(TEMP_STATE);
    break;
    case V_HVAC_FLOW_STATE:
      Serial.println("V_HVAC_FLOW_STATE");
      if (recvData != MODE_STATE_STRING) { changed = true; }
      
      if (recvData.equalsIgnoreCase("coolon")) {
        POWER_STATE = NULL;
        MODE_STATE = HVAC_COLD;
        MODE_STATE_STRING = recvData;
      }
      else if (recvData.equalsIgnoreCase("heaton")) {
        POWER_STATE = NULL;
        MODE_STATE = HVAC_HOT;
        MODE_STATE_STRING = recvData;
      }
      else if (recvData.equalsIgnoreCase("autochangeover")) {
        POWER_STATE = NULL;
        MODE_STATE = HVAC_HOT;
        MODE_STATE_STRING = recvData;
      }
      else if (recvData.equalsIgnoreCase("off")){
        POWER_STATE = 1;
        MODE_STATE_STRING = recvData;
      }
      break;
  }
  if (changed) { sendHeatpumpCommand(); }
  sendNewStateToGateway();
}

void sendNewStateToGateway() {
  send(msgHVACSetPointC.set(TEMP_STATE));
  send(msgHVACSetPointH.set(TEMP_STATE));
  send(msgHVACSpeed.set(FAN_STATE));
  send(msgHVACFlowState.set(MODE_STATE));
}

void sendHeatpumpCommand() {
  Serial.println("Power = " + (String)POWER_STATE);
  Serial.println("Mode = " + (String)MODE_STATE);
  Serial.println("Fan = " + (String)FAN_STATE);
  Serial.println("Temp = " + (String)TEMP_STATE);

 irsend.sendHvacMitsubishi(MODE_STATE, TEMP_STATE, FAN_STATE, VANNE_AUTO_MOVE, POWER_STATE);
}

