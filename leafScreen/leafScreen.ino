const int humidityInputPin = A0;
const int releeController = D0;
int humidityValue = 0;


void setup() {
  pinMode(humidityInputPin, INPUT);
  pinMode(releeController, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  
  humidityValue = analogRead(humidityInputPin);
  
  Serial.print("\n\nhumidity sensor = ");
  Serial.print(humidityValue);
  
  if(humidityValue < 800){
    digitalWrite(releeController,LOW);
  }else{
    digitalWrite(releeController,HIGH);
  }
  
  delay(2000);
  
}
