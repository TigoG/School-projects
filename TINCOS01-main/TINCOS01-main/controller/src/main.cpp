#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include "./secret.h"
#define PUBLISHTOPIC "data/BOT1"
#define SUBSCRIBETOPIC "move/1"
#define E_STOPTOPIC "stop/1"
const char* BOT_ID = "1";

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqtt_server = MQTT_HOST;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

int currentX = 0;       // huidige x-positie  
int currentY = 0;       // huidige y-positie
int LED_FORWARDS = 25;  // led vooruit
int LED_BACKWARDS = 26; // led achteruit
int LED_LEFT = 27;      // led naar links
int LED_RIGHT = 14;     // led naar rechts
int E_BUTTON = 12;      // input E-stop

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void move(int* coordinates) {
    int x = coordinates[0];
    int y = coordinates[1];
    delete[] coordinates;
    Serial.printf("Move function received X: %d, y: %d\n", x, y);
    if (x > currentX) {
        Serial.printf("Moving right %d steps\n", x);
        currentX = x;
        digitalWrite(LED_RIGHT, HIGH);
        digitalWrite(LED_LEFT, LOW);
        digitalWrite(LED_FORWARDS, LOW);
        digitalWrite(LED_BACKWARDS, LOW);
    } else if (x < currentX) {
        Serial.printf("Moving left %d steps\n", -x);
        currentX = x;
        digitalWrite(LED_RIGHT, LOW);
        digitalWrite(LED_LEFT, HIGH);
        digitalWrite(LED_FORWARDS, LOW);
        digitalWrite(LED_BACKWARDS, LOW);
    }
    if (y > currentY) {
        Serial.printf("Moving up %d steps\n", y);
        currentY = y;
        digitalWrite(LED_RIGHT, LOW);
        digitalWrite(LED_LEFT, LOW);
        digitalWrite(LED_FORWARDS, HIGH);
        digitalWrite(LED_BACKWARDS, LOW);
    } else if (y < currentY) {
        Serial.printf("Moving down %d steps\n", -y);
        currentY = y;
        digitalWrite(LED_RIGHT, LOW);
        digitalWrite(LED_LEFT, LOW);
        digitalWrite(LED_FORWARDS, LOW);
        digitalWrite(LED_BACKWARDS, HIGH);
    } else {
        Serial.println("Not moving, already at the given position");
    }
}

// function that takes a string and parses it into an array of integers of size 2
// function stops after finding two integers or if the string ends
int* parseString(char* message) {
    int* returnValue = new int[2];
    returnValue[0] = 0;
    returnValue[1] = 0;
    char* token = strtok(message, ",");
    uint8_t i = 0;
    while (token != NULL && i < 2) {
        returnValue[i] = atoi(token);
        token = strtok(NULL, ",");
        i++;
    }
    Serial.printf("Parsed values: %d, %d\n", returnValue[0], returnValue[1]);
    return returnValue;
}


void callback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1]; // +1 for null terminator
    for (int i = 0; i < length; i++) {
        message[i] = (char)payload[i];
    }
    message[length] = '\0';
    // for debugging
    // Serial.printf("Message arrived [%s] %s\n", topic, message);

    // expects X:Y:Z where X, Y, Z is a number
    if (strcmp(topic, SUBSCRIBETOPIC) == 0) {
        move(parseString(message));
    }

    // Print out the message for debugging
    if (strcmp(message, "test") == 0) {
        Serial.println("Test message received");
        client.publish(PUBLISHTOPIC, "Test message received");
    }
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");

        // Attempt to connect
        if (client.connect(BOT_ID, MQTT_USER, MQTT_PASS)) {
            Serial.println("connected");

            // Once connected, publish an announcement...
            client.publish(PUBLISHTOPIC, "Bot connected!");

            // ... and resubscribe
            client.subscribe(SUBSCRIBETOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
    pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    Serial.begin(115200);

    setup_wifi();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();
    client.setServer(mqtt_server, MQTT_PORT);
    client.setCallback(callback);

    pinMode(LED_FORWARDS, OUTPUT);  // led vooruit
    pinMode(LED_BACKWARDS, OUTPUT); // led achteruit
    pinMode(LED_LEFT, OUTPUT);      // led naar links
    pinMode(LED_RIGHT, OUTPUT);     // led naar rechts
    pinMode(E_BUTTON, INPUT_PULLUP);       // input E-stop
}

void loop() {
    if (!digitalRead(E_BUTTON)) {
        client.publish(E_STOPTOPIC, "true");
    }
    if (!client.connected()) {
        delay(2000);
        reconnect();
    }
    client.loop();
}
