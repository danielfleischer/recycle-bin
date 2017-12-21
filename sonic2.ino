#include <TM1637Display.h>

// defines pins numbers
const int speakerPin = 3;
const int buttonPin = 7;
const int trigPin = 9;
const int echoPin = 10;
const int CLK = 12; //Set the CLK pin connection to the display
const int DIO = 13; //Set the DIO pin connection to the display

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 130;

// defines variables
int val = 0;
int counter = 0;
const int full_box = 10;
const int threshold = 30;
bool inRange = false;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  display.setBrightness(0x0a);
  pinMode(buttonPin, INPUT);  // declare LED as output
  digitalWrite(buttonPin, HIGH);
  pinMode(speakerPin, OUTPUT);
  display.showNumberDec(counter);
}

void play_melody() {
    for (int i = 0; i < length; i++) {
      if (notes[i] == ' ') {
        delay(beats[i] * tempo); // rest
      } else {
        playNote(notes[i], beats[i] * tempo);
      }
  
      // pause between notes
      delay(tempo / 2); 
    }
}

void something_went_down() {
      counter ++;
      tone(speakerPin, 440, 100);
//      tone(speakerPin, 540, 500);
//      tone(speakerPin, 640, 500);

      display.showNumberDec(counter);
      Serial.println("Something went down!");
      if (counter % full_box == 0) {
        play_melody();
      }
      else {
        delay(500);
      }
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  long distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  if (digitalRead(buttonPin) == LOW) {
    counter = 0;
    display.showNumberDec(counter);
  }

  if (inRange) {
    if (distance > threshold) {
      inRange = false;
    }
  }
  else {
    if (distance <= threshold) {
      inRange = true;
      something_went_down();
    }
  }
}

