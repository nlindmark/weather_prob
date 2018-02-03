#include <Arduino.h>
#include "iotUpdater.h"
#include "credentials.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Weather.h"
#include <Ticker.h>
extern "C" {
  #include "user_interface.h"

}

#define CODE_VERSION "V1.0.0"
// FORWARD DECLARATIONS

bool setup_wifi();
void tack();
void eventListener();
boolean mqttReconnect();

// VARIABLES
Weather* weather = Weather::getInstance();

Ticker timer1(tack, 1000,0, MILLIS);

char ssid[] = SSIDNAME;
char password[] = PASSWORD;

WiFiClient espClient;

IPAddress mqttServer(192, 168, 1, 200);
void mqttCallback(char* topic, byte* payload, unsigned int length);
PubSubClient mqttClient(mqttServer, 1883, mqttCallback, espClient);

long lastMqttReconnectAttempt = 0;
int mqttReconnectCounter = 0;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  delay(2000);


  setup_wifi();

  weather->setCallback(eventListener);

  timer1.start();

  mqttReconnect();


}

void loop() {
  // put your main code here, to run repeatedly:
  //
  if (!mqttClient.connected()) {
    mqttReconnect();
  } else {
    // Client connected
    mqttClient.loop();
  }

  timer1.update();
  weather->update();
}


bool setup_wifi() {

  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSIDNAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);


  if(WiFi.status() == WL_CONNECTED){

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
  }

return false;
}

boolean mqttReconnect(){

  char clientName[50];
  long now = millis();

  if (now - lastMqttReconnectAttempt > 3000) {
    lastMqttReconnectAttempt = now;
    // Attempt to reconnect

    snprintf (clientName, 50, "%ld", system_get_chip_id());
    if (mqttClient.connect(clientName, MQTTUSER, MQTTPASSWORD)) {
      char str[50];
      strcpy(str, "The Weather station is (re)connected ");
      strcat(str, CODE_VERSION);
      mqttClient.publish("/home/weather/hello", str);
      snprintf (str, 50, "%i", mqttReconnectCounter++);
      mqttClient.publish("/home/weather/reconnect", str);
      mqttClient.subscribe("/home/weather/status");
      mqttClient.subscribe("/home/weather/reprogram");

    }
  }
  return mqttClient.connected();
}


// CALLBACKS
//


void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived

  // Internally, the mqtt client uses the same buffer for both inbound and outbound
  // messages. The application should create its own copy of the values if they
  // are required beyond this.
  char t[50], p[length+1], buffer[50];
  snprintf (t, 50, "%s", topic);
  snprintf (p, length, "%s",payload);
  Serial.println(t);

if(strcmp(t, "/home/weather/status") == 0){
    mqttClient.publish("/home/weather/temp",  weather->getTemp(buffer), true);
    mqttClient.publish("/home/weather/humid", weather->getHumid(buffer), true);
}  else if (strcmp(t, "/home/weather/reprogram") == 0) {
    iotUpdater(true);
    Serial.println("Received a reprogram command");
  }
}


void eventListener(){
  // Something wonderful has happened
  char eventStr[50];
  mqttClient.publish("/home/weather/temp",  weather->getTemp(eventStr), true);
  mqttClient.publish("/home/weather/humid", weather->getHumid(eventStr), true);
}

void tack() {
  char buffer[50];
  Serial.print("Temp: ");
  Serial.println(weather->getTemp(buffer));
  Serial.print("Humid: ");
  Serial.println(weather->getHumid(buffer));
}
