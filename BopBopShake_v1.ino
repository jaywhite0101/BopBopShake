#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay delay_1s; 
bool slideSwitch;
volatile int count = 0;
volatile bool switchFlag = false; // Initialize to false
volatile bool buttonFlag = false; // Initialize to false
bool switchState; 
const byte switchPin = 7;
const byte buttonPin1 = 4;
const byte buttonPin2 = 5;

int pixelTime = 5000;
int level = 0;
int points = 0;

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
  attachInterrupt(digitalPinToInterrupt(buttonPin1), buttonLeftFlagISR, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), buttonRightFlagISR, RISING);
  attachInterrupt(digitalPinToInterrupt(switchPin), switchFlagISR, CHANGE);
  randomSeed(analogRead(0)); 
  while (!Serial);
  Serial.println("Starting game");

  // Start counting
  delay_1s.start(1000, AsyncDelay::MILLIS);
}

void switchFlagISR() {
  switchFlag = !switchFlag;
}

void buttonLeftFlagISR() {
  buttonLeftFlag = true;
}

void buttonRightFlagISR() {
  buttonRightFlag = true;
}

void randomColor() {
  int pixel = random(0, 10); // Select a random pixel (0 to 9)
  int color = random(0, 3); // Select a random color (0 to 2)
  switch (color) {
    case 0:
      CircuitPlayground.setPixelColor(pixel, 0, 255, 0); // Green
      break;
    case 1:
      CircuitPlayground.setPixelColor(pixel, 0, 0, 255); // Blue
      break;
    case 2:
      CircuitPlayground.setPixelColor(pixel, 255, 0, 0); // Red
      break;
  }
}

void youFailed() {
  for(int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 136, 8, 8); // turn on each LED at same time
    }
    //half a second on
    delay(500); 
    //turn off LEDS at same time
    for(int i = 0; i < 10; ++i) {
      CircuitPlayground.setPixelColor(i, 0, 0, 0); 
    }
    //half a second off
    delay(500); 
}

void rainbowCycle(int 10, int 10) {
  // Make an offset based on the current millisecond count scaled by the current speed.
  uint32_t offset = millis() / currentSpeed;
  // Loop through each pixel and set it to an incremental color wheel value.
  for(int i=0; i<10; ++i) {
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(((i * 256 / stripLen) + offset) & 255));
  }
}

void levelUp() { 
  // Play a level up jingle
  for(int i = 0; i < 3; ++i) {
    CircuitPlayground.playTone(523, 100); // C note
    delay(100);
    CircuitPlayground.playTone(659, 100); // E note
    delay(100);
    CircuitPlayground.playTone(783, 100); // G note
    delay(100);
  }
  // Rainbow cycle animation
  for(int i = 0; i < 10; ++i) {
    rainbowCycle(50, 10);
  }
  // Light up LEDs based on level
  for(int i = 0; i < 10; i += 2) {
    CircuitPlayground.setPixelColor(i, 255, 223, 0); // Golden LEDs
    if (i / 2 >= level) break;
  }
}

void youWin() {
  for(int i=0, i<10; ++i) {
    CircuitPlayground.setPixelColor(i, 229, 184, 11)
  }
}

void loop() {
  if(switchFlag) { //is the switch "on"?
    if(buttonPin1 == HIGH || buttonPin2 == HIGH) { //is a button being pressed? 
      //timer starts (level = 0, so pixel time is 5 seconds)
      //random color function runs on random pixel
      //if random color = blue, if random color = green, if random color = red: if flag thrown/acceleration > 20, points +=1
      //else, play youFailed, go back to beginning
      //cycle until points % 10 = 0, level up, play level up function
      //level changes to (pixelTime - level), reducing the amount of time the pixel shows based on the level
      //check if level = 5, if so, play youWin, return to beginning
    }
  }

}
