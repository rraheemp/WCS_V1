#include "Arduino.h"
#include "Stepper.h"
#include <max6675.h>
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>


#define BLYNK_USE_DIRECT_CONNECT
#define BLYNK_PRINT Serial

const int stepsPerRevolution = 200;
const int thermoDO = 13;
const int thermoCS = 12;
const int thermoCLK = 14;


MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Stepper myStepper(stepsPerRevolution, 4, 18, 19, 21);

BlynkTimer timer1, timer2, timer3;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
bool Kill = false;
char auth[] = "_Jo-e2fTo4BuQpLF22Ildt6HIpmblvbo";
uint8_t CurrentTemperature, SetTemperature = 0;
long MotorSpeed = 0L;

BLYNK_WRITE(V0)
{
  MotorSpeed = param.asInt();
  Serial.println(MotorSpeed);
}

BLYNK_WRITE(V1)
{
  SetTemperature = param.asInt();
  Serial.println(SetTemperature);
}

BLYNK_WRITE(V10)
{
  Kill = param.asInt();
  if (Kill)
    ESP.restart();
}

void MotorControll()
{
  if (MotorSpeed == 0)
    myStepper.step(0);
  else
  {
    myStepper.setSpeed(MotorSpeed);
    Serial.println(MotorSpeed);
    myStepper.step(2);
  }
}

void TemperatureGet()
{
  Serial.print("C = ");
  Serial.println(thermocouple.readCelsius());
  Blynk.virtualWrite(V3, thermocouple.readCelsius());
  CurrentTemperature = thermocouple.readCelsius();
  Serial.print("F = ");
  Blynk.virtualWrite(V4, thermocouple.readFahrenheit());
  Serial.println(thermocouple.readFahrenheit());
}

void HeaterControll()
{
  Serial.println("test funxtion");
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("Wireless System Control");
  Blynk.begin(auth);
  timer1.setInterval(1L, MotorControll);
  timer2.setInterval(500L, TemperatureGet);
  timer3.setInterval(250L, HeaterControll);
}

void loop()
{
  Blynk.run();
  timer1.run();
  timer2.run();
  timer3.run();
}