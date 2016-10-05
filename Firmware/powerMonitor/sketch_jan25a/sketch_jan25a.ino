#define LED 13
volatile byte state = LOW;

void setup() {
  pinMode(LED, OUTPUT);
  attachInterrupt(0, toggle, RISING);
}

void loop() { 
  digitalWrite(LED, state); 
}

void toggle() { 
  state = !state; 
}
