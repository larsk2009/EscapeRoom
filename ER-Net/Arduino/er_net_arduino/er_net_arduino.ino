#include "ER_NET.h"

ErNet erNet;

void setup(){
  Serial.begin(115200);

  erNet.Setup();
}

void loop(){
  //this must be called for ethercard functions to work.
  erNet.Loop();
  int test;
  erNet.GetDisplayNumber(&test);
}