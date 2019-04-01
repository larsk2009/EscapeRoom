// Demonstrates usage of the new udpServer feature.
//You can register the same function to multiple ports, and multiple functions to the same port.
//
// 2013-4-7 Brian Lee <cybexsoft@hotmail.com>

#include "ER_NET.h"

ErNet erNet;

void setup(){
  Serial.begin(115200);
  Serial.println(F("\n[backSoon]"));

  erNet.Setup();
}

void loop(){
  //this must be called for ethercard functions to work.
  erNet.Loop();
}