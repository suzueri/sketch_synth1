

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include <MsTimer2.h>

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

const int buttonPin2 = 2; 
const int buttonPin3 = 3;
const int buttonPin4 = 4; 
const int buttonPin5 = 5;
const int buttonPin6 = 6; 
const int buttonPin7 = 7;
const int buttonPin8 = 8; 
const int buttonPin10 = 10; 
const int buttonPin11 = 11; 

int currentButtonState2 = 0;  
int currentButtonState3 = 0;
int currentButtonState4 = 0;
int currentButtonState5 = 0;
int currentButtonState6 = 0;  
int currentButtonState7 = 0;
int currentButtonState8 = 0;
int currentButtonState10 = 0;
int currentButtonState11 = 0;

int formerButtonState2  = LOW;  
int formerButtonState3  = LOW;
int formerButtonState4  = LOW;
int formerButtonState5  = LOW;
int formerButtonState6 = HIGH;  
int formerButtonState7 = HIGH;
int formerButtonState8 = HIGH;
int formerButtonState10 = HIGH;
int formerButtonState11 = HIGH;

bool button2On = false;
bool button3On = false;
bool button4On = false;
bool button5On = false;
bool button6On = false;
bool button7On = false;
bool button8On = false;
bool button10On = false;
bool button11On = false;

// シーケンサーのデータ
enum {
  NUMBER_OF_STEPS = 16,
  NUMBER_OF_VOICE = 8,
};

uint8_t sequenceData[NUMBER_OF_STEPS][NUMBER_OF_VOICE] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  
};

uint16_t frequencies[NUMBER_OF_VOICE] = {
  262, 294, 330, 349, 392, 440, 494, 523
};

uint8_t currentStep = 0;

void timer(){
 currentStep++;
  if(currentStep >= NUMBER_OF_STEPS) {
     currentStep = 0;
  }
  
  uint8_t* const currentSequence = sequenceData[currentStep];
  
  bool hasNote = false;
  for (int i = 0; i < NUMBER_OF_VOICE; ++i) {
    if (currentSequence[i]) {
      aSin.setFreq((int)frequencies[i]);
      hasNote = true;
    }
  }  
  
  if (!hasNote) {
    aSin.setFreq(0);
  }
  
  const int volume = mozziAnalogRead(1);
  int tempo = 1023 - volume; // ボリュームを時計回りに回すと早くしたいので、値を逆に。
  if (tempo < 40) { // 速すぎ防止
    tempo = 40;
  }  
 
  MsTimer2::set(tempo, timer);
  MsTimer2::start();
}

void clearCurrentStep()
{
  uint8_t* const currentSequence = sequenceData[currentStep];
  for (int i = 0; i < NUMBER_OF_VOICE; ++i) {
    currentSequence[i] = 0;
  }
}

void setup(){
    // シーケンサーの開始
   MsTimer2::set(120, timer); // 120ms経つと timerが呼ばれる
   MsTimer2::start();
  
   startMozzi(CONTROL_RATE);
   pinMode(buttonPin2, INPUT);     
   pinMode(buttonPin3, INPUT);  
   pinMode(buttonPin4, INPUT);  
   pinMode(buttonPin5, INPUT);  
   pinMode(buttonPin6, INPUT_PULLUP);     
   pinMode(buttonPin7, INPUT_PULLUP);  
   pinMode(buttonPin8, INPUT_PULLUP);  
   pinMode(buttonPin10, INPUT_PULLUP);  
   pinMode(buttonPin11, INPUT_PULLUP);  
   Serial.begin(115200);
  
}

void updateControl(){
   formerButtonState2 = currentButtonState2;
   formerButtonState3 = currentButtonState3;
   formerButtonState4 = currentButtonState4;
   formerButtonState5 = currentButtonState5;
   formerButtonState6 = currentButtonState6;
   formerButtonState7 = currentButtonState7;
   formerButtonState8 = currentButtonState8;
   formerButtonState10 = currentButtonState10;
   formerButtonState11 = currentButtonState11;
   
   currentButtonState2 = digitalRead(buttonPin2);
   currentButtonState3 = digitalRead(buttonPin3);
   currentButtonState4 = digitalRead(buttonPin4);
   currentButtonState5 = digitalRead(buttonPin5);
   currentButtonState6 = digitalRead(buttonPin6);
   currentButtonState7 = digitalRead(buttonPin7);
   currentButtonState8 = digitalRead(buttonPin8);
   currentButtonState10 = digitalRead(buttonPin10);
   currentButtonState11 = digitalRead(buttonPin11);

   //チャタリング防止かつ配列に
   if (currentButtonState2 == formerButtonState2
     && currentButtonState2 == HIGH) {
       button2On = true;
       clearCurrentStep();
       sequenceData[currentStep][0] = 1;
       aSin.setFreq((int)frequencies[0]);
    } else {
      button2On = false;
    }
    
   if (currentButtonState3 == formerButtonState3
     && currentButtonState3 == HIGH) {
       button3On = true;
      clearCurrentStep();
       sequenceData[currentStep][1] = 1;
       aSin.setFreq((int)frequencies[1]);

    } else {
      button3On = false;
    }

   if (currentButtonState4 == formerButtonState4
     && currentButtonState4 == HIGH) {
       button4On = true;
       clearCurrentStep();
       sequenceData[currentStep][2] = 1;
       aSin.setFreq((int)frequencies[2]);
    } else {
      button4On = false;
    }

 if (currentButtonState5 == formerButtonState5
     && currentButtonState5 == HIGH) {
       button5On = true;
       clearCurrentStep();
       sequenceData[currentStep][3] = 1;
        aSin.setFreq((int)frequencies[3]);
    } else {
      button5On = false;
    }

    if (currentButtonState6 == formerButtonState6
     && currentButtonState6 == LOW) {
       button6On = true;
       clearCurrentStep();
       sequenceData[currentStep][4] = 1;
        aSin.setFreq((int)frequencies[4]);
    } else {
      button6On = false;
    }
    
   if (currentButtonState7 == formerButtonState7
     && currentButtonState7 == LOW) {
       button7On = true;
       clearCurrentStep();
       sequenceData[currentStep][5] = 1;
        aSin.setFreq((int)frequencies[5]);
    } else {
      button7On = false;
    }

    if (currentButtonState8 == formerButtonState8
     && currentButtonState8 == LOW) {
       button8On = true;
       clearCurrentStep();
       sequenceData[currentStep][6] = 1;
       aSin.setFreq((int)frequencies[6]);
    } else {
      button8On = false;
    }
    
     if (currentButtonState10 == formerButtonState10
     && currentButtonState10 == LOW) {
       button10On = true;
       clearCurrentStep();
       sequenceData[currentStep][7] = 1;
       aSin.setFreq((int)frequencies[7]);
    } else {
      button10On = false;
    }
   
     if (currentButtonState11 == formerButtonState11
     && currentButtonState11 == LOW) {
       button11On = true;
       clearCurrentStep();

    } else {
      button11On = false;
    }

}

int updateAudio()
{ 
  return aSin.next();
}

void loop(){
  audioHook();
}
