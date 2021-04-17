#define TIMEOUT_MS 1100
#define BUTTON_PIN 49


void setup() {
  pinMode (BUTTON_PIN, INPUT_PULLUP);
  DDRF = 0xFF;
  PORTF = 0;
}

void loop() {
  if (digitalRead (BUTTON_PIN)== LOW) {
    led_blink();
  }
}

void led_blink() {
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
