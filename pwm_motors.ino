/*  
  On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.
*/

// General variables
int motor_1_pin = 9;        // the PWM pin
int motor_2_pin = 10;        // the PWM pin
int motor_3_pin = 11;
int motor_4_pin = 6;
int max_pwm = 150;      // max pwm to feed with 5 V and obtain 3 V.
int std_pwm = 30;  // 40 leads out of the table
int effect_pwm = 10; // to increase/decrease the pwm of a motor in order to obtain an effect
int pwm_change = 0;    // how many points to fade the motor speed by

// Initialization values for the speeds of the motors
int motor_1_pwm = std_pwm;  // Speed of motor 1. 
int motor_2_pwm = std_pwm;  // Speed of motor 2. 
int motor_3_pwm = std_pwm;  // Speed of motor 3. 
int motor_4_pwm = 94; //Used to recovery of balls. pwm min is 85. 94 max (95 changes direction).

// setup run once when arduino starts (or reset):
void setup() {
  // declare pin 9, 10, 11 as outputs:
  pinMode(motor_1_pin, OUTPUT);
  pinMode(motor_2_pin, OUTPUT);
  pinMode(motor_3_pin, OUTPUT);
  pinMode(motor_4_pin, OUTPUT);

  // Changing freq of pins 5 and 6
  //TCCR0B = TCCR0B & B11111000 | B00000001;	62500 Hz (62.5 KHz)
  //TCCR0B = TCCR0B & B11111000 | B00000010;	7812.50 Hz (7.8 KHz)
  //TCCR0B = TCCR0B & B11111000 | B00000011;	976.56 Hz (default PWM frequency)
  //TCCR0B = TCCR0B & B11111000 | B00000100;	244.14 Hz
  //TCCR0B = TCCR0B & B11111000 | B00000101;	61.04 Hz
  TCCR0B = TCCR0B & B11111000 | B00000100;	//244.14 Hz on the motor_4 

  // Setting the pwm for the motors on start/reset
  analogWrite(motor_1_pin, motor_1_pwm);
  analogWrite(motor_2_pin, motor_2_pwm);
  analogWrite(motor_3_pin, motor_3_pwm);
  analogWrite(motor_4_pin, motor_4_pwm);
}


// Function to give effects to the ball
std::tuple<int, int, int> ball_effect(string effect_mode) {
  if (effect_mode == "topspin") {
    return std::make_tuple(std_pwm + effect_pwm, std_pwm + effect_pwm, std_pwm - effect_pwm); 
  }
  return std::make_tuple(std_pwm, std_pwm, std_pwm); 
}

// main loop:
void loop() {
  ball_effect_prev = "whatever"; 
  ball_effect_set = "topspin";
   
  if (ball_effect_set != ball_effect_prev){
    // A new effect has been defined
    ball_effect_prev = ball_effect_set;
    tie(motor_1_pwm, motor_2_pwm, motor_3_pwm) = ball_effect(ball_effect_set)
    // set the motors speed:
    analogWrite(motor_1_pin, motor_1_pwm);
    analogWrite(motor_2_pin, motor_2_pwm);
    analogWrite(motor_3_pin, motor_3_pwm);  
  }
  
  // sleeping between cycles
  delay(100);  
}