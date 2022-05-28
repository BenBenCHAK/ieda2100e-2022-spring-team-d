#include <Wire.h>
#include "BlueDot_BME280.h"
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "DEF" 
#define WIFI_PASSWORD "IEDA2100E" 

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "https://test-ieda2100e-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "7eS7BIngLTtWoactvHz1Y8dwLD9Ym4deiq8S8cU5"

/* 3. Define the Firebase Data object */
FirebaseData fbdo0;
FirebaseData fbdo1;
FirebaseData fbdo2;
FirebaseData fbdo3;
FirebaseData fbdo4;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

BlueDot_BME280 bme;  //Object for Sensor
int bmeDetected = 0; //Checks if sensor is available

//D1-SCL
//D2-SCK

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(" ");
  String Mac = WiFi.macAddress();
  Mac.remove(2,1);
  Mac.remove(4,1);
  Mac.remove(6,1);
  Mac.remove(8,1);
  Mac.remove(10,1); 
  Serial.print("ESP Board MAC Address:  ");
  //Serial.println(WiFi.macAddress());
  Serial.println(Mac);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the database URL and database secret(required) */
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  Firebase.reconnectWiFi(true);

  /* Initialize the library with the Firebase authen and config */
  Firebase.begin(&config, &auth);
  
  //Set up 12C mode
  bme.parameter.communication = 0;

  //I2C Adress
  bme.parameter.I2CAddress = 0x76;

  //Setup Sensor mode
  //0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
  //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
  //0b11:     In normal mode the sensor measures continually (default value)
  
  bme.parameter.sensorMode = 0b11;
                   
  //IIR (Infinite Impulse Response) Filter Set up, higher factor = less noise and less responsive
  //0b000:      factor 0 (filter off)
  //0b001:      factor 2
  //0b010:      factor 4
  //0b011:      factor 8
  //0b100:      factor 16 (default value)

  bme.parameter.IIRfilter = 0b100;

  //Humidity oversampling factor
  //0b000:      factor 0 (Disable humidity measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

  bme.parameter.humidOversampling = 0b101;

  //Temperature oversampling factor
  //0b000:      factor 0 (Disable temperature measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

  bme.parameter.tempOversampling = 0b100;

  //Pressure oversampling factor
  //0b000:      factor 0 (Disable pressure measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)  

  bme.parameter.pressOversampling = 0b101;

  //For precise altitude measurements please put in the current pressure corrected for the sea level
  //On doubt, just leave the standard pressure as default (1013.25 hPa);
  
  bme.parameter.pressureSeaLevel = 1013.25;
   
  //Also put in the current average temperature outside (yes, really outside!)
  //For slightly less precise altitude measurements, just leave the standard temperature as default (15°C and 59°F);
  
  bme.parameter.tempOutsideCelsius = 15;

  if (bme.init() != 0x60)
  {    
    //Serial.println(F("Ops! BME280 Sensor not found!"));
    bmeDetected = 0;
  }

  else
  {
    //Serial.println(F("BME280 Sensor detected!"));
    bmeDetected = 1;
  }
}

  float val;
  float Temperature;
  float Humidity;
  float Pressure;
  float Altitude;
  
void loop() {
  // put your main code here, to run repeatedly:
  String Mac = WiFi.macAddress();
  Mac.remove(2,1);
  Mac.remove(4,1);
  Mac.remove(6,1);
  Mac.remove(8,1);
  Mac.remove(10,1); 
  
  int sensorValue = analogRead(A0);   // read the input on analog pin 0

  float Voltage = sensorValue * (5.0 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)

  Serial.print("Voltage: ");
  Serial.println(Voltage);   // print out the value you read
  Firebase.RTDB.setFloat(&fbdo0, "/Sensor Data/"+Mac+"/Light_Sensor", Voltage);
  
  if (bmeDetected)
   {
    
     float Temperature = bme.readTempC();
     Serial.print("Temperature: ");
     Serial.println(Temperature);
     //Serial.print( ",");

     float Humidity = bme.readHumidity();
     Serial.print("Humidity: ");
     Serial.println(Humidity);
     //Serial.print( ",");

     float Pressure = bme.readPressure();
     Serial.print("Pressure: ");
     Serial.println(Pressure);

     //String web = 
     Firebase.RTDB.setFloat(&fbdo1, "/Sensor Data/"+Mac+"/Temperature", Temperature);
     //delay(10);
     Firebase.RTDB.setFloat(&fbdo2, "/Sensor Data/"+Mac+"/Humidity", Humidity);
     //delay(10);
     Firebase.RTDB.setFloat(&fbdo3, "/Sensor Data/"+Mac+"/Pressure", Pressure);
     //delay(10);
    }
   else
   {
     Serial.println("Not Connected");
   }
  
   delay(10000);
}
