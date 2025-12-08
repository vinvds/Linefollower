// Motor A pins
const int PWMA = 5;
const int AIN1 = 7;
const int AIN2 = 8;

// Motor B pins
const int PWMB = 6;
const int BIN1 = 9;
const int BIN2 = 10;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // Kleine delay voor veiligheid
  delay(500);
}

void loop() {

  // --- Motor A vooruit ---
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 180); 
  delay(2000);
    // snelheid

  // --- Motor B vooruit ---
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 180);

  delay(2000);

  // --- Motoren stoppen ---
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  delay(100);

  // --- Motor A achteruit ---
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 180);

  // --- Motor B achteruit ---
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 180);

  delay(2000);

  // Stop
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  delay(200);
}
