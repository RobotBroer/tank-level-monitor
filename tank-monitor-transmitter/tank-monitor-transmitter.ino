#include <SoftEasyTransfer.h>
#include <Narcoleptic.h>


SoftwareSerial mySerial(10, 11);

#define ECHOPIN 2// Pin to receive echo pulse

#define TRIGPIN 3// Pin to send trigger pulse

#define GATE 5


//create object
SoftEasyTransfer ET; 
SoftEasyTransfer SX;

struct SEND_DATA_STRUCTURE{

  int16_t distance;
  byte randomNUM;
};

struct RECEIVE_DATA_STRUCTURE{

  byte randomSEND;
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;
RECEIVE_DATA_STRUCTURE mydataRECEIVE;



int getDistance() {

  
  pinMode(TRIGPIN, OUTPUT);
  digitalWrite(ECHOPIN, HIGH);
  
  digitalWrite(TRIGPIN, LOW); // Set the trigger pin to low for 2uS

  delayMicroseconds(2);

  digitalWrite(TRIGPIN, HIGH); // Send a 10uS high to trigger ranging

  delayMicroseconds(10);

  digitalWrite(TRIGPIN, LOW); // Send pin low again

  int distance = pulseIn(ECHOPIN, HIGH,26000); // Read in times pulse

  distance= distance/58;

  //Serial.print(F("distance: "));
 // Serial.println(distance);

  digitalWrite(ECHOPIN, LOW);
  

  return distance;

}

void setup(){
  mySerial.begin(9600);
    Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port.
  ET.begin(details(mydata), &mySerial);
  SX.begin(details(mydataRECEIVE), &mySerial);
  
 // pinMode(13, OUTPUT);

  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  digitalWrite(ECHOPIN, HIGH);

  pinMode(GATE, OUTPUT);
  digitalWrite(GATE, LOW);
  
}

void loop(){
  //this is how you access the variables. [name of the group].[variable name]
  
  
  digitalWrite(GATE, HIGH);

  delay(500);
  
  mydata.distance = getDistance();
  mydata.randomNUM = random(1,128);

  unsigned long timer = millis();
  ET.sendData();

  delay(500);

  if(SX.receiveData()){
      unsigned long timer2 = millis();
      if(mydata.randomNUM == mydataRECEIVE.randomSEND){

        Serial.print(F("time: " ));
        Serial.println(timer2 - timer);
            
      
        pinMode(TRIGPIN, INPUT);
        digitalWrite(GATE, LOW);
  
        for(int i = 0; i < 10; i++){
           Narcoleptic.delay(1000);
      }
    } 
  }

  delay(500);//
  
}
