
// This program rotates the motors for each rotor front and rear
// in clockwise or anticlockwise direction. It is made specifically 
// to rotate the Stepper motors for the robotic arm. 

// After connecting new Arduino, go to "Tools" - "Port" and it will 
// find and set the correct port. Select the one which is connected to Arduino UNO. 

// table gear ratio driven to driving => 1:1 (working with direct values)
#define TABLE_RATIO 1

// defines pins numbers for base for speed
const int stepPin_table = 2;
// Direction PIN for rotating base
const int dirPin_table = 5;

// defines pins numbers for driving motor for speed
const int stepPin_jt = 3; 
// Direction PIN for driving motor
const int dirPin_jt = 6; 


// For DRV8825 a HIGH on pin 8 DISABLES the driver and 
// LOW on this pin 8 ENABLES the driver.
const int enPin = 8;

// Each step for a stepper motor is 1.8 degrees. 
// This makes 360 degrees / 1.8 degrees == 200 steps for one rotation. 
// With 100 (M0 M1 M2) jumper setting, the motor moves (1.8) degrees per step. 
// This means it needs 200 * 32 == 6400 steps per rotation. 

String inpval = "";
String temp_str;
long int table_deg;
int table_speed;

void rotateMotor(long int degrees, float speed_rpm, const int ratio, const int stepPin, const int dirPin){
  /*
  Function to drive motor using stepper driver
  parameters of degrees, speed, ratio, steps, direction 
  
  */
  long int steps;
  steps = ratio * degrees * (6400 / 360);

  // -ve is CW and +ve is CCW
  if (degrees < 0){
    // Clockwise rotation
    digitalWrite(dirPin, LOW); 

    steps = -1 * steps;
    }
  else if (degrees >= 0){
    // Anti Clockwise rotation
    digitalWrite(dirPin, HIGH);   
    }

  unsigned int step_delay;
  
  // for zero speed no motion, else moves
  if (speed_rpm > 0) {
    // formula: ((10^6 x 360) / (6400*step_delay*2))/6 = rpm
    step_delay = (3600000)/(64 * 12 * speed_rpm * ratio); 
    Serial.println("Steps: " + String(steps));
    Serial.println("Delay: " + String(step_delay));

    for(long int x = 0; x < steps; x++){
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(step_delay); 
      digitalWrite(stepPin, LOW); 
      delayMicroseconds(step_delay); 
      }
    }
  }


void setup() {

  Serial.begin(9600);
  
  // Sets the two pins as Outputs for rotary selector
  pinMode(stepPin_table,OUTPUT); 
  pinMode(dirPin_table,OUTPUT);

  // Sets the two pins as Outputs for arm joint actuator
  pinMode(stepPin_jt,OUTPUT); 
  pinMode(dirPin_jt,OUTPUT);

  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);    // enable driver 
}

void loop() {


  Serial.println("Command:");  
  while (Serial.available() == 0)   
  { }  //Wait for user input  
  inpval = Serial.readString();

  // Serial format is TT <degrees> <rpm>
  // TT for turntable degrees +ve for CCW and -ve for CW
  table_deg = 0;
  table_speed = 20; // default values
   
  if (inpval.startsWith("TT") && (inpval.indexOf('<') != inpval.lastIndexOf('<')) ){
    
    temp_str = inpval.substring( inpval.indexOf('<')+1, inpval.indexOf('>'));
    table_deg = temp_str.toInt();
    temp_str = inpval.substring( inpval.lastIndexOf('<')+1, inpval.lastIndexOf('>'));
    table_speed = temp_str.toInt();
    Serial.println("Degrees: " + String(table_deg) + ", Speed(rpm): " + String(table_speed));
    rotateMotor(table_deg, table_speed, TABLE_RATIO, stepPin_table, dirPin_table);
    }    
  
  // JT for joint rotation
  else if (inpval.startsWith("JT") && (inpval.indexOf('<') != inpval.lastIndexOf('<')) ){
    
    temp_str = inpval.substring( inpval.indexOf('<')+1, inpval.indexOf('>'));
    table_deg = temp_str.toInt();
    temp_str = inpval.substring( inpval.lastIndexOf('<')+1, inpval.lastIndexOf('>'));
    table_speed = temp_str.toInt();
    Serial.println("Degrees: " + String(table_deg) + ", Speed(rpm): " + String(table_speed));
    rotateMotor(table_deg, table_speed, TABLE_RATIO, stepPin_jt, dirPin_jt);
    }
  else{
    Serial.println("Err: Invalid Input");
    }    
  
  
  delay(500); // wait for some time before restart
}
