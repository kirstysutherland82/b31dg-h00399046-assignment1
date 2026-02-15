#include "header.h"
bool outputEnable = true; //is data on or off? controlled by pb1
bool outputSelect = false; //alternative (reverse) behaviour controlled by pb2 
//button control variables
bool statePB1 = false; //tracks last stable state
bool statePB2 = false; //tracks last stable state
bool prevPB1 = false; //last raw button value
bool prevPB2 = false; //last raw button value
unsigned long lastDebouncePB1 = 0;
unsigned long lastDebouncePB2 = 0;

void buttonCheck(); //button control function
void outputData(); //data output control function

void setup() {
  //initialise pinMode for data, sync, pb1 and pb2
  pinMode(DATA, OUTPUT);
  pinMode(SYNC, OUTPUT);
  pinMode(PB1, INPUT);
  pinMode(PB2, INPUT);
  //initialise both outputs low
  digitalWrite(DATA, LOW);
  digitalWrite(SYNC, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  buttonCheck();

  if(outputEnable){
    outputData();
  }
  else{
    digitalWrite(DATA, LOW);
    digitalWrite(SYNC, LOW);
  }

}

//button time - using delay (only believes it as a valid press if held for 20ms)
void buttonCheck(){
  bool pb1 = digitalRead(PB1);
  bool pb2 = digitalRead(PB2);
  unsigned long now = millis();

  //detect pb1 rising edge
  if(pb1 != prevPB1){
    lastDebouncePB1 = now;
    prevPB1 = pb1;}


  if((now-lastDebouncePB1)>20){ //20 milliseconds debounce delay
    if(pb1 != statePB1 ){
      statePB1 = pb1;
      if(statePB1 == true){
        outputEnable = !outputEnable; //toggel outputEnable
      }
    }
  }

    //detect pb2 rising edge
  if(pb2 != prevPB2){
    lastDebouncePB2 = now;
    prevPB2 = pb2;}


  if((now-lastDebouncePB2)>20){ //20 milliseconds debounce delay
    if(pb2 != statePB2){
      statePB2 = pb2;
      if(statePB2 == true){
        outputSelect = !outputSelect; //toggle outputEnable
      }
    }
  }
S
}

//main output data function, called continuously while last button pressed = pushButton1
void outputData(){
  //set SYNC high for 50us at the start of every cycle
  digitalWrite(SYNC, HIGH);
  delayMicroseconds(TSYNC);
  digitalWrite(SYNC, LOW);

  //original data pulse pattern, carried out c times
  if(!outputSelect){
    for (int n = 1; n<=C; n++){
      buttonCheck();
      if (!outputEnable) return;
    //initialise T_on
    unsigned long on = A + ((n-1)*50*SCALE); //scale=1000 when debug toggled
    
    //high impulse for calculated time T_on
    digitalWrite(DATA, HIGH);
    delayMicroseconds(on);
    //low impulse
    digitalWrite(DATA, LOW);

    //wait for b us unless last impulse per cycle
    if(n<C){
      delayMicroseconds(B);
    }
    }
  }
  //alternative behaviour - reversed
  else{
    for (int n = C; n>=1; n--){
      buttonCheck();
      if (!outputEnable) return;
      //initialise T_on
      unsigned long on = A + ((n-1)*50*SCALE); //scale=1000 when debug toggled

      //high impulse for calculated time T_on
      digitalWrite(DATA, HIGH);
      delayMicroseconds(on);
      //low impulse
      digitalWrite(DATA, LOW);

      //wait for b us unless last impulse per cycle
     if(n>1){
        delayMicroseconds(B);
      }
    }
  }
//wait for d after final pulse
delayMicroseconds(D);
}

