#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display > 0x3F in my case for a 2004 lcd display
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// you can change the pin numbers to match your wiring:
SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);

#define GPSECHO true

int timezone = +3;

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

//define no lock coordinates
byte customCharNoCoord[] = {
  B11111,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B11111
};
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  //opening credit
  lcd.setCursor(1, 0);
  lcd.print("GPS Realtime Clock");
  lcd.setCursor(3, 1);
  lcd.print("and Geolocator");
  lcd.setCursor(7, 3);
  lcd.print("TA2BXX");
  delay(5000);

  Serial.begin(115200);
  delay(5000);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_2HZ);  // 2 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);

  //create lock character
  lcd.createChar(1, customCharLock);

  //create no lock character
  lcd.createChar(2, customCharNolock);

  //create no lock coordinates character
  lcd.createChar(3, customCharNoCoord);

  //clear screen before we start
  lcd.clear();
}

uint32_t timer = millis();
void loop()
{
  char c = GPS.read();
  if ((c) && (GPSECHO))
    Serial.write(c);
  if (GPS.newNMEAreceived()) {
     if (!GPS.parse(GPS.lastNMEA()))  
      return;                        
  if (timer > millis()) timer = millis();

  // approximately every 1 second or so, print out the current stats
  if (millis() - timer > 1000) {
    timer = millis();

    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC);
    Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC);
    Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC);
    Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC);
    Serial.print('/');
    Serial.print(GPS.month, DEC);
    Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: ");
    Serial.print((int)GPS.fix);
    Serial.print(" quality: ");
    Serial.println((int)GPS.fixquality);

    //output LAT LON
if (GPS.fix) 
{
  if ((GPS.latitudeDegrees<100) && (GPS.longitudeDegrees<100))
  {
      lcd.setCursor(0, 0);
      lcd.print(GPS.latitudeDegrees, 4);
      lcd.print((char)223);
      lcd.print(GPS.lat);
      lcd.setCursor(11, 0);
      lcd.print(GPS.longitudeDegrees, 4);
      lcd.print((char)223);
      lcd.print(GPS.lon);
    } 
    else 
    {
      lcd.setCursor(0, 0);
      lcd.print(GPS.latitudeDegrees, 3);
      lcd.print((char)223);
      lcd.print(GPS.lat);
      lcd.setCursor(11, 0);
      lcd.print(GPS.longitudeDegrees, 3);
      lcd.print((char)223);
      lcd.print(GPS.lon);
    }
}
    
      else 
{
      lcd.setCursor(0, 0);
      lcd.print("--.----");
      lcd.print((char)223);
      lcd.write(3);
      lcd.setCursor(11, 0);
      lcd.print("--.----");
      lcd.print((char)223);
      lcd.write(3);
}

    // output GMT
    if (GPS.fix) {
      lcd.setCursor(0, 1);  
      lcd.print("        ");
      lcd.setCursor(0, 1);  
      lcd.print("GMT ");
      int gmt = (GPS.hour); 
      if (gmt < 10) { lcd.print('0'); }
      lcd.print(gmt, DEC);
      lcd.print(':');
      if (GPS.minute < 10) { lcd.print('0'); }
      lcd.print(GPS.minute, DEC);
      lcd.print(':');
      if (GPS.seconds < 10) { lcd.print('0'); }
      lcd.print(GPS.seconds, DEC);
      lcd.setCursor(16, 1);
      lcd.print("Zulu");
    } else {
      lcd.setCursor(0, 1); 
      lcd.print("GMT ");
      lcd.print("--:--:--");
      lcd.setCursor(16, 1);
      lcd.print("Zulu");
    }

    // output to LCD Display
    if (GPS.fix) {
      lcd.setCursor(0, 2);  
      lcd.print("        ");
      lcd.setCursor(0, 2);  
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
      lcd.setCursor(14, 2);
      lcd.print("GMT +3");
    } else {
      lcd.setCursor(0, 2);
      lcd.print("TRT ");
      lcd.print("--:--:--");
      lcd.setCursor(14, 2);
      lcd.print("GMT +3");
    }

    //output date
    if (GPS.fix) {
      lcd.setCursor(0, 3); 
      lcd.print("          ");
      lcd.setCursor(0, 3); 
      if (GPS.day < 10) { lcd.print('0'); }
      lcd.print(GPS.day);
      lcd.print('/');
      if (GPS.month < 10) { lcd.print('0'); }
      lcd.print(GPS.month);

      lcd.print("/20");
      lcd.print(GPS.year);
    } else {
      lcd.setCursor(0, 3);
      lcd.print("--/--/----");
    }

    
    }
      //GPS Lock state
          if ((GPS.fix)) {
          lcd.setCursor(19, 3);
          lcd.print(" ");
          lcd.setCursor(19, 3);
          lcd.write(1);
        } else {
          lcd.setCursor(19, 3);
          lcd.print(" ");
          lcd.setCursor(19, 3);
          lcd.write(2);
        }

        
    //Locked Satellites
        lcd.setCursor(12, 3);
        lcd.print("SATS:");
if ((GPS.satellites)>1)
{
  if (((GPS.satellites)>0) && ((GPS.satellites)<10))
  {
        lcd.setCursor(17,3);
        lcd.print('0');
        lcd.print(GPS.satellites);
      } 
      else 
      {
        lcd.print("  ");
        lcd.setCursor(17,3);
        lcd.print(GPS.satellites);
      }
}
  if (GPS.satellites<1)
  {
    lcd.setCursor(17,3);
    lcd.print("--");
  }
  }

  } 
void serial() 
{
  if (GPS.fix) {
  Serial.print("Location: ");
  Serial.print(GPS.latitude, 6);
  Serial.print(GPS.lat);
  Serial.print(", ");
  Serial.print(GPS.longitude, 6);
  Serial.println(GPS.lon);

  Serial.print("Speed (knots): ");
  Serial.println(GPS.speed);
  Serial.print("Angle: ");
  Serial.println(GPS.angle);
  Serial.print("Altitude: ");
  Serial.println(GPS.altitude);
  Serial.print("Satellites: ");
  Serial.println((int)GPS.satellites);
                }
}