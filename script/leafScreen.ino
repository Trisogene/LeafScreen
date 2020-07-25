const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
const int digitalInPin = D0;

int AsensorValue = 0;
int DsensorValue = 0;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(analogInPin, INPUT);
  pinMode(digitalInPin, INPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  AsensorValue = analogRead(analogInPin);
  DsensorValue = digitalRead(digitalInPin);
  
  Serial.print("\n\nAnalog sensor = ");
  Serial.print(AsensorValue);

  Serial.print("\n\nDigital sensor = ");
  Serial.print(DsensorValue);
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);                      // Wait for two seconds (to demonstrate the active low LED)
}
