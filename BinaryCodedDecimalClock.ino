byte binaryClock[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
int myMatrix[8][8] = { 0 };

int hours = 0;
int minutes = 0;
int seconds = 0;

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
    updateTimeMatrix();
    matrixToByteArray();
    refreshLEDMatrix(); 
  }
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

void updateTimeMatrix() {
  intToBCDMatrix(hours, 0);
  intToBCDMatrix(minutes, 3);
  intToBCDMatrix(seconds, 6);
}

void intToBCDMatrix(int num, int col) {
  int tens = num / 10;
  int ones = num % 10;
  // Convert tens and ones to 4-bit binary (BCD)
  for (int i = 3; i >= 0; i--) {
    myMatrix[i + 4][col] = (tens >> i) & 1;      // Extract each bit of 'tens'
    myMatrix[i + 4][col + 1] = (ones >> i) & 1;  // Extract each bit of 'ones'
  }
}

void matrixToByteArray() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if ((bool)myMatrix[i][j]) {          // If the element is 1 (true)
        binaryClock[i] |= (1 << (7 - j));  // Set the corresponding bit
      }
    }
  }
}

void refreshLEDMatrix() {
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
  digitalWrite(y + 10, LOW);
  delay(1);
}