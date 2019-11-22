#include <LowPower.h>
#include <avr/wdt.h>
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>

#define hm10 Serial // pour faciliter la lecture.

void interrupt();
void readEtat();

long timer = 0;

void setup() {
  // put your setup code here, to run once:
  hm10.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  //wdt_enable(WDTO_8S);
  timer = millis();
}

int i = 0;
String token = "";
void loop() {
  // put your main code here, to run repeatedly:
    char carlu = '0';
    if (hm10.available()) 
    {
      carlu = hm10.read();
      //token += carlu;
      //Serial.print(token);
      readEtat();
      //PCintPort::attachInterrupt(0, interrupt, CHANGE);
      //LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    }
}

void interrupt()
{
  wdt_enable(WDTO_8S);
  PCintPort::detachInterrupt(0);
  //Serial.println("interruption");
}

void readEtat()
{
  Serial.print(digitalRead(2));
  Serial.print(digitalRead(3));
  Serial.print(digitalRead(4));
  Serial.println();
  //String etat = String(digitalRead(2)) + ';' + String(digitalRead(3)) + ';' + String(digitalRead(4));
  //return etat;
}
