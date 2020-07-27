#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define HOST "www.website.com"
#define WIFI_SSID "SSID"                                      
#define WIFI_PASSWORD "PASSWORD"    

const int humidityInputPin = A0;
const int releeController = D0;
const int wetSoil = 500;
const int waitTime = 60;

int humidityValue = 0;
String sendval, postData;

void setup() {
  // Set the board I/O
  pinMode(humidityInputPin, INPUT);
  pinMode(releeController, OUTPUT);
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) 
  { 
    Serial.print(".");
    delay(500); 
  }
}

void loop() {
  // Prepare data
  HTTPClient http;
  humidityValue = analogRead(humidityInputPin);
  sendval = String(humidityValue);
  postData = "sendval=" + sendval;

  // Start HTTP session and send data
  http.begin("http://website.com/dbwrite.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);

  // Check for connection error
  if (httpCode != 200) {
    http.end(); 
    return; 
  }
  
  if(humidityValue < wetSoil){
    // Turn off the pump is soil is wet
    digitalWrite(releeController,LOW);
  }else{
    // Turn on the pump if soil is not wet
    digitalWrite(releeController,HIGH);
    while(humidityValue > wetSoil - 50){
        // Keep the pump on while soil isnt wet enough
        humidityValue = analogRead(humidityInputPin);
        delay(1000);
    }
    // Turn the pump off because the soil is wet enough
    digitalWrite(releeController,LOW);
  }
  
  delay(waitTime * 1000);
}