#define TIMEOUT_MS 1100
#define BUTTON1_PIN 49
#define BUTTON2_PIN 47

void blinkAlgorithm1();
void blinkAlgorithm2();

void setup() {
  Serial.begin(9600);
  pinMode (BUTTON1_PIN, INPUT_PULLUP);
  pinMode (BUTTON2_PIN, INPUT_PULLUP);
  DDRF = 0xFF;
  PORTF = 0;
}

void loop() {

  if (digitalRead (BUTTON1_PIN)== LOW) {
    Serial.write(0xB2);
    delay(200);
  } else if (digitalRead (BUTTON2_PIN)== LOW) {
    Serial.write(0xB1);
    delay(200);
  }

  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == 0xA1) {
      blinkAlgorithm1();
    } else if (inByte == 0xB1) {
      blinkAlgorithm2();
    }
  }
}

void blinkAlgorithm1() {
  uint8_t start = B10000000, end = B00000001;
  while (start > end) {
    PORTF = end;
    delay(TIMEOUT_MS);
    PORTF = start;
     delay(TIMEOUT_MS);
    start >>= 1;
    end <<= 1;
  }
  PORTF = 0;
}

void blinkAlgorithm2() {
  int counter = 0;
  uint8_t led = B10000000;
  while (counter < 8) {
    PORTF = led;
    delay(TIMEOUT_MS);
    led >>= 1;
    counter ++;
  }
  PORTF = 0;
}
