void setup()
{
  Serial.begin(9600);
}
void loop()
{
 while(1)
  { 
  Serial.print("ATD18688162761;\r");
  delay(5000);

  }
}