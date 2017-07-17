/*
//Test RAMPS board 
//Ionel Ciobanuc
*/

#define X_MIN_PIN           3
#define X_MAX_PIN           2
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19
//
// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24
#define E0_CS_PIN          42

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30
#define E1_CS_PIN          44

//
// Temperature Sensors
//
#define TEMP_0_PIN         13   // Analog Input
#define TEMP_1_PIN         15   // Analog Input
#define TEMP_BED_PIN       14   // Analog Input
//
//Hotend, Hotend, Bed 
//
#define HEATER_0_PIN     10 
#define HEATER_1_PIN     9
#define HEATER_2_PIN     8 

#define LED_PIN            13
#define PS_ON_PIN          12 

//*******************************
//redefine
/*
#undef TEMP_0_PIN
#define TEMP_0_PIN          11   // Analog Input
#undef X_MIN_PIN
#define X_MIN_PIN          3   // Analog Input
#undef Y_MIN_PIN
#define Y_MIN_PIN          2   // Analog Input
#undef Z_MAX_PIN
#define Z_MAX_PIN          18   // Analog Input
*/
 
 
//local variable. Not modify.
int TEMP_0_VAL=0;
int TEMP_1_VAL=0;
int TEMP_2_VAL=0;
int X_MIN_VAL=0;
int X_MAX_VAL=0;
int Y_MIN_VAL=0;
int Y_MAX_VAL=0;
int Z_MIN_VAL=0;
int Z_MAX_VAL=0;
int pasi =1;
int x_pasi=0;
int y_pasi=0;
int z_pasi=0;
int s_pasi=1;
int h0_val=0;
int h1_val=0;
int h2_val=0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
    Serial.begin(250000);
    
    inputString.reserve(200);
    set_pini();
      //
      //custom set pin for power endstop
      //
 /*
      pinMode(15, OUTPUT);
      digitalWrite(15, LOW); 
      pinMode(14, OUTPUT);
      digitalWrite(14, HIGH); 
 */
      //end custom
      
    digitalWrite(PS_ON_PIN    , LOW);
    Serial.println("Test RAMPS board");
    Serial.println("Check the endstop and temperature sensors, move steppers,ON OFF HEATER");
    Serial.println("Send serial command ");
    Serial.println("x=nr (nr- The number of steps for X)");
    Serial.println("x=-nr (The number of steps in the opposite direction)");
    Serial.println("y=nr or y=-nr or z=nr or z=-nr");
    Serial.println("Ex:    x=1000");
    Serial.println("Ex:    x=-1000y1000");
    Serial.println("Ex:    x=-500y-500z-500");
    Serial.println("s=1 Time between two steps");
    Serial.println("h0=1 digitalWrite(HEATER_0_PIN,h0)");
    Serial.println("h1=1 digitalWrite(HEATER_1_PIN,h1)");
    Serial.println("h2=0 digitalWrite(HEATER_2_PIN,h2)");
    Serial.println("------------------------------------------------------");
}


void loop () {
    if (stringComplete) {
      Serial.println(inputString);
      char charBuf[100];
      inputString.toCharArray(charBuf, 100);
      sscanf(charBuf, "x=%i", &x_pasi);
      sscanf(charBuf, "y=%i", &y_pasi);
      sscanf(charBuf, "z=%i", &z_pasi);
      sscanf(charBuf, "s=%i", &s_pasi);
      sscanf(charBuf, "x=%iy=%i", &x_pasi,&y_pasi);
      sscanf(charBuf, "y=%iz=%i", &y_pasi,&z_pasi);
      sscanf(charBuf, "x=%iy=%iz=%i", &x_pasi,&y_pasi,&z_pasi);
      if (1==sscanf(charBuf, "h0=%i", &h0_val)){digitalWrite(HEATER_0_PIN, h0_val);Serial.println("h0=");Serial.println(h0_val);} 
      if (1==sscanf(charBuf, "h1=%i", &h1_val)){digitalWrite(HEATER_1_PIN, h1_val);Serial.println("h1=");Serial.println(h1_val);} 
      if (1==sscanf(charBuf, "h2=%i", &h2_val)){digitalWrite(HEATER_2_PIN, h2_val);Serial.println("h2=");Serial.println(h2_val);} 
      
      inputString = "";
      stringComplete = false;
  }
  misc();
  temperatura();
  endstop();
}

  
void misc(){
      delay(s_pasi);
    digitalWrite(X_ENABLE_PIN    , LOW);
    digitalWrite(Y_ENABLE_PIN    , LOW);
    digitalWrite(Z_ENABLE_PIN    , LOW);
  if (x_pasi!=0){
      digitalWrite(X_DIR_PIN    ,x_pasi>0?HIGH:LOW);
      digitalWrite(X_STEP_PIN    , HIGH);
      delay(1);
      digitalWrite(X_STEP_PIN    , LOW);  
      x_pasi+=x_pasi>0?-1:1;
        Serial.print("x");
    }
  if (y_pasi!=0){
      digitalWrite(Y_DIR_PIN    ,y_pasi>0?HIGH:LOW);
      digitalWrite(Y_STEP_PIN    , HIGH);
      delay(1);
      digitalWrite(Y_STEP_PIN    , LOW);  
      y_pasi+=y_pasi>0?-1:1;
        Serial.print("y");
    }
  if (z_pasi!=0){
      digitalWrite(Z_DIR_PIN    ,z_pasi>0?HIGH:LOW);
      digitalWrite(Z_STEP_PIN    , HIGH);
      delay(1);
      digitalWrite(Z_STEP_PIN    , LOW);  
      z_pasi+=z_pasi>0?-1:1;
        Serial.print("z");
    }
    if (x_pasi+y_pasi+z_pasi==0){
    digitalWrite(X_ENABLE_PIN    , HIGH);
    digitalWrite(Y_ENABLE_PIN    , HIGH);
    digitalWrite(Z_ENABLE_PIN    , HIGH);
    }
  }

void endstop(){
  int sensorVal=0;  
  if (X_MIN_PIN>0){
     sensorVal = digitalRead(X_MIN_PIN);
     if (X_MIN_VAL!=sensorVal){
         X_MIN_VAL=sensorVal;
         x_pasi=0;
         afisez();
      }
    }
  if (X_MAX_PIN>0){
     sensorVal = digitalRead(X_MAX_PIN);
     if (X_MAX_VAL!=sensorVal){
         X_MAX_VAL=sensorVal;
        x_pasi=0;
        afisez();
      }
    }
  //Y
  if (Y_MIN_PIN>0){
     sensorVal = digitalRead(Y_MIN_PIN);
     if (Y_MIN_VAL!=sensorVal){
         Y_MIN_VAL=sensorVal;
        y_pasi=0;
         afisez();
      }
    }
  if (Y_MAX_PIN>0){
     sensorVal = digitalRead(Y_MAX_PIN);
     if (Y_MAX_VAL!=sensorVal){
         Y_MAX_VAL=sensorVal;
        y_pasi=0;
         afisez();
      }
    }
  //Z
  if (Z_MIN_PIN>0){
     sensorVal = digitalRead(Z_MIN_PIN);
     if (Z_MIN_VAL!=sensorVal){
         Z_MIN_VAL=sensorVal;
        z_pasi=0;
         afisez();
      }
    }
  if (Z_MAX_PIN>0){
     sensorVal = digitalRead(Z_MAX_PIN);
     if (Z_MAX_VAL!=sensorVal){
         Z_MAX_VAL=sensorVal;
        z_pasi=0;
         afisez();
      }
    }
  
    
}
void temperatura(){
  int val=0;
  if (TEMP_0_PIN>0){
    val = analogRead(TEMP_0_PIN);
    val=(val+1)/100;
    if (TEMP_0_VAL!=val){
         //Serial.print(TEMP_0_VAL);
         //Serial.print("t");
         //Serial.println(val);
      TEMP_0_VAL=val;
      afisez();
    }
  }

 if (TEMP_1_PIN>0){
    val = analogRead(TEMP_1_PIN);
    val=(val+1)/100;
    if (TEMP_1_VAL!=val){
      TEMP_1_VAL=val;
      afisez();
    }
  }

   if (TEMP_BED_PIN>0){
    val = analogRead(TEMP_BED_PIN);
    val=(val+1)/100;
    if (TEMP_2_VAL!=val){
      TEMP_2_VAL=val;
      afisez();
    }
  }
 
}
void afisez(){
  Serial.print("min x=");
  Serial.print(X_MIN_VAL);
  Serial.print(",y=");
  Serial.print(Y_MIN_VAL);
  Serial.print(",z=");
  Serial.print(Z_MIN_VAL);

  Serial.print(" MAX X=");
  Serial.print(X_MAX_VAL);
  Serial.print(",Y=");
  Serial.print(Y_MAX_VAL);
  Serial.print(",Z=");
  Serial.print(Z_MAX_VAL);

  Serial.print(", T0=");
  Serial.print(TEMP_0_VAL);
  Serial.print(",T1=");
  Serial.print(TEMP_0_VAL);
  Serial.print(",T2=");
  Serial.print(TEMP_0_VAL);
  Serial.print(", s=");
  Serial.print(s_pasi);
  
  Serial.println(" ");
  }

void set_pini(){
  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE_PIN    , OUTPUT);
  pinMode(LED_PIN  , OUTPUT);
  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE_PIN    , OUTPUT);
  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE_PIN    , OUTPUT);
  pinMode(Z_STEP_PIN  , OUTPUT);
  pinMode(Z_DIR_PIN    , OUTPUT);
  pinMode(Z_ENABLE_PIN    , OUTPUT);
  pinMode(E0_STEP_PIN  , OUTPUT);
  pinMode(E0_DIR_PIN    , OUTPUT);
  pinMode(E0_ENABLE_PIN    , OUTPUT);
  if (X_MIN_PIN>0){pinMode(X_MIN_PIN , INPUT_PULLUP);}
  if (X_MAX_PIN>0){pinMode(X_MAX_PIN , INPUT_PULLUP);}
  if (Y_MIN_PIN>0){pinMode(Y_MIN_PIN , INPUT_PULLUP);}
  if (Y_MAX_PIN>0){pinMode(Y_MAX_PIN , INPUT_PULLUP);}
  if (Z_MIN_PIN>0){pinMode(Z_MIN_PIN , INPUT_PULLUP);}
  if (Z_MAX_PIN>0){pinMode(Z_MAX_PIN , INPUT_PULLUP);}
  pinMode(PS_ON_PIN   , OUTPUT);
  pinMode(HEATER_0_PIN    , OUTPUT);
  pinMode(HEATER_1_PIN    , OUTPUT);
  pinMode(HEATER_2_PIN    , OUTPUT);
  pinMode(LED_PIN    , OUTPUT);

}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
