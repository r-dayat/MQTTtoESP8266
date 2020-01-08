#include <LiquidCrystal_I2C.h> // Lib LCD
#include <ESP8266WiFi.h> // Lib WiFi
#include <PubSubClient.h> // Lib MQTT Client
#include <Wire.h> // Lib LCD


LiquidCrystal_I2C lcd(0x27,20,4);
WiFiClient espClient;
PubSubClient client(espClient);

// Connect to the WiFi
const char* ssid = "Fruity"; // Access Point Name
const char* password = "111222333"; // Access Point Name


// Connect to the MQTT Broker
const char* mqttClientID = "LCD01"; // MQTT Client ID


const char* mqttServer = "52.29.249.133"; // Link MQTT Broker
const char* mqttUsername = "dayat"; // Username MQTT
const char* mqttPassword = "dayat"; // Password MQTT
const char* mqttTopicShowtext = "LCD01/my_topic"; // Topic MQTT - Showtext



uint16_t portMQTT = 1883;

//this constant won't change:
const int led = 5;


void append(char* s, char c){
  int len = strlen(s);
  s[len]=c;
  s[len+1]='\0';
}

void setup() {

  lcd.begin();
 
  // Backlight On
  lcd.backlight();
 
  // Set to cursor column 0 and row 0
  lcd.setCursor(0, 0);
 
  Serial.begin(115200);
  // initialize the LED as an output:
    pinMode(led, OUTPUT);
  
  // Connect Wifi
  connectWiFi();
  
  // Connect MQTT Broker
  client.setServer(mqttServer, portMQTT);
  connectMQTT();
  client.setCallback(callbackMQTT);
  Serial.print("setCallback : ");
  Serial.print("callbackMQTT");
}

void loop() {


    if (WiFi.status() != WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi Disconnected");
    Serial.println("Trying to Reconnect");
    Serial.println();
    connectWiFi();
  }

  if (!client.connected()) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println();
      Serial.println("WiFi Disconnected");
      Serial.println("Trying to Reconnect");
      Serial.println();
      connectWiFi();
    }
    connectMQTT();
  }
  client.loop();
}

void connectWiFi() {
  Serial.print("WiFi connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  Serial.println();

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting . . . .");
  }
  Serial.println();

  Serial.println("WiFi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.print(WiFi.localIP());
  Serial.print("/");
  Serial.print(WiFi.subnetMask());
  Serial.print(" with Gateway IP : ");
  Serial.println(WiFi.gatewayIP());
  Serial.println();
}

void connectMQTT() {
  // Looping until Connected
  while (!client.connected()) {
    Serial.print("connecting MQTT Broker : ");
    Serial.println("Attempting MQTT connection...");
    Serial.println(mqttServer);

    // Try Connect to Broker
    // boolean rc = mqttClient.connect("myClientID", willTopic, willQoS, true, willMessage);
    if (client.connect(mqttClientID)) {
      // Connected Success
      Serial.println("Success Connected Broker");
      Serial.println("Connected");

      // Try Subscribe TopicShowtext
      client.subscribe(mqttTopicShowtext);
      Serial.print("Subscribe topic : ");
      Serial.println(mqttTopicShowtext);
      Serial.println();

    } else {
      // Fail Connect to MQTT
      Serial.print("Failed Connect, rc=");
      Serial.println(client.state());
      Serial.println("Try again in 5 seconds");
      // Wait for 5s and will connect again
      delay(5000);
    }
  }
}

void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  char aray[84] = {'\0'};
  Serial.print("Get Data from subscribe :");
  Serial.print(mqttTopicShowtext);
  Serial.print(" with Payload : ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //append(aray, (char)payload[i]);
    aray[i] = (char) payload[i];
  }
  Serial.println();
  //lcd.clear();
  //for (int i = 0; i < length; i++) {
  //  lcd.print((char)payload[i]);
  //  append(aray,(char)payload[i]);
  //}
  Serial.println(aray);
  lcd.clear();
  //lcd.print(aray);
  char* token = strtok(aray, ";");
  int t = 0;
  String tok[3];
  while(token != NULL){
    Serial.println(token);
    tok[t] = token;
    t++;
    token = strtok(NULL, ";");
    }
  lcd.setCursor(0,0);
  lcd.print(tok[0]);
  lcd.setCursor(0,1);
  lcd.print(tok[1]);
  lcd.setCursor(0,2);
  lcd.print(tok[2]);
  lcd.setCursor(0,3);
  lcd.print(tok[3]);
  delay(10000);
}
