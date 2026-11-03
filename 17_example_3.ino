
#include <Servo.h>

// Arduino pin assignment

#define PIN_IR    0         // IR sensor at Pin A0
#define PIN_LED   9
#define PIN_SERVO 10

// 서보 실험에서 측정한 값으로 수정할 것
#define _DUTY_MIN  500      // 0 degree
#define _DUTY_NEU  1500     // 90 degree
#define _DUTY_MAX  2500     // 180 degree

#define _DIST_MIN  100.0    // minimum distance 100mm
#define _DIST_MAX  250.0    // maximum distance 250mm

#define EMA_ALPHA  0.2      // EMA Filter

#define LOOP_INTERVAL  20   // Loop Interval (unit: msec)

Servo myservo;
unsigned long last_loop_time = 0;

float dist_prev = _DIST_MIN;
float dist_ema = _DIST_MIN;

void setup()
{
  pinMode(PIN_LED, OUTPUT);

  myservo.attach(PIN_SERVO);
  myservo.writeMicroseconds(_DUTY_NEU);

  Serial.begin(1000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int duty;
  float a_value, dist_raw;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;

  last_loop_time += LOOP_INTERVAL;

  // 1. IR sensor value
  a_value = analogRead(PIN_IR);

  // 2. Convert ADC value to distance (mm)
  if (a_value > 9.0)
    dist_raw = (6762.0 / (a_value - 9.0) - 4.0) * 10.0 - 60.0;
  else
    dist_raw = -1.0;

  // 3. Range Filter (100mm ~ 250mm)
  if (dist_raw >= _DIST_MIN && dist_raw <= _DIST_MAX)
  {
    digitalWrite(PIN_LED, HIGH);

    // 4. EMA Filter
    dist_ema = EMA_ALPHA * dist_raw
             + (1.0 - EMA_ALPHA) * dist_ema;
  }
  else
  {
    digitalWrite(PIN_LED, LOW);
  }

  // 5. Convert distance to servo duty
  duty = _DUTY_MIN
       + (dist_ema - _DIST_MIN)
       * (_DUTY_MAX - _DUTY_MIN)
       / (_DIST_MAX - _DIST_MIN);

  myservo.writeMicroseconds(duty);

  // 6. Serial Plotter output
  Serial.print("_DUTY_MIN:");  Serial.print(_DUTY_MIN);
  Serial.print(",_DIST_MIN:"); Serial.print(_DIST_MIN);
  Serial.print(",IR:");        Serial.print(a_value);
  Serial.print(",dist_raw:");  Serial.print(dist_raw);
  Serial.print(",ema:");       Serial.print(dist_ema);
  Serial.print(",servo:");     Serial.print(duty);
  Serial.print(",_DIST_MAX:"); Serial.print(_DIST_MAX);
  Serial.print(",_DUTY_MAX:"); Serial.print(_DUTY_MAX);
  Serial.println("");
}
