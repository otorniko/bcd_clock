byte binaryClock[8] = { 0 };
int timeMatrix[8][8] = { 0 };

int hours = 13;
int minutes = 36;
int seconds = 45;

void setup() {
  for (int i = 2; i < 18; i++) {
    pinMode(i, OUTPUT);
  }
  clear_();
}

void clear_(void) {
  for (int i = 2; i < 10; i++) {
    digitalWrite(i, LOW);
  }
  for (int i = 0; i < 8; i++) {
    digitalWrite(i + 10, HIGH);
  }
}

void loop() {
  static unsigned long previousMillis = 0;
  const unsigned long interval = 1000;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    updateClock();
    matrixToByteArray();
  }
  refreshLEDs();
}

void updateClock() {
  seconds++;
  if (seconds == 60) {
    seconds = 0;
    minutes++;
    if (minutes == 60) {
      minutes = 0;
      hours++;
      if (hours == 24) {
        hours = 0;
      }
    }
  }
}

void matrixToByteArray() {
  timeToMatrix(seconds, 0);
  timeToMatrix(minutes, 3);
  timeToMatrix(hours, 6);

  for (int i = 0; i < 8; i++) {
    binaryClock[i] = 0;
    for (int j = 0; j < 8; j++) {
      if ((bool)timeMatrix[i][j]) {
        binaryClock[i] |= (1 << j);
      }
    }
  }
}

void timeToMatrix(int num, int col) {
  int tens = num / 10;
  int ones = num % 10;
  for (int i = 0; i < 4; i++) { 
    timeMatrix[3 - i][col + 1] = (tens >> i) & 1;
    timeMatrix[3 - i][col] = (ones >> i) & 1;
  }
}

void refreshLEDs() {
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      if ((binaryClock[row] >> col) & 1) {
        Draw_point(col, row);
      }
    }
  }
} 

void Draw_point(unsigned char x, unsigned char y) {
  clear_();
  digitalWrite(x + 2, HIGH);
  digitalWrite(y + 14, LOW);
  delay(2);
}
