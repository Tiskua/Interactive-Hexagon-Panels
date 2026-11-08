#pragma once

constexpr int NUM_LEDS = 48;
constexpr int LED_PIN = 18;
constexpr int LEDS_PER_HEXAGON = 12;

constexpr int NUM_HEXAGONS =
    NUM_LEDS / LEDS_PER_HEXAGON;

constexpr int MAX_SIMON_LENGTH = 50;

constexpr int WEBSOCKET_PORT = 81;