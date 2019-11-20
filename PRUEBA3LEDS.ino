#include <Wire.h>
#include "RTClib.h"
 
 
RTC_DS3231 rtc;
 
void setup() {
   
   pinMode(7, OUTPUT);
   pinMode(6, OUTPUT);
   pinMode(8, OUTPUT);
   
   Serial.begin(9600);

 
   if (!rtc.begin()) {
      Serial.println(F("Couldn't find RTC"));
      while (1);
   }
 
   if (rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   }
}
 
// Comprobar si esta programado el encendido
bool isScheduledON(DateTime date)
{
   int weekDay = date.dayOfTheWeek();
   float minutes = date.minute() + date.second() / 60.0;
 
   // De 09:30 a 11:30 y de 21:00 a 23:00
   bool minutesCondition = (minutes > 30 && minutes <31);
 
   // Miercoles, Sabado o Domingo
   bool dayCondition = (weekDay == 1 || weekDay == 2 || weekDay == 3 || weekDay == 4 || weekDay == 5 || weekDay == 6 || weekDay == 0); 
   if (minutesCondition && dayCondition)
   {
      return true;
   }
   return false;
}
 
void loop() {
   DateTime now = rtc.now();
 
   if (isScheduledON(now))      // Apagado y debería estar encendido
   {
  digitalWrite(7, HIGH);
  delay(500);
  digitalWrite(7, LOW);
  delay(500);
  digitalWrite(6, LOW);
  digitalWrite(8, LOW);

  Serial.print("Activado");
   }
   else if (!isScheduledON(now))  // Encendido y deberia estar apagado
   {
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(20000);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(20000);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  delay(2000);

  Serial.print("Desactivado");
   }
 
}
