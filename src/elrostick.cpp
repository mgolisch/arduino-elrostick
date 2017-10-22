#include "RCLSwitch.h"
#include <SoftwareSerial.h>
#include <SerialCommand.h>

SerialCommand SCmd;
const uint8_t RCLpin = 10;
CRCLSwitch<RCLpin> RCLSwitch;

// Channel: 00001 Button: 100000
//uint8_t channel = 0B01111;
//uint8_t unit = 0B01000;
//char *s1 = "01111";
//char *s2 = "01000";
// OFF 01111 01000
// ON 01111 01000

int fromBinary(char *s) {
  return (int) strtol(s, NULL, 2);
}

void switchOn(char  *schannel,char *sunit){
  uint8_t channel = fromBinary(schannel);
  uint8_t unit = fromBinary(sunit);
  RCLSwitch.write(channel,unit,true);
}

void switchOff(char  *schannel,char *sunit){
  uint8_t channel = fromBinary(schannel);
  uint8_t unit = fromBinary(sunit);
  RCLSwitch.write(channel,unit,false);
}

void switchOnCommand(){
  char *schannel = SCmd.next();
  char *sunit =  SCmd.next();
  switchOn(schannel,sunit);
  Serial.println("recieved on command");
}

void switchOffCommand(){
  char *schannel = SCmd.next();
  char *sunit =  SCmd.next();
  switchOff(schannel,sunit);
  Serial.println("recieved off command");
}

void Help(){
  Serial.println("available commands:");
  Serial.println("on");
  Serial.println("parameters: string channel , string unit");
  Serial.println("off");
  Serial.println("parameters: string channel , string unit");
  Serial.println("parameter explanation:");
  Serial.println("channel: channel to switch (binary representation of first 5 dip switches) example: 01000");
  Serial.println("unit: unit to switch (binary representation of second 5 dip switches) example: 01000");
  Serial.println("full example: on 01000 01000");
}

void unrecognized() {
  Serial.println("unknown command!");
  Serial.println("see help command for help");
}

void setup() {
  RCLSwitch.begin();
  Serial.begin(9600);
  SCmd.addCommand("on",switchOnCommand);        //switch socket on
  SCmd.addCommand("off",switchOffCommand);        //switch socket off
  SCmd.addCommand("help",Help);        //help
  SCmd.setDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")
  Serial.println("Ready");
}

void loop() {

 SCmd.readSerial();
}
