#define hm10 Serial // pour faciliter la lecture.
long timer = 0;
void setup() {
  // put your setup code here, to run once:
  hm10.begin(9600);
  timer = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
    while (hm10.available()) {
    int c = hm10.read();
    if (c != -1) Serial.write((byte) c);
  }

  if(millis() - timer > 10000)
  {
    timer = millis();
    hm10.println("je parle");
  }
}
