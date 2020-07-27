### Spiegazione
Il progetto è un irrigatore automatico per piante con monitoraggio su sito internet , l'attuatore (la pompa dell'acqua) agisce in base al sensore (sensore umidità).
Il microcontroller controlla l'umidità del terreno ogni x ore , manda il dato a un database e controlla se è necessario far partire la pompa dell'acqua.

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
**leafScreen.ino**

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

#### Note


## Parte 2 : Innaffiare il terreno
Spiegazione fase 2
#### Componenti
* Relay JQC3F (HW-482)
<img src="/img/JQC3F.png" alt="drawing" width="100" height = 100/>
* Pompa dell'acqua JT80SL
<img src="/img/JT80SL.jpg" alt="drawing" width="100" height = 100/>

#### Codice
**leafScreen.ino**

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
TODO : modificare schema fritzing per farlo funzionare a batteria
<img src="img/FRITZING2.png" alt="drawing"/>

## Parte 3 : Wifi e SQL
#### Codice
**leafScreen.ino**

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

**dbRead.php**
    
    <?php

    // Set DB variables
    $host = "localhost";
    $dbname = "dbName";   
    $username = "dbUsername";
    $password = "dbPassword";

    // Establish connection to MySQL database
    $conn = new mysqli($host, $username, $password, $dbname);

    // Check if connection established successfully
    if ($conn->connect_error)
        die("Connection failed: " . $conn->connect_error);

    // Select values from MySQL database table
    $sql = "SELECT id, value, date, time FROM table";
    $result = $conn->query($sql);

    // Print the value on the website
    echo "<center>";
    if ($result->num_rows > 0) {
        while($row = $result->fetch_assoc()) {
            echo "<strong> Id:</strong> " . $row["id"]. " &nbsp <strong>value:</strong> " . $row["value"]. " &nbsp <strong>Date:</strong> " . $row["date"]." &nbsp <strong>Time:</strong>" .$row["time"]. "<p>";
        }
    } else {
        echo "0 results";
    }
    echo "</center>";

    // Close the connection
    $conn->close();

    ?>

**dbWrite.php**

    <?php

    // Set DB variables
    $host = "localhost";
    $dbname = "dbName";   
    $username = "dbUsername";
    $password = "dbPassword";

    // Establish connection to MySQL database
    $conn = new mysqli($host, $username, $password, $dbname);

    // Check if connection established successfully
    if ($conn->connect_error)
        die("Connection failed: " . $conn->connect_error);

    // Get date and time variables
    date_default_timezone_set('Europe/Amsterdam');
    $d = date("Y-m-d");
    $t = date("H:i:s");
        
    // Insert value into SQL db
    if(!empty($_POST['sendval']) )
    {
        $val = $_POST['sendval'];
        $sql = "INSERT INTO table (value, date, time) VALUES ('".$val."', '".$d."', '".$t."')"; 
    }

    // Close the connection
    $conn->close();

    ?>