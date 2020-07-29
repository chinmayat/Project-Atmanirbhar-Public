#include <SoftwareSerial.h> 
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <Temboo.h>

#define TEMBOO_ACCOUNT "Chinmayat"  // Your Temboo account name 
#define TEMBOO_APP_KEY_NAME "First"  // Your Temboo app key name

const char ssid[] = "SweetHome";
const char password[] = "xxxxxx5";
WiFiClient client;

int sensorPin = A0;
int relay_pin = D0;
int led_pin = D1;
int sensorValue =0;
int pirPin = D2;    //the digital pin connected to the PIR sensor's output
int pirStat = 0; 
int brightness = 250;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
const int sensorPin = A0; // Defining LDR PIN 
int input_val = 0;  // Varible to store Input values

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 5; 


void setup(){ 
  pinMode(relay_pin,OUTPUT);
  pinMode(led_pin,OUTPUT);  
  digitalWrite(led_pin,HIGH);
  pinMode(pirPin, INPUT);
    Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  // initializeESP8266() verifies communication with the WiFi
  // shield, and sets it up.
  initializeESP8266();
  // connectESP8266() connects to the defined WiFi network.
  connectESP8266();

  Serial.begin(9600);
}
void loop(){
   sensorValue = analogRead(sensorPin);      // Reading Input
   Serial.print("LDR value is : " );                        
   Serial.println(sensorValue);        // Writing input on serial monitor.
   delay(1000);       
  
// pirStat = digitalRead(pirPin);
 //Serial.println(pirStat);  


  analogWrite(led_pin, brightness);
  //Serial.println("LED Brightness is" + my ); 
  
  if ((sensorValue > 200 ) && pirStat==LOW ) //only when it is dark outside and no moment is seen 
  {
    Serial.println("Its Night and no motion is present");
    digitalWrite(relay_pin,LOW); //switch on light
    //delay(1000);
    analogWrite(led_pin, brightness-100);
     if (led_pin==LOW)
  Serial.println("Light is Off"); 
  else
   Serial.println("Light is On");
   // digitalWrite(relay_pin,HIGH); //switch off light
    //delay(1000);
  }

else if ((sensorValue > 200 ) && pirStat==HIGH )  //only when it is dark outside and moment is seen 
  {
    Serial.println("Its Night and motion is present");
    //analogWrite(led_pin, brightness);
    digitalWrite(relay_pin,LOW); //switch on light
   // delay(1000);
    analogWrite(led_pin, brightness-100);
     if (led_pin==LOW)
  Serial.println("Light is Off"); 
  else
   Serial.println("Light is On");
   // digitalWrite(relay_pin,HIGH); //switch off light
    //delay(5000);
  }
  else if (sensorValue < 200) //during dy time
  {
      //Serial.println("Its Day time");
      digitalWrite(relay_pin,HIGH); //swich off light
       if (led_pin==HIGH)
  Serial.println("Light is On"); 
  else
   Serial.println("Light is Off");
   callsms();
   //delay(1000);
  }
}

void initializeESP8266()
{
  // esp8266.begin() verifies that the ESP8266 is operational
  // and sets it up for the rest of the sketch.
  // It returns either true or false -- indicating whether
  // communication was successul or not.
  // true
  int test = WiFi.begin();
//  Serial.println(F("ESP8266 Shield Present"));
}

void connectESP8266()
{
      WiFi.begin(ssid, password);
      Serial.print("Successfully connected to wifi");
      delay(5000);
    Serial.println("\nConnected.");
}

void callsms() 
{
    if (numRuns <= maxRuns) {
    Serial.println("Running SendSMS - Run #" + String(numRuns++));
    delay(1000);
    TembooChoreo SendSMSChoreo(client);

    // Invoke the Temboo client
    SendSMSChoreo.begin();

    // Set Temboo account credentials
    SendSMSChoreo.setAccountName("Chinmayat");
    SendSMSChoreo.setAppKeyName("First");
    SendSMSChoreo.setAppKey("ZVs7ECLhp1KkoBeSL3n1brKfeQdPopwl");

    // Set Choreo inputs
    String AuthTokenValue = "30ee7f0710fb51dc2ecccd37a79d56f6";
    SendSMSChoreo.addInput("AuthToken", AuthTokenValue);
    String ToValue = "+919821608088";
    SendSMSChoreo.addInput("To", ToValue);
    String FromValue = "+19282373956";
    //+19282373956,+919821608088
    SendSMSChoreo.addInput("From", FromValue);
    String BodyValue = "From PROJECT ATMANIRBHAR : Light is off";
    SendSMSChoreo.addInput("Body", BodyValue);
    String AccountSIDValue = "ACf0d73c586a372b79b9281cd5a882c89b";
    SendSMSChoreo.addInput("AccountSID", AccountSIDValue);

    // Identify the Choreo to run
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");

    // Run the Choreo; when results are available, print them to serial
    SendSMSChoreo.run();
    Serial.println("DONE!");
    while(SendSMSChoreo.available()) {
      char c = SendSMSChoreo.read();
      Serial.print(c);
    }
    SendSMSChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between SendSMS calls
} 
