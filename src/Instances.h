#ifndef INSTANCES_H
#define INSTANCES_H

#include "Arduino.h"
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include "Stepper.h"
#include <max6675.h>
#include <PID_v1.h>
#include <startup.h>

#define RELAY_PIN 22
#define Range 5
WidgetLED HeatStateLED(V5);

const int stepsPerRevolution = 200;

uint16_t CurrentTemperature = 0;
const int thermoDO = 13;
const int thermoCS = 12;
const int thermoCLK = 14;

bool Kill, InitSet_Flag, HEATER_MODE = false;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "_Jo-e2fTo4BuQpLF22Ildt6HIpmblvbo";

uint8_t SetTemperature = 0;
uint8_t MotorSpeed = 0;

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp = 2, Ki = 5, Kd = 1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;

TaskHandle_t Task1;
void Task1code(void *parameter);

//This files defines objective instances etc.

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Stepper myStepper(stepsPerRevolution, 4, 18, 19, 21);

BlynkTimer Timer1, Timer2, Timer3, Timer4;


#endif