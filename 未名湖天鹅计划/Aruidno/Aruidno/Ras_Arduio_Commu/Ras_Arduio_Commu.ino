int LED_Pin = 8;
int Read;
String comdata = "";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_Pin,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0)
  {
    comdata = comdata + char(Serial.read());
    Serial.println(comdata);
    }
   
}
