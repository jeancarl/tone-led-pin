// This #include statement was automatically added by the Particle IDE.
#include "FastLED/FastLED.h"

// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"

/***************************************************************************
# Copyright 2016 IBM
#
#   Tone LED Pin
#
#   By JeanCarl Bisson (@dothewww)
#   More info: https://github.com/jeancarl/tone-led-pin
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#***************************************************************************
*/

char server[] = "<orgid>.messaging.internetofthings.ibmcloud.com";
char clientId[] = "d:<orgid>:<devicetype>:<deviceid>";
char token[] = "<token>";

FASTLED_USING_NAMESPACE;

// How many leds in your strip?
#define NUM_LEDS 24

CRGB leds[NUM_LEDS];

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6

#define MQTT_TOPIC "iot-2/cmd/color/fmt/string"

MQTT client(server, 1883, callback);

// Callback to handle messages from the IoT platform
void callback(char* topic, byte* payload, unsigned int length) {
    animateWipeClean();

    // Sets the color for each pin based on the message from the Watson IoT Platform
    for(int i=0; i<length; i++) {
        switch(payload[i]) {
            case 'r':   leds[i] = CRGB::Red;        break;
            case 'g':   leds[i] = CRGB::Green;      break;
            case 'b':   leds[i] = CRGB::Blue;       break;
            case 'y':   leds[i] = CRGB::Yellow;     break;
            case 'p':   leds[i] = CRGB::Purple;     break;
        }

        delay(50);
        FastLED.show();
    }
}

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.clear();
    LEDS.setBrightness(40);
}

void loop() {
    if(client.isConnected()) {
        client.loop();
    } else {
        client.connect(clientId, "use-token-auth", token);
        client.subscribe(MQTT_TOPIC);
    }
}

// Animation that turns of the LED one-by-one clockwise.
void animateWipeClean() {
    for(int i=0; i<24; i++) {
        if(i>0) {
            leds[i-1] = CRGB::Black;
        }

        leds[i] = CRGB::Blue;

        FastLED.show();
        delay(50);
    }

    FastLED.clear();
}
