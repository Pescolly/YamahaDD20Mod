#define STEP_COUNT 8
#define DIM_LIGHT 100

int tempoInput = A0;

int button0Output = 2;
int button0Input = 36;
int button1Output = 3;
int button1Input = 34;
int button2Output = 4;
int button2Input = 32;
int button3Output = 5;
int button3Input = 30;
int button4Output = 6;
int button4Input = 28;
int button5Output = 7;
int button5Input = 26;
int button6Output = 8;
int button6Input = 24;
int button7Output = 9;
int button7Input = 22;

int track1Output = 31;
int track2Output = 33;
int track3Output = 35;
int track4Output = 37;


enum StepState {
  CURRENT,
  ACTIVE,
  OFF
};

class Step
{
    private:
    StepState currentLEDState; 
    
    int buttonInPin;
    int buttonLEDPin;

    bool active = false;
    
    public:
    Step(int _buttonInPin, int _buttonLEDPin)
    {
        this->currentLEDState = OFF;
        this->buttonInPin = _buttonInPin;
        this->buttonLEDPin = _buttonLEDPin;
        this->active = false;
    }

    void pollInput(int trackOutputPin)
    {      
       int val = digitalRead(buttonInPin);
       if (val == HIGH)
       {
          if (this->active)
          {
            Serial.println("turing off");
            this->currentLEDState = OFF;
            this->active = false;
            digitalWrite(buttonLEDPin, LOW);             
          }
          else 
          {
            Serial.println("turing on");
            this->currentLEDState = ACTIVE;
            this->active = true;
            digitalWrite(buttonLEDPin, HIGH);               
          }
       }
       
    }

    void setOutputs()
    {      
       if (this->currentLEDState == CURRENT){
          Serial.println("current");
          analogWrite(buttonLEDPin, 255);
       }
       else if (this->currentLEDState == ACTIVE){
          analogWrite(buttonLEDPin, 70);
       }
       else {
          analogWrite(buttonLEDPin, 0);
       }

       if (this->active){
          Serial.println("Writing to pin");
         
           digitalWrite(31, HIGH);
           delay(random(100));
           digitalWrite(31, LOW);
       }
    }

    void enterStep()
    {
        this->currentLEDState = CURRENT;
        digitalWrite(buttonLEDPin, HIGH);
    }

    void exitStep()
    {
      if(this->active){
        this->currentLEDState = ACTIVE;
      }
      else {
        this->currentLEDState = OFF; 
        digitalWrite(buttonLEDPin, LOW);
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

    int outputPin;

  public:
  Track(int _outputPin)
  {     
      this->outputPin = _outputPin;
      
      this->step0 = new Step(button0Input, button0Output);
      this->step1 = new Step(button1Input, button1Output);
      this->step2 = new Step(button2Input, button2Output);
      this->step3 = new Step(button3Input, button3Output);
      this->step4 = new Step(button4Input, button4Output);
      this->step5 = new Step(button5Input, button5Output);
      this->step6 = new Step(button6Input, button6Output);
      this->step7 = new Step(button7Input, button7Output);
      
      this->currentStep = step0;
  }

  void incrementCurrentStep()
  {
      currentStep->exitStep();
    
      if (currentStep == step0) {   
          currentStep = step1;
      }
      else if (currentStep == step1){
          currentStep = step2;
      }
      else if (currentStep == step2){
          currentStep = step3;
      }
      else if (currentStep == step3){
          currentStep = step4;
      }
      else if (currentStep == step4){
          currentStep = step5;
      }
      else if (currentStep == step5){
          currentStep = step6;
      }
      else if (currentStep == step6){
          currentStep = step7;
      }
      else if (currentStep == step7){
          currentStep = step0;
      }

//      if (step0 != currentStep){ step0->exitStep(); }
//      if (step1 != currentStep){ step1->exitStep(); }
//      if (step2 != currentStep){ step2->exitStep(); }
//      if (step3 != currentStep){ step3->exitStep(); }
//      if (step4 != currentStep){ step4->exitStep(); }
//      if (step5 != currentStep){ step5->exitStep(); }
//      if (step6 != currentStep){ step6->exitStep(); }
//      if (step7 != currentStep){ step7->exitStep(); }
      
      currentStep->enterStep();
  }

  void pollInputs()
  {
    step0->pollInput(this->outputPin);
    step1->pollInput(this->outputPin);
    step2->pollInput(this->outputPin);
    step3->pollInput(this->outputPin);
    step4->pollInput(this->outputPin);
    step5->pollInput(this->outputPin);
    step6->pollInput(this->outputPin);
    step7->pollInput(this->outputPin);
  }

  void setOutputs()
  {
      
      this->incrementCurrentStep();
      currentStep->setOutputs();
      
  }
};

//main prog

Track track(31);
bool running = true;
float previousTime = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(button0Output, OUTPUT);
  pinMode(button0Input, INPUT);
  pinMode(button1Output, OUTPUT);
  pinMode(button1Input, INPUT);
  pinMode(button2Output, OUTPUT);
  pinMode(button2Input, INPUT);
  pinMode(button3Output, OUTPUT);
  pinMode(button3Input, INPUT);
  pinMode(button4Output, OUTPUT);
  pinMode(button4Input, INPUT);
  pinMode(button5Output, OUTPUT);
  pinMode(button5Input, INPUT);
  pinMode(button6Output, OUTPUT);
  pinMode(button6Input, INPUT);
  pinMode(button7Output, OUTPUT);
  pinMode(button7Input, INPUT);

  pinMode(31, OUTPUT);

  analogReadResolution(12);
}


float tempoMSDelay = 1000;
void loop() 
{

  tempoMSDelay = analogRead(tempoInput);
  float tDelta = millis() - previousTime;

  int suck = random(0,100);
 
  track.pollInputs();
  if (running && tDelta > tempoMSDelay)
  {
      delay(suck);
      previousTime = millis();
      track.setOutputs();
  }
}
