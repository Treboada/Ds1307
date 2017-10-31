/** SetDateTime.cpp
 *
 * Example of setting the date and time to the RTC.
 *
 * @version 1.0.1
 * @author Rafa Couto <caligari@treboada.net>
 * @license GNU Affero General Public License v3.0
 * @see https://github.com/Treboada/Ds1307
 *
 */

#include <Arduino.h>
#include <Ds1307.h>

#define DS1307_I2C_ADDRESS 0x68


const static char* WeekDays[] =
{
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};


// DS1307 RTC instance
Ds1307 rtc(DS1307_I2C_ADDRESS);


uint8_t parseDigits(char* str, uint8_t count)
{
    uint8_t val = 0;
    while(count-- > 0) val = (val * 10) + (*str++ - '0');
    return val;
}


void show()
{
    Serial.println();

    if (rtc.isHalted()) {

        Serial.println("Clock is halted.");
    }
    else {

        Ds1307::DateTime dt;
        rtc.getDateTime(&dt);

        Serial.print("20");
        Serial.print(dt.year);    // 00-99
        Serial.print('-');
        if (dt.month < 10) Serial.print('0');
        Serial.print(dt.month);   // 01-12
        Serial.print('-');
        if (dt.day < 10) Serial.print('0');
        Serial.print(dt.day);     // 01-31
        Serial.print(' ');
        Serial.print(WeekDays[dt.dow - 1]); // 1-7
        Serial.print(' ');
        if (dt.hour < 10) Serial.print('0');
        Serial.print(dt.hour);    // 00-23
        Serial.print(':');
        if (dt.minute < 10) Serial.print('0');
        Serial.print(dt.minute);  // 00-59
        Serial.print(':');
        if (dt.second < 10) Serial.print('0');
        Serial.print(dt.second);  // 00-59
        Serial.println();
    }
}


void setup()
{
    // initialize the RTC
    rtc.init();

    Serial.begin(9600);

    show();
    Serial.println("Input the date and time (YYMMDDWhhmmss): ");
}


void loop()
{
    static char buffer[13];
    static uint8_t char_idx = 0;

    if (char_idx == 13)
    {
        // structure to manage date-time
        Ds1307::DateTime dt;

        dt.year = parseDigits(buffer, 2);
        dt.month = parseDigits(buffer + 2, 2);
        dt.day = parseDigits(buffer + 4, 2);
        dt.dow = parseDigits(buffer + 6, 1);
        dt.hour = parseDigits(buffer + 7, 2);
        dt.minute = parseDigits(buffer + 9, 2);
        dt.second = parseDigits(buffer + 11, 2);

        // set the date and time
        rtc.setDateTime(&dt);
        show();

        char_idx = 0;
    }

    if (Serial.available())
    {
        buffer[char_idx++] = Serial.read();
    }
}

