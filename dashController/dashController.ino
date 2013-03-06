#define IN_HAZ 4
#define IN_STRT 5


#define OP_HORN 6
#define OP_LIND 7
#define OP_RIND 8
#define OP_DIP 9
#define OP_GRUP 10
#define OP_GRDN 11
#define OP_STRT 12


#define ST_HORN  0
#define ST_LIND  1
#define ST_RIND 2
#define ST_DIP 3
#define ST_GRUP 4
#define ST_GRDN 5
#define ST_HAZ  6
#define ST_STRT 7

volatile bool featureStates[8];

#define DW digitalWrite
#define DR digitalRead

#define FLASHRATE 500
unsigned long flashCounter;
bool flashing;

void setup(){
  // Serial communication is on
  Serial.begin(115200);
  pinMode(IN_HAZ, INPUT);
  pinMode(IN_STRT, INPUT);
  DW(IN_HAZ, LOW);
  DW(IN_STRT, LOW);
  int i;
  for (i=6;i<=12;++i){
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
 
  
  flashing=true;
  flashCounter=millis()+ FLASHRATE ;
}

void loop(){
  checkFlash();
  readStates();
  getSimpleStates();
  updateOutputs();
}

void checkFlash(){
  if (millis()>flashCounter){
    flashing=!flashing;
    flashCounter=millis()+FLASHRATE;
  }  
}

void readStates(){
  while(Serial.available()){
    Serial.read();
  }
  char c;
  // Read all serial data to the start of the next line.
  while (1){
    c=Serial.read();
    if (c=='\n'){
      break;
    }
  }
  int i;
  for (i=0;i<=6;++i){
    while(1){
      c=Serial.read();
      if (c==0){
        featureStates[i]=false;
        break;
      }else if (c==1){
        featureStates[i]=true;
        break;
      }
    }
  }
}

void getSimpleStates(){
  if (DR(IN_STRT)==LOW){
    // <12V at battery, then light up the start light.
    featureStates[ST_STRT]=true;
  }else{
    // else shut it down.
    featureStates[ST_STRT]=false;
  }
}


void updateOutputs(){
  DW(OP_HORN, featureStates[ST_HORN] ? HIGH:LOW);
  DW(OP_DIP, featureStates[ST_DIP] ? HIGH:LOW);
  DW(OP_GRUP, featureStates[ST_GRUP] ? HIGH:LOW);
  DW(OP_GRDN, featureStates[ST_GRDN] ? HIGH:LOW);
  DW(OP_STRT, featureStates[ST_STRT] ? HIGH:LOW);
  
  if (featureStates[ST_HAZ]||featureStates[ST_LIND]){
    DW(OP_LIND,flashing ? HIGH : LOW);
  }else{
    DW(OP_LIND,LOW);
  }
  if (featureStates[ST_HAZ]||featureStates[ST_RIND]){
    DW(OP_RIND,flashing ? HIGH : LOW);
  }else{
    DW(OP_RIND,LOW);
  }

}

