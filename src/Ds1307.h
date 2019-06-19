/** Ds1307.h
 *
 * Ds1307 class.
 *
 * @version 1.0.2
 * @author Rafa Couto <caligari@treboada.net>
 * @license GNU Affero General Public License v3.0
 * @see https://github.com/Treboada/Ds1307
 *
 */

#ifndef _DS_1307_H
#define _DS_1307_H

#include <stdint.h>

class Ds1307
{
    public:

        typedef struct {
            uint8_t year;
            uint8_t month;
            uint8_t day;
            uint8_t hour;
            uint8_t minute;
            uint8_t second;
            uint8_t dow;
        } DateTime;

        /**
         * Months of year
         */
        enum MONTH : uint8_t {
            MONTH_JAN = 1,
            MONTH_FEB = 2,
            MONTH_MAR = 3,
            MONTH_APR = 4,
            MONTH_MAY = 5,
            MONTH_JUN = 6,
            MONTH_JUL = 7,
            MONTH_AUG = 8,
            MONTH_SET = 9,
            MONTH_OCT = 10,
            MONTH_NOV = 11,
            MONTH_DEC = 12
        };

        /**
         * Days of week
         */
        enum DOW : uint8_t {
            DOW_SUN = 1,
            DOW_MON = 2,
            DOW_TUE = 3,
            DOW_WED = 4,
            DOW_THU = 5,
            DOW_FRI = 6,
            DOW_SAT = 7
        };

        /**
         * Constructor (I2C configuration).
         */
        Ds1307(uint8_t i2c_address);

        /**
         * Initializes the DS1307 chip.
         */
        void init();

        /**
         * Returns when the oscillator is disabled.
         */
        bool isHalted();

        /**
         * Stops the oscillator.
         */
        void halt();

        /**
         * Returns the current date and time.
         */
        void getDateTime(DateTime* dt);;

        /**
         * Sets the current date and time.
         */
        void setDateTime(DateTime* dt);

    private:

        uint8_t _i2c_address;

        void _readBytes(uint8_t* buffer, uint8_t count);
        void _writeBytes(uint8_t* buffer, uint8_t count);

        uint8_t _dec2bcd(uint8_t dec);
        uint8_t _bcd2dec(uint8_t bcd);
};

#endif // _DS_1307_H

