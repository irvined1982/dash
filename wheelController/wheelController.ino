#define OP_HORN 1
#define OP_LIND 2
#define OP_RIND 3
#define OP_DIP 5
#define OP_GRUP 6
#define OP_GRDN 7

#define IN_HORN 6
#define IN_LIND 7
#define IN_RIND 8
#define IN_HAZ  9
#define IN_DIP 10
#define IN_GRUP 11
#define IN_GRDN 12

#define ST_HORN  0
#define ST_LIND  1
#define ST_RIND 2
#define ST_HAZ  3
#define ST_DIP 4
#define ST_GRUP 5
#define ST_GRDN 6

#define DW digitalWrite
#define DR digitalRead

#define FLASHRATE 500

bool featureStates[7];

unsigned long flashCounter;
bool flashing;

void setup(){
  flashing=true;
  flashCounter=millis()+ FLASHRATE ;
}

void loop(){
  checkFlash();
  getSimpleStates();
  updateOutputs();
}





void checkFlash(){
  if (millis()>flashCounter){
    flashing=!flashing;
    flashCounter=millis()+FLASHRATE;
  }  
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
   if (featureStates[ST_HAZ] || featureStates[ST_RIND] || featureStates[ST_LIND]){
     featureStates[ST_HAZ]=false;
     featureStates[ST_LIND]=false;
     featureStates[ST_RIND]=false;
   }else{
     featureStates[ST_LIND]=true;
   }
  }
  if ( DR(IN_RIND) ){
    if (featureStates[ST_HAZ] || featureStates[ST_RIND] || featureStates[ST_LIND]){
     featureStates[ST_HAZ]=false;
     featureStates[ST_LIND]=false;
     featureStates[ST_RIND]=false;
   }else{
     featureStates[ST_RIND]=true;
   }
  }
  if ( DR(IN_HAZ) ){
    if (featureStates[ST_RIND] || featureStates[ST_LIND]){
     featureStates[ST_HAZ]=true;
     featureStates[ST_LIND]=false;
     featureStates[ST_RIND]=false;
   }else if(featureStates[ST_HAZ]){
     featureStates[ST_HAZ]=false;
   }else{
     featureStates[ST_HAZ]=true;
   }
  }
}

void updateOutputs(){
  DW(OP_HORN, featureStates[ST_HORN] ? HIGH:LOW);
  DW(OP_DIP, featureStates[ST_DIP] ? HIGH:LOW);
  DW(OP_GRUP, featureStates[ST_GRUP] ? HIGH:LOW);
  DW(OP_GRDN, featureStates[ST_GRDN] ? HIGH:LOW);
  
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

