#include <SPI.h>
#include <mcp2515.h>
#include <Wire.h>

//--- CAN configuration ---//
#define debugCAN


//--- Pin assignment ---//
//OUTPUTS
#define ESP_EN 7

//INPUTS
#define RV_MODE_PIN 6
