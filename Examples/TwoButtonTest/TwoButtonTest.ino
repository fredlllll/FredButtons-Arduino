#include <FredButtons.h>

FredButtons<2,1> buttons;

void setup(){
  buttons.pins[0] = 4;
  buttons.pins[1] = 5;
  buttons.groups[0].btn1 = 0;
  buttons.groups[0].btn2 = 1;

  buttons.buttonPressed = buttonPressed;
  buttons.groupPressed = groupPressed;

  buttons.begin();

  Serial.begin(9600);
}

long t;

void loop(){
  if(millis()-t > 1000){
    t = millis();
    Serial.println("ping");
  }
  buttons.process();
  delay(50);//debouncing. should be done by hardware though
}

void buttonPressed(int index){
  Serial.print("pressed ");
  Serial.println(index);
}

void groupPressed(int index){
  Serial.println("pressed both");
}

