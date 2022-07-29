#include <WiFi.h>
#include <ESPmDNS.h>
#include <Wire.h>

//--- WIFI configuration ---//
const char *ssid = "Rifter";
const char *passphrase = "70199265";

//--- CAN configuration ---//
#define debugCAN


//--- Pin assignment ---//
//OUTPUTS
#define Kofferraumbeleuchtung_unten 16
#define Kofferraumklappe 17
#define Zigarettenanzuender_1 18
#define Zigarettenanzuender_2 19

//INPUTS
#define SW_I_pin 32
#define SW_II_pin 33
