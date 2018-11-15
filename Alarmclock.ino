#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
//#include "Sodaq_DS3231.h"
#include <Sodaq_DS3231.h>
#include <SPI.h>
#include "pitches.h"

#define TFT_CS     10
#define TFT_RST    8                      
#define TFT_DC     9
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

#define TFT_SCLK 13
#define TFT_MOSI 11

char timeChar[100];
char dateChar[50];
char dayChar[50];

String dateString;
String dayString;
int minuteNow=0;
int minutePrevious=0;

char alarmChar[10];
String alarmString = "14:38";

// notes in the melody:
int melody[] = {
  NOTE_E7
};

int noteDurations[] = {
  12
};

void setup () 
{
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK);
    Serial.begin(57600);
    Wire.begin();
    rtc.begin();
    
}

uint32_t old_ts;

void loop () 
{
  DateTime now = rtc.now(); //get the current date-time
  uint32_t ts = now.getEpoch();

    if (old_ts == 0 || old_ts != ts) {
	old_ts = ts;
  
  minuteNow = now.minute();
  if(minuteNow!=minutePrevious)
  {
    dayString = getDayOfWeek(now.dayOfWeek())+"\n";
    dateString = String(now.date())+"/"+String(now.month());
    dateString= dateString+"/"+ String(now.year()); 
    minutePrevious = minuteNow;
    String hours = String(now.hour());
    if(now.minute()<10)
    {
      hours = hours+":0"+String(now.minute());
    }else
    {
      hours = hours+":"+String(now.minute());
    }
   
    hours.toCharArray(timeChar,100);
    tft.fillRect(5,10,160,80,ST7735_BLACK);
    printText(timeChar, ST7735_WHITE,5,60,4);
    dateString.toCharArray(dateChar,50);
    printText(dateChar, ST7735_GREEN,12,10,2);
    dayString.toCharArray(dayChar, 50);
    printText(dayChar, ST7735_GREEN,19,30,2);
    alarmString.toCharArray(alarmChar, 10);
    printText(alarmChar, ST7735_RED, 35, 100, 2);

    if(hours == alarmString){
      for(int i = 0; i < 5; i++){
          alarmTone();
        }
      }
  }
}
    delay(1000);
}

void setRTCTime()
{
  DateTime dt(2017, 4, 13, 14, 9, 30, 4); // Year, Month, Day, Hour, Minutes, Seconds, Day of Week
  rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 
}

void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}

String getDayOfWeek(int i)
{
  switch(i)
  {
    case 1: return "Mandag";break;
    case 2: return "Tirsdag";break;
    case 3: return "Onsdag";break;
    case 4: return "Torsdag";break;
    case 5: return "Fredag";break;
    case 6: return "Lørdag";break;
    case 7: return "Søndag";break;
    default: return "Mandag";break;
  }
}

void alarmTone(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(7, melody[thisNote], noteDuration);
    delay(1000);
  }
}



