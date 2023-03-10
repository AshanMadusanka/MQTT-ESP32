#include <Arduino.h>
#include <WiFi.h>
#include<PubSubClient.h>
#include <Adafruit_AHTX0.h>




const char* ssid = "Dialog 4G 628";
const char* password = "B59F81d1";

const char* brokerUser = "broker.hivemq.com";
const char* brokerPass = "o7i6012568";
const char* broker = "broker.hivemq.com";
const char* inTopic = "/AshanMQTT/in";
const char* outTopic = "/AHT/Temp";
const char* outTopic2 = "/AHT/Humi";
WiFiClient espClient;
PubSubClient client(espClient);
long currentTime, lastTime;
int count = 0;
char messages[50];

Adafruit_AHTX0 aht;

void wifiSetup(){

  delay(100);
  Serial.print("\nConnecting to");
  Serial.println(ssid);

  WiFi.begin(ssid,password);


  while (WiFi.status()!=WL_CONNECTED)
{
  delay(100);
  Serial.print("-");
}
Serial.print("\nConnected to");
Serial.println(ssid);

}
void reconnect(){

  while (!client.connected())
  {
    Serial.print("\nConnecting to ");
    Serial.print(broker);
    if (client.connect("koikoikoi",brokerUser, brokerPass))
    {
      Serial.print("\nConnecting to ");
      Serial.println(broker);
      client.subscribe(inTopic);
    }else{

      Serial.println("\nTrying connect again");
      delay(5000);
    }
    

  }
  
}
void callback(char* topic,byte* payload,unsigned int length){

  Serial.print("Received messages; ");
  Serial.print(topic);
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}


void readTemp(){

 sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");

  float temperature = temp.temperature;
  float humiditi = humidity.relative_humidity;

  // Publish temperature and humidity to MQTT topic
  char tempString[6];
  char humString[6];
  dtostrf(temperature, 5, 2, tempString);
  dtostrf(humiditi, 5, 2, humString);
  // client.publish("temperature", tempString);
  // client.publish("humidity", humString);

  client.publish(outTopic, tempString);
  client.publish(outTopic2, humString);


  delay(1000);


}

void setup() {

 
  Serial.begin(115200);
  wifiSetup();
  client.setServer(broker,1883);
  client.setCallback(callback); 
  Wire.begin();
  aht.begin();
}

void loop() {
 if (!client.connected())
 {
  reconnect();
 }
 client.loop();
currentTime = millis();
if (currentTime - lastTime >2000)
{
  // count++;
  // snprintf(messages,75,"Count: %ld",count);
  // Serial.print("Sending messages: ");
  // Serial.println(messages);
  // client.publish(outTopic,messages);
  // lastTime = millis();


}

readTemp();
}