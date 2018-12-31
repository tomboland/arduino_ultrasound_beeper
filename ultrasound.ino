#define MAX_RANGE_METRES 0.5
#define MIN_RANGE_METRES 0.02
#define MIN_TONE_HZ 2000
#define MAX_TONE_HZ 5000
#define SPEAKER_MIN_TONE_DURATION_MS 1
#define LOOP_INTERVAL_MS 1000
#define SPEED_OF_SOUND_METRES_PER_SECOND 343
#define SPEAKER_PIN 8
#define ULTRASONIC_TRIGGER_PIN 11
#define ULTRASONIC_ECHO_PIN 12
#define ULTRASONIC_SIGNAL_DURATION_MICRO_SECS 10

long echo_duration_micro_secs;
float distance_metres;
float normalised_distance;
float range_percentage;
float beep_duration;

void setup() {
  Serial.begin (9600);
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
}
 
void loop() {
  // Send an ultrasonic pulse for the given duration
  digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(ULTRASONIC_SIGNAL_DURATION_MICRO_SECS);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);

  // Wait for high signal and read the number of microseconds it took to receive the signal
  echo_duration_micro_secs = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);

  distance_metres = (((float)echo_duration_micro_secs / 2) * SPEED_OF_SOUND_METRES_PER_SECOND) / 1000000; 
  
  Serial.print(distance_metres);
  Serial.print(" metres");
  Serial.println();

  // MAX_RANGE_METRES or more will be 100% of the range, causing the minimal blip of bleepage
  if (distance_metres > MAX_RANGE_METRES) {
    normalised_distance = MAX_RANGE_METRES - MIN_RANGE_METRES;
  } else if (distance_metres - MIN_RANGE_METRES < 0) {
    normalised_distance = 0;
  } else {
    normalised_distance = distance_metres - MIN_RANGE_METRES;
  }

  Serial.print(normalised_distance);
  Serial.print(" normalised metres");
  Serial.println();
  range_percentage = (normalised_distance * 100.0) / (MAX_RANGE_METRES - MIN_RANGE_METRES);
  beep_duration = LOOP_INTERVAL_MS - (((float)LOOP_INTERVAL_MS - (float)SPEAKER_MIN_TONE_DURATION_MS) / 100.0) * range_percentage;

  tone(SPEAKER_PIN, MIN_TONE_HZ, beep_duration);

  delay(1000);
}
