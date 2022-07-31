//Programming procedure: TX auf RX bei Programming port und UART programmer

#include "variables.h"


IPAddress local_IP(192, 168, 4, 1);
//IPAddress gateway(192, 168, 4, 9);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

//ROTARY ENCODER
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  startAP();
  pinConfiguration();
  rotaryEncoderSetup();
  checkSwitch();
}

void loop() {
  //Serial.print("[Server Connected] ");
  //Serial.println(WiFi.softAPIP());
  ////Serial.printf("Anzahl der Verbundenen Geräte= %d\n", WiFi.softAPgetStationNum());

  /*---MIT CAN
    delay(100);

    updateCAN();
    printDebugMsg();

    checkSwitch();
    digitalWrite(Kofferraumbeleuchtung_unten, DOOR_T);
  */
  checkSwitch();
  digitalWrite(Kofferraumbeleuchtung_unten, SW_I);
  digitalWrite(Kofferraumklappe, SW_II);

  if (rotaryEncoder.encoderChanged())
  {
    switch (Upper_Drehknopf.state) {
      case 1:
        Upper_Drehknopf.luminance = rotaryEncoder.readEncoder();
        break;
      case 2:
        Upper_Drehknopf.leftmax = rotaryEncoder.readEncoder();
        if (Upper_Drehknopf.leftmax >= Upper_Drehknopf.rightmax) {
          Upper_Drehknopf.leftmax = Upper_Drehknopf.rightmax - 1;
        }
        break;
      case 3:
        Upper_Drehknopf.rightmax = rotaryEncoder.readEncoder();
        if (Upper_Drehknopf.rightmax <= Upper_Drehknopf.leftmax) {
          Upper_Drehknopf.rightmax = Upper_Drehknopf.leftmax + 1;
        }
        break;
      case 4:
        Upper_Drehknopf.RED = rotaryEncoder.readEncoder();
        break;
      case 5:
        Upper_Drehknopf.GREEN = rotaryEncoder.readEncoder();
        break;
      case 6:
        Upper_Drehknopf.BLUE = rotaryEncoder.readEncoder();
        break;
      case 7:
        Upper_Drehknopf.effect = rotaryEncoder.readEncoder();
        break;
    }
    //Send out command
    String temporaryHTTPreq = "http://";
    temporaryHTTPreq.concat(IPofKofferraumklappe);
    temporaryHTTPreq.concat("/win&A=");
    temporaryHTTPreq.concat(Upper_Drehknopf.luminance);
    temporaryHTTPreq.concat("&S=");
    temporaryHTTPreq.concat(Upper_Drehknopf.leftmax);
    temporaryHTTPreq.concat("&S2=");
    temporaryHTTPreq.concat(Upper_Drehknopf.rightmax);
    temporaryHTTPreq.concat("&R=");
    temporaryHTTPreq.concat(Upper_Drehknopf.RED);
    temporaryHTTPreq.concat("&G=");
    temporaryHTTPreq.concat(Upper_Drehknopf.GREEN);
    temporaryHTTPreq.concat("&B=");
    temporaryHTTPreq.concat(Upper_Drehknopf.BLUE);
    temporaryHTTPreq.concat("&FX=");
    temporaryHTTPreq.concat(Upper_Drehknopf.effect);
    http.begin(temporaryHTTPreq); //Specify the URL
    int httpCode = http.GET();
    http.end();
    //Serial.println(temporaryHTTPreq);
  }
  if (rotaryEncoder.isEncoderButtonClicked())
  {
    Upper_Drehknopf.state = Upper_Drehknopf.state + 1;
    if (Upper_Drehknopf.state == 8)
    {
      Upper_Drehknopf.state = 1;
    }
    switch (Upper_Drehknopf.state) {
      case 1: //Luminance
        rotaryEncoder.setBoundaries(0, 255, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
        rotaryEncoder.reset(Upper_Drehknopf.luminance);
        rotaryEncoder.setAcceleration(80);
        break;
      case 2: //left max
        rotaryEncoder.setBoundaries(0, 56, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
        rotaryEncoder.reset(Upper_Drehknopf.leftmax);
        rotaryEncoder.setAcceleration(45);
        break;
      case 3: //right max
        rotaryEncoder.setBoundaries(0, 56, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
        rotaryEncoder.reset(Upper_Drehknopf.rightmax);
        rotaryEncoder.setAcceleration(45);
        break;
      case 4: //color RED
        rotaryEncoder.setBoundaries(0, 255, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
        rotaryEncoder.reset(Upper_Drehknopf.RED);
        rotaryEncoder.setAcceleration(80);
        break;
      case 5: //color GREEN
        rotaryEncoder.setBoundaries(0, 255, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
        rotaryEncoder.reset(Upper_Drehknopf.GREEN);
        rotaryEncoder.setAcceleration(80);
        break;
      case 6: //color BLUE
        rotaryEncoder.setBoundaries(0, 255, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
        rotaryEncoder.reset(Upper_Drehknopf.BLUE);
        rotaryEncoder.setAcceleration(80);
        break;
      case 7: //color preset
        rotaryEncoder.setBoundaries(0, 25, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
        rotaryEncoder.reset(Upper_Drehknopf.effect);
        rotaryEncoder.setAcceleration(45);
        break;
    }

    //Serial.print("Drehknopfstate: ");
    //Serial.println(Upper_Drehknopf.state);
  }
  /*
  http.begin("http://192.168.4.2/win"); //Specify the URL
  int httpCode = http.GET();
  if (httpCode == 200) { //Check for the returning code
    String payload = http.getString();
    String temp_name = payload.substring(payload.indexOf("<ds>") + 4, payload.indexOf("</ds>"));
    if (temp_name == "Kofferraumklappe") {
      String temp_luminance = payload.substring(payload.indexOf("<ac>") + 4, payload.indexOf("</ac>"));
      String temp_fx = payload.substring(payload.indexOf("<fx>") + 4, payload.indexOf("</fx>"));
      IPofKofferraumklappe = "192.168.4.2";
      Upper_Drehknopf.luminance = temp_luminance.toInt();
      //Upper_Drehknopf.RED = 255;
      //Upper_Drehknopf.GREEN = 255;
      //Upper_Drehknopf.BLUE = 255;
      Upper_Drehknopf.effect = temp_fx.toInt();
      Serial.println("4.2: Name: " + temp_name);
      Serial.println("4.2: Luminance: " + temp_luminance);
    }
  }
  http.end();
  */
}
// --- END MAIN --- //






// ---------- Rotary Encoder ---------- //
void IRAM_ATTR readEncoderISR()
{
  rotaryEncoder.readEncoder_ISR();
}
// ---------- Rotary Encoder ---------- //




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

void rotaryEncoderSetup() {
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(0, 255, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder.reset(100);
  rotaryEncoder.setAcceleration(63);
  Upper_Drehknopf.state = 1;
  Upper_Drehknopf.luminance = 100;
  Upper_Drehknopf.leftmax = 0;
  Upper_Drehknopf.rightmax = 56;
  Upper_Drehknopf.RED = 255;
  Upper_Drehknopf.GREEN = 255;
  Upper_Drehknopf.BLUE = 255;
  Upper_Drehknopf.effect = 0;
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
