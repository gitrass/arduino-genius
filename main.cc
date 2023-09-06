#define RED 5
#define GRN 6
#define YLW 7
#define BLU 8

#define BTR 1
#define BTG 2
#define BTY 3
#define BTB 4

#define UNDEFINED -1
#define MAX_LEDS 4

int leds[MAX_LEDS] = {RED, GRN, YLW, BLU};
int buttons[MAX_LEDS] = {BTR, BTG, BTY, BTB};

int sequence[MAX_LEDS]; // array alocates the random leds sequence
int playersequence[MAX_LEDS]; // array alocates the answer sequence

int points = 0;
int answered = 0;

typedef enum States {
  LIGHT_LEDS,
  ANSWERING,
  ENDGAME
} States;

States GameState = LIGHT_LEDS;

void setup(){ // setup ports
  Serial.begin(9600);
  initialize();
}

void loop() {
  int playeranswer = UNDEFINED;
  switch(GameState) {
    case LIGHT_LEDS:
      delay(500);
      randomize();
      roundleds();
      GameState = ANSWERING;
      break;
    case ANSWERING:
      playeranswer = read()+MAX_LEDS;
      light(playeranswer);
      process(playeranswer);
      break;
    case ENDGAME:	
      endgame();
      break;
  }
}

//setup functions
void initialize(){
  for(int i = 1; i <= MAX_LEDS; i++){
    pinMode(i, INPUT_PULLUP);
    pinMode(i+4, OUTPUT);
  }
}

int light(int LED){
  digitalWrite(LED, HIGH);
  delay(400);
  digitalWrite(LED, LOW);
  delay(200);
  return LED;
}
//

//lightled functions
void randomize(){
  int seed = analogRead(0); // captures random inputs from empty analog 0
  randomSeed(seed);
  for(int i = 0; i < MAX_LEDS; i++){
    sequence[i] = random(5,9); // random cap is digital led number
  }
}

void roundleds(){
  for(int i = 0; i < MAX_LEDS; i++){
    light(sequence[i]);
  }
}
//

//answering functions
void process(int playeranswer){
  if(playeranswer == sequence[answered]){
    answered++;
    if (answered == MAX_LEDS) {
      if(points == 5){
    	endgame();
  	  }
      points++;
      answered = 0;
      correct();
      GameState = LIGHT_LEDS;
    }
  }
  else{
    wrong();
    answered = 0;
    GameState = LIGHT_LEDS;
  }
}

int read(){
  int buttonstate[MAX_LEDS];
  for(int i = 0; i < MAX_LEDS; i++){
    buttonstate[i] = 1;
  }
  
  int btn_pressed = UNDEFINED;
  do {
    for(int i = 0; i < MAX_LEDS; i++) {
    	if(digitalRead(buttons[i]) == 0) {
          	btn_pressed = buttons[i];
          	break;
    	}
  	}
    
    delay(100);
  } while(btn_pressed == UNDEFINED);
  
  do {
    delay(100);
  } while (digitalRead(btn_pressed) == 0);
  
  return btn_pressed;
}

int checkzero(int *arr){
  for(int i = 0; i < MAX_LEDS; i++){
    if(arr[i] == 0 ){
      return i;
    }
  }
  return UNDEFINED;
}

//

//state functions
void correct(){
  digitalWrite(GRN, HIGH);
  delay(200);
  digitalWrite(GRN, LOW);
  delay(200);
  digitalWrite(GRN, HIGH);
  delay(200);
  digitalWrite(GRN, LOW);
  delay(200);
  digitalWrite(GRN, HIGH);
  delay(200);
  digitalWrite(GRN, LOW);
  delay(1000);
  
}

void wrong() {
  digitalWrite(RED, HIGH);
  delay(200);
  digitalWrite(RED, LOW);
  delay(200);
  digitalWrite(RED, HIGH);
  delay(200);
  digitalWrite(RED, LOW);
  delay(200);
  digitalWrite(RED, HIGH);
  delay(200);
  digitalWrite(RED, LOW);
  delay(1000);
}


void endgame(){
  delay(400);
  for(int i = 0; i < MAX_LEDS; i++){
    digitalWrite(leds[i], HIGH);
  }
  delay(700);
  for(int i = 0; i < MAX_LEDS; i++){
    digitalWrite(leds[i], LOW);
  }
  for(int i = 0; i < MAX_LEDS; i++){
    digitalWrite(leds[i], HIGH);
  }
  delay(700);
  for(int i = 0; i < MAX_LEDS; i++){
    digitalWrite(leds[i], LOW);
  }
}
//
