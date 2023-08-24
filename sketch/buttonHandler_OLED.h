#include "myDHT22.h"
#include "mySSD1306.h"
#include "dailySet.h"
// #include "myClock.h"
// #include "timeSetting.h"
// #include "basicSetting.h"


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
int count24 = 0;
int count12 = 0;
int setDateArray[3] = {-1,-1,-1};
int setTimeArray[4] = {-1,-1};
int twoDigits[] ={0,1};
int digits[] = {0,1,2,3,4,5,6,7,8,9};
int ten_digits[] = {0,1,2,3,4,5,6,7,8,9};
int x_rect[] = {27,40,61,74,95,110};
int timeArray[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59};
const char* periods[] = {"am","pm"};
int count = 0;
int count_2 = -1;
int count_3 = -1;
int count_time_adjust;
int count_time_radix = 0;
int countDownTime = 0;
int previousValue = 0;
int nextValue = 0;
int selected_menu = 0;
int confirmCount;
int confirmDigit;
int setYear1;
int setYear2;
int setDate1;
int setDate2 = 1;
int setDate3;
int setDate4=1;
int setAPM;
int setHour1=0;
int setHour2=0;
int setMin1=0;
int setMin2=0;
int test;
int hour;
int secTest = 50;
bool stopClock = false;
bool enterAdjustTiming;
bool setTiming;
bool returnState;
bool confirmReset;
bool confirmResetOption;
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
bool buttonReady;
bool optionFunc = true;
bool optionFunc2 = true;
const int NUM_ITEMS = 5; // number of items in the list and also the number of screenshots and screenshots with QR codes (other screens)
const int MAX_ITEM_LENGTH = 20; // maximum characters for the item name
const int NUM_OPTIONS = 4;
const int MAX_OPTION_LENGTH = 20;
bool yesNoChoose = true;
bool timeSetting = false;
bool yesNo = false;
bool setted;
bool confirmYesNo = false;
bool adjustTime; //調整日期
bool adjustTimeOption;
bool adjustTiming; //調整時間
bool adjustTimingOption;
bool initializeOption = true;
bool chooseTimeSetting;
bool confirmAdjustTimingOption;
bool confirmAdjustTiming = false;
bool confirmAdjustTiming_ENTER;
bool buttonTURNenterOn;



char menu_options [NUM_OPTIONS][MAX_OPTION_LENGTH] = {
  {"Setted"},
  {"Daily set"},
  {"Reset"},
  {"Cancel"}
};

char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {
  {"Countdown"},
  {"Soil data"},
  {"Temp."},
  {"Wifi"},
  {"Time setting"}
};




void yesNoFunc(bool yesNo_value){
  if(yesNo_value==true && digitalRead(BUTTON)==LOW){
    display.drawRect(0,40,40,23,WHITE);
    yesNo_value = !yesNo_value;

  }
  else if(yesNo_value==false && digitalRead(BUTTON)==LOW){
    display.drawRect(0,100,40,23,WHITE);
    yesNo_value = !yesNo_value;
  }


}



void optionSetting(byte count_v, byte previousValue_v, byte nextValue_v){
      // display.drawBitmap(1,1,bitmap_icons[previousValue_v],16,16,1);
      display.setCursor(5,15); 
      display.print(menu_options[previousValue_v]);
      display.setCursor(5,37);     
      display.print(menu_options[count_v]);
      // display.drawBitmap(1,24,bitmap_icons[count],16,16,1);
      display.setCursor(5,57);             
      display.print(menu_options[nextValue_v]);
      // display.drawBitmap(1,46,bitmap_icons[nextValue_v],16,16,1);
      display.display();
}


void chooseModeSetting(int drawRectValue ,byte finalTimeValue_v){
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
 
if(stopClock==false){
   DateTime now = rtc.now();   
if (setDateArray[0] != -1 && setDateArray[1] != -1 && setTimeArray[0] != -1 && setTimeArray[1] != -1) {
   
    display.setTextSize(0);
    display.clearDisplay();
  

    display.setCursor(30, 40);
    display.println(":");
    display.setCursor(70, 40);
    display.println(":");

    
    if(now.hour()>12){ //處理下午時間 13~23
      int hour12set = now.hour()-12;
      display.setCursor(0, 40);
      display.print("0");
      display.println(hour12set, DEC);
      // Serial.println("now.hour()>12");

    }
    else if(now.hour()<12 && now.hour()!=0){ //處理午夜到上午時間 12~11
      display.setCursor(0, 40);
      display.print("0");
      display.println(now.hour(),DEC);
      // Serial.println("now.hour()<12 && now.hour()!=0");
    }

    else if(now.hour()==12){ 
      display.setCursor(0, 40);
      display.println(now.hour(), DEC);
      // Serial.println("現在走now.hour()==0 && setTimeArray[2]==0");
    }
    
    else if(now.hour()==0){
      int hour12set = now.hour()+12;
      display.setCursor(0,40);
      display.println(hour12set ,DEC);
      // Serial.println("現在走now.hour()==0");
    }

    
    if(now.hour()==12 && now.minute()==0 && now.second()==0 || now.hour()==0 && now.minute()==0 && now.second()==0){     
      count12 = count12+1;
      Serial.print("count12:");
      Serial.println(count12);
      if(count12%2==1){
          setTimeArray[2] = 1; //pm
          Serial.println("現在應該要輸出pm");
        }else{
          setTimeArray[2] = 0; //am
          Serial.println("現在應該要輸出am"); 
          
        }
          delay(900);
      }

    if(setTimeArray[2]==0){
      display.setCursor(80,55);
      display.print(periods[0]); //am
    }  
      else{
        display.setCursor(80,55);
        display.print(periods[1]); //pm
      }
    
    // display.setCursor(70,40);  
    // display.println(setTimeArray[2]);

    if(now.minute()<10){
      display.setCursor(40, 40);
      display.print("0");
      display.println(now.minute(), DEC); 
    }
    else{
      display.setCursor(40,40);
      display.println(now.minute(),DEC);
    }
    
    if(now.second()<10){
    display.setCursor(80, 40);
    display.print("0");
    display.println(now.second(), DEC);
    }
    else{
      display.setCursor(80,40);
      display.println(now.second(),DEC);
    }      
    display.setCursor(0, 20);
    display.println(now.year(), DEC);

    display.setCursor(55, 20);
    display.println("-");

    display.setCursor(85, 20);
    display.println("-");
     
    if(now.month()<10){
      display.setCursor(60, 20);
      display.print("0");
      display.println(now.month(), DEC);  
    }else{
      display.setCursor(60, 20);
      display.print("0");
      display.println(now.month(), DEC);  
    }
    
    if(now.day()<10){
      display.setCursor(90, 20);
      display.print("0");
      display.println(now.day(), DEC);
    }
    else{
      display.setCursor(90,20);
      display.println(now.day(), DEC);
    }
       
    


if(count12%2==0){
  count12 = 0;
}
else if (count12%2==1){
  count12 = 1;
}


} else {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(0); 
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(1, 15);
    display.print("Push green button to set the timing.");
}


display.display();




}

  // bool buttonReady = buttonCheck(BUTTON);
  if (digitalRead(BUTTON)==0 && timeCheckMode != true && buttonReady !=true) {
    display.clearDisplay();
    stopClock = true;
    Serial.println("what");
    Serial.print("timeSetting:");
    Serial.println(timeSetting);
    Serial.print("buttonReady:");
    Serial.println(buttonReady);

    // Serial.print("timeCheckMode:");
    // Serial.println(timeCheckMode);
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

  //REBOOT
  // if ((unsigned long)(currentMillis - last_button_time) >= 10000 && waterPumpMode != true) {
  //   display.clearDisplay();
  //   display.display();
  //   // ledReady = false;
  //   digitalWrite(LED, LOW);
  //   count = 0;
  //   count_2 = 0;
  //   count_3 = 0;
  //   timeCheckMode = false;
  //   timeSetting=false;
    
  //   }



  if(digitalRead(LED) ==HIGH && digitalRead(ENTER_BUTTON)==0 &&  timeCheckMode != true && timeSetting != true){
    
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

      case 4:
        //Time Setting
        Serial.print("This is: ");
        Serial.println(menu_items[count]);
        timeSetting = true;
        digitalWrite(LED,LOW);
        display.clearDisplay();
        // buttonPressStartTime = millis();
       
        display.setTextColor(WHITE);     
        display.drawRect(0,20,128,23,WHITE);
        optionSetting(0,3,1);
        display.display();
        
        break;
      

      default:

        Serial.println(menu_items[count]);
        break;
    }
  }


  if(timeSetting==true){
    buttonReady=true;
    int arraySize = sizeof(menu_options)/sizeof(menu_options[0]);
   
    
    display.setFont(&FreeSansBold9pt7b);
  
    // optionSetting(count,previousValue,nextValue);
    if(digitalRead(BUTTON)==LOW && buttonReady==true && digitalRead(LED)==LOW){
      
    
      // check if the button has been pressed
      last_button_time = currentMillis;
      count = (count + 1) % arraySize; // increase count value each time button is pressed
      display.clearDisplay();
      display.setTextColor(WHITE);     
      Serial.print("count:");
      Serial.println(count);
      if(count==0){
        //setted
        optionSetting(0,3,1);
        Serial.println("setted");

      }
      else if (count==1){
        //daily set
        optionSetting(1,0,2);
        Serial.println("dailySet");
      }
      else if (count==2){
        //reset
        Serial.println("reset");
        optionSetting(2,1,3);
      }
      else if (count==3){
        //cancel
        optionSetting(3,2,0);
        Serial.print("cancel");
      }
      display.drawRect(0,20,128,23,WHITE);
      display.display();
      
      
  };  
    if(digitalRead(ENTER_BUTTON)==LOW && digitalRead(LED)==LOW){
     
      //Serial.println(menu_options[count]);
     switch(count){
      case 0:
      //setted
      Serial.println(menu_options[count]);
      setted = true;
      
      break;

      case 1:
      //dailySet
      Serial.println(menu_options[count]);
      dailySet(BUTTON);


      break;

      case 2:
      yesNo=true;
      Serial.println("這邊給yesNo賦值為true");
      //confirmYesNo = false;
      // Serial.println(menu_options[count]);
      digitalWrite(LED,HIGH);
      // Serial.println(buttonReady);
      display.clearDisplay();
      display.setCursor(1,12); 
      display.print("Initialize all ti-ming data?");
      // display.drawRect(0,40,40,23,WHITE);
      display.setCursor(1,57);
      display.print("Yes");
      display.setCursor(70,57);
      display.print("No");
      display.display();
      break;

      case 3:
      Serial.println(menu_options[count]);
      display.clearDisplay();
      digitalWrite(LED,HIGH);
      timeSetting = false;
      count = 4 ;
      previousValue = 3;
      nextValue = 0;
      buttonReady = false;
      //digitalWrite(LED,HIGH);
      display.drawRect(0,20,128,23,WHITE);
      displaySetting(count,previousValue,nextValue);
      Serial.print("yesNo:");
      Serial.println(yesNo);
      break;
     }
     display.display();
    }
    // timSetting
    delay(10);
  };

  if(setted==true){

  }

  if(yesNo==true){

    if (digitalRead(BUTTON)==LOW && yesNoChoose==true && initializeOption ==true){
              Serial.println("進入yesNO模式");
    display.clearDisplay();
    display.setCursor(1,12); 
      display.print("Initialize all ti-ming data?");
      // display.drawRect(0,40,40,23,WHITE);
      display.setCursor(1,57);
      display.print("Yes");
      display.setCursor(70,57);
      display.print("No");
    display.drawRect(0,40,40,23,WHITE);
    yesNoChoose = false;
   
    display.display();

    }

    else if (digitalRead(BUTTON)==LOW && yesNoChoose==false && initializeOption ==true){
      display.clearDisplay();
      display.setCursor(1,12); 
      display.print("Initialize all ti-ming data?");
      // display.drawRect(0,40,40,23,WHITE);
      display.setCursor(1,57);
      display.print("Yes");
      display.setCursor(70,57);
      display.print("No");
      Serial.println("no");
      display.drawRect(60,40,40,23,WHITE);
      yesNoChoose = true;
      
      display.display();
    }
    delay(50);

    if (digitalRead(ENTER_BUTTON)==LOW && yesNoChoose ==false && initializeOption ==true){
      Serial.println("You push the yes button.");
      yesNo = false;
      adjustTime = true;
      digitalWrite(LED,LOW);
      adjustTimeOption = true;
      // initializeOption = false;
      //digitalWrite(LED,HIGH);
      //confirmYesNo = true;
      display.clearDisplay();
      display.setTextSize(0);
      display.setFont(&FreeSansBold12pt7b);
      display.setCursor(1,40);
      display.print("2000-01-01");

      // display.setCursor(1,40);
      // display.drawRect(27,20,15,23,WHITE);
      display.display();

      setYear1 = 0;
      setYear2 = 0;
      setDate1 = 0;
      setDate2 = 1;
      setDate3 = 0;
      setDate4=1;
      setAPM;
      setHour1=0;
      setHour2=0;
      setMin1=0;
      setMin2=0;
    }


    else if (digitalRead(ENTER_BUTTON)==LOW && yesNoChoose ==true && initializeOption ==true){
      Serial.println("You push the no button.");
      display.clearDisplay();
      //confirmYesNo = false;
      /*
      count = 4 ;
      previousValue = 3;
      nextValue = 0;
      */
      digitalWrite(LED,LOW);
      
      optionSetting(2,1,3);
      display.drawRect(0,20,128,23,WHITE);
      // displaySetting(count,previousValue,nextValue);
      
      Serial.print("yesNo:");
      Serial.println(yesNo);
      
      timeSetting = true;
      adjustTime=false;
      adjustTimeOption = false;
      yesNo = false;
      display.display();
    }

  }

  if (adjustTime==true){
        

    digitalWrite(LED,HIGH);
    initializeOption = false;
    display.clearDisplay();        
    display.setTextSize(0);
    display.setFont(&FreeSansBold12pt7b);
    
    if (digitalRead(ENTER_BUTTON)==LOW && digitalRead(LED)==HIGH && adjustTimeOption ==true && returnState == true){
      // int newCountTimeRadix = updateCountTimeRadix(x_rect, count_time_radix);
      Serial.println("進入adjustTime模式");
      display.clearDisplay();

      int digitSize = sizeof(x_rect) / sizeof(x_rect[0]);
      count_time_radix = (count_time_radix + 1) % digitSize;
      Serial.println(count_time_radix);
      // display.print(x_rect[count_time_radix]);
   
      // display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
      display.setCursor(1,40);
      display.print("20");
      display.display();

      // Serial.println(newCountTimeRadix);
      switch(count_time_radix){
      case 0:
      Serial.print("count_time_radix:");
      Serial.println(count_time_radix);
      display.print(setYear1);
      display.print(setYear2);
      display.print("-");
      display.print(setDate1);
      display.print(setDate2);
      display.print("-");
      display.print(setDate3);
      display.print(setDate4);
      display.setFont(&FreeSansBold9pt7b);
      display.setCursor(1,60);
      display.print("YES");
      display.setCursor(70,60);
      display.print("NO");
      display.drawRect(0,41.8,45,23,WHITE);
      adjustTimeOption = false;
      confirmReset = true;
      chooseTimeSetting =true;
      confirmResetOption = false;


      break;
      case 1:
      display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
      Serial.print("setYear1:");
        Serial.println(setYear1);
        display.print(setYear1);
        display.print("0-01-01");

      break;
      case 2:
       display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
        display.print(setYear1);
        display.print(setYear2);
        display.print("-01-01");


      break;
      case 3:
      display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
      display.print(setYear1);
      display.print(setYear2);
        // display.drawRect(0,20,15,23,WHITE);
      display.print("-");
      display.print(setDate1);
      display.print("1-01");

      break;
      case 4:
      display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
      display.print(setYear1);
      display.print(setYear2);
      display.print("-");
      display.print(setDate1);
      display.print(setDate2);
      display.print("-01");


      break;
      case 5:
      display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
        display.print(setYear1);
        display.print(setYear2);
        display.print("-");
        display.print(setDate1);
        display.print(setDate2);
        display.print("-");
        display.print(setDate3);
        display.print(setDate4);
        // returnState = false;

      break;
           Serial.println(count_time_radix);


      }
      display.display();

    }

    if(digitalRead(BUTTON)==LOW && digitalRead(LED)==HIGH && adjustTimeOption ==false && confirmReset==true &&  confirmResetOption==true){
      display.clearDisplay();
      display.setCursor(1,40);
           display.print("20");
        display.print(setYear1);
        display.print(setYear2);
        display.print("-");
        display.print(setDate1);
        display.print(setDate2);
        display.print("-");
        display.print(setDate3);
        display.print(setDate4);
        display.setFont(&FreeSansBold9pt7b);
        display.setCursor(1,60);
        display.print("YES");
        display.drawRect(0,41.8,45,23,WHITE);
        display.setCursor(70,60);
        display.print("NO");
        display.display();
        confirmResetOption = false;
        Serial.print("confirmResetOption:");
        Serial.println(confirmResetOption);
    }

    else if (digitalRead(BUTTON)==LOW && digitalRead(LED)==HIGH && adjustTimeOption ==false && confirmReset==true &&  confirmResetOption==false){
      display.clearDisplay();
      display.setCursor(1,40);
      display.print("20");
      display.print(setYear1);
      display.print(setYear2);
      display.print("-");
      display.print(setDate1);
      display.print(setDate2);
      display.print("-");
      display.print(setDate3);
      display.print(setDate4);
      display.setFont(&FreeSansBold9pt7b);
      display.setCursor(1,60);
      display.print("YES"); 
      display.setCursor(70,60);
      display.drawRect(69,41.8,30,23,WHITE);
      display.print("NO");
      display.display();
      confirmResetOption = true;

      Serial.print("confirmResetOption:");
      Serial.println(confirmResetOption);
    }

    if(digitalRead(ENTER_BUTTON)==LOW && confirmResetOption ==false && adjustTimeOption == false &&confirmReset ==true){
      display.clearDisplay();
      display.setCursor(1,25);
      display.print("20");
      display.print(setYear1);
      display.print(setYear2);
      display.print("-");
      display.print(setDate1);
      display.print(setDate2);
      display.print("-");
      display.print(setDate3);
      display.print(setDate4);
      display.setCursor(1,50);
      display.print("00");
      display.setCursor(35,50);
      display.print(":");
      display.setCursor(50,50);
      display.print("00");
      display.setCursor(80,50);

      if(setTimeArray[2]==0){
         display.setCursor(80,50);
         display.print(periods[0]);
      }

      else{
        display.setCursor(80,50);
        display.print(periods[1]);
      }
      // display.drawRect(0,30,30,23,WHITE);
      Serial.println("Yes!");
       int setYearFinal = 2000+setYear1*10+setYear2; 
       Serial.println(setYearFinal);
       int setMonthFinal = setDate1*10+setDate2;
       Serial.println(setMonthFinal);
       int setDateFinal = setDate3*10+setDate4;  
       Serial.println(setDateFinal);

       setDateArray[0] = setYearFinal;
       setDateArray[1] = setMonthFinal;
       setDateArray[2] = setDateFinal;

      Serial.print(setDateArray[0]);
      Serial.print("-");
      Serial.print( setDateArray[1]);
      Serial.print("-");
      Serial.println( setDateArray[2]);

 

      // confirmReset = false;
      adjustTimingOption = true;
      adjustTiming=true;

      confirmReset = false;
      int digitSize = sizeof(x_rect) / sizeof(x_rect[0]);
      count_time_radix = (count_time_radix + 1) % digitSize;
      Serial.println(count_time_radix);

      display.display();
      count = 0;

    }

    if (digitalRead(ENTER_BUTTON)==LOW&& adjustTiming==true && adjustTimingOption==true && enterAdjustTiming ==true){
      //count12 = 0;
      
      adjustTimingOption = false;
      
      display.clearDisplay();
      display.setCursor(1,25);
      display.print("20");
      display.print(setYear1);
      display.print(setYear2);
      display.print("-");
      display.print(setDate1);
      display.print(setDate2);
      display.print("-");
      display.print(setDate3);
      display.print(setDate4);
      display.drawRect(50,30,30,23,WHITE);
      display.setCursor(35,50);
      display.print(":");
      display.setCursor(50,50);
      display.print("00");
      display.setCursor(80,50);
      if(setTimeArray[2]==0){
         setTimeArray[0] = count;
         setTimeArray[3] = count;
         display.setCursor(80,50);
         display.print(periods[0]);
      }

      else{
        count24 = count+12;
        setTimeArray[3] = count24;
        setTimeArray[0] = count;
        Serial.print("24小時制：");
        Serial.println(setTimeArray[3]);
        display.setCursor(80,50);
        display.print(periods[1]);
      }

      
      
      Serial.print("確認時針：");
      Serial.println(setTimeArray[0]);
      if(setTimeArray[0]<10){
        display.setCursor(1,50);
        display.print(0);
        display.setCursor(15,50);
        display.print(setTimeArray[0]);
      }
      else{
        display.setCursor(1,50);
        display.print(setTimeArray[0]);
      }
      display.display();
      count24 = 0;
      count = 0;

      adjustTimingOption = false;


    }

    if (digitalRead(ENTER_BUTTON)==LOW&& adjustTiming==true && adjustTimingOption==false ){
      Serial.println("adjustTiming==false");
      display.setFont(&FreeSansBold9pt7b);
      display.clearDisplay();
      
      display.setCursor(1,12);
      display.print("Confirm the ti-me setting?");
      display.setCursor(1,57);
      display.print("Yes");
      display.setCursor(70,57);
      display.print("No");   
      display.display();

      
      adjustTiming = false;
      confirmAdjustTiming = true;
      // enterAdjustTiming = true;
    }



    if (digitalRead(BUTTON)==LOW  && adjustTiming==true && adjustTimingOption==true){
      Serial.println("調整時針");
      display.clearDisplay();
      display.setCursor(1,25);
      display.setCursor(1,25);
      display.print("20");
      display.print(setYear1);
      display.print(setYear2);
      display.print("-");
      display.print(setDate1);
      display.print(setDate2);
      display.print("-");
      display.print(setDate3);
      display.print(setDate4);
      
      int arraySize = sizeof(timeArray)/sizeof(timeArray[0]);
      count = ((count%12)+1);
      Serial.println(count);
      if (count<10){
        display.setCursor(1,50);
        display.print(0);
        display.setCursor(15,50);
        display.println(count);
      }
      else{
        display.setCursor(1,50);
        display.print(count);
      }

      if (count==12){
        count12++;
        Serial.print("count12:");
        Serial.println(count12);
        if(count12%2==1){
          setTimeArray[2] = 1; //pm
        }else{
          setTimeArray[2] = 0; //am
        }
        Serial.print("count12%2:");
        int count13 = count12%2;
        Serial.println(count13);
      }

      Serial.print("setTimeArray[2]:");
      Serial.println(setTimeArray[2]);


      display.setCursor(35,50);
      display.print(":");
      display.setCursor(50,50);
      display.print("00");


      display.drawRect(0,30,30,23,WHITE);

      if(setTimeArray[2]==0){
         display.setCursor(80,50);
         display.print(periods[0]); //am
      }

      else{
        display.setCursor(80,50);
        display.print(periods[1]); //pm
      }



      display.display();

      enterAdjustTiming = true;


      delay(10);



    }

    else if (digitalRead(BUTTON)==LOW && adjustTiming==true && adjustTimingOption==false){
   
      display.clearDisplay();
      display.clearDisplay();
      display.setCursor(1,25);
      display.setCursor(1,25);
      display.print("20");
      display.print(setYear1);
      display.print(setYear2);
      display.print("-");
      display.print(setDate1);
      display.print(setDate2);
      display.print("-");
      display.print(setDate3);
      display.print(setDate4);
      if(setTimeArray[0]<10){
        display.setCursor(1,50);
        display.print(0);
        display.setCursor(15,50);
        display.print(setTimeArray[0]);
      }
      else{
        display.setCursor(1,50);
        display.print(setTimeArray[0]);
      }

      int arraySize = sizeof(timeArray)/sizeof(timeArray[0]);
      count = (count + 1) % arraySize;
      if(count<10){
        display.setCursor(50,50);
        display.print(0);
        display.setCursor(65,50);
        display.print(count);
      }
      else{
        display.setCursor(50,50);
        display.print(count);
      }
      display.drawRect(50,30,30,23,WHITE);
      display.setCursor(35,50);
      display.print(":");

            if(setTimeArray[2]==0){
         display.setCursor(80,50);
         display.print(periods[0]);
      }

      else{
        display.setCursor(80,50);
        display.print(periods[1]);
      }

      display.display();
 

    }



    else if (digitalRead(ENTER_BUTTON)==LOW && confirmReset==true && confirmResetOption==true && adjustTimeOption == false){
      Serial.println("No");
    setYear1 = 0;
    setYear2 = 0;
    setDate1 = 0;
    setDate2 = 1;
    setDate3 = 0;
    setDate4 = 1;
    count_time_radix = 0;
    display.setCursor(1,40);
    display.print("20");
     display.print(setYear1);
        display.print(setYear2);
        display.print("-");
        display.print(setDate1);
        display.print(setDate2);
        display.print("-");
        display.print(setDate3);
        display.print(setDate4);
        display.display();
    adjustTimeOption =true;
    returnState = false;


    }
    
    if (digitalRead(BUTTON) == LOW && digitalRead(LED) == HIGH && adjustTimeOption == true) {
      display.clearDisplay();
      returnState = true;
      int tenDigitYearOption = sizeof(twoDigits) / sizeof(twoDigits[0]);
      int digitYearOption = sizeof(digits) /sizeof(digits[0]);
      //int ten_digitsSize = sizeof(ten_digits)/sizeof(ten_digits[0]);
      display.setCursor(1,40);
      display.print(20);


       switch(count_time_radix){
        case 0:
        //十位數年份
        setYear1 = (setYear1 + 1) % digitYearOption;
        display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
        display.print(setYear1);
        display.print("0-01-01");
        break;

        case 1:
        //個位數年分
        setYear2 = (setYear2 + 1) % digitYearOption;
        display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
        display.print(setYear1);
        display.print(setYear2);
        display.print("-01-01");
        break;

        case 2:
        //十位數月份
        
        setDate1 = (setDate1 + 1) % tenDigitYearOption; 
        display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
        display.print(setYear1);
        display.print(setYear2);
        // display.drawRect(0,20,15,23,WHITE);
        display.print("-");
        display.print(setDate1);
        display.print("1-01");
        Serial.print("setDate:");
        Serial.println(setDate1);
        break;

        case 3:
        //個位數月份
        display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
        setDate2  = (setDate2 % (digitYearOption-1))+1;

        if (setDate1==1){
          setDate2 = (setDate2+3) % 3;

        }

        else if(setDate2==0){
          setDate2 = digitYearOption -1;
        }

        display.print(setYear1);
        display.print(setYear2);
        display.print("-");
        display.print(setDate1);
        display.print(setDate2);
        display.print("-01");
        break;

        case 4:
        //十位數日期
        display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
        setDate3  = (setDate3 % (digitYearOption-1))+1;
        if (setDate1 == 0  && setDate2 !=2){
           setDate3 = (setDate3 % 4);
        }

        else if (setDate1 == 1){
          setDate3 = (setDate3 % 4);
          setDate4 = 0;
        }

        else if (setDate2 ==2){
          setDate3 = (setDate3) % 3;
        }
        display.print(setYear1);
        display.print(setYear2);
        display.print("-");
        display.print(setDate1);
        display.print(setDate2);
        display.print("-");
        display.print(setDate3);
        display.print(setDate4);
       
        break;

        case 5:
        //個位數日期
        display.drawRect(x_rect[count_time_radix],20,15,23,WHITE);
        setDate4  = (setDate4 % (digitYearOption-1));
        //如果日期是0開頭，那個位數只會出現1~9
        if(setDate3==0){
          setDate4  = (setDate4 % (digitYearOption-1))+1;
          
          
        }
        //處理二月份的日期
        else if(setDate2==2 && setDate3==2){
            setDate4= (setDate4 + 1) % 9;
        }

        //如果日期是1,2開頭，個位數會出現0~9
       
        else if (setDate3==1||setDate3==2){
          
          test = (test+1) % 10;
          Serial.println(test);
          setDate4 = test;
        }

        //如果是大月，並且日期十位數為3時，個位數要出現0或1
        else if(setDate3==3 && setDate1==0 && setDate2==1||setDate2==3||setDate2==5||setDate2==7||setDate2==8){
          setDate4 = (setDate4 +1)%2;
        }

        else if (setDate3==3&&setDate1==1 && setDate2==0||setDate2==2){
          setDate4 = (setDate4 +1)%2;
        }

        //如果是小月，並且日期十位數為3，個位數為0
        else if(setDate3==3 && setDate1==1 && setDate2==4||setDate2==6||setDate2==9|| setDate1 ==1 && setDate2==1){
          setDate4 = 0;
        }

        display.print(setYear1);
        display.print(setYear2);
        display.print("-");
        display.print(setDate1);
        display.print(setDate2);
        display.print("-");
        display.print(setDate3);
        display.print(setDate4);
         break;

       }
      
      display.display();   
    }

      if (digitalRead(BUTTON)==LOW && adjustTiming == false && confirmAdjustTiming ==true && confirmAdjustTimingOption ==true){
      display.setFont(&FreeSansBold9pt7b);
      display.clearDisplay();
      display.drawRect(0,40,40,23,WHITE);
      display.setCursor(1,12);
      display.print("Confirm the ti-me setting?");
      display.setCursor(1,57);
      display.print("Yes");
      display.setCursor(70,57);
      display.print("No");
      display.display();
      confirmAdjustTimingOption = false;
      confirmAdjustTiming = true;
      confirmAdjustTiming_ENTER = true;
      buttonTURNenterOn = true;

    }

     if (digitalRead(BUTTON)==LOW && adjustTiming == false && confirmAdjustTiming ==true && confirmAdjustTimingOption ==false ){
      display.setFont(&FreeSansBold9pt7b);
      display.clearDisplay();
      display.drawRect(60,40,40,23,WHITE);
      display.setCursor(1,12);
      display.print("Confirm the ti-me setting?");
      display.setCursor(1,57);
      display.print("Yes");
      display.setCursor(70,57);
      display.print("No");
      display.display();
      confirmAdjustTimingOption = true;
      confirmAdjustTiming_ENTER = false;
      buttonTURNenterOn = true;
      // confirmAdjustTiming = false;
    }

    if (digitalRead(ENTER_BUTTON)==LOW && adjustTiming == false && confirmAdjustTiming ==true && confirmAdjustTiming_ENTER == false && buttonTURNenterOn == true){
      Serial.println("NoNo");  
      display.clearDisplay();
      display.setCursor(1,25);
      display.setCursor(1,25);
      display.print("20");
      display.print(setYear1);
      display.print(setYear2);
      display.print("-");
      display.print(setDate1);
      display.print(setDate2);
      display.print("-");
      display.print(setDate3);
      display.print(setDate4);
/*      
      if (count<10){
        display.setCursor(1,50);
        display.print(0);
        display.setCursor(15,50);
        display.println(count);
      }
      else{
        display.setCursor(1,50);
        display.print(count);
      }

      if (count==12){
        count12++;
        Serial.println(count12);
        if(count12%2==1){
          setTimeArray[2] = 1;
        }else{
          setTimeArray[2] = 0;
        }
      }
*/      
      display.setCursor(1,50);
      display.print("00");
      display.setCursor(35,50);
      display.print(":");
      display.setCursor(50,50);
      display.print("00");
      display.setCursor(80,50);
      display.print(periods[0]);


      display.drawRect(0,30,30,23,WHITE);
/*
      if(setTimeArray[2]==1){
         display.setCursor(80,50);
         display.print(periods[1]);
      }

      else{
        display.setCursor(80,50);
        display.print(periods[0]);
      }
*/

      adjustTiming=true;
      adjustTimingOption=true;
      display.display();
      count = 0;
      //count12 = 0;
      buttonTURNenterOn = false;



      delay(10);

 
    }
 
    if (digitalRead(ENTER_BUTTON)==LOW && adjustTiming ==false && confirmAdjustTiming == true && confirmAdjustTiming_ENTER ==true){

      Serial.println("This will run RTC Sensor finally.");
      Serial.print("count12:");
      Serial.println(count12);
      digitalWrite(LED,LOW);
      // Serial.println(digitalRead(LED));
      setTimeArray[1] = count;
      display.clearDisplay();
      // display.setCursor(1,25);
      Serial.print(setDateArray[0]);
      Serial.print("-");
      Serial.print( setDateArray[1]);
      Serial.print("-");
      Serial.println( setDateArray[2]);
      Serial.print(setTimeArray[0]);
      Serial.print(":");
      Serial.print(setTimeArray[1]);
      if(setTimeArray[2]==0){
         display.setCursor(80,50);
         Serial.println(periods[0]); //am
      }

      else{
        display.setCursor(80,50);
        Serial.println(periods[1]); //pm
      }

      display.display();
      // timeCheckMode != true;
      // timeSetting != true;
      initializeOption = true;
      // adjustTime = false;
      yesNo = false;
      buttonReady = false;
      adjustTime = false;
      adjustTimeOption = false;
      confirmAdjustTiming = false;
      confirmAdjustTiming_ENTER = false;
      yesNoChoose = true;
      returnState = false;
      enterAdjustTiming = false;
      count_time_radix = 0;
      //count12 = 0;
      count = 0;
      stopClock = false;
      timeSetting = false;
      buttonTURNenterOn == false;
      DateTime newTime(setDateArray[0],setDateArray[1],setDateArray[2],setTimeArray[3],setTimeArray[1],secTest);
      rtc.adjust(newTime);

   


    }

    // if (digitalRead(ENTER_BUTTON)==LOW && adjustTiming ==false &&  confirmAdjustTiming ==false ){
    //   count = 0;
    //   adjustTiming =true;
    //   adjustTimingOption==true;
    //   display.clearDisplay();
    //   display.display();


    // }



    delay(10);
    
    // int valueOfAdjustment = timeAdjustment();
  

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
       chooseModeSetting(20,finalTimeValue);
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
      chooseModeSetting(20,finalTimeValue);
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
      chooseModeSetting(40,finalTimeValue);
      Serial.print("chooseMode:");
      Serial.println(chooseMode);

      
    }


    else if(digitalRead(BUTTON)==0 && chooseMode==true){
      last_button_time = currentMillis;
      digits_confirm = false;
      ten_digits_confirm = false;
      chooseMode = false;
      chooseModeSetting(20,finalTimeValue);
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

  if (digitalRead(BUTTON)==0 && !buttonPressed){
    buttonPressed = true;
    buttonPressStartTime = millis();
  }

  else if (buttonPressed==true && (millis()-buttonPressStartTime>=2000)){
    break;
   
  }

  if (minutes < 10 && seconds < 10) {
    display.clearDisplay();
    bool setMin =true;
    bool setSecond =true;
    // tiktokSetting(setMin,setSecond,minutes,seconds);
    display.setCursor(1, 50);
    display.print("0");   
    display.setCursor(25, 50);
    display.print(minutes);
    display.setCursor(50, 50);
    display.print(":");
    display.setCursor(65, 50);
    display.print("0");
    display.setCursor(90, 50);
    display.print(seconds);
  } 
  
  else if (minutes < 10 && seconds>10) {
    display.clearDisplay();
    bool setMin =true;
    bool setSecond =false;
    // tiktokSetting(setMin,setSecond,minutes,seconds);
    display.setCursor(1, 50);
    display.print("0");   
    display.setCursor(25, 50);
    display.print(minutes);
    display.setCursor(50, 50);
    display.print(":");
    display.setCursor(65, 50);
    display.print(seconds);
    Serial.println("In the minutes<10 situation.");
  }

  else if (minutes>=10 && seconds>10){
    display.clearDisplay();
    bool setMin = false;
    bool setSecond = false;
    // tiktokSetting(setMin,setSecond,minutes,seconds);
    display.setCursor(1,50);
    display.print(minutes);
    display.setCursor(50, 50);
    display.print(":");
    display.setCursor(65, 50);
    display.print(seconds);

  }
  
  else if(minutes>=10 && seconds<10){
    display.clearDisplay();
    bool setMin = false;
    bool setSecond = true;
    // tiktokSetting(setMin,setSecond,minutes,seconds);
    display.setCursor(1,50);
    display.print(minutes);
    display.setCursor(50, 50);
    display.print(":");
    display.setCursor(65, 50);
    display.print("0");
    display.setCursor(90, 50);
    display.print(seconds);
 
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

  buttonPressed =false;
  Serial.println("跳出迴圈、pump中止");


}


    //回到上一頁
    else if(digitalRead(ENTER_BUTTON)==0 && chooseMode==true && digitalRead(LED)==false){
      Serial.println("重新回到選擇十進位頁面");
      count_3 = 0;
      count_2 = 0;
      count = 0 ;
      nextValue = 1;
      previousValue = 4;
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
