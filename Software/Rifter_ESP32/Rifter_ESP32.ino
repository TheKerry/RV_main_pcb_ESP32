#include "variables.h"
#include "configuration.h"


IPAddress local_IP(192, 168, 4, 1);
//IPAddress gateway(192, 168, 4, 9);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);


void setup()
{
  Serial.begin(115200);
  Wire.begin();
  startAP();
  pinConfiguration();
  checkSwitch();
}

void loop() {
  //Serial.print("[Server Connected] ");
  //Serial.println(WiFi.softAPIP());
  ////Serial.printf("Anzahl der Verbundenen Geräte= %d\n", WiFi.softAPgetStationNum());
  //delay(2000);
  delay(100);

  updateCAN();
  printDebugMsg();

  checkSwitch();
  digitalWrite(Kofferraumbeleuchtung_unten, DOOR_T);
  //

}
// --- END MAIN --- //







// ---------- Configuration ---------- //
void pinConfiguration() {
  pinMode(Kofferraumbeleuchtung_unten, OUTPUT);
  pinMode(Kofferraumklappe, OUTPUT);
  pinMode(Zigarettenanzuender_1, OUTPUT);
  pinMode(Zigarettenanzuender_2, OUTPUT);
  pinMode(SW_I_pin, INPUT_PULLUP);
  pinMode(SW_II_pin, INPUT_PULLUP);
  Serial.print("Pinconfiguration complete!");
}
void checkSwitch() {
  SW_I = !digitalRead(SW_I_pin);
  SW_II = !digitalRead(SW_II_pin);
}
void printDebugMsg() {
#ifdef debugCAN
  PrintCANVehicleStatus();
#endif
}
// ---------- Configuration ---------- //

// ---------- WLAN AP ---------- //
void startAP() {
  Serial.println();
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, passphrase) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  if (!MDNS.begin("esp32")) {
    Serial.println("Error starting mDNS");
  }

}
// ---------- WLAN AP ---------- //




// ---------- CAN ---------- //
void updateCAN() {
  Wire.requestFrom(8, 1);     // request 6 bytes from peripheral device #8
  while (Wire.available()) {  // peripheral may send less than requested
    int msg = Wire.read();     // receive a byte as character
    //Serial.print(msg, HEX);          // print the character
    DOOR_FL = bitRead(msg, 0);
    DOOR_FR = bitRead(msg, 1);
    DOOR_RL = bitRead(msg, 2);
    DOOR_RR = bitRead(msg, 3);
    DOOR_T = bitRead(msg, 4);
    parkingbrake = bitRead(msg, 5);
    lock = bitRead(msg, 6);
  }
}
void PrintCANVehicleStatus() {
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
// ---------- CAN ---------- //
