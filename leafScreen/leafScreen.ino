    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>

    #define HOST "http://leafscreen.000webhostapp.com/dbwrite.php"
    #define WIFI_SSID "FASTWEB-HL6LTX"                                      
    #define WIFI_PASSWORD "KSTFD60FC9"    

    const int humidityInputPin = A0;
    const int releeController = D1;
    const int wetSoil = 800;
    const int waitTime = 20;

    int humidityValue = 0;
    String sendval, postData;

    void setup() {
        // Set the board I/O
        pinMode(humidityInputPin, INPUT);
        pinMode(releeController, OUTPUT);
        Serial.begin(9600);
        delay(2000);
        
        // Connect to WiFi
        WiFi.mode(WIFI_STA);           
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        while (WiFi.status() != WL_CONNECTED) 
        { 
            delay(500); 
        }
        Serial.println("[0] Wifi Connected");
    }

    void loop() {
        //Init variables
        HTTPClient http;
        humidityValue = 0;
        int tempHumidity[6];

  
        // Prepare data
        for (int i = 0; i<5;i++){
            tempHumidity[i] = analogRead(humidityInputPin);
            Serial.print("[1] Collected value : ");
            Serial.println(tempHumidity[i]);
            humidityValue += tempHumidity[i]; 
            delay(400);
        }
        
        humidityValue /= 5;
        Serial.print("[2] Final value = ");
        Serial.println(humidityValue);
        sendval = String(humidityValue);
        postData = "sendval=" + sendval;

        // Start HTTP session and send data
        http.begin(HOST);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpCode = http.POST(postData);

        // Check for connection error
        if (httpCode != 200) {
            http.end(); 
            return; 
        }

        Serial.print("[3] Value sent");
        if(humidityValue < wetSoil){
            // Turn off the pump is soil is wet
            digitalWrite(releeController,LOW);
            Serial.print("[4] Pump off");
        }else{
            // Turn on the pump if soil is not wet
            digitalWrite(releeController,HIGH);
            while(humidityValue > wetSoil - 50){
                // Keep the pump on while soil isnt wet enough
                Serial.print("[4] Pump on because humidty = ");
                Serial.print(humidityValue);
                Serial.println();

                humidityValue = analogRead(humidityInputPin);
                delay(1000);
            }
            // Turn the pump off because the soil is wet enough
            digitalWrite(releeController,LOW);
            Serial.println("[4] Pump off");
        }
        
        ESP.deepSleep(waitTime * 1000000);
    }
