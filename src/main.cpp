#include <Instances.h>

//#define Debug Serial.println

BLYNK_WRITE(V0)
{
  MotorSpeed = param.asInt();
  Serial.println(MotorSpeed);
}

BLYNK_WRITE(V1)
{
  Setpoint = param.asInt();
  Serial.print("Temperature Set Point Changed To: ");
  Serial.println(Setpoint);
}

BLYNK_WRITE(V2)
{
  HEATER_MODE = param.asInt();
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
  //For Pid heater
  CurrentTemperature = thermocouple.readFahrenheit();

  Serial.println(thermocouple.readFahrenheit());
  Serial.println();
}

void HeaterControl()
{
  if (HEATER_MODE == true)
  {
    Serial.println("HEATER MODE: ON!");
    Serial.println();

    if ((double)CurrentTemperature < (Setpoint - Range))
    {
      digitalWrite(RELAY_PIN, HIGH);
      Blynk.setProperty(V5, "color", "#04C0F8");
      HeatStateLED.on();
      Serial.print("Setpoint is: ");
      Serial.println(Setpoint);
      Serial.print("CurrentTemperature is: ");
      Serial.println(CurrentTemperature);
      Serial.println("Lower Than Set--Raise!");
    }

    if ((double)CurrentTemperature > (Setpoint + Range))
    {
      digitalWrite(RELAY_PIN, LOW);
      Blynk.setProperty(V5, "color", "#D3435C");
      HeatStateLED.on();
      Serial.print("Setpoint is: ");
      Serial.println(Setpoint);
      Serial.print("CurrentTemperature is: ");
      Serial.println(CurrentTemperature);
      Serial.println("Higher Than Set--Reduce!");
    }

    if ((double)CurrentTemperature > (Setpoint - Range) && CurrentTemperature < (Setpoint + Range))
    {
      digitalWrite(RELAY_PIN, LOW);

      Blynk.setProperty(V5, "color", "#23C48E");
      HeatStateLED.on();
      ////RGB_FOR_HEATER(2);
      Serial.print("Setpoint is: ");
      Serial.println(Setpoint);
      Serial.print("CurrentTemperature is: ");
      Serial.println(CurrentTemperature);
      Serial.println("Good Range!");
    }
  }
  else
  {
    HeatStateLED.off();
    Serial.println("HEATER MODE: OFF!");
    digitalWrite(RELAY_PIN, LOW);
  }
}

void Checks()
{
  if (Blynk.connected() && InitSet_Flag == 0x00)
  {
    InitSet_Flag = 0x01;
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 0);
    Serial.println("Disabling timer now");
    Timer1.disable(Timer1.getNumTimers());
    Serial.println("Timer Disabled.");
  }
  else
    return;
}



void setup()
{
  StartUp();

  pinMode(RELAY_PIN, OUTPUT);
  //FreeRTOS initialization of Core 0 to control the Motor primarily.
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 0);

  // Debug console
  Serial.begin(115200);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("Wireless System Control");
  Blynk.begin(auth);

  //Blynk timer settings
  Timer1.setInterval(500L, TemperatureGet);
  Timer2.setInterval(500L, HeaterControl);
  Timer3.setInterval(5000L, Checks);
  Timer4.setInterval(500L, Blink);
  //Set Initalize value flag as false;
  InitSet_Flag = 0x00;

  HeatStateLED.off();
  delay(500);

  //windowStartTime = millis();
  //tell the PID to range between 0 and the full window size
  //myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  // myPID.SetMode(AUTOMATIC);
}

void loop()
{
  Blynk.run();
  Timer1.run();
  Timer2.run();
  Timer3.run();
  Timer4.run();
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