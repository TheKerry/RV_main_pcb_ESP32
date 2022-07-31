#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include "AiEsp32RotaryEncoder.h"

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

//Upper rotary Encoder
#define ROTARY_ENCODER_A_PIN 26
#define ROTARY_ENCODER_B_PIN 25
#define ROTARY_ENCODER_BUTTON_PIN 27
#define ROTARY_ENCODER_STEPS 3
