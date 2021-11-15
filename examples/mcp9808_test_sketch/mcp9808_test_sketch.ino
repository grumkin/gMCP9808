
#include <Wire.h>
#include "gMCP9808.h"

gMCP9808 temp_sensor = gMCP9808(); // create sensor object

void setup() {
  Serial.begin(9600);
  while (!Serial); // wait for serial terminal to be open

  if (!temp_sensor.start(MCP9808_I2CADDR_DEFAULT)) {
    Serial.println("Couldn't find MCP9808");
    while(1);
  }
  Serial.println("Found MCP9808!");

}

void loop() {
  

}
