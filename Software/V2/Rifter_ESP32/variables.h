#include "configuration.h"

//--- variables for I2C Atmega328p - ESP32 communitation ---//
bool DOOR_FL;
bool DOOR_FR;
bool DOOR_RL;
bool DOOR_RR;
bool DOOR_T;
bool lock;
bool parkingbrake;

//--- variables GPIO input ---//
bool SW_I;
bool SW_II;

//TEST
HTTPClient http;

// IP-Addresses
String IPofKofferraumklappe = "192.168.4.2";

// Drehknopf
struct Drehknopf {
  int state;
  int luminance;
  int leftmax;
  int rightmax;
  int RED;
  int GREEN;
  int BLUE;
  int effect;
};

Drehknopf Upper_Drehknopf;
