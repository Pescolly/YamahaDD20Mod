#define STEP_COUNT 8
#define DIM_LIGHT 100


int button0Output = 2;
int button0Input = 2;
int button1Output = 3;
int button1Input = 3;
int button2Output = 4;
int button2Input = 4;
int button3Output = 5;
int button3Input = 5;
int button4Output = 6;
int button4Input = 6;
int button5Output = 7;
int button5Input = 7;
int button6Output = 8;
int button6Input = 8;
int button7Output = 9;
int button7Input = 9;

enum StepState {
  CURRENT,
  ACTIVE,
  OFF
};

class Step
{
    private:
    StepState currentState; 
    
    int buttonInPin;
    int buttonLEDPin;

    bool active = false;
    
    public:
    Step(int _buttonInPin, int _buttonLEDPin)
    {
        this->currentState = OFF;
        this->buttonInPin = _buttonInPin;
        this->buttonLEDPin = _buttonLEDPin;
    }

    void poll()
    {
      Serial.println("poll");
       if (this->currentState == CURRENT){
            analogWrite(buttonLEDPin, 255);
       }
       else if (this->currentState == ACTIVE){
          analogWrite(buttonLEDPin, 70);
       }
       else {
        Serial.println("off");
          analogWrite(buttonLEDPin, 0);
       }
    }

    void enterStep()
    {
        this->currentState = CURRENT;
    }

    void exitStep()
    {
      Serial.println("exiting state");

      if(this->active){
        this->currentState = ACTIVE;
      }
      else {
        this->currentState = OFF; 
      }
    }

    void setActive(){
      this->active = true;
    }

};


class Track {
  private:
    Step *step0;
    Step *step1;
    Step *step2;
    Step *step3;
    Step *step4;
    Step *step5;
    Step *step6;
    Step *step7;

    Step *currentStep;

  public:
  Track()
  {     
   step0 = new Step(2,2);
   step1 = new Step(3,3);
   step2 = new Step(4,4);
   step3 = new Step(5,5);
   step4 = new Step(6,6);
   step5 = new Step(7,7);
   step6 = new Step(8,8);
   step7 = new Step(9,9);

   currentStep = step0;
   currentStep->setActive();
  }

  void incrementCurrentStep()
  {
      if (currentStep == step0) {   
        Serial.println("Setting to step 1");   
          currentStep = step1;
      }
      else if (currentStep == step1){
        Serial.println("Setting to step 2");
          currentStep = step2;
      }
      else if (currentStep == step2){
          Serial.println("Setting to step 0");
          currentStep = step3;
      }
      else if (currentStep == step3){
        Serial.println("Setting to step 0");
          currentStep = step4;
      }
      else if (currentStep == step4){
        Serial.println("Setting to step 0");
          currentStep = step5;
      }
      else if (currentStep == step5){
        Serial.println("Setting to step 0");
          currentStep = step6;
      }
      else if (currentStep == step6){
        Serial.println("Setting to step 0");
          currentStep = step7;
      }
      else if (currentStep == step7){
        Serial.println("Setting to step 0");
          currentStep = step0;
      }

      if (step0 != currentStep){ step0->exitStep(); }
      if (step1 != currentStep){ step1->exitStep(); }
      if (step2 != currentStep){ step2->exitStep(); }
      if (step3 != currentStep){ step3->exitStep(); }
      if (step4 != currentStep){ step4->exitStep(); }
      if (step5 != currentStep){ step5->exitStep(); }
      if (step6 != currentStep){ step6->exitStep(); }
      if (step7 != currentStep){ step7->exitStep(); }
      
      currentStep->enterStep();
  }

  void poll()
  {
      this->incrementCurrentStep();
      step0->poll();
      step1->poll();
      step2->poll();
      step3->poll();
      step4->poll();
      step5->poll();
      step6->poll();
      step7->poll();
  }
  
};

//main prog

Track track;
bool running = true;
float previousTime = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(button0Output, OUTPUT);
  pinMode(button1Output, OUTPUT);
  pinMode(button2Output, OUTPUT);
  pinMode(button3Output, OUTPUT);
  pinMode(button4Output, OUTPUT);
  pinMode(button5Output, OUTPUT);
  pinMode(button6Output, OUTPUT);
  pinMode(button7Output, OUTPUT);

  
}

void loop() 
{

  float tDelta = millis() - previousTime;
  float tempo = 1000;
  
  if (running && tDelta > tempo)
  {
      previousTime = millis();
      track.poll();
  }
}
