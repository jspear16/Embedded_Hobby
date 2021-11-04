/*  NOTES
  Distance = Velocity*Time
  Speed of Sound = 340.29 m/s = 0.034 cm/us = 0.34 mm/us
  1ft = 30.48cm = 304.8mm
*/

// Definitions
#define TRIG_PIN        (3)
#define ECHO_PIN        (2)
#define SEG_A           (12)
#define SEG_B           (8)
#define SEG_C           (5)
#define SEG_D           (4)
#define SEG_E           (14)
#define SEG_F           (11)
#define SEG_G           (6)
#define SEG_MSD         (13)
#define SEG_2ND_MSD     (10)
#define SEG_2ND_LSD     (9)
#define SEG_LSD         (7)
#define BUZZER          (15)
#define WARNING_LED     (16)

#define DELAY_TIME               (3)
#define ERROR_DISTANCE_TONE      (6000)
#define COUNTER_MAX              (15)

// Globals
static int _distance = 0;
static long _duration = 0;
static int leastDigit = 0;
static int thirdMostDigit = 0;
static int secondMostDigit = 0;
static int mostDigit = 0;
static int counter = 0;


// Function Prototypes
int getDistance();
int cm_to_ft(int dist_in_cm);
void getDigits(int number);
void switchDisplay(int number);
void setBuzzerPitch(int number);
void display0();
void display1();
void display2();
void display3();
void display4();
void display5();
void display6();
void display7();
void display8();
void display9();
void displayErr();
void displayAllOff();

// Setup 
void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_MSD, OUTPUT);
  pinMode(SEG_2ND_MSD, OUTPUT);
  pinMode(SEG_2ND_LSD, OUTPUT);
  pinMode(SEG_LSD, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(WARNING_LED, OUTPUT);
}


// Loop
void loop() {
  int dist = getDistance(); // Returns distance in mm

  if(dist >= 0 && dist < 4500){
    
    // Write Number on far left
    displayAllOff();
    digitalWrite(SEG_MSD, LOW);
    switchDisplay(dist/1000);
    delay(DELAY_TIME);

    // Write Number on middle left
    displayAllOff();
    digitalWrite(SEG_2ND_MSD, LOW);
    switchDisplay((dist%1000)/100);
    delay(DELAY_TIME);
    
    // Write Number on middle right
    displayAllOff();
    digitalWrite(SEG_2ND_LSD, LOW);
    switchDisplay((dist%100)/10);
    delay(DELAY_TIME);

    // Write Number on far right
    displayAllOff();
    digitalWrite(SEG_LSD, LOW);
    switchDisplay(dist%10);
    delay(DELAY_TIME);
    
    if(dist > 65){
      analogWrite(WARNING_LED, 0);
      setBuzzerPitch(dist);
      counter = 0;
    }
    else if (counter < COUNTER_MAX){
      setBuzzerPitch(1200);
      analogWrite(WARNING_LED, 128);
      counter++;
    }
    else if (counter >= COUNTER_MAX && counter < 2*COUNTER_MAX){
      noTone(BUZZER);
      analogWrite(WARNING_LED, 0);
      counter++;
    }
    else{
      analogWrite(WARNING_LED, 0);
      counter = 0;
    }
  }
  else{
     // Write Error on far left
    displayAllOff();
    digitalWrite(SEG_MSD, LOW);
    displayErr();
    delay(DELAY_TIME);

    // Write Error on middle left
    displayAllOff();
    digitalWrite(SEG_2ND_MSD, LOW);
    displayErr();
    delay(DELAY_TIME);

    // Write Error on middle right
    displayAllOff();
    digitalWrite(SEG_2ND_LSD, LOW);
    displayErr();
    delay(DELAY_TIME);

    // Write Error on far right
    displayAllOff();
    digitalWrite(SEG_LSD, LOW);
    displayErr();
    delay(DELAY_TIME);


    if (counter < COUNTER_MAX){
      setBuzzerPitch(ERROR_DISTANCE_TONE);
      analogWrite(WARNING_LED, 128);
      counter++;
    }
    else if (counter >= COUNTER_MAX && counter < 2*COUNTER_MAX){
      noTone(BUZZER);
      analogWrite(WARNING_LED, 0);
      counter++;
    }
    else{
      analogWrite(WARNING_LED, 0);
      counter = 0;
    }
  }
  
  //Serial.print("The distance from the object is: ");
  //Serial.print(dist);
  //Serial.println("mm.");
  
  
  //delay(500);
}




// Functions for getting the distance
int getDistance(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  _duration = pulseIn(ECHO_PIN, HIGH);    // Returns duration of the HIGH in us
  _distance = (_duration * 0.34) / 2.0;   // Convert into mm

  return _distance;
}

int cm_to_ft(int dist_in_cm){
  float temp = ((float)dist_in_cm)/30.48;
  return (int)temp;
}

// Functions to get Digit information
void getDigits(int number){
  // Get LSD
  leastDigit = number % 10;
  number /= 10;

  // Get 3rd MSD
  thirdMostDigit = number % 10;
  number /= 10;

  // Get 2nd MSD
  secondMostDigit = number % 10;
  number /= 10;

  // Get MSD
  mostDigit = number % 10;
}

void setBuzzerPitch(int number){
  noTone(BUZZER);
  float rate = (5000.0 / 4500.0);
  int desiredTone = (int)rate * number + 250;
  tone(BUZZER,desiredTone);
}

void switchDisplay(int number){
  switch (number){
    case 0:
      display0();
      break;
    case 1:
      display1();
      break;
    case 2:
      display2();
      break;
    case 3:
      display3();
      break;
    case 4:
      display4();
      break;
    case 5:
      display5();
      break;
    case 6:
      display6();
      break;
    case 7:
      display7();
      break;
    case 8:
      display8();
      break;
    case 9:
      display9();
      break;
    default:
      displayErr();
      break;
  }
}


// Functions for the 7-Seg Display

void display0(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  analogWrite(SEG_E, 128);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, LOW);
}

void display1(){
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  analogWrite(SEG_E, 0);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
}

void display2(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, HIGH);
  analogWrite(SEG_E,128);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, HIGH);
}

void display3(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  analogWrite(SEG_E, 0);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, HIGH);
}

void display4(){
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  analogWrite(SEG_E, 0);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void display5(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  analogWrite(SEG_E, 0);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void display6(){
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  analogWrite(SEG_E,128);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void display7(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  analogWrite(SEG_E, 0);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, LOW);
}

void display8(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  analogWrite(SEG_E,128);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void display9(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  analogWrite(SEG_E, 0);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void displayErr(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, HIGH);
  analogWrite(SEG_E,128);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void displayAllOff(){
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  analogWrite(SEG_E,0);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_MSD, HIGH);
  digitalWrite(SEG_2ND_MSD, HIGH);
  digitalWrite(SEG_2ND_LSD, HIGH);
  digitalWrite(SEG_LSD, HIGH);
}
