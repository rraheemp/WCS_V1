#include "Arduino.h"
#include <Instances.h>

#ifndef STARTUP_H
#define STARTUP_H

bool LEDstate = false;

 void Blink()
{
  digitalWrite(2, LEDstate);
  LEDstate = !LEDstate;
}

void StartUp()
{
    pinMode(2, OUTPUT);
    Blink();
    Blink();
    delay(2000);
    Blink();
}

#endif