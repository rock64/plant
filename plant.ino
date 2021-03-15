#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>

const int Relay_1 = 5;       // led grow light
const int Relay_2 = 4;        //Oxygen 

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = ""; //Enter the Auth code which was send by Blink


 
String apiKey = "";     //  Enter your Write API key from ThingSpeak
const char* server = "api.thingspeak.com";



// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";  //Enter your WIFI Name
char pass[] = "";  //Enter your WIFI Password

#define DHTPIN 12          // Digital pin D6

WiFiClient client;

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}

void setup()
{
    Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);

  pinMode(Relay_1, OUTPUT);   //Define LED GROW LIGHT
  pinMode(Relay_2, OUTPUT);   //DEFINE OXYGEN
  digitalWrite(Relay_1, HIGH);  //LED GROW LIGHT ON
  digitalWrite(Relay_2, HIGH);  //OXYGEN ON

  dht.begin();
 WiFi.begin(ssid, pass);
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
Blynk.run();
Blynk.run(); // Initiates Blynk
timer.run(); // Initiates SimpleTimer

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  
  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                      }
                      

 //client.stop();
 //delay(1000);
}


/********************** Relay 1  ON/OFF V1 *******************************/
BLYNK_WRITE(V1)
  {
  int pinValue = param.asInt();
  if (pinValue == 1)
    {
    digitalWrite(Relay_1, HIGH);
    Serial.println("Relay 1 On");
    }
  else if (pinValue == 0)
    {
    digitalWrite(Relay_1, LOW);
    Serial.println("Relay 1 Off");
    }
  }



/********************** Relay 2  ON/OFF V2 *******************************/
BLYNK_WRITE(V2)
  {
  int pinValue = param.asInt();
  if (pinValue == 1)
    {
    digitalWrite(Relay_2, HIGH);
    Serial.println("Relay 2 On");
    }
  else if (pinValue == 0)
    {
    digitalWrite(Relay_2, LOW);
    Serial.println("Relay 2 Off");
    }
  }

  
  
