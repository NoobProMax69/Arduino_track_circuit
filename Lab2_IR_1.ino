/************************************************************************
 * Lab 2 - Embedded Systems 
 * Purpose: Getting familiar with the IR sensors of the Rollbot
 *          Brief introduction to the motors of the Rollbot
 *          
 * Arduino IDE setup:
 *          + Plug in Rollbot
 *          + Tools -> Board: Arduino Uno
 *          + Tools -> Port : Choose the correct one
 *          + Tools -> Serial Monitor: When serial communication is wanted 
 * 
 * Please visit https://www.arduino.cc/reference/en/ for information
 * regarding the functions used in this script.
 * 
 * Make sure you understand the following:
 * 
 * The elements of Arduino (C++) code,
 * https://www.arduino.cc/reference/en/#structure:
 *          + Sketch
 *              * setup()
 *              * loop()
 *          + Control Sctructure:
 *              * if, else, for, while
 *          
 * Functions for controlling the Rollbot (Arduino),
 * https://www.arduino.cc/reference/en/#functions
 *          + digitalWrite()
 *          + digitalRead()
 *          + analogWrite()
 *          + analogRead()
 *          + pinMode()
 *          + delay()
 *          + Serial.begin()
 *          + Serial.print() and Serial.println()
 * 
 * Arduino data types and constants,
 * https://www.arduino.cc/reference/en/#variables:
 *          + array (very important)
 *          + const
 *          + int
 *          + char
 * 
 ************************************************************************/

const int LEFT_LED = 3;
const int RIGHT_LED = 2;
const int THRESHOLD = 200;

/* Declare and initialize a char array of the sensors */
char sensors[5] = {A0, A1, A2, A3, A7};

/* Declare an int array to be used for storing raw sensor data */
int sensor_values[5];

/*Used to store the normalized_ sensor values*/
int normalized_sensor_values[5];

/*Used to find the maximum value (over white surface) for each sensor*/
int max_sensor_values[5] = {0, 0, 0, 0, 0};

/*Used to find the minimum value (over black surface) for each sensor*/
int min_sensor_values[5] = {1023, 1023, 1023, 1023, 1023};


/* Declare and initialize const int for motors */
const int LEFT_MOTOR_DIR = 4;      /* Control pin */
const int RIGHT_MOTOR_DIR = 7;     /* Control pin */
const int LEFT_MOTOR_SPEED = 5;    /* Set motor speed */
const int RIGHT_MOTOR_SPEED = 6;   /* Set motor speed */

/* Entry point of program */


void setup() {
  Serial.begin(9600);

  /* Activate the array of sensor pins as INPUT */
  for (int i = 0; i < 5; i++) {
    pinMode(sensors[i], INPUT);
  }

  /* Activate the LED pins as OUTPUT */
  pinMode(LEFT_LED, OUTPUT);
  // Implement for the right led here
  pinMode(RIGHT_LED, OUTPUT);

  /* Activate the control and motor speed pins */
  pinMode(LEFT_MOTOR_DIR, OUTPUT);
  // Implement for the right motor direction here
  pinMode(RIGHT_MOTOR_DIR, OUTPUT);

  pinMode(LEFT_MOTOR_SPEED, OUTPUT);
  // Implement for the right motor speed here
  pinMode(RIGHT_MOTOR_SPEED, OUTPUT);

  /* Set the direction pins here. OBS! One pin should be HIGH
   *  and the other pin should be LOW!
   */
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  // Implement for right motor direction. Should it be HIGH or LOW?


   /* Open up the Serial Monitor (Tools -> Serial Monitor) */

   /* Instructions - Use the "Testing" page in the Lab 2 instructions.
    *                Go over the white, gray and black surfaces.
    *                Observe and reason about the numbers diplayed 
    *                in the Serial Monitor.
    *                
    *                To get an idea of what sensor_calibration() is doing,
    *                write a program without calling sensor_calibration()
    *                nor normalize_sensor_values(). Without calling those functions,
    *                you will get the raw data from the sensors.
    *                Then, do the same procedure, but also call the
    *                sensor_calibration() and normalize_sensor_values().
    *                
    *                Did you notice any difference?
    *                
    */
   
   /* Call the sensor_calibration function */
   sensor_calibration();
   
  
} /* End setup() */



void loop() {

  /* Call get_adc_data() */
  get_adc_data();
  
  /* Call normalize_sensor_values() */
  normalize_sensor_values();

  /* Call print_array(normalized_sensor_values)
   * or print_array(sensor_values)
   */
   print_array(normalized_sensor_values);

  /* Call toggle_LEDs() */
  toggle_LEDs();
  
    
} /* End loop() */


void activate_LEDs(void) {
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);

} /* End activate_LEDs() */


void toggle_LEDs(void) {
  if (sensor_values[0] > THRESHOLD) {
    digitalWrite(LEFT_LED, HIGH);
  } else if (sensor_values[0] <= THRESHOLD) {
    digitalWrite(LEFT_LED, LOW);
  } 
  
  if (sensor_values[4] > THRESHOLD) {
    digitalWrite(RIGHT_LED, HIGH);
  } else if (sensor_values[4] <= THRESHOLD) {
    digitalWrite(RIGHT_LED, LOW);
  }
  
} /* End toogle_LEDs() */


void activate_sensors(void) {
  for (int i = 0; i < 5; i++) {
    pinMode(sensors[i], INPUT);
  }
  
} /* End activate_sensors() */


void get_adc_data(void) {
  for (int i = 0; i < 5; i++) {
    sensor_values[i] = analogRead(sensors[i]);
  }
  
} /* End get_adc_data() */


void print_array(int arr[]) {
  for (int i = 0; i < 5; i++) {
    Serial.print(arr[i]);
    Serial.print("\t");
  }
  Serial.println();
  
} /* End print_array() */



/************************************************************************
 * sensor_calibration() - Brief description of sensor calibration.
 *                        Calibration Function Used To Find Out Max-
 *                        and Min Reading Value For Each Sensor.
 * 
 ************************************************************************/
void sensor_calibration(void) {

  Serial.println("Starting The Calibration!");
  Serial.println("Place the Robot On The Black Line, with sensor A2 on the Black Line (00X00)");
  Serial.println("Keep in mind - The Rollbot will drive around in circles during the calibration.");
  Serial.println();
  Serial.println("Press Start Button To Start To Calibrate");

  /*Turn on the left LED on the robot*/
  digitalWrite(LEFT_LED, HIGH);   

  /*Wait Until Button is Pressed*/
  while (digitalRead(10) == HIGH) {
    ; 
  }
  /*Turn on the right LED on the robot*/
  digitalWrite(RIGHT_LED, HIGH);

  /* Make Rollbot run in circles during calibration 
   * Set LEFT_MOTOR_SPEED to 0 if you don't want
   * the Rollbot to move during the calibration.
   */
  analogWrite(LEFT_MOTOR_SPEED, 200);
  analogWrite(RIGHT_MOTOR_SPEED, 0);

  /***********************************************************************
  * This functionality reads 5000 values for each sensor while you move the
  * robot back and forth over the black line. Every time the new value is
  * either less than the min value or bigger than the max value for that sensor
  * it stores that value as the new min- respectively max value.
  ************************************************************************/
  for (int i = 0; i < 5000 ; i++) {
    for (int j = 0 ; j < 5; j++) {
      if (analogRead(sensors[j]) > max_sensor_values[j]) {
        max_sensor_values[j] = analogRead(sensors[j]);
        Serial.print("New max value: ");
        Serial.println(max_sensor_values[j]);
      }
      if (analogRead(sensors[j]) < min_sensor_values[j]) {
        min_sensor_values[j] = analogRead(sensors[j]);
        Serial.print("New min value: ");
        Serial.println(min_sensor_values[j]);
      }
    }
  }


  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);

  /*Turn off the left LED on the robot*/
  digitalWrite(LEFT_LED, LOW);
  
  Serial.println();
  Serial.println("The obtained maximum and minimum sensor values for each sensor:");
  
  for (int i = 0; i < 5; i++) {
    if (i == 4) {
      Serial.print("A7: ");
    } else {
      Serial.print("A");
      Serial.print(i);
      Serial.print(": ");
    }
    Serial.print(max_sensor_values[i]);
    Serial.print("  ");
    Serial.println(min_sensor_values[i]);
  }
  Serial.println();
  Serial.println("Calibration Is Over!");
  Serial.println("Press Start When You Have Placed The Robot On The Racing Track");

  /*Wait Until Button is Pressed*/
  while (digitalRead(10) == HIGH) {
    ; 
  }
  
  /*Turn off the right LED on the robot*/
  digitalWrite(RIGHT_LED, LOW);   
  Serial.println("Start");

} /* End sensor_calibration() */


/************************************************************************
 * normalize_sensor_values() - Brief description of normalizing sensor
 *                             values.
 * 
 ************************************************************************/
void normalize_sensor_values(void) {

  /***********************************************************************
  * This functionality will take your sensorvalue and will map it to a value
  * between 0 and 1023 depending on the minimum- respectively maximum value
  * obtained from the Calibration Function.  In case the sensor value becomes
  * less or greater than the obtained minimum- and maximum value you will
  * get a normalized_ value which is <0 or >1023. In this case you set your
  * value to 0 respectively 1023
  ***********************************************************************/
  for (int i = 0; i < 5; i++) {
    normalized_sensor_values[i] = map(sensor_values[i], min_sensor_values[i], max_sensor_values[i], 0, 1023);
    if (normalized_sensor_values[i] < 0) {
      normalized_sensor_values[i] = 0;
    } else if (normalized_sensor_values[i] > 1023) {
      normalized_sensor_values[i] = 1023;
    }
  }
  
} /* End normalize_sensor_values() */

  int follow_line(){

    int lap_no = 0;

    analogWrite(LEFT_MOTOR_SPEED, 200);
    analogWrite(RIGHT_MOTOR_SPEED, 200);

    if((normalized_sensor_values[2] > max_sensor_values[2] / 2) && (normalized_sensor_values[3] < max_sensor_values[3] / 2)){
        analogWrite(LEFT_MOTOR_SPEED, 50);
    }

    if((normalized_sensor_values[2] > max_sensor_values[2] / 2) && (normalized_sensor_values[1] < max_sensor_values[1] / 2)){
        analogWrite(RIGHT_MOTOR_SPEED, 50);    
    }

    if ((normalized_sensor_value[0] < max_sensor_values[2] / 2) &&
        (normalized_sensor_value[1] < max_sensor_values[2] / 2) &&
        (normalized_sensor_value[2] < max_sensor_values[2] / 2) &&
        (normalized_sensor_value[3] < max_sensor_values[2] / 2) &&
        (normalized_sensor_value[4] < max_sensor_values[2] / 2)) {

        lap_no++;
        }
        
    if(lap_no == 2){
      return 0;
    }
  }
/* End Lab2-motors.ino */
