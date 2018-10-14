#include <SoftEasyTransfer.h>

/*   For Arduino 1.0 and newer, do this:   */
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);

SoftEasyTransfer ET; 
SoftEasyTransfer SX;

const int RELAY = 7; 
const int MINIMUM = 50;


struct RECEIVE_DATA_STRUCTURE{

  int16_t distance = 55;
  byte randomNUM;
};

struct SEND_DATA_STRUCTURE{

  byte randomSEND;
};

RECEIVE_DATA_STRUCTURE mydata;
SEND_DATA_STRUCTURE mydataSEND;

const int RGB[] = {3,4,5};

int analogInPin = A1;
int sensorValue = 0;        // value read from the carrier board     
unsigned long msec = 0;
float timeSec = 0.0;
int sample = 0;
float totalCharge = 0.0;
float averageAmps = 0.0;
float ampSeconds = 0.0;
float ampHours = 0.0;
float amps = 0.0;


const boolean RED[] = {HIGH, LOW, LOW};    
const boolean GREEN[] = {LOW, HIGH, LOW};
const boolean BLUE[] = {LOW, LOW, HIGH};
const boolean WHITE[] = {HIGH, HIGH, HIGH};
const boolean DOWN[] = {LOW, LOW, LOW};

unsigned long timer = 0;
unsigned long heartbeat = 15000;

void setColour (int*RGB, const boolean*colour)
// Sets colour of RGB to defined colours. 
{  
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(RGB[i], colour[i]); 
  } 
}

void setup(){

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  
  for(int i=0; i<3; i++)
    {
      pinMode(RGB[i], OUTPUT);
    }
    
  mySerial.begin(9600);

  SX.begin(details(mydataSEND), &mySerial);
  ET.begin(details(mydata), &mySerial);
  
  
  Serial.begin(9600);
  
  timer = millis();
  setColour(RGB,BLUE);

}

void loop(){

   
  if(millis() - timer > heartbeat){
    setColour(RGB,RED);
    digitalWrite(RELAY, LOW);
  }

   if(ET.receiveData()){
      Serial.print("distance = ");     
      Serial.println(mydata.distance); 
      Serial.println("");  
      timer = millis();  
      setColour(RGB,GREEN);
      mydataSEND.randomSEND = mydata.randomNUM;
      SX.sendData();
      delay(500);
   }

  if(mydata.distance < MINIMUM){
    digitalWrite(RELAY, HIGH);
  }
  else{
    digitalWrite(RELAY, LOW);
  }

 /*
  int sampleAmpVal = 0;
  float avgSAV = 0;

  for (int x = 0; x < 10; x++){ // run through loop 10x

  // read the analog in value:
  sensorValue = analogRead(analogInPin);  
  sampleAmpVal = sampleAmpVal + sensorValue; // add samples together
  delay (100); // let ADC settle before next sample
  }
  avgSAV = sampleAmpVal / 10;

  // convert to milli amps
  amps = (avgSAV  - 512)  / 37.888; 

  sample = sample + 1;

  msec = millis();
 
  timeSec = (float) msec / 1000.0;
 
  totalCharge = totalCharge + amps;
 
  averageAmps = totalCharge / sample;

  ampSeconds = averageAmps*timeSec;
  
  ampHours = ampSeconds/3600;

  Serial.print("avgSAV= ");     
  Serial.println(avgSAV); 
  Serial.println("");
  
  Serial.print("Current (amps) = ");     
  Serial.println(amps); 
  Serial.println("");

  Serial.print("Average (amps) = ");     
  Serial.println(averageAmps); 
  Serial.println("");

  
  delay(2000);
 */
}
