// ============================================
// INTERSTELLAR PIANO v1.1 (Arduino Mega)
// ============================================

#include <Keypad.h>

#define NUM_NOTE_LEDS 8
const int NOTE_LEDS[NUM_NOTE_LEDS] = {22, 24, 26, 28, 30, 32, 34, 36};

int noteToLed(int noteIdx) {
  switch (noteIdx) {
    case 12: case 24: return 0; // DO (C4, C5)
    case 14: case 26: return 1; // RE (D4, D5)
    case 16: case 28: return 2; // MI (E4, E5)
    case 17: case 29: return 3; // FA (F4, F5)
    case 19: case 31: return 4; // SOL(G4, G5)
    case 21: case 33: return 5; // LA (A4, A5)
    case 23: case 35: return 6; // SI (B4, B5)
    default: return -1; 
  }
}

#define BUZZER_PIN  8
#define LED_PIN     13
#define LED_EXT_PIN 12

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {62, 63, 64, 65};  
byte colPins[COLS] = {66, 67, 68, 69};  

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const float NOTE_FREQS[] = {
    130.81,138.59,146.83,155.56,164.81,174.61,185.00,196.00,207.65,220.00,233.08,246.94,
    261.63,277.18,293.66,311.13,329.63,349.23,369.99,392.00,415.30,440.00,466.16,493.88,
    523.25,554.37,587.33,622.25,659.25,698.46,739.99,783.99,830.61,880.00,932.33,987.77
};
const char* NOTE_NAMES[] = {
  "C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3",
  "C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4",
  "C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5"
};

struct KeyMapping { char key; int noteIdx; int chordRoot; int chordType; };
const KeyMapping keyMap[] = {
  {'1', 12, -1, 0}, {'2', 14, -1, 0}, {'3', 16, -1, 0},
  {'4', 17, -1, 0}, {'5', 19, -1, 0}, {'6', 21, -1, 0},
  {'7', 23, -1, 0}, {'8', 24, -1, 0}, {'9', 26, -1, 0},
  {'*', -1, 12, 0}, {'#', -1, 14, 1}, {'A', -1, 17, 0},
  {'B', -1, 19, 0}, {'C', -1, 21, 1}
};

int currentMelody = 0;
bool isPlaying = false;

// MELODIA 1: Fra Martino
const int m1_notes[] = { 12,14,16,12, 12,14,16,12, 16,17,19,-1, 16,17,19,-1, 19,21,19,17,16,12, 19,21,19,17,16,12, 12,7,12,-1, 12,7,12 };
const int m1_dur[] = { 400,400,400,400, 400,400,400,400, 400,400,800,200, 400,400,800,200, 200,200,200,200,400,400, 200,200,200,200,400,400, 400,400,800,200, 400,400,800 };
const int m1_len = sizeof(m1_notes)/sizeof(m1_notes[0]);

// MELODIA 2: Interstellar "Cornfield Chase"
const int m2_notes[] = {
  // Intro e Costruzione
  21,28, 21,28, 21,28, 21,28, 21,28, 21,28, 21,28, 21,28, 
  21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 
  21,31, 21,31, 21,31, 21,31, 21,31, 21,31, 21,31, 21,31, 
  21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 
  // Crescendo
  21,28, 21,28, 21,28, 21,28, 21,28, 21,28, 21,28, 21,28, 
  21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 
  21,31, 21,31, 21,31, 21,31, 21,31, 21,31, 21,31, 21,31, 
  21,33, 21,33, 21,33, 21,33, 21,33, 21,33, 21,33, 21,33, 
  // Climax massimo
  21,35, 21,35, 21,35, 21,35, 21,35, 21,35, 21,35, 21,35, 
  21,33, 21,33, 21,33, 21,33, 21,33, 21,33, 21,33, 21,33, 
  // Discesa
  21,31, 21,31, 21,31, 21,31, 21,31, 21,31, 21,31, 21,31, 
  21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 21,29, 
  21,28, 21,28, 21,28, 21,28, 21,28, 21,28, 21,28, 21,28, 
  // Finale
  28, 21
};
const int m2_dur[] = {
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  150,150,150,150,150,150,150,150, 150,150,150,150,150,150,150,150,
  1200, 2000
};
const int m2_len = sizeof(m2_notes)/sizeof(m2_notes[0]);

void setup() {
  Serial.begin(115200);   
  Serial1.begin(115200);  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_EXT_PIN, OUTPUT);
  for (int i = 0; i < NUM_NOTE_LEDS; i++) {
    pinMode(NOTE_LEDS[i], OUTPUT); digitalWrite(NOTE_LEDS[i], LOW);
  }
  for (int i = 0; i < ROWS; i++) pinMode(rowPins[i], INPUT_PULLUP);
  for (int i = 0; i < COLS; i++) pinMode(colPins[i], INPUT_PULLUP);
  Serial.println(F("=== PIANO INTERSTELLARE v2.4 ==="));
}

void playSingleNote(int noteIdx) {
  if (noteIdx < 0 || noteIdx >= 36) return;
  Serial1.print(F("N,")); Serial1.print(NOTE_NAMES[noteIdx]); Serial1.print(F(",")); Serial1.println(NOTE_FREQS[noteIdx]);
  tone(BUZZER_PIN, NOTE_FREQS[noteIdx], 400);
  int ledIdx = noteToLed(noteIdx);
  if (ledIdx >= 0) digitalWrite(NOTE_LEDS[ledIdx], HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(300);
  if (ledIdx >= 0) digitalWrite(NOTE_LEDS[ledIdx], LOW);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
}

void playChord(int root, int type) {
  int thirdNote = root + ((type == 0) ? 4 : 3);
  int fifthNote = root + 7;
  Serial1.print(F("C,")); Serial1.print(NOTE_NAMES[root]);
  if(type == 0) Serial1.println(F("maj")); else Serial1.println(F("min"));
  
  int led1 = noteToLed(root); int led2 = noteToLed(thirdNote); int led3 = noteToLed(fifthNote);
  if (led1 >= 0) digitalWrite(NOTE_LEDS[led1], HIGH);
  if (led2 >= 0) digitalWrite(NOTE_LEDS[led2], HIGH);
  if (led3 >= 0) digitalWrite(NOTE_LEDS[led3], HIGH);
  digitalWrite(LED_EXT_PIN, HIGH);
  
  for (int i = 0; i < 3; i++) {
    digitalWrite(NOTE_LEDS[7], HIGH); tone(BUZZER_PIN, NOTE_FREQS[root], 100); delay(120); digitalWrite(NOTE_LEDS[7], LOW);
    digitalWrite(NOTE_LEDS[7], HIGH); tone(BUZZER_PIN, NOTE_FREQS[thirdNote], 100); delay(120); digitalWrite(NOTE_LEDS[7], LOW);
    digitalWrite(NOTE_LEDS[7], HIGH); tone(BUZZER_PIN, NOTE_FREQS[fifthNote], 100); delay(120); digitalWrite(NOTE_LEDS[7], LOW);
  }
  tone(BUZZER_PIN, NOTE_FREQS[root], 500); delay(450); noTone(BUZZER_PIN);
  
  if (led1 >= 0) digitalWrite(NOTE_LEDS[led1], LOW);
  if (led2 >= 0) digitalWrite(NOTE_LEDS[led2], LOW);
  if (led3 >= 0) digitalWrite(NOTE_LEDS[led3], LOW);
  digitalWrite(LED_EXT_PIN, LOW); digitalWrite(NOTE_LEDS[7], LOW);
}

void playMelody(int melodyId) {
  if (isPlaying) return;
  isPlaying = true;
  const int* notes; const int* durations; int len;
  
  if (melodyId == 1) { notes = m1_notes; durations = m1_dur; len = m1_len; } 
  else if (melodyId == 2) { notes = m2_notes; durations = m2_dur; len = m2_len; } 
  else { isPlaying = false; return; }
  
  for (int i = 0; i < len; i++) {
    if (notes[i] < 0) {
      noTone(BUZZER_PIN);
      for (int j = 0; j < NUM_NOTE_LEDS; j++) digitalWrite(NOTE_LEDS[j], LOW);
      digitalWrite(LED_PIN, LOW);
      delay(durations[i]);
    } else {
      for (int j = 0; j < NUM_NOTE_LEDS; j++) digitalWrite(NOTE_LEDS[j], LOW);
      Serial1.print(F("N,")); Serial1.print(NOTE_NAMES[notes[i]]); Serial1.print(F(",")); Serial1.println(NOTE_FREQS[notes[i]]);
      tone(BUZZER_PIN, NOTE_FREQS[notes[i]], durations[i] - 30);
      int ledIdx = noteToLed(notes[i]);
      if (ledIdx >= 0) digitalWrite(NOTE_LEDS[ledIdx], HIGH);
      digitalWrite(LED_PIN, HIGH);
      delay(durations[i] - 30);
      noTone(BUZZER_PIN);
      if (ledIdx >= 0) digitalWrite(NOTE_LEDS[ledIdx], LOW);
      digitalWrite(LED_PIN, LOW);
      delay(30);
    }
    
    char key = keypad.getKey();
    if (key == NO_KEY && Serial1.available() > 0) {
      char c = Serial1.read();
      if (c != '\n' && c != '\r') key = c;
    }
    if (key == 'D') {
      noTone(BUZZER_PIN);
      for (int j = 0; j < NUM_NOTE_LEDS; j++) digitalWrite(NOTE_LEDS[j], LOW);
      isPlaying = false;
      return;
    }
  }
  for (int j = 0; j < NUM_NOTE_LEDS; j++) digitalWrite(NOTE_LEDS[j], LOW);
  digitalWrite(LED_PIN, LOW);
  isPlaying = false;
}

void handleKey(char key) {
  for (int i = 0; i < 14; i++) {
    if (keyMap[i].key == key) {
      if (keyMap[i].noteIdx >= 0) playSingleNote(keyMap[i].noteIdx);
      else playChord(keyMap[i].chordRoot, keyMap[i].chordType);
      return;
    }
  }
}

void loop() {
  if (isPlaying) return;
  char key = keypad.getKey();
  
  if (key == NO_KEY && Serial1.available() > 0) {
    char c = Serial1.read();
    if (c != '\n' && c != '\r') key = c;
  }
  
  if (key != NO_KEY) {
    if (key == '0') {
      currentMelody++;
      if (currentMelody > 2) currentMelody = 1;
      delay(200); playMelody(currentMelody); return;
    }
    if (key == 'E') { playMelody(1); return; } 
    if (key == 'F') { playMelody(2); return; } 
    if (key == 'D') { noTone(BUZZER_PIN); return; }
    handleKey(key);
  }
  delay(20);
}
