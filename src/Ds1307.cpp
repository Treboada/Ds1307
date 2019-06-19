/** Ds1307.cpp
 *
 * Ds1307 class.
 *
 * @version 1.0.2
 * @author Rafa Couto <caligari@treboada.net>
 * @license GNU Affero General Public License v3.0
 * @see https://github.com/Treboada/Ds1307
 *
 */

#include "Ds1307.h"

#include <Arduino.h>
#include <Wire.h>


Ds1307::Ds1307(uint8_t i2c_address)
{
    _i2c_address = i2c_address;
}


void Ds1307::init()
{
    Wire.begin();
}


bool Ds1307::isHalted()
{
    uint8_t buffer[1];
    buffer[0] = 0x00;
    _writeBytes(buffer, 1);
    _readBytes(buffer, 1);
    return (buffer[0] & 0b10000000);
}


void Ds1307::getDateTime(DateTime* dt)
{
    uint8_t buffer[7];
    buffer[0] = 0x00;
    _writeBytes(buffer, 1);
    _readBytes(buffer, 7);
    dt->second = _bcd2dec(buffer[0] & 0b01111111);
    dt->minute = _bcd2dec(buffer[1] & 0b01111111);
    dt->hour   = _bcd2dec(buffer[2] & 0b00111111);
    dt->dow    = _bcd2dec(buffer[3] & 0b00000111);
    dt->day    = _bcd2dec(buffer[4] & 0b00111111);
    dt->month  = _bcd2dec(buffer[5] & 0b00011111);
    dt->year   = _bcd2dec(buffer[6] & 0b01111111);
}


void Ds1307::setDateTime(DateTime* dt)
{
    uint8_t buffer[8];
    buffer[0] = 0x00;
    buffer[1] = _dec2bcd(dt->second % 60 );
    buffer[2] = _dec2bcd(dt->minute % 60 );
    buffer[3] = _dec2bcd(dt->hour   % 24 );
    buffer[4] = _dec2bcd(dt->dow    % 8  );
    buffer[5] = _dec2bcd(dt->day    % 32 );
    buffer[6] = _dec2bcd(dt->month  % 13 );
    buffer[7] = _dec2bcd(dt->year   % 100);
    _writeBytes(buffer, 8);
}


void Ds1307::halt()
{
    uint8_t buffer[2];
    buffer[0] = 0x00;
    buffer[1] = 0b10000000;
    _writeBytes(buffer, 2);
}


void Ds1307::_readBytes(uint8_t* buffer, uint8_t count)
{
    Wire.requestFrom(_i2c_address, count);
    while (count-- > 0) {
        *buffer++ = Wire.read();
    }
    delay(1);
}


void Ds1307::_writeBytes(uint8_t* buffer, uint8_t count)
{
    Wire.beginTransmission(_i2c_address);
    while (count-- > 0) {
        Wire.write(*buffer++);
    }
    Wire.endTransmission();
    delay(1);
}


uint8_t Ds1307::_dec2bcd(uint8_t dec)
{
    return ((dec / 10 * 16) + (dec % 10));
}


uint8_t Ds1307::_bcd2dec(uint8_t bcd)
{
    return ((bcd / 16 * 10) + (bcd % 16));
}


