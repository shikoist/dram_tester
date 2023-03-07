#define LED_RED    A1
#define LED_GREEN  A2
#define LED_DELAY  500
#define LED_POWER  128

#define SERIAL_SPEED 115200

#define STAGE_INIT_DELAY    0
#define STAGE_INITIALIZING  1
#define STAGE_TESTING       2
#define STAGE_RESULT        3
#define STAGE_LOOP_LED      4

#define ADDRESS_LENGTH 9
#define DATA_LENGTH 16
//#define MAX_ROWS 511
//#define MAX_COLUMNS 511
#define MAX_ROWS 4
#define MAX_COLUMNS 4

const int adr_pins[ADDRESS_LENGTH] = {25, 24, 23, 22, 21, 20, 19, 18, 17}; // ARDUINO PINS 
//const int data_pins[DATA_LENGTH] = {35, 34, 33, 32, 31, 30, 29, 28, 0, 1, 2, 3, 4, 5, 6, 7};  // ARDUINO PINS 
const int data_pins[DATA_LENGTH] = {35, 34, 33, 32, 31, 30, 29, 28, 37, 36, 2, 3, 4, 5, 6, 7};  // ARDUINO PINS 
const int RAS = 26; // Row Array Strobe -- ACTIVE = LOW 
const int LCAS = 14; // Column Array Strobe -- ACTIVE = LOW 
const int UCAS = 15;
const int OE = 16; // Output Enable -- ACTIVE = LOW  
const int WE = 27; // Write Enable -- ACTIVE = LOW 

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(RAS, OUTPUT);
  pinMode(LCAS, OUTPUT);
  pinMode(UCAS, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(WE, OUTPUT);

  for(int n = 0; n < ADDRESS_LENGTH; n++)
    pinMode(adr_pins[n], OUTPUT); 
  
  for(int n = 0; n < DATA_LENGTH; n++)
    pinMode(data_pins[n], OUTPUT);

  digitalWrite(RAS, HIGH);  // disable
  digitalWrite(LCAS,HIGH);  // disable 
  digitalWrite(UCAS,HIGH);  // disable 
  digitalWrite(WE,HIGH);    // disable 
  digitalWrite(OE,HIGH);    // disable 

  
  Serial.begin(SERIAL_SPEED);
  Serial.println(" *** DRAM 256K X 16 TESTER *** ");
  
  int randomInput = analogRead(A0);
  randomSeed(randomInput);
  Serial.println(" *** Random seed: " + String(randomInput) + " *** ");
  Serial.println(" *** Boot failure time delay... *** ");
}

int a1 = 128;
int a2 = 128;

int stage = 0;
long errors = 0;
long cells = 0;
#define TEST_VALUES_LENGTH 4
unsigned int testValues[TEST_VALUES_LENGTH] = {
  0,
  65535,
  43690,
  21845
}; // Patterns: 1111, 0000, 1010, 0101

int init_loops = 10;

void loop() {
  
  // put your main code here, to run repeatedly:
  if (stage == STAGE_INIT_DELAY)
  {
    analogWrite(LED_RED, LED_POWER);
    analogWrite(LED_GREEN, 0);
    delay(250);
    analogWrite(LED_RED, 0);
    analogWrite(LED_GREEN, LED_POWER);
    delay(250);
    
    init_loops--;
    if (init_loops < 0) {
      stage++;
    }
  }
  
  if (stage == STAGE_INITIALIZING)
  {
    Serial.println(" *** RAS Pulse... *** ");
    RAS_pulse();
  
    stage++;
  }

  if (stage == STAGE_TESTING)
  {
    Serial.println(" *** Testing... *** ");

    analogWrite(LED_GREEN, LED_POWER);
    fill();
    analogWrite(LED_GREEN, 0);
    
    stage++;
  }

  if (stage == STAGE_RESULT)
  {
    Serial.println(" *** All tests finished. *** ");
    Serial.end();
    stage++;
  }

  if (stage == STAGE_LOOP_LED)
  {
    if (errors > 0)
    {
      analogWrite(LED_RED, LED_POWER);
      delay(LED_DELAY);
      analogWrite(LED_RED, 0);
      delay(LED_DELAY);
    }
    else
    {
      analogWrite(LED_GREEN, LED_POWER);
      delay(LED_DELAY);
      analogWrite(LED_GREEN, 0);
      delay(LED_DELAY);
    }
  }
}

void RAS_pulse()
{
  //noInterrupts();
  for (int i = 0; i < 512; i++) {
    digitalWrite(RAS, LOW);
    digitalWrite(RAS, HIGH);
  }
  //interrupts();
}

//**** fill **** 
void fill()
{
//  long addr;
//  unsigned int rdValue = 0;
//  unsigned int wrValue = 0;
//
//  //random cells
//  errors = 0;
//  cells = 0;

//  bool cycleBreak = false;
//
//  for (int k = 0; k < 1; k++) {
//    if (cycleBreak) break;
//    for (int i = 0; i < 128; i++) {
//      for (int j = 0; j < 128; j++) {
//        //long row = random(0, MAX_ROWS);
//        //long column = random(0, MAX_COLUMNS);
//        int row = i;
//        int column = j;
//        int cellErrors = 0;
//        Serial.println("[" + String(row) + ", " + String(column) + "]");
//        
//        for (int i = 0; i < TEST_VALUES_LENGTH; i++) {
//          wrValue = testValues[i];
//          
//          writeValue(row, column, wrValue, true);
//          rdValue = readValue(row, column, true);
//          if (rdValue != wrValue) {
//            cellErrors++;
//          }
//        }
//        if (cellErrors > 0) {
//          errors++;
//        }
//        else {
//          Serial.println("WOW! We have found right results. [" + String(row) + ", " + String(column) + "]");
//          cycleBreak = true;
//          break;
//        }
//        cells++;
//      }
//    }
//  }
//
//  if (errors > 0) {
//    Serial.println(" *** Test finished. Bad " + String(errors) + " cells from " + String(cells) + " cells found. *** ");
//  }
//  else {
//    Serial.println(" *** Test finished. No errors in " + String(cells) + " cells. *** ");
//  } 

  // Option like Scandisk
  // only "b" or "."

  // Start variant A, loop one cell
//  unsigned int rdValue = 0;
//  unsigned int wrValue = 0;
//  for (int j = 0; j < 65535; j++) {
//      writeValue(0, 0, j, true);
//      rdValue = readValue(0, 0, true);
//  }
  // End variant A


  // Start variant B, default

  // Set address check
  Serial.println("Addressing check:");

  unsigned int aTestValues[8] = {
        0,
    65535,
    43690,
    21845,
        0,
    65535,
    43690,
    21845
  };

  for (int i = 0; i < 8; i++) {
    writeValue(0, i, aTestValues[i], false);
  }

//  digitalWrite(RAS, HIGH);
//  digitalWrite(UCAS, HIGH);
//  digitalWrite(LCAS, HIGH);  

  unsigned int aReadValues[8];

  // Read value and clear buffer
  readValue(0, 0, true);
  
  for (int i = 0; i < 8; i++)
    aReadValues[i] = readValue(0, i+1, true);

  bool aCheck = false;

  for (int i = 0; i < 8; i++)
    if (aReadValues[i] != aTestValues[i])
      aCheck = true;
  
  if (aCheck == true)
  {
    Serial.println("ERROR! Addressing failed.");
    for (int i = 0; i < 8; i++)
    {
      Serial.println("Read " + String(aReadValues[i]) + " should be " + String(aTestValues[i]));
    }
    errors = 1;
    stage = STAGE_RESULT;
    return;
  }
  else 
  {
    Serial.println("SUCCESS! Addressing completed without errors.");
  }
  // End addressing check
  
  unsigned int rdValue = 0;
  unsigned int wrValue = 0;

  errors = 0;
  cells = 0;
  errors = 0;
  cells = 0;
  bool cycleBreak = false;
  Serial.println("ROWS:");
  for (int row = 0; row <= MAX_ROWS; row++) {
  //for (int row = 0; row <= 20; row++) {
    if (cycleBreak) break;
    Serial.print(String(row) + " ");
    for (int column = 0; column <= MAX_COLUMNS; column++) {
      if (cycleBreak) break;
      int cellErrors = 0;
      for (int i = 0; i < TEST_VALUES_LENGTH + 2; i++) {
        if (i < TEST_VALUES_LENGTH)
          wrValue = testValues[i];
        else if (i == TEST_VALUES_LENGTH)
          wrValue = row;
        else if (i == TEST_VALUES_LENGTH + 1)
          wrValue = column;
        writeValue(row, column, wrValue, false);
        rdValue = readValue(row, column, false);
        
        if (rdValue != wrValue) {
          Serial.println("0 write " + String(wrValue) + " -> read " + String(rdValue));  
          cellErrors++;
        }
      }
      if (cellErrors > 0)
      {
        Serial.println(" *** Error in " + String(row) + "," + String(column));
        cycleBreak = true;
        errors++;

        for (int i = 0; i < TEST_VALUES_LENGTH; i++) {
          wrValue = testValues[i];
          writeValue(row, column, wrValue, true);
          rdValue = readValue(row, column, true);
          Serial.println("1 write " + String(wrValue) + " -> read " + String(rdValue));
        }
      }
      else
      {
        //Serial.print(".");
      }
      cells++;
    }
  }
  if (errors > 0)
  {
    Serial.println(" *** Test finished. Bad " + String(errors) + " cells from " + String(cells) + " cells found. *** ");
  }
  else 
  {
    Serial.println(" *** Test finished. No errors in " + String(cells) + " cells. *** ");
  }
  // End variant B

  // verbose
//  if (false)
//  {
//
//    errors = 0;
//    cells = 0;
//    
//    for (int row = 0; row < 8; row++) {
//      for (int column = 0; column < 8; column++) {
//        int cellErrors = 0;
//        for (int i = 0; i < TEST_VALUES_LENGTH; i++) {
//          Serial.print("[" + String(row) + "," + String(column) + "]");
//          wrValue = testValues[i];
//          writeValue(row, column, wrValue, true);
//          rdValue = readValue(row, column, true);
//          
//          if (rdValue != wrValue) {
//            cellErrors++;
//          }
//        }
//        if (cellErrors > 0)
//        {
//          //Serial.print("B");
//          errors++;
//        }
//        else
//        {
//          //Serial.print(".");
//        }
//        cells++;
//      }
//    }
//    if (errors > 0)
//    {
//      Serial.println(" *** Test finished. Bad " + String(errors) + " cells from " + String(cells) + " cells found. *** ");
//    }
//    else 
//    {
//      Serial.println(" *** Test finished. No errors in " + String(cells) + " cells. *** ");
//    }
//  }
} //**** fill **** 

void standBy()
{
  // stand by
  //digitalWrite(RAS, HIGH);
  //digitalWrite(UCAS, HIGH);
  //digitalWrite(LCAS, HIGH);
  //digitalWrite(WE, HIGH);
  //digitalWrite(OE, HIGH);
  
  digitalWrite(RAS, LOW);
  digitalWrite(UCAS, LOW);
  digitalWrite(LCAS, LOW);
  digitalWrite(WE, LOW);
  digitalWrite(OE, LOW);  
} // *** stand by ***

// set values 0-1 on address bus
void setAddressBus(unsigned int address) {
  for (int i = 0; i < ADDRESS_LENGTH; i++) {
    digitalWrite(adr_pins[i], bitRead(address, i));
  }
} // *** setAddressBus ***

void setAddress(unsigned int row, unsigned int column) {
  setAddressBus(row);
  //digitalWrite(RAS, LOW);
  digitalWrite(RAS, HIGH);

  setAddressBus(column);
  //digitalWrite(UCAS, LOW);
  //digitalWrite(LCAS, LOW);
  digitalWrite(UCAS, HIGH);
  digitalWrite(LCAS, HIGH);

} // *** setAddress ***

void writeValue(unsigned int row, unsigned  int column, unsigned int value, bool dbg)
{ 
  standBy();
  
  setAddress(row, column);
  // HINT: LOW - ENABLE, HIGH - DISABLE
  // allow writing
  digitalWrite(WE, LOW);
  digitalWrite(OE, HIGH);
  //digitalWrite(WE, HIGH);
  //digitalWrite(OE, LOW);
  //delay(5);
  // set values 0-1 on data bus
  for (int n = 0; n < DATA_LENGTH; n++) {
    pinMode(data_pins[n], OUTPUT);
    digitalWrite(data_pins[n], bitRead(value, n));
  }
  //standBy();
  //delay(5);
  if (dbg) {
    Serial.print("w:");
    for (int n = 0; n < DATA_LENGTH; n++) {
      Serial.print(String(bitRead(value, n)));
    }
    //Serial.println("");
  }
} // end writeValue

unsigned int readValue(int row, int column, bool dbg) { 
  standBy();
  
  setAddress(row, column);
  // HINT: LOW - ENABLE, HIGH - DISABLE
  // allow reading (chip enables output)
  digitalWrite(WE, HIGH);
  digitalWrite(OE, LOW); 
  //digitalWrite(WE, LOW);
  //digitalWrite(OE, HIGH);
  //7 READ DATA 
  bool rValue[DATA_LENGTH];
  for (int n = 0; n < DATA_LENGTH; n++) {
    pinMode(data_pins[n], INPUT);
    rValue[n] = digitalRead(data_pins[n]);
  }
  //standBy();
  if (dbg) {
    Serial.print("r:");
    for (int n = 0; n < DATA_LENGTH; n++) {
      Serial.print(String(rValue[n]));
    }
    Serial.println("");
  }
  
  unsigned int value = 0;
  for (int n = 0; n < DATA_LENGTH; n++) {
    value = value + rValue[n] * (1 << n);
  }
  
  return value;
} // readValue 
