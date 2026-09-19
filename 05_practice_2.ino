#define PIN_LED 7
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);

  count = 0;
  toggle = 1;
  digitalWrite(PIN_LED, toggle);
}

void loop() {
  toggle = 0;
  digitalWrite(PIN_LED, toggle);
  delay(1000);

  for (count = 0; count < 5; count++) {
    toggle = 1;              // LED 끄기
    digitalWrite(PIN_LED, toggle);
    delay(100);

    toggle = 0;              // LED 켜기
    digitalWrite(PIN_LED, toggle);
    delay(100);
  }

  toggle = 1;
  digitalWrite(PIN_LED, toggle);

  while (1) {
  }
}
