#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display > 0x3F in my case for a 2004 lcd display
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO  true

int timezone = +3; // Europe/Istanbul (UTC +0300) > NMEA is UTC oriented

//define lock symbol
      byte customCharLock[] = {
      B00000,
      B01110,
      B10001,
      B10001,
      B11111,
      B11111,
      B11111,
      B11111
    };

//define no lock symbol
      byte customCharNolock[] = {
      B00000,
      B01110,
      B10000,
      B10000,
      B11111,
      B11111,
      B11111,
      B11111
    };
void setup()
{
  lcd.init();                      
  lcd.backlight();
  lcd.clear();
  
  //opening credit
  lcd.setCursor(1,1);
  lcd.print("GPS Realtime Clock");
  lcd.setCursor(7,2);
  lcd.print("TA2BXX");
  delay(5000);
  Serial.begin(115200);
  delay(5000);
  Serial.println("Adafruit GPS library basic test!");
  GPS.begin(9600); // > in my case I use UBLOX 6M: GY-GPS6Mv2

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
      
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_2HZ);   // 2 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);
  
  //create lock character
  lcd.createChar(1, customCharLock);

  //create no lock character
  lcd.createChar(2, customCharNolock);

  //clear screen from credits
  lcd.clear();

}

uint32_t timer = millis();
void loop()                     // run over and over again
{
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if ((c) && (GPSECHO))
    Serial.write(c);
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))   
      return;
  }
  if (timer > millis())  timer = millis();

  // approximately every 1 second or so, print out the current stats
  if (millis() - timer > 1000) {
    timer = millis(); // reset the timer

    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
  
    int GPSFix = (GPS.fix);    
    
    //output LAT LON
      lcd.setCursor(0,0);
      lcd.print(GPS.latitudeDegrees,4);
      lcd.print((char)223);
      lcd.print(GPS.lat);
      lcd.print(", ");
      lcd.print(GPS.longitudeDegrees,4);
      lcd.print((char)223);
      lcd.print(GPS.lon);
    
    // output GMT
    lcd.setCursor(0,1); // ,0 = first line
        lcd.print("GMT ");
        int gmt = (GPS.hour);
        if (gmt < 10) { lcd.print('0'); }
        lcd.print(gmt, DEC); lcd.print(':');
        if (GPS.minute < 10) { lcd.print('0'); }
        lcd.print(GPS.minute, DEC); lcd.print(':');
        if (GPS.seconds < 10) { lcd.print('0'); }
        lcd.print(GPS.seconds, DEC); 
      lcd.setCursor(16,1);
        lcd.print("Zulu");


    // output local time
    lcd.setCursor(0,2);
    lcd.print("TRT ");
    int hour = (GPS.hour) + timezone;
        if (hour < 10) { lcd.print('0'); }
        lcd.print(hour, DEC); 
        lcd.print(':');
        if (GPS.minute < 10) { lcd.print('0'); }
        lcd.print(GPS.minute, DEC); 
        lcd.print(':');
        if (GPS.seconds < 10) { lcd.print('0'); }
        lcd.print(GPS.seconds, DEC); 
        lcd.setCursor(14,2);
        lcd.print("GMT +3");

    //output date 
          lcd.setCursor(0,3);
          lcd.print("  ");
          lcd.setCursor(0,3);
          if (GPS.day < 10) { lcd.print('0'); }
          lcd.print(GPS.day); 
          
          lcd.print('/');
          lcd.setCursor(3,3);
          lcd.print("  ");
          lcd.setCursor(3,3);
          if (GPS.month < 10) { lcd.print('0'); }
          lcd.print(GPS.month);

          lcd.print("/20");
          lcd.print(GPS.year);

    //GPS Lock state
    lcd.setCursor(11,3);
    if (GPS.fix > 0)
    {
    lcd.print(" ");
    lcd.setCursor(11,3);
    lcd.write(1);
    }
    else
    {
    lcd.setCursor(11,3);
    lcd.print(" ");
    lcd.setCursor(11,3);
    lcd.write(2);
    }          

    //output locked satelite number
          int fix = (GPS.satellites, DEC);
          lcd.setCursor(13,3);
          lcd.print("Sats:"); 
          lcd.print("  ");
          lcd.setCursor(18,3);
          lcd.print(GPS.satellites);
  }
}
