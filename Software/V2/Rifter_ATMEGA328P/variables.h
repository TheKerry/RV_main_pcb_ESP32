//--- variables for I2C Atmega328p - ESP32 communitation ---//
//Doors
bool DOOR_FL;
bool DOOR_FR;
bool DOOR_RL;
bool DOOR_RR;
bool DOOR_T;

//lights
bool left_indicator;
bool right_indicator;
bool rear_fog_light;
bool front_fog_light;
bool high_beam;
bool low_beam;
bool sidelights;

//misc
bool lock;
bool parkingbrake;

//--- variables GPIO input ---//
bool RV_MODE;

//--- variables CAN ---//
struct can_frame canMsg;
MCP2515 mcp2515(10);
bool debug_can_messages = true;
