#ifndef OBJECTS_H
#define OBJECTS_H
//This files defines objective instances etc.

TaskHandle_t Task1;
void Task1code(void *parameter);

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Stepper myStepper(stepsPerRevolution, 4, 18, 19, 21);

BlynkTimer Timer1, Timer2, Timer3;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).


TaskHandle_t Task1;
void Task1code(void *parameter);

#endif