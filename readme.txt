# GPS-RTC
GPS Realtime Clock with Arduino Nano and NEO-6M. Heavily influenced from Ingo Lohs's "A cheap and accurate clock based on GPS" project at https://www.hackster.io/ingo-lohs/a-cheap-and-accurate-clock-based-on-gps-adc0d9.

This is a GPS Realtime Clock project with Arduino Nano, I2C LCD 20x4 and NEO-6M GPS module.

GPS to Arduino Connection
    NEO-6M GND to GND
    NEO-6M VCC to +5v
    NEO-6M TX to D8
    NEO-6M RX to D7
    
I2C Screen to Arduino Connection
    I2C GND to GND
    I2C VCC to +5V
    I2C SDA to A4
    I2C SCL to A5
    

Screen is as follows;
V1
-------------------------
|LAT.LATT°N, LON.LONG°E |
|GMT: 12:59:59     ZULU |
|TRT: 15:59:59     GMT+3|
|DD/MM/YYYY  *   Sats:99|
-------------------------
*:GPS Lock Status. Either a locked or unlocked padlock

V2
-------------------------
|LAT.LATT°N  LON.LONG°E |
|GMT: 12:59:59     ZULU |
|TRT: 15:59:59     GMT+3|
|DD/MM/YYYY     Sats:99*|
-------------------------
*:GPS Lock Status. Either a locked or unlocked padlock

To customize to your local time;
  V1:
  Change line 10 to your timezone,
  Change line 145 to your desired timezone name (3 characters),
  Change line 156 to your GMT or UTC timezone value (GMT+3, UTC-5, etc.)
  
  V2:
  Change line 13 to your timezone,
  Change line 197 to your desired timezone name (3 characters),
  Change line 208 to your GMT or UTC timezone value (GMT+3, UTC-5, etc.)
  Use the following lines to update to MM/DD/YYYY date format instead of lines 217-233
    
    //output date
    if (GPS.fix) {
      lcd.setCursor(0, 3); 
      lcd.print("          ");
      lcd.setCursor(0, 3); 
      if (GPS.month < 10) { lcd.print('0'); }
      lcd.print(GPS.month);
      lcd.print('/');
      if (GPS.day < 10) { lcd.print('0'); }
      lcd.print(GPS.day);
      lcd.print("/20");
      lcd.print(GPS.year);
    } else {
      lcd.setCursor(0, 3);
      lcd.print("--/--/----");
    }
    }
  
Arduino with this sketch can also be used as a GPS receiver for computer via COM port (CH-34X) using Visual GPS View at https://www.visualgps.net/#visualgpsview-content

Updates on V2:
-Screen doesn't show any values unless GPS fix is acquired
