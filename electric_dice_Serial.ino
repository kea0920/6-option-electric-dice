/* LED REPRESENTATION */
// red -> 4 faces
// orange -> 6 faces
// yellow -> 8 faces
// green -> 10 faces
// blue -> 12 faces
// purple -> 20 faces

const int redLED = A2;
const int greenLED = A4; 
const int blueLED = A3; 

int colorIndex = 0;
int diceIndex = 0; 

// button constants
const int pickButtonPin = A0;
const int rollButtonPin = A1;  

int pickState = 0;
int lastPickState = 0;
int pickCounter = 0;

int rollState = 0;
int lastRollState = 0;
int rollCounter = 0;

/* 555 TIMER PULSE CONSTANTS */
const byte pulsePin = A5;
unsigned long pulseHigh = 0;
unsigned long pulseLow = 0;
unsigned long pulseTotal = 0;

// number generator
int firstNum = 0;
int lastFinalNum = 0;
int finalNum = 0;

/* ARRAYS */
// display constants
// ones
int ones[7] = { 0,    // A
                1,    // B
                2,    // C
                3,    // D
                4,    // E
                5,    // F
                6 };  // G

// tens
int tens[7] = { 7,    // A
                8,    // B
                9,    // C
                10,   // D
                11,   // E
                12,   // F
                13 }; // G

int allNumbers[20] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                       11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

int digitArray[11][7] = { { 1,1,1,1,1,1,0 },    // 0
                          { 0,1,1,0,0,0,0 },    // 1
                          { 1,1,0,1,1,0,1 },    // 2
                          { 1,1,1,1,0,0,1 },    // 3
                          { 0,1,1,0,0,1,1 },    // 4
                          { 1,0,1,1,0,1,1 },    // 5
                          { 1,0,1,1,1,1,1 },    // 6
                          { 1,1,1,0,0,0,0 },    // 7
                          { 1,1,1,1,1,1,1 },    // 8
                          { 1,1,1,0,0,1,1 },    // 9
                          { 0,0,0,0,0,0,0 }};   // blank
                          
int colorArray[7][3] = {  { 80, 0, 80 },    // purple
                          { 255, 0, 0 },    // red
                          { 255, 255, 0 },  // yellow
                          { 0, 255, 0 },    // green
                          { 0, 255, 255 },  // aqua
                          { 0, 0, 255 },    // blue
                          { 0, 0, 0 }};     // unlit

int diceArray[7] = { 20, 4, 6, 8, 10, 12 };

void setup() {
  // serial
  Serial.begin(9600);
  Serial.println("initialize");
  
  // inputs
  pinMode(pickButtonPin, INPUT);
  pinMode(rollButtonPin, INPUT);
  pinMode(pulsePin, INPUT);

  // outputs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  // display pins
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);   
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);   
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  setColor(1); // shows red -> setting roll speed incomplete
  Serial.println("calculating roll speed");
  generateFirstNumber();
  Serial.println("finished calculating roll speed");
  setColor(6); // shows clear -> setting roll speed complete
}

void loop() {
  // read state of button:
  pickState = digitalRead(pickButtonPin);
  rollState = digitalRead(rollButtonPin);

  comparePickStates();
  compareRollStates();
}

void chooseDiceType(){
  colorIndex = pickCounter % 6;
  diceIndex = colorIndex;
  
  if (colorIndex == 0) {
    if (pickCounter == 0){
      // show nothing
      setColor(7);
    } else {
      // purple LED
       setColor(colorIndex);
    }
  }

  else {
    setColor(colorIndex);
  }
}

void generateFirstNumber() {
  pulseLow = pulseIn(pulsePin, LOW, 5000000);
  pulseHigh = pulseIn(pulsePin, HIGH, 5000000);
  
  pulseTotal = pulseHigh + pulseLow;
  firstNum = pulseTotal/1000;

  String one = "pulseHigh: ";
  String two = one + pulseHigh;
  String three = "pulseLow: ";
  String four = three + pulseLow;
  
  String five = "pulseTotal: ";
  String six = five + pulseTotal;
  
  Serial.println(two);
  Serial.println(four);
  Serial.println(six);
  Serial.println(firstNum);
  Serial.println("");
  
}

void generateRandomNumber(int diceFaces) {  
  int secondNum = 0;
  int finalIndex = 0;
  
  secondNum = random(firstNum);
  finalIndex = secondNum % diceFaces;
  finalNum = allNumbers[finalIndex];
    
  String seven = "finalNum: ";
  String eight = seven + finalNum;
  String nine = "lastFinalNum: ";
  String ten = nine + lastFinalNum;
  Serial.println(ten);
  Serial.println(eight);
  
  lastFinalNum = finalNum; // updates previous final number with current
}

void checkRandomNumber() {
  if (finalNum == lastFinalNum) {
    generateRandomNumber(diceArray[diceIndex]);
  }
}

void displayNumber(int num) {
  int onesDigit = 0;
  int tensDigit = 0;
  
  if (num > 9 && num < 20) { // numbers 10 to 19 inclusive
    onesDigit = num - 10;
    tensDigit = 1;
  } else if (num == 20) { // number = 20
    onesDigit = 0;
    tensDigit = 2;
  } else { // numbers less than 10
    onesDigit = num;
    tensDigit = 0;
  }
  
  onesWrite(onesDigit);
  tensWrite(tensDigit);
}

void setColor(int index) {
  analogWrite(redLED, colorArray[index][0]);
  analogWrite(greenLED, colorArray[index][1]);
  analogWrite(blueLED, colorArray[index][2]);
}

void onesWrite(int number) {
  int pinIndex = 0;
  for (int j = 0; j < 7; j++) {
    digitalWrite(ones[pinIndex], digitArray[number][j]);
    pinIndex++;
  }
}

void tensWrite(int number) {
  int pinIndex = 0;
  for (int j = 0; j < 7; j++) {
    digitalWrite(tens[pinIndex], digitArray[number][j]);
    pinIndex++;
  }
}

void comparePickStates(){
  if (pickState != lastPickState) {
    // increment counter if state changes
    if (pickState == HIGH) {
      pickCounter++;
      chooseDiceType();
      Serial.println("on");
      Serial.println("number of button pushes: ");
      Serial.println(pickCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // delay to avoid bouncing
    delay(50);
  }
  // update last state with current state, for next time through the loop
  lastPickState = pickState;
}

void compareRollStates(){
  if (rollState != lastRollState) {
    if (rollState == HIGH) {
      Serial.println("rolling dice");
      generateRandomNumber(diceArray[diceIndex]);
      checkRandomNumber();
      displayNumber(finalNum);
    }
    // delay to avoid bouncing
    delay(50);
  }
  // update last state with current state, for next time through the loop
  lastRollState = rollState;
}
