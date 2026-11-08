#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

#include "Config.h"
#include "LEDController.h"
#include "TouchController.h"
#include "SimonGame.h"
#include "ReactionGame.h"
#include "GameManager.h"

const char *ssid = "RIT-WiFi";
const char *password = "";

WebSocketsServer webSocket(WEBSOCKET_PORT);

LEDController leds;
TouchController touch;

SimonGame simon(leds);
ReactionGame reaction(leds);

GameManager games(simon, reaction);

void broadcastReactionResult(
    int panel,
    unsigned long reactionTime)
{
  JsonDocument doc;

  doc["command"] = "STOP_REACTION";
  doc["panel"] = panel;
  doc["time"] = reactionTime;

  String json;
  serializeJson(doc, json);

  webSocket.broadcastTXT(json);
}

void webSocketEvent(
    uint8_t client,
    WStype_t type,
    uint8_t *payload,
    size_t length)
{
  if (type != WStype_TEXT)
    return;

  JsonDocument doc;

  DeserializationError error =
      deserializeJson(doc, payload, length);

  if (error)
  {
    Serial.println("Invalid JSON received");
    return;
  }

  String command = doc["command"] | "";

  if (command == "START_SIMON")
  {
    games.startGame(GameType::Simon);
  }
  else if (command == "START_REACTION")
  {
    games.startGame(GameType::Reaction);
  }
  else if (command == "STOP_GAME")
  {
    games.stopGame();
  }
  else if (command == "HOLD")
  {
    games.stopGame();

    CRGB color(
        doc["color"]["r"] | 0,
        doc["color"]["g"] | 0,
        doc["color"]["b"] | 0);

    leds.fill(color);
  }
  else if (command == "SET_PANEL")
  {
    games.stopGame();

    int panel = doc["panelId"] | -1;

    CRGB color(
        doc["color"]["r"] | 0,
        doc["color"]["g"] | 0,
        doc["color"]["b"] | 0);

    leds.setHexagon(panel, color);
  }
  else if (command == "BRIGHTNESS")
  {
    int brightness = doc["brightness"] | 100;

    leds.setBrightness(
        constrain(brightness, 0, 255));
  }
  else if (command == "RAINBOW")
  {
    games.stopGame();
    leds.rainbowSections();
  }
}

void setup()
{
  Serial.begin(115200);

  Wire.begin(21, 22);

  leds.begin();

  if (!touch.begin())
  {
    Serial.println("MPR121 not found!");

    while (true)
    {
      delay(1000);
    }
  }

  touch.onPress([](int panel)
                { games.onTouch(panel); });

  touch.onRelease([](int panel)
                  { games.onRelease(panel); });

  reaction.onResult(broadcastReactionResult);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("Setup complete!");
}

void loop()
{
  webSocket.loop();
  touch.update();
  games.update();

  delay(5);
}