const int phase1pin = 2;
const int phase2pin = 3;
const int phase3pin = 4;

float holdTime; 
float targetSpeed;
 
unsigned long p1start,
              p1end,
              p2start,
              p2end,
              p3start,
              p3end;
              
// Change these until you find your min and max speeds.
float min = 3200;
float max = 50000;

void setup() {
  Serial.begin(115200);
  pinMode(phase1pin, OUTPUT);
  pinMode(phase2pin, OUTPUT);
  pinMode(phase3pin, OUTPUT);
}

String direction;
bool running = false;
float status;

void chkP1(int pin){
  unsigned long currentTime = micros();
  unsigned long td = currentTime - p1start;
  unsigned long refractory = 2.25*holdTime;
  if(digitalRead(pin)){
    if(td > holdTime){
      digitalWrite(pin, LOW);
      p1end = currentTime;
    }
  }else if(td > refractory){
    digitalWrite(pin, HIGH);
    p1start = currentTime;
  }
}
 
void chkP2(int pin){
  unsigned long currentTime = micros();
  unsigned long td = currentTime - p1start;
  if(digitalRead(pin)){
    if(td > 1.75*holdTime || td < 0.75*holdTime){
      digitalWrite(pin, LOW);
      p2end = currentTime;
    }
  }else if(td > 0.75*holdTime && td < 1.75*holdTime){
    digitalWrite(pin, HIGH);
    p2start = currentTime;
  }
}
 
void chkP3(int pin){
  unsigned long currentTime = micros();
  unsigned long td = currentTime - p1start;
  if(digitalRead(pin)){
    if(td > 0.25*holdTime && p3start < p1start){
      digitalWrite(pin, LOW);
      p3end = currentTime;
    }
  }else if(td > 1.5*holdTime){
    digitalWrite(pin, HIGH);
    p3start = currentTime;
  }
}

void setupMotor(float input_targetSpeed, String new_direction)
{
  // Converts the percent value into something usable
  targetSpeed = max-((max-min)*(input_targetSpeed*0.01));

  // See if the direction has changed
  if(new_direction != direction)
  {
    holdTime = 20000;
    direction = new_direction;
    // Wait for it to stop, weird things can happen if it's still moving.
    delay(500);

    // Initialize first phase
    if(direction == "f")
    {
      digitalWrite(phase1pin, HIGH);
    }
    else if(direction == "r")
    {
      digitalWrite(phase3pin, HIGH);
    }
  }
  
  running = true;
}
 
void loop(){

  if(Serial.available() > 0) 
  {
    // Read serial input ex. "100 reverse" or "50 forward"
    float input_targetSpeed = Serial.readStringUntil(' ').toInt();
    Serial.print(input_targetSpeed);
    //float input_targetSpeed = 100;
  
    //Serial.read(); //next character is space, so skip it using this
    String new_direction = Serial.readStringUntil('\n');
    Serial.print(new_direction);
    //String new_direction = "r";
    setupMotor(input_targetSpeed, new_direction);
  }
  
  if(running) {
  
    if(direction == "f")
    {
        chkP1(phase3pin);
        chkP2(phase2pin);
        chkP3(phase1pin); 
    }
    else if(direction == "r")
    {
        chkP1(phase1pin);
        chkP2(phase2pin);
        chkP3(phase3pin);
    }
    
    delayMicroseconds(100);
   
    if(holdTime > targetSpeed)
    {
      holdTime -= 0.5;
    }
    else if(holdTime < targetSpeed)
    {
      holdTime += 0.25;
    }

    // Monitors the current speed
    status = -(holdTime-max)/((max-min)*(0.01));
    
    if(status == 100)
    {
      // setupMotor(100, "reverse");
    }
  }
    
}
