// Example 2 - Receive with an end-marker
#include <Servo.h>

// 6 servos to be controlled for the arm and servo to store the selected servo
Servo curr_servo,servo1,servofix,servo3,servo4,servo5,servo6;

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

// selected servo
String servo_sel = "",serial_in = "";

// output to each servo
int servo_pos = 0,start_pos = 0,end_pos = 0;
int start_pos1 = 0,start_pos2 = 0,start_pos3 = 0,start_pos4 = 0,start_pos5 = 0,start_pos6 = 0;

int start_lim = 0,start_lim1 = 90,start_lim2 = 90,start_lim3 = 0,start_lim4 = 40,start_lim5 = 0,start_lim6 = 0;
int end_lim = 0,end_lim1 = 180,end_lim2 = 180,end_lim3 = 360,end_lim4 = 120,end_lim5 = 0,end_lim6 = 0;

void setup() {
    // servo1
    servo1.attach(0);
    servo1.attach(3);
    //servofix
    servofix.write(0);
    servofix.attach(5);
    //servo3
    servo3.attach(0);
    servo3.attach(6);
    //servo4
    servo4.attach(0);
    servo4.attach(9);
    //servo1
    servo5.attach(0);
    servo5.attach(10);
    //servo1
    servo6.attach(0);
    servo6.attach(11);
    
    Serial.begin(9600);
    Serial.println("<Arduino is ready>");
}

void loop() {
    recvWithEndMarker();
    showNewData();
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;

    Serial.flush();
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        serial_in = String(receivedChars);
        servo_sel = serial_in.substring(0,2);
        end_pos = serial_in.substring(2).toInt();
        
        selectPose();
        
        if(end_pos>=start_lim and end_pos<=end_lim){
          end_pos = end_pos;
          goToPose();              // tell servo to go to position in variable 'pos'
        }
        else if(end_pos<90){
          end_pos = 90;
        }
          
        newData = false;
    }
}

// select the corresponding servo based on the channel
void selectPose(){
  if(servo_sel == "01"){
    Serial.println("CH1..."+String(start_pos)+" "+String(end_pos));
    start_lim = start_lim1;
    end_lim = end_lim1;
    start_pos = start_pos1;
    curr_servo = servo1;
  }
  else if(servo_sel == "02"){
    Serial.println("CH2..."String(start_pos)+" "+String(end_pos));
    start_lim = start_lim2;
    end_lim = end_lim2;
    start_pos = start_pos2;
    //curr_servo = servo2;
  }
  else if(servo_sel == "03"){
    Serial.println("CH3..."+String(start_pos)+" "+String(end_pos));
    start_lim = start_lim3;
    end_lim = end_lim3;
    start_pos = start_pos3;
    curr_servo = servo3;
  }
  else if(servo_sel == "04"){
    Serial.println("CH4..."+String(start_pos)+" "+String(end_pos));
    start_pos = start_pos4;
    curr_servo = servo4;
  }
  else if(servo_sel == "05"){
    Serial.println("CH5..."+String(start_pos)+" "+String(end_pos));
    start_pos = start_pos5;
    curr_servo = servo5;
  }
  else if(servo_sel == "06"){
    Serial.println("CH6..."+String(servo_pos));
    start_pos = start_pos6;
    curr_servo = servo6;
  }
}

// store pose 
void storePose(){
  if(servo_sel == "01"){
    start_pos1 = start_pos;  
  }
  else if(servo_sel == "02"){
    start_pos2 = start_pos;
  }
  else if(servo_sel == "03"){
    start_pos3 = start_pos;
  }
  else if(servo_sel == "04"){
    start_pos4 = start_pos;
  }
  else if(servo_sel == "05"){
    start_pos5 = start_pos;
  }
  else if(servo_sel == "06"){
    start_pos6 = start_pos;
  }
}

// go to pose
void goToPose(){
  for (int pos = start_pos; pos <= end_pos; pos += 1) { // goes from 0 degrees to 180 degrees
    start_pos = pos;
    storePose();
    // in steps of 1 degree
    curr_servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (int pos = start_pos; pos >= end_pos; pos -= 1) { // goes from 180 degrees to 0 degrees
    start_pos = pos;
    storePose();
    // in steps of 1 degree
    curr_servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
