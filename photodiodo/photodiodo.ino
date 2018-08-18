int pd=10;                      //Photodiode to digital pin 2

 int buzz=3;                   //piezo buzzer to digital pin 13  
 int senRead=0;                 //Readings from sensor to analog pin 0  
 int limit=800;                 //Threshold range of an obstacle  
 void setup()    
 {  
  pinMode(pd,OUTPUT);  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(pd,HIGH);       //supply 5 volts to photodiode  
  digitalWrite(buzz,LOW);      //set the buzzer in off mode (initial condition)  
  Serial.begin(9600);          //setting serial monitor at a default baund rate of 9600  
 }  
 void loop()  
 {  
  int val=analogRead(senRead);  //variable to store values from the photodiode  
  Serial.println(val);          // prints the values from the sensor in serial monitor  
  if(val <= limit)              //If obstacle is nearer than the Threshold range  
  {  
   pinMode(LED_BUILTIN, HIGH);     // Buzzer will be in ON state  
   delay(20);  
  }  
  else if(val > limit)          //If obstacle is not in Threshold range  
  {  
   pinMode(LED_BUILTIN, LOW);      //Buzzer will be in OFF state  
   delay(20);  
  }  
 }  

