/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
  Example based on the Universal Arduino Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/FlashLED/FlashLED.ino

  Small changes by Nicu FLORICA (niq_ro) to find CHAT_ID
*/

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "nume_retea_wifi";
const char* password = "berica_2";

// Initialize Telegram BOT
#define BOTtoken "API_key_from_Botfather"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "read in serial your code" 

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      Serial.println(chat_id);
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
   //   String welcome = "Welcome, " + from_name + ".\n";
      String welcome = "Salut " + from_name + ".\n";
   //   welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "Foloseste urmatoarele comenzi:\n\n";
   //   welcome += "/led_on to turn GPIO ON \n";
      welcome += "/aprinde pentru alimentare led \n";
   //   welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/stinge pentru oprire alimentare \n";
   //   welcome += "/state to request current GPIO state \n";
      welcome += "/stare aflarea situatiei ledului \n";
      bot.sendMessage(chat_id, welcome, "");
    }

  //  if (text == "/led_on") {
  //    bot.sendMessage(chat_id, "LED state set to ON", "");
    if (text == "/aprinde") {
      bot.sendMessage(chat_id, "Led alimentat !", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
 //   if (text == "/led_off") {
  //    bot.sendMessage(chat_id, "LED state set to OFF", "");
    if (text == "/stinge") {
      bot.sendMessage(chat_id, "Led nealimentat !", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
/*    
    if (text == "/state") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }
*/
    if (text == "/stare") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "Ledul este nealimentat !", "");
      }
      else{
        bot.sendMessage(chat_id, "Ledul este alimentat !", "");
      }
    }
    
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
