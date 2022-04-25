#include <Arduino.h>

enum state
{
  OFF,
  DROP,
  HOLD
};
bool vibrate = false;
int vibrationPin = 4;
bool suck = false;
int suckPin = 2;
bool blow = false;
int blowPin = 1;
int iterator = 0;

void setup()
{
  pinMode(suckPin, OUTPUT);      // suck
  pinMode(blowPin, OUTPUT);      // blow
  pinMode(vibrationPin, OUTPUT); // vibration
}

void loop()
{
  enum state currentState;
  currentState = OFF;
  while (true)
  {
    //iterator++;
    //Serial.println(iterator);
    Serial.println("Set State Parameters");
    switch (currentState)
    {
    case OFF:
      vibrate = true;
      blow = true;
      suck = true;
      break;
    case DROP:
      vibrate = false;
      blow = false;
      suck = false;
      break;
    case HOLD:
      vibrate = false;
      blow = true;
      suck = true;
      break;
    default:
      Serial.println("NO STATE");
    }

    // vibration
    if (vibrate)
    {
      digitalWrite(vibrationPin, HIGH); // sets the digital pin 13 on
      Serial.println("vibrate");
    }
    else
    {
      digitalWrite(vibrationPin, LOW); // sets the digital pin 13 on
      Serial.println("don't vibrate");
    }

    // blow
    if (blow)
    {
      Serial.println("blow");
      digitalWrite(blowPin, HIGH); // sets the digital pin 13 on
    }
    else
    {
      Serial.println("don't blow");
      digitalWrite(blowPin, LOW); // sets the digital pin 13 on
    }

    // suck
    if (suck)
    {
      Serial.println("suck");
      digitalWrite(suckPin, HIGH); // sets the digital pin 13 on
    }
    else
    {
      Serial.println("don't suck");
      digitalWrite(suckPin, LOW); // sets the digital pin 13 on
    }

    // read
    Serial.print("Serial Read: ");
    int read = -1;
    while (true)
    {
      //Serial.println("trying to read");
      //delay(200);
      if (Serial.available() > 0)
      {
        int received = Serial.read() - 48;
        Serial.print("recieved ");
        Serial.println(received);
        if (received == -1)
          continue;
        if (received == '\n')
          Serial.println("read = recieved ");
          read = received;
          break;
      }
    }
    Serial.print("read is ");
    Serial.println(read);
    Serial.println("");
    switch (read)
    {
    case 1:
      currentState = OFF;
      Serial.println("  OFF");
      break;
    case 2:
      currentState = DROP;
      Serial.println("  DROP");
      break;
    case 3:
      currentState = HOLD;
      Serial.println("  HOLD");
      break;
    default:
      currentState = OFF;
      Serial.println("  default");
      break;
    }
  }
}