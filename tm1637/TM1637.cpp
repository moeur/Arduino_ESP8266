
#include "TM1637.h"
#include "Arduino.h"

tm1637::tm1637(uint8_t ClockPin, uint8_t DataPin)
{
	_clk = ClockPin;
	_dio = DataPin;
	pinMode(_dio, OUTPUT);
	digitalWrite(_dio, LOW); //pin will be low when acting as output
	pinMode(_dio, INPUT_PULLUP);  //pin will be high when acting as input
	pinMode(_clk, OUTPUT);
	initDisplay();
}

/*  i2c Methods for handling data transfer to and from the tm1637. */
/// =======================================
void tm1637::i2cStart(void)
{
    CLK_HIGH();
    DIO_HIGH();
    delayMicroseconds(2);
    DIO_LOW();
}
/// =============================================
void tm1637::i2cAck (void)
{
    CLK_LOW();
    DIO_HIGH();
    delayMicroseconds (5); 
    while (digitalRead(_dio));
    CLK_HIGH();
    delayMicroseconds (2);
    CLK_LOW();
}
/// ========================================
void tm1637::i2cStop (void) 
{
    CLK_LOW();
    delayMicroseconds (2);
    DIO_LOW();
    delayMicroseconds (2);
    CLK_HIGH();
    delayMicroseconds (2);
    DIO_HIGH();
}
/// ========================================= 

void tm1637::i2cWrite (uint8_t oneByte)
{
    uint8_t i;
    for (i=0; i<8; i++)
    {
        CLK_LOW();
        (oneByte & 0x01)? DIO_HIGH(): DIO_LOW();
        delayMicroseconds (3);
        oneByte = oneByte >> 1;
        CLK_HIGH();
        delayMicroseconds (3);
    }
}
/* End i2c Methods ---------------------------------- */

void tm1637::initDisplay(void)
{
    i2cStart();
    i2cWrite(0x40); 
    i2cAck();
    i2cStop();
    clear();
}

void tm1637::writeArray()
{
    i2cStart();
    i2cWrite(0xc0);
    i2cAck();
	i2cWrite(data[0]);
	i2cAck();
	i2cWrite(data[1]);
	i2cAck();
	i2cWrite(data[2]);
	i2cAck();
	i2cWrite(data[3]);
	i2cAck();
    i2cStop();
}

/*  Public Methods  *******************************************/
void tm1637::setBrightness(uint8_t b)
{
    _brightness = (b & 0b111);
    i2cStart();
    i2cWrite( _power | _brightness );
    i2cAck();
    i2cStop();
}

void tm1637::power(bool state)
{
    _power = (state?TM1637_CMD_POWER_ON:TM1637_CMD_POWER_OFF);  
    i2cStart();
    i2cWrite( _power | _brightness );
    i2cAck();
    i2cStop();
}

void tm1637::clear(void)
{
  i2cStart();
    i2cWrite(0xc0); // Set the starting address
    i2cAck();
    i2cWrite(0); 
    i2cAck();
    i2cWrite(0);
    i2cAck();
    i2cWrite(0); 
    i2cAck();
    i2cWrite(0); 
    i2cAck();
  i2cStop();
}

//set one of the four digits [0 - 3]
//value [0 - 9]
void tm1637::writeDigit(uint8_t digit, uint8_t value)
{
    i2cStart();
    i2cWrite(0xc0 | digit);
    i2cAck();
    i2cWrite(data[value]);
    i2cAck();
    i2cStop(); 
}

void tm1637::writeTime(uint8_t hours, uint8_t minutes, bool colon)
{
    uint8_t digit0, digit1;
    uint8_t digit2, digit3;
    /* Handle hours */
    digit0 = (uint8_t)(hours / 10);
    digit1 = hours - digit0*10;
    if (digit0) 
        data[0] = digits[digit0];
    else
        data[0] = 0;
    data[1] = digits[digit1];
    if (colon) data[1] |= 0b10000000;
    
    /* Now minutes */
    digit2 = (uint8_t)(minutes / 10);
    digit3 = minutes - digit2*10;
    data[2] = digits[digit2];
    data[3] = digits[digit3];
    writeArray();    
}

void tm1637::writeInteger(uint16_t number) //value [0 - 9999]
{
  int8_t i = 3;
     if (number == 0){
       data[3] = digits[0];
       data[1] = 0;
       data[2] = 0;
       data[0] = 0;
       writeArray();
       return;
     }
      do {
        data[i--] =  digits[number % 10];
        if (i < 0) break;
        number /= 10;
    } while (number);
 
    while (i >= 0)
            data[i--] = 0;   

    writeArray();    

}
