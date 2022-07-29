#include "configuration.h"
#include "variables.h"

void setup() {
  //-INITIALIZE SERIAL-//
  Serial.begin(9600);

  //-INITIALIZE GPIOs-//
  pinConfiguration();
  ESP_status(1);      //switch ESP32 on

  //-INITIALIZE I2C-//
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event

  //-INITIALIZE CAN-//
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  Serial.println("------- CONF_CAN_Smart_home_sniffer.io started ----------");
#ifdef debugCAN
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
#endif
}

void loop() {

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
  #ifdef debugCAN
      Serial.print(canMsg.can_id, HEX); // print ID
      Serial.print(" ");
      Serial.print(canMsg.can_dlc, HEX); // print DLC
      Serial.print(" ");
      for (int i = 0; i < canMsg.can_dlc; i++)  { // print the data
        Serial.print(canMsg.data[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
  #endif

    if (canMsg.can_id == 0x236) {
      //Doorstatus
      DOOR_FL = bitRead(canMsg.data[5], 6);
      DOOR_FR = bitRead(canMsg.data[7], 1);
      DOOR_RL = bitRead(canMsg.data[7], 4);
      DOOR_RR = bitRead(canMsg.data[7], 5);
      DOOR_T = bitRead(canMsg.data[5], 7);
    }
    else if (canMsg.can_id == 0x630) {
      //lockstatus
      lock = bitRead(canMsg.data[2], 6);
    }
    else if (canMsg.can_id == 0x128) {
      //parking brake
      parkingbrake = bitRead(canMsg.data[3], 1);
      //lights
      left_indicator = bitRead(canMsg.data[0], 1);
      right_indicator = bitRead(canMsg.data[0], 2);
      rear_fog_light = bitRead(canMsg.data[0], 3);
      front_fog_light = bitRead(canMsg.data[0], 4);
      high_beam = bitRead(canMsg.data[0], 5);
      low_beam = bitRead(canMsg.data[0], 6);
      sidelights = bitRead(canMsg.data[0], 7);
    }

  }
  if (debug_can_messages) {
    Serial.print("Driver:");
    Serial.print(DOOR_FL);
    Serial.print(", Codriver:");
    Serial.print(DOOR_FR);
    Serial.print(", PAX left:");
    Serial.print(DOOR_RL);
    Serial.print(", PAX right:");
    Serial.print(DOOR_RR);
    Serial.print(", Trunk:");
    Serial.print(DOOR_T);
    Serial.print(", Brake:");
    Serial.print(parkingbrake);
    Serial.print(", lock:");
    Serial.println(lock);
  }
}

void requestEvent() {
  int msg = 0;
  bitWrite(msg, 0, DOOR_FL);    //https://www.arduino.cc/reference/de/language/functions/bits-and-bytes/bitwrite/
  bitWrite(msg, 1, DOOR_FR);
  bitWrite(msg, 2, DOOR_RL);
  bitWrite(msg, 3, DOOR_RR);
  bitWrite(msg, 4, DOOR_T);
  bitWrite(msg, 5, parkingbrake);
  bitWrite(msg, 6, lock);
  bitWrite(msg, 7, 0);
  Wire.write(msg); // respond with message of 1 bytes
}

void ESP_status(bool ESP_status) {
  digitalWrite(ESP_EN, !ESP_status);
  Serial.print("ESP status: ");
  Serial.println(ESP_status);
}


// ---------- Configuration ---------- //
void pinConfiguration() {
  pinMode(ESP_EN, OUTPUT);
  pinMode(RV_MODE_PIN, INPUT_PULLUP);
  Serial.println("Pinconfiguration complete! :-)");
}
void checkSwitch() {
  RV_MODE = !digitalRead(RV_MODE_PIN);
}
