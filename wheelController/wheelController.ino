#define IN_LIND 4 // Left Indicator Switch connected to pin 4
#define IN_RIND 5 // Left Indicator Switch connected to pin 5
#define IN_HORN 6 // Horn Switch connected to pin 6
#define IN_DIP 7  // Dip Switch connected to pin 7
#define IN_GRUP 8 // Gear Up Switch connected to pin 8
#define IN_GRDN 9 // Gear Down Switch connected to pin 9


// Array indexes of states
#define ST_HORN  0
#define ST_LIND  1
#define ST_RIND 3
#define ST_DIP 4
#define ST_GRUP 5
#define ST_GRDN 6
bool featureStates[7];

#define DR digitalRead
#define DW digitalWrite



void setup(){
  // setup Serial communication
  Serial.begin(115200);
  // Set the interrupt pins to inputs
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  // Hold the interrupt pins high, they will then be 
  // pulled low when the switches are activated and the 
  // pins on the switch will be high when read.
  DW(2,HIGH);
  DW(3,HIGH);
  int i;
  // Set all pins to input
  // Hold them low.
  for (i=4;i<=9;++i){
    pinMode(i,INPUT);
    DW(1,LOW);
  }
  // Attatch an interrupt handler to the interrupt pins.  Right now
  // This is only used to detect input from the indicator switches
  // however multiple switches can use the same pin.
  attachInterrupt(0, buttonPress, CHANGE);
  attachInterrupt(1, buttonPress, CHANGE);
}

void loop(){
  // Check the state of pins that are on or off directly, as opposed 
  // to those that toggle the values or have special actions
  getSimpleStates();
  // send the states to the main controller
  sendStates();
}


void getSimpleStates(){
 if ( DR(IN_HORN) ){
    featureStates[ST_HORN]=true;
  }else{
    featureStates[ST_HORN]=false;
  }
  
  if ( DR(IN_DIP) ){
    featureStates[ST_DIP]=true;
  }else{
    featureStates[ST_DIP]=false;
  }
  if ( DR(IN_GRUP) && DR(IN_GRDN) ){ 
    featureStates[ST_GRUP]=false;
    featureStates[ST_GRDN]=false;
  }else if( DR(IN_GRUP)){
    featureStates[ST_GRUP]=true;
    featureStates[ST_GRDN]=false;
  }else if(DR(IN_GRDN)){
    featureStates[ST_GRUP]=false;
    featureStates[ST_GRDN]=true;
  }else{
    featureStates[ST_GRUP]=false;
    featureStates[ST_GRDN]=false;
  }
}
void buttonPress(){
 
  if ( DR(IN_LIND) ){
   if (featureStates[ST_RIND] || featureStates[ST_LIND]){
     featureStates[ST_LIND]=false;
     featureStates[ST_RIND]=false;
   }else{
     featureStates[ST_LIND]=true;
   }
  }
  if ( DR(IN_RIND) ){
    if (featureStates[ST_RIND] || featureStates[ST_LIND]){
     featureStates[ST_LIND]=false;
     featureStates[ST_RIND]=false;
   }else{
     featureStates[ST_RIND]=true;
   }
  }
}

void sendStates(){
  unsigned int i;
  for (i=0;i<sizeof(featureStates)/sizeof(featureStates[0]); ++i){
    Serial.print(featureStates[i] ? 1:0);
  }
  Serial.print("\n");
}

