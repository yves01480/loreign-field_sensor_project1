#include "myDHT22.h"
#include "mySSD1306.h"

const byte BUTTON=33; // our button pin
const byte ENTER_BUTTON = 25;
const byte LED=32; // LED (built-in on Uno)
unsigned long last_button_time;
unsigned long  buttonPushedMillis = 0; // when button was released
unsigned long ledTurnedOnAt; // when led was turned on
unsigned long previousMillis;
unsigned long finalTimeValue;
unsigned long buttonPressStartTime = 0;
const long interval = 3000;
bool ledReady = false; // flag for when button is let go
int digits[] = {0,1,2,3,4,5,6,7,8,9};
int ten_digits[] = {0,1,2,3,4,5,6,7,8,9};
int count = 0;
int count_2 = -1;
int count_3 = -1;
int countDownTime = 0;
int previousValue = 0;
int nextValue = 0;
int selected_menu = 0;
int confirmCount;
int confirmDigit;
bool digits_choose_zero;
bool preventStupidMode;
bool chooseMode;
bool soilmode;
bool timeCheckMode;
bool ten_digits_confirm;
bool ten_digits_set;
bool waterPumpMode;
bool digits_confirm;
bool buttonPressed = false;
const int NUM_ITEMS = 5; // number of items in the list and also the number of screenshots and screenshots with QR codes (other screens)
const int MAX_ITEM_LENGTH = 20; // maximum characters for the item name

char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {
  {"Time setting"},
  {"Soil data"},
  {"Temp."},
  {"Wifi"},
  {"Daily setting"}
};

void tiktokSetting(bool true_25 ,bool second_65 ,byte minutes_v ,byte seconds_v){
  if (true_25==true){
    display.setCursor(25, 50);
    display.print(minutes_v);
  }

  if (second_65==true){
    display.setCursor(65, 50);
    display.print(seconds_v);

}
  else if(second_65==false){
    display.setCursor(65, 50);
    display.print("0");
    display.setCursor(90, 50);
    display.print(seconds_v);

  }
    display.setCursor(1, 50);
    display.print("0");   
    display.setCursor(50, 50);
    display.print(":");
    

  }
     

void choosrModeSetting(int drawRectValue ,byte finalTimeValue_v){
  
      display.setTextSize(0);
      display.setFont(&FreeSansBold9pt7b);
      display.clearDisplay();
      display.drawRect(0,drawRectValue,90,23,WHITE);
      display.setCursor(5,36);             
      display.print("confirm?");
      display.setCursor(94,36);
      display.println(finalTimeValue_v);
      display.setCursor(5,55);
      display.print("cancel");
      display.display();

}

void displaySetting(byte count_v,byte previousValue_v,byte nextValue_v){
      display.drawBitmap(1,1,bitmap_icons[previousValue_v],16,16,1);
      display.setCursor(20,14); 
      display.println(menu_items[previousValue_v]);
      display.setCursor(20,36);     
      display.println(menu_items[count_v]);
      display.drawBitmap(1,24,bitmap_icons[count],16,16,1);
      display.setCursor(20,58);             
      display.println(menu_items[nextValue_v]);
      display.drawBitmap(1,46,bitmap_icons[nextValue_v],16,16,1);
      display.display();
};


void ledCheck(byte BUTTON, unsigned long currentMillis, byte ENTER_BUTTON){

  // bool buttonReady = buttonCheck(BUTTON);
  if (digitalRead(BUTTON)==0 && timeCheckMode != true) {
    
    display.clearDisplay();
    Serial.print("timeCheckMode:");
    Serial.println(timeCheckMode);
    digitalWrite(LED, HIGH);
    int arraySize = sizeof(menu_items)/sizeof(menu_items[0]);
    display.setFont(&FreeSansBold9pt7b);

      // check if the button has been pressed
      last_button_time = currentMillis;
      count = (count + 1) % arraySize; // increase count value each time button is pressed

      previousValue = count-1;
      nextValue = count+1;   
      display.setTextColor(WHITE);     
      display.drawRect(0,20,128,23,WHITE);
      if (count == 0){
        previousValue = arraySize-1;
        displaySetting(count,previousValue,nextValue);
      }

      else if (count == 4){
      nextValue = 0;
      displaySetting(count,previousValue,nextValue);
      }
      
 
      else{
      displaySetting(count,previousValue,nextValue);   
    }

    
      delay(10);

    }


  if ((unsigned long)(currentMillis - last_button_time) >= 10000 && waterPumpMode != true) {
    display.clearDisplay();
    display.display();
    // ledReady = false;
    digitalWrite(LED, LOW);
    count = 0;
    count_2 = 0;
    count_3 = 0;
    timeCheckMode = false;
    
    }

  if(digitalRead(LED) ==HIGH && digitalRead(ENTER_BUTTON)==0 &&  timeCheckMode != true){
    
    last_button_time = currentMillis;
    float temp_data = dht22_temp();
    float humid_data = dht22_humid();
    int selected_menu = 0;
    
    switch (count) {
      case 0:
      Serial.print("Count:");
      Serial.println(count);
      Serial.println("lock on timeCheckMode.");
      timeCheckMode = true;
      display.setFont(&FreeSansBoldOblique24pt7b);
      display.setTextSize(1); 
      //ten digits
      display.clearDisplay();
      display.setCursor(4,50);
      display.print(ten_digits[count]);
      //digits
      display.setCursor(30,50); 
      display.print(digits[0]);
      //units
      display.setCursor(60,50);
      display.setFont(&FreeSansBold9pt7b);
      display.setTextSize(0);
      display.print("mins ");
      display.display();
        break;
      case 1:
        // Soil data 的程式碼

        Serial.println(menu_items[count]);

        break;
      case 2:
        
        display.setFont(&FreeSans9pt7b);
        display.setTextSize(0); 
        display.clearDisplay();
        display.drawLine(63,0,63,63,WHITE);
        // display.drawBitmap(1,2,epd_bitmap_60X60[2],60,60,1);
        display.setCursor(1,40);      
        display.print(humid_data);  
        display.setCursor(71,40);
        display.print("%RH");
        
        display.setCursor(1,62);      
        display.print(temp_data);  
        display.setCursor(71,62);
        // display.print("°C");
        // display.print(&deg;);
        display.println("C");
        display.display();
        
        break;
      case 3:
        // Wifi 的程式碼

        Serial.println(menu_items[count]);
        break;
      

      default:

        Serial.println(menu_items[count]);
        break;
    }
  }

  if(timeCheckMode==true){
    previousMillis = millis();
    display.setFont(&FreeSansBoldOblique24pt7b);
    display.setTextSize(1); 


    if(digitalRead(BUTTON)==0 && digitalRead(LED)==HIGH){
      ten_digits_set = true;

      last_button_time = currentMillis;
      int ten_digitsSize = sizeof(ten_digits)/sizeof(ten_digits[0]);
      count_3 = (count_3 + 1) % ten_digitsSize;
      display.clearDisplay();
      display.setCursor(4,50);
      display.print(ten_digits[count_3]); 
      display.setCursor(30,50);
      display.print(digits[0]);
      display.setCursor(60,50);
      display.setFont(&FreeSansBold9pt7b);
      display.setTextSize(0);
      display.print("mins");
      display.display();
      Serial.println("十進位數據選擇");
    }

    //進入time Setting後直接按紅色按鈕
    else if(digitalRead(ENTER_BUTTON)==0 && digitalRead(LED)==HIGH && ten_digits_set ==false){
      confirmCount = 0;
      Serial.println("confirm 十進位數據");
      ten_digits_confirm = true;
      ten_digits_set = false;
      digitalWrite(LED,LOW);
      digits_choose_zero = false;
    }


    else if (digitalRead(ENTER_BUTTON)==0 && ten_digits_set == true){
      
      confirmCount = count_3;
      Serial.println("confirm 十進位數據");
      ten_digits_confirm = true;
      ten_digits_set = false;
      digitalWrite(LED,LOW);
      digits_choose_zero = true;
    }

      
    else if(digitalRead(BUTTON)==0 && ten_digits_confirm ==true && digitalRead(LED)==LOW){
      Serial.println("選擇個位數數據");
      last_button_time = currentMillis;
      int digitSize = sizeof(digits)/sizeof(digits[0]);
      
      count_2 = (count_2 + 1) % digitSize;
      display.clearDisplay();
      display.setCursor(4,50);
      display.print(ten_digits[confirmCount]);
      display.setCursor(30,50); 
      display.print(digits[count_2]);
      display.setCursor(60,50);
      display.setFont(&FreeSansBold9pt7b);
      display.setTextSize(0);
      display.print("mins");
      display.display();

    
      digits_confirm = true;
      digits_choose_zero = false;
    }

    else if(digitalRead(ENTER_BUTTON)==0 && digits_choose_zero==true){
       last_button_time = currentMillis;
       confirmDigit = 0;
       int ten_digits_value = ten_digits[confirmCount]*10;
      finalTimeValue = ten_digits_value+digits[confirmDigit];
       Serial.println("confirm 個位數數據");
       choosrModeSetting(20,finalTimeValue);
       Serial.println(finalTimeValue);
       ten_digits_confirm = false;
       digits_confirm = false;
       chooseMode = false;
       digits_choose_zero=false;
       
    }


    else if(digitalRead(ENTER_BUTTON)==0 && digits_confirm==true){
      last_button_time = currentMillis;
      Serial.println("confirm 個位數數據");
      confirmDigit = count_2;
      int ten_digits_value = ten_digits[confirmCount]*10;
      finalTimeValue = ten_digits_value+digits[confirmDigit];
      choosrModeSetting(20,finalTimeValue);
      Serial.println(finalTimeValue);
      ten_digits_confirm = false;
      digits_confirm = false;
      chooseMode = false;
    }

    else if(digitalRead(BUTTON)==0 && chooseMode==false){
      last_button_time = currentMillis;
      digits_confirm = false;
      ten_digits_confirm = false;
      chooseMode =true;
      choosrModeSetting(40,finalTimeValue);
      Serial.print("chooseMode:");
      Serial.println(chooseMode);

      
    }


    else if(digitalRead(BUTTON)==0 && chooseMode==true){
      last_button_time = currentMillis;
      digits_confirm = false;
      ten_digits_confirm = false;
      chooseMode = false;
      choosrModeSetting(20,finalTimeValue);
      Serial.print("chooseMode:");
      Serial.println(chooseMode);

    }

    //倒數計時開始
else if (digitalRead(ENTER_BUTTON) == 0 && chooseMode == false && timeCheckMode == true) {


  waterPumpMode = true;
  unsigned long finalTimeValue_mins = finalTimeValue * 1000ULL * 60ULL;
  unsigned long startTime = millis();

  
    Serial.println("倒數計時：");
    
    unsigned long elapsedTime = 0;
    unsigned long passValue;
    

for (unsigned long elapsedTime = 0; elapsedTime <= finalTimeValue_mins; elapsedTime += 1000) {
  display.setTextSize(1);
  display.clearDisplay();
  display.setFont(&FreeSansBoldOblique24pt7b);

  unsigned long remainingTime = finalTimeValue_mins - elapsedTime;
  unsigned long minutes = remainingTime / (60 * 1000);
  unsigned long seconds = (remainingTime / 1000) % 60;

  // if (digitalRead(BUTTON)==0 && !buttonPressed){
  //   buttonPressed = true;
  //   buttonPressStartTime = millis();
  // }

  // else if (buttonPressed && (millis()-buttonPressStartTime>=3000)){
  //   break;
  //   buttonPressed =false;
  // }

  if (minutes < 10 && seconds < 10) {
    display.clearDisplay();
    bool setMin =true;
    bool setSecond =false;
    tiktokSetting(setMin,setSecond,minutes,seconds);
  } 
  
  else if (minutes < 10) {
    display.clearDisplay();
    bool setMin =true;
    bool setSecond =true;
    tiktokSetting(setMin,setSecond,minutes,seconds);
  }
    
  else if (seconds < 10) {
    display.clearDisplay();
    bool setMin =false;
    bool setSecond =false;
    tiktokSetting(setMin,setSecond,minutes,seconds);
  }

  
  else {
    bool setMin =false;
    bool setSecond =true;
    tiktokSetting(setMin,setSecond,minutes,seconds);
  }

  Serial.print("剩餘時間：");
  Serial.print(minutes);
  Serial.print(" 分 ");
  Serial.print(seconds);
  Serial.println(" 秒");
  Serial.print("elapsedTime:");
  Serial.println(elapsedTime);
  Serial.print("finalTimeValue_mins:");
  Serial.println(finalTimeValue_mins);

  display.display();
  delay(1000);
}

  
  Serial.println("跳出迴圈、pump中止");


}


    //回到上一頁
    else if(digitalRead(ENTER_BUTTON)==0 && chooseMode==true && digitalRead(LED)==false){
      Serial.println("重新回到選擇十進位頁面");
      count_3 = 0;
      count_2 = 0;
      count = 0 ;
      nextValue = 1;
      previousValue = 3;
      finalTimeValue = 0;
      digitalWrite(LED,HIGH);
      ten_digits_set = false;
      timeCheckMode = false;
      chooseMode = false;
      Serial.print("timeCheckMode:");
      Serial.println(digitalRead(timeCheckMode));

       display.clearDisplay();
       display.setFont(&FreeSansBold9pt7b);
       display.drawRect(0,20,128,23,WHITE);
       displaySetting(count,previousValue,nextValue);

      count = 0;
      
    }


    delay(100);
}

};
