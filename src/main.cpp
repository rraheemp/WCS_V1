#include "Arduino.h"
#include "Stepper.h"
#include <max6675.h>
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <vector>

#define BLYNK_USE_DIRECT_CONNECT
#define BLYNK_PRINT Serial

const int stepsPerRevolution = 200;
const int thermoDO = 13;
const int thermoCS = 12;
const int thermoCLK = 14;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Stepper myStepper(stepsPerRevolution, 4, 18, 19, 21);

BlynkTimer Timer1, Timer2, Timer3;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
bool Kill, InitSet_Flag = false;
char auth[] = "_Jo-e2fTo4BuQpLF22Ildt6HIpmblvbo";
uint8_t SetTemperature = 0;
uint8_t MotorSpeed = 0;

TaskHandle_t Task1;
void Task1code(void *parameter);

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

void TemperatureGet()
{
  Serial.print("C = ");
  Serial.println(thermocouple.readCelsius());
  Blynk.virtualWrite(V3, thermocouple.readCelsius());
  Serial.print("F = ");
  Blynk.virtualWrite(V4, thermocouple.readFahrenheit());
  Serial.println(thermocouple.readFahrenheit());
}

void HeaterControl()
{
  Serial.println("HeaterControl: Test funxtion");
}

void Checks()
{
  if (Blynk.connected() && InitSet_Flag == 0x00)
  {
    InitSet_Flag = 0x01;
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 0);
    Serial.println("Disabling timer now");
    Timer1.disable(Timer1.getNumTimers());
  }
  else
    return;
}

void setup()
{
  //FreeRTOS initialization of Core 0 to control the Motor primarily.
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 0);

  // Debug console
  Serial.begin(115200);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("Wireless System Control");
  Blynk.begin(auth);

  //Blynk timer settings
  Timer1.setInterval(500L, TemperatureGet);
  Timer2.setInterval(250L, HeaterControl);
  Timer3.setInterval(3000L, Checks);
  InitSet_Flag = 0x00;
  delay(500);
}

void loop()
{
  Blynk.run();
  Timer1.run();
  Timer2.run();
  Timer3.run();
}

void Task1code(void *parameter)
{
  while (1)
  {
 
    //Serial.print("MotorCode is running on core ");
    //Serial.println(xPortGetCoreID());
    if (MotorSpeed == 0)
    {
      myStepper.step(0);
    }
    else
    {
      myStepper.setSpeed(MotorSpeed);
      //Serial.println(MotorSpeed);
      myStepper.step(stepsPerRevolution / 50);
    }
    // vTaskDelay(10);
    yield();
    vTaskDelay(1);
  }
}