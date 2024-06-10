#include <Adafruit_CircuitPlayground.h>

volatile bool switchFlag = false;
volatile bool buttonLeftFlag = false;
volatile bool buttonRightFlag = false;
const byte switchPin = 7;
const byte buttonPin1 = 4;
const byte buttonPin2 = 5;

int level = 0;
int points = 0;
int activePixel = -1;
int activeColor = -1;
bool gameActive = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
  pinMode(switchPin, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin1), buttonLeftFlagISR, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), buttonRightFlagISR, RISING);
  attachInterrupt(digitalPinToInterrupt(switchPin), switchFlagISR, CHANGE);
  randomSeed(analogRead(0));
  
  // Initial switch state read
  switchFlag = digitalRead(switchPin) == HIGH;
  if (switchFlag) {
    Serial.println("Game started");
    gameActive = true;
  } else {
    Serial.println("Switch is off");
  }
}

void switchFlagISR() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentTime;
    
    bool switchState = digitalRead(switchPin) == HIGH;
    if (switchState != switchFlag) {
      switchFlag = switchState;
      if (!switchFlag) {
        resetGame();
        Serial.println("Switch is off");
      } else {
        Serial.println("Game started");
        gameActive = true;
      }
    }
  }
}

void buttonLeftFlagISR() {
  buttonLeftFlag = true;
}

void buttonRightFlagISR() {
  buttonRightFlag = true;
}

void randomColor() {
  activePixel = random(0, 10);
  activeColor = random(0, 2); // Only two colors now: 0 (green) and 1 (red)
  switch (activeColor) {
    case 0:
      CircuitPlayground.setPixelColor(activePixel, 0, 255, 0); // Green
      break;
    case 1:
      CircuitPlayground.setPixelColor(activePixel, 255, 0, 0); // Red
      break;
  }
}

void youFailed() {
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 136, 8, 8);
  }
  delay(500);
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
  delay(500);
}

void rainbowCycle(int delay_ms, int speed) {
  uint32_t offset = millis() / speed;
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(((i * 256 / 10) + offset) & 255));
  }
  delay(delay_ms);
}

void levelUp() { 
  for (int i = 0; i < 3; ++i) {
    CircuitPlayground.playTone(523, 100); // C note
    delay(100);
    CircuitPlayground.playTone(659, 100); // E note
    delay(100);
    CircuitPlayground.playTone(783, 100); // G note
    delay(100);
  }
  for (int i = 0; i < 10; ++i) {
    rainbowCycle(50, 10);
  }
  for (int i = 0; i < 10; i += 2) {
    CircuitPlayground.setPixelColor(i, 255, 223, 0); // Golden LEDs
    if (i / 2 >= level) break;
  }
}

void youWin() {
  for (int i = 0; i < 3; ++i) {
    CircuitPlayground.playTone(723, 100); 
    delay(100);
    CircuitPlayground.playTone(859, 100); 
    delay(100);
    CircuitPlayground.playTone(983, 100); 
    delay(100);
  }
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 229, 184, 11); // Golden color
  }
  delay(2000);
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
}

void resetGame() {
  points = 0;
  level = 0;
  gameActive = false;
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
  Serial.println("Game reset");
}

void loop() {
  if (switchFlag && gameActive && level < 5) {
    if (activePixel == -1 && activeColor == -1) {
      randomColor();
    }

    if (buttonLeftFlag || buttonRightFlag) {
      bool correctPress = false;
      
      if (buttonLeftFlag && activeColor == 0) { // Green LED, Left button press
        points++;
        correctPress = true;
      } else if (buttonRightFlag && activeColor == 1) { // Red LED, Right button press
        points++;
        correctPress = true;
      } else { // Wrong button press
        Serial.println("You failed.");
        CircuitPlayground.playTone(223, 100);
        youFailed();
        points = 0;
        level = 0;
      }
      
      if (correctPress) {
        Serial.println("Points: " + String(points));
        CircuitPlayground.playTone(523, 100); // Ding sound
      }
      
      buttonLeftFlag = false;
      buttonRightFlag = false;
      CircuitPlayground.setPixelColor(activePixel, 0, 0, 0);
      activePixel = -1;
      activeColor = -1;

      if (points > 0 && points % 10 == 0) { // Level up condition
        level++;
        levelUp();
        if (level == 5) {
          youWin();
          points = 0;
          level = 0;
        }
      }
    }
  }
}
