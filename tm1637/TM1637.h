/****************************************************************************
  tm1637.h - Library for 4-digit 7-segment display that uses the tm1637 chip.
  Created by William Moeur; 5/28/2015.
*****************************************************************************/

#ifndef TM1637_H
#define	TM1637_H
#include <stdint.h>

class tm1637
{
  public:
    tm1637(uint8_t ClockPin, uint8_t DataPin);
	void setBrightness(uint8_t b);
        void power(bool state);
        void writeDigit(uint8_t digit, uint8_t value);
	void writeTime(uint8_t hours, uint8_t minutes, bool colon);
        void writeInteger(uint16_t value);
        void clear(void);
  private:
        #define CLK_HIGH()  digitalWrite(_clk, HIGH)
        #define CLK_LOW()  digitalWrite(_clk, LOW)
        #define DIO_LOW() pinMode(_dio, OUTPUT)
        #define DIO_HIGH() pinMode(_dio, INPUT)
        #define TM1637_CMD_POWER_ON 0b10001000
        #define TM1637_CMD_POWER_OFF 0b10000000
        uint8_t _clk;
        uint8_t _dio;
        uint8_t _brightness;
        uint8_t _power = TM1637_CMD_POWER_ON;
	void i2cStart(void);
	void i2cAck (void);
	void i2cStop (void);
	void i2cWrite (uint8_t oneByte);
	void initDisplay(void);
	void writeArray();
	uint8_t data[4];
	uint8_t digits[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
};

#endif	/* TM1637_H */



