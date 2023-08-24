#include "buttonHandler_OLED.h"



void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(ENTER_BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(115200);
  Wire.begin();



  if(!rtc.begin()){
    Serial.println("Couldn't find RTC ");
    while(1);
  }
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed."));
    for(;;);
  }

  DateTime newTime(setDateArray[0],setDateArray[1],setDateArray[2],setTimeArray[0],setTimeArray[1]);
  rtc.adjust(newTime);




  delay(100);
  // display.clearDisplay();
  display.setTextSize(0); 
  display.setFont(&FreeSansBold9pt7b);

}

void loop() {
  // get the time at the start of this loop()
  unsigned long currentMillis = millis();
  
  ledCheck(BUTTON,currentMillis, ENTER_BUTTON);


  


  }
