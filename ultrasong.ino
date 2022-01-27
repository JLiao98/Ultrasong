// ---------------------------------------------------------------- //
// CPSC 599 Physical & Tangible HCI
// Ultrasong
// Jian Liao
// ---------------------------------------------------------------- //

// defind the constant variables
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 // attach pin D3 Arduino to pin Trig of HC-SR04
#define buttonPin 4 // button pin for digital read to determine if the button has been pressed
#define buzzerPin 6 // buzzer pin to make sound
#define red_light_pin 11 // RGB LED - Red Leg
#define green_light_pin 10 // RGB LED - Green Leg
#define blue_light_pin 9 // RGB LED - Blue Leg
#define distance_threshold 40 // set the distance threshold to 40 cm, because my little desk from IKEA is 40 cm wide


// Music Notes from Vicetone & Tony Igy - Astronomia

#define NOTE_F1 350
#define NOTE_F2 393
#define NOTE_F3 441
#define NOTE_F4 495
#define NOTE_F5 556
#define NOTE_F6 624
#define NOTE_F7 661
#define NOTE_0  0
#define NOTE_FL1 175
#define NOTE_FL2 196
#define NOTE_FL3 221
#define NOTE_FL4 234
#define NOTE_FL5 262
#define NOTE_FL6 294
#define NOTE_FL7 330
#define NOTE_FH1 700
#define NOTE_FH2 786
#define NOTE_FH3 882
#define NOTE_FH4 935
#define NOTE_FH5 1049
#define NOTE_FH6 1178
#define NOTE_FH7 1322
#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625

// tune
int tune[] =
{

  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F6, NOTE_F6, NOTE_F6, NOTE_F6,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_FH1, NOTE_FH1, NOTE_FH1, NOTE_FH1,
  NOTE_FH2, NOTE_FH2, NOTE_FH2, NOTE_FH2, NOTE_FH2, NOTE_FH2, NOTE_FH2, NOTE_FH2,
  NOTE_F5, NOTE_F4, NOTE_F3, NOTE_F1, NOTE_F2, NOTE_0, NOTE_F2, NOTE_F6,
  NOTE_F5, NOTE_0, NOTE_F4, NOTE_0, NOTE_F3, NOTE_0, NOTE_F3, NOTE_F3, NOTE_F5, NOTE_0, NOTE_F4, NOTE_F3, NOTE_F2, NOTE_0, NOTE_F2, NOTE_FH4,
  NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_F2, NOTE_0, NOTE_F2, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_F2, NOTE_0, NOTE_F2, NOTE_F6,
  NOTE_F5, NOTE_0, NOTE_F4, NOTE_0, NOTE_F3, NOTE_0, NOTE_F3, NOTE_F3, NOTE_F5, NOTE_0, NOTE_F4, NOTE_F3, NOTE_F2, NOTE_0, NOTE_F2, NOTE_FH4,
  NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_F2, NOTE_0, NOTE_F2, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4,
};

// beats
float durt[] =
{

  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5
};


// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int buttonState = 0; // current state of the button
int notesLength; // length of the notes
int delay_time = 500; // delay time for playing notes


void setup() {
  pinMode(trigPin, OUTPUT); // set the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // set the echoPin as an INPUT
  pinMode(buttonPin, INPUT); // set the buttonPin as an INPUT
  pinMode(buzzerPin, OUTPUT); // set the buzzerPin as an OUTPUT
  notesLength = sizeof(tune) / sizeof(tune[0]); // get the length of 'tune'
  randomSeed(12345); // randomSeed
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed
}

// AnalogWrite to control the LED Light
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

// Control the RGB LED on/off (ANODE)
void RGB_control (int rgb_switch)
{
  if (rgb_switch == 1)
  {
    pinMode(red_light_pin, OUTPUT);
    pinMode(green_light_pin, OUTPUT);
    pinMode(blue_light_pin, OUTPUT);
  } else {
    pinMode(red_light_pin, INPUT);
    pinMode(green_light_pin, INPUT);
    pinMode(blue_light_pin, INPUT);
  }
}

// Calculate the distance from the ultrasonic sensor (HC-SR04)
int calculateDistance () {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

// main function
void loop() {
  // get the button state to see if it has been pressed
  buttonState = digitalRead(buttonPin);

  // if the button has been pressed
  if (buttonState) {
    int i = 5;
    // play the song for 5 times
    while (i--) {
      // turn on the RGB LED
      RGB_control(1);
      for (int x = 0; x < notesLength; x++)
      {
        // retrieve the distance in real-time
        int distance = calculateDistance();
        
        if (distance <= distance_threshold) {
          delay_time = distance * 10;
        } else {
          delay_time = 500;
        }
        // change the RGB LED color randomly to blink to the beats
        RGB_color(random(256), random(256), random(256));
        tone(buzzerPin, tune[x]);
        delay(delay_time * durt[x]); // delay for playing notes
        noTone(buzzerPin);
      }
      // turn off the RGB LED
      RGB_control(0);
    }
  }
}
