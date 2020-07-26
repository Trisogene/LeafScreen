### Spiegazione
lorem ipsus..
### Componenti
* Flying fish MH Sensor
* Soil Mosture Sensor
* Jumper wire
* Relay JQC3F (HW-482)
* Pompa dell'acqua JT80SL

### Installazione

* installare l'ultima versione di arduino IDE dal sito ufficiale di arduinoESP
* dall'IDE di arduino aprire "preferences"
* aggiungere "https://arduino.esp8266.com/stable/package_esp8266com_index.json" nell' "Additional Board Manager URL"
* aprire "Tools" > "Board" > "Board Manager" e installare "ESP8266 community"
* aprire "Tools" > "Board" > "ESP8266 boards" e selezionare "NodeMCU 1.0"
* Collegare ESP8266 al computer
> Per controllare che tutto funzioni correttamente andare su "File" > "Example" > "ESP8266" > "Blink" e premere sul tasto upload.

## Parte 1 : Prendere l'umidità
#### Componenti
* Flying fish MH Sensor
<img src="/img/FFMS.png" alt="drawing" width="100" height = 100/>
* Soil Mosture Sensor
<img src="/img/SMS.png" alt="drawing" width="100" height ="100"/>
* Jumper wire
<img src="/img/JW.png" alt="drawing" width="100" height ="100"/>

#### Schema fritzing
<img src="img/FRITZING1.png" alt="drawing"/>

#### Codice

    const int humidityInputPin = A0;
    int humidityValue = 0;
    
    void setup() {
        pinMode(humidityInputPin, INPUT);
        Serial.begin(9600);
    }

    void loop() {
        humidityValue = analogRead(analogInPin);
        Serial.println("humidity sensor = ");
        Serial.print(AsensorValue);
        delay(500);
    }

## Parte 2 : Innaffiare il terreno
#### Componenti
* Relay JQC3F (HW-482)
<img src="/img/JQC3F.png" alt="drawing" width="100" height = 100/>
* Pompa dell'acqua JT80SL
<img src="/img/JT80SL.jpg" alt="drawing" width="100" height = 100/>

#### Codice
    const int humidityInputPin = A0;
    const int releeController = D0;
    int humidityValue = 0;

    void setup() {
        pinMode(humidityInputPin, INPUT);
        pinMode(releeController, OUTPUT);
        Serial.begin(9600);
    }

    void loop() {    
        humidityValue = analogRead(humidityInputPin);
        if(humidityValue < 800){
            digitalWrite(releeController,LOW);
        }else{
            digitalWrite(releeController,HIGH);
        }
        delay(2000);
    }

#### Schema fritzing
<img src="img/FRITZING2.png" alt="drawing"/>