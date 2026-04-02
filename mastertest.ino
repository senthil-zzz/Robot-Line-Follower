#include <Wire.h>
#include <math.h>
#define SLAVE_ADDRESS 0x08
#define IR1 33
#define IR2 34
#define IR3 35
#define IR4 27
#define IR5 25
#define IR6 26
void setup() {
  Serial.begin(115200);
  Wire.begin(); // Initialize I2C as master
  Serial.println("Master initialized. Waiting for commands...");
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);  
  pinMode(IR6, INPUT);
}




void loop() {
  int input1 = analogRead(IR1);
  int input2 = analogRead(IR2);
  int input3 = analogRead(IR3);
  int input4 = analogRead(IR4);
  int input5 = analogRead(IR5);
  int input6 = analogRead(IR6);
  int state1 = 0, state2 = 0, state3 = 0, state4 = 0, state5 = 0, state6 = 0;
  Serial.print("input 1: ");
  Serial.println(input1);
  if(input1 >= 400){
  state1 = 1;
  Serial.print(" sensor 1  --  white");
  }
  if(input1 < 400){
    state1 = 0;
    Serial.print(" sensor 1  --  black");
    sendCommandToSlave("LEFT");
  }
  Serial.print("input 2: ");
  Serial.println(input2);
  if(input2 >= 400){
  state2 = 1;
  Serial.print(" sensor 2 -- white");
  }
  if(input2 < 400){
    state2 = 0;
    Serial.print(" sensor 2  --  black");
  }
  Serial.print("input 3: ");
  Serial.println(input3);
  if(input3 >= 2500){
  state3 = 1;
  Serial.print(" sensor 3  --  white");
  }
  if(input3 < 2500){
    state3 = 0;
    Serial.print(" sensor 3  --  black");
  }
  Serial.print("input 4: ");
  Serial.println(input4);
  if(input4 >= 2500){
  state4 = 1;
  Serial.print(" sensor 4  --  white");
  }
  if(input4 < 2500){
    state4 = 0;
    Serial.print(" sensor 4  --  black");
  }
  Serial.print("input 5: ");
  Serial.println(input5);
  if(input5 >= 400){
  state5 = 1;
  Serial.print(" sensor 5  --  white");
  }
  if(input5 < 2500){
    state5 = 0;
    Serial.print(" sensor 5  --  black");
  }
  Serial.print("input 6: ");
  Serial.println(input6);
  if(input6 >= 2500){
  state6 = 1;
  Serial.print(" sensor 6  --  white");
  if(state1 == 1){
    sendCommandToSlave("FORWARD");
  }
  }
  if(input6 < 2500){
    state6 = 0;
    Serial.print(" sensor 6  --  black");
    sendCommandToSlave("RIGHT");
  }
}
void sendCommandToSlave(const char* command) {
  Wire.beginTransmission(SLAVE_ADDRESS);
  while (*command) {
    Wire.write((uint8_t)*command++); // Send each character as a byte
  }
  Wire.endTransmission();
  Serial.print("Command sent to slave: ");
  Serial.println(command);
}
