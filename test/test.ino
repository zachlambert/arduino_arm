
int i = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  i+=1;
  Serial.println(i);
  delay(5);
}
