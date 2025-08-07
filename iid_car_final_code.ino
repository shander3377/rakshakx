int trig = 2;
int echo = 4;

int MOTOR_RIGHT_FORWARD_PIN = 13;  // Motor 1 forward IN1 blue
int MOTOR_RIGHT_BACKWARD_PIN = 12;  // Motor 2 forward IN2 red

int MOTOR_LEFT_FORWARD_PIN = 7;  // Motor 1 reverse IN4 brown

int MOTOR_LEFT_BACKWARD_PIN = 8;  // Motor 2 reverse // IN3

int redPin2= 9;
int greenPin2 = 10;
int bluePin2 = 11;
#define buz A5
int pos = 0;
    
long getUltrasonicDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Read the echo time
  long duration = pulseIn(echo, HIGH);

  // Calculate distance in cm
  long distance = (duration / 2) / 29.1;
  return distance;
}
void stopCar() {

    digitalWrite(MOTOR_LEFT_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
    digitalWrite(buz, HIGH);
    delay(600);
    digitalWrite(buz, LOW);

}

void right() {
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, HIGH);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, HIGH);
}
void left() {
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, HIGH);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, HIGH);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
}
void forward() {
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, HIGH);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, HIGH);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
}
void backward() {
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, HIGH);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, HIGH);
}
void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_RIGHT_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD_PIN, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);


  pinMode(redPin2,  OUTPUT);              
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);

  pinMode(buz, OUTPUT);

 
  Serial.begin(9600);
}
void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(redPin2, redValue);
  analogWrite(greenPin2,  greenValue);
  analogWrite(bluePin2, blueValue);
}
bool stop = false;
void loop() {
  // put your main code here, to run repeatedly:
    long distance = getUltrasonicDistance();
    // Serial.println(distance);
    if (distance < 20) {
        // Serial.println("ayyay");
        setColor(255, 0, 0); // red Color
        // stopCar();
        // turnRight();  // Turn the car to avoid obstacle
    } else {
        if(stop==false){
          setColor(0, 0, 255); // blue Color        
          forward();  // Continue driving forward if no obstacle
        }
    }
    if (Serial.available()) {
      String data = Serial.readStringUntil('\n'); // Rad the incoming data until newline character
      Serial.println(data);
      Serial.println(data.length());
      if(data.length() > 0){
if(data.length()==4){
  stopCar();
} 
if(data.length()==5){
  left();
} 
if(data.length()==9){
  backward();
} 

if(data.length()==6){
  right();
} 

if(data.length()==8){
  forward();
} 
      }   
  }
}