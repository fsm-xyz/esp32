#define switchPin1 1
#define switchPin2 2

#include "Arduino.h"

void pin1111()
{
    pinMode(switchPin1, INPUT);
    while (1)
        if (digitalRead(switchPin1) == 0)
        {
            Serial.println("ok", 0);
            digitalWrite(ledPin, HIGH);
        }
    if (digitalRead(UPin) == 1)
    {
        send_lock = 0;
    }
}
start = false;
}