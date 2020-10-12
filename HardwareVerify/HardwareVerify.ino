/**
* Test hardware for ATMEL 28C series EEPROMs.
*
* The hardware uses two 74LS164 shift registers as the low and
* high address registers.
**/

#include "Configure.h"

#define LED 13

PromDevice28C  prom(32 * 1024L, 64, 10, true);


/*****************************************************************************/
/*****************************************************************************/

/**
 * CLI parse functions
 */
const char hex[] = "0123456789abcdef";


// Read a line of data from the serial connection.
char * readLine(char * buffer, int len)
{
    for (int ix = 0; (ix < len); ix++)
    {
         buffer[ix] = 0;
    }

    // read serial data until linebreak or buffer is full
    char c = ' ';
    int ix = 0;
    do {
        if (Serial.available())
        {
            c = Serial.read();
            if ((c == '\b') && (ix > 0))
            {
                // Backspace, forget last character
                --ix;
            }
            buffer[ix++] = c;
            Serial.write(c);
        }
    } while ((c != '\n') && (ix < len));

    buffer[ix - 1] = 0;
    return buffer;
}




/************************************************************
* convert a single hex character [0-9a-fA-F] to its value
* @param char c single character (digit)
* @return byte value of the digit (0-15)
************************************************************/
byte hexDigit(char c)
{
    if ((c >= '0') && (c <= '9'))
    {
        return c - '0';
    }
    else if ((c >= 'a') && (c <= 'f'))
    {
        return c - 'a' + 10;
    }
    else if ((c >= 'A') && (c <= 'F'))
    {
        return c - 'A' + 10;
    }
    else
    {
        return 0xff;
    }
}


/************************************************************
* Convert a hex string to a uint32_t value.
* Skips leading spaces and terminates on the first non-hex
* character.  Leading zeroes are not required.
*
* No error checking is performed - if no hex is found then
* defaultValue is returned.  Similarly, a hex string of more than
* 8 digits will return the value of the last 8 digits.
* @param pointer to string with the hex value of the word (modified)
* @return unsigned int represented by the digits
************************************************************/
uint32_t getHex32(char *& pData, uint32_t defaultValue=0)
{
    uint32_t u32 = 0;

    while (isspace(*pData))
    {
        ++pData;
    }

    if (isxdigit(*pData))
    {
        while (isxdigit(*pData)) {
            u32 = (u32 << 4) | hexDigit(*pData++);
        }
    }
    else
    {
        u32 = defaultValue;
    }

    return u32;
}

void printByte(byte b)
{
    char line[3];

    line[0] = hex[b >> 4];
    line[1] = hex[b & 0x0f];
    line[2] = '\0';

    Serial.print(line);
}


void printWord(word w)
{
    char line[5];

    line[0] = hex[(w >> 12) & 0x0f];
    line[1] = hex[(w >>  8) & 0x0f];
    line[2] = hex[(w >>  4) & 0x0f];
    line[3] = hex[(w)       & 0x0f];
    line[4] = '\0';

    Serial.print(line);
}


void zapTest(uint32_t start)
{
    byte testData[] =
    {
        'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
        0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe,
        0x00, 0xff, 0x55, 0xaa, '0',  '1',  '2',  '3'
    };

    if (!prom.writeData(testData, sizeof(testData), start))
    {
        Serial.println("Write failed");
        return;
    }

    delay(100);
    for (unsigned ix = 0; ix < sizeof(testData); ix++)
    {
        byte val = prom.readData(start + ix);
        if (val != testData[ix])
        {
            Serial.print(F("Verify failed, addr="));
            Serial.print(start + ix, HEX);
            Serial.print(F(", read="));
            Serial.print(val, HEX);
            Serial.print(F(", expected="));
            Serial.println(testData[ix], HEX);
            return;
        }
    }
    Serial.println("Write test successful");
}

void pokeBytes(char * pCursor)
{
    uint32_t val;
    uint32_t start;
    unsigned byteCtr = 0;

    enum { BLOCK_SIZE = 32 };
    byte data[BLOCK_SIZE];

    //first value returned is the starting address
    start = getHex32(pCursor, 0);

    while (((val = getHex32(pCursor, 0xffff)) != 0xffff) && (byteCtr < BLOCK_SIZE))
    {
        data[byteCtr++] = byte(val);
    }

    if (byteCtr > 0)
    {
        if (!prom.writeData(data, byteCtr, start))
        {
            Serial.println(F("Write failed"));
            return;
        }
    }
    else
    {
        Serial.println(F("Missing address or data"));
        return;
    }
    delay(100);

    for (unsigned ix = 0; ix < byteCtr ; ix++)
    {
        byte val = prom.readData(start + ix);
        if (val != data[ix])
        {
            Serial.print(F("Verify failed, addr="));
            Serial.print(start + ix, HEX);
            Serial.print(F(", read="));
            Serial.print(val, HEX);
            Serial.print(F(", expected="));
            Serial.println(data[ix], HEX);
            return;
        }
    }
    Serial.println("Poke successful");
}

void dumpBlock(uint32_t start, uint32_t end)
{
    char line[81];
//  01234567891 234567892 234567893 234567894 234567895 234567896 234567897 23456789
//  01234: 01 23 45 67  89 ab cf ef  01 23 45 67  89 ab cd ef  1.2.3.4. 5.6.7.8.
    int count = 0;

    memset(line, ' ', sizeof(line));

    char * pHex = line;
    char * pChar = line + 59;
    for (uint32_t addr = start; (addr <= end); addr++)
    {
        if (count == 0)
        {
            //print out the address at the beginning of the line
            pHex = line;
            pChar = line + 59;
            *pHex++ = hex[(addr >> 16) & 0x0f];
            *pHex++ = hex[(addr >> 12) & 0x0f];
            *pHex++ = hex[(addr >>  8) & 0x0f];
            *pHex++ = hex[(addr >>  4) & 0x0f];
            *pHex++ = hex[(addr)       & 0x0f];
            *pHex++ = ':';
            *pHex++ = ' ';
        }

        byte data = prom.readData(addr);
        *pHex++ = hex[data >> 4];
        *pHex++ = hex[data & 0x0f];
        *pHex++ = ' ';
        *pChar++ = ((data < 32) | (data >= 127)) ? '.' : data;

        if ((count & 3) == 3)
        {
            *pHex++ = ' ';
        }
        if ((count & 7) == 7)
        {
            *pChar++ = ' ';
        }
        if ((++count >= 16) || (addr == end))
        {
            *pChar = '\0';
            Serial.println(line);
            memset(line, ' ', sizeof(line));
            count = 0;
        }
    }

    if (count)
    {
        Serial.println();
    }
}

/************************************************
* MAIN
*************************************************/
word addr = 0;

void setup()
{
    // Do this first so that it initializes all of the hardware pins into a
    // non-harmful state.  The Arduino or the target EEPROM could be damaged
    // if both writing to the data bus at the same time.
    prom.begin();

    Serial.begin(115200);
}

void loop()
{
    commandLoop();
}

static void commandLoop()
{
    byte b;
    uint32_t arg;
    const uint32_t noValue = uint32_t(-1);
    char line[120];
    char * cursor = line + 1;
    unsigned long timeStart;
    unsigned long timeEnd;
    bool cmdError = false;
    bool unknownCmd = false;

    Serial.print("\n#");
    Serial.flush();
    readLine(line, sizeof(line));
    byte c = tolower(line[0]);
    if ((c >= 'A') && (c <= 'Z')) {
        c |= 0x20;
    }

    switch (c)
    {
    case 'a':
        if ((arg = getHex32(cursor, noValue)) != noValue)
        {
            prom.setAddress(word(arg));
        }
        else
            cmdError = true;
        break;

    case 'd':
        if ((arg = getHex32(cursor, noValue)) != noValue)
        {
            prom.disableOutput();
            prom.setDataBusMode(OUTPUT);
            prom.writeDataBus(byte(arg));
        }
        else
            cmdError = true;
        break;

    case 'c':
    case 'o':
    case 'w':
        if ((line[1] != 'd') && (line[1] != 'e'))
        {
            cmdError = true;
        }
        else
        {
            bool enable = line[1] == 'e';
            if (c == 'c') {
                if (enable) prom.enableChip(); else prom.disableChip();
            } else if (c == 'w') {
                if (enable) prom.enableWrite(); else prom.disableWrite();
            } else { // c == 'o'
                if (enable) {
                    // Don't allow the prom and the data bus to output at the same time
                    prom.setDataBusMode(INPUT);
                    prom.enableOutput();
                } else {
                    prom.disableOutput();
                }
            }
        }
        break;

    case 'r':
        prom.setDataBusMode(INPUT);
        prom.enableOutput();
        b = prom.readDataBus();
        printByte(b);
        Serial.println();
        prom.disableOutput();
        break;

    case 'l':
        Serial.println(F("Writing the lock code to enable Software Write Protect mode."));
        prom.enableSoftwareWriteProtect();
        break;

    case 'u':
        Serial.println(F("Writing the unlock code to disable Software Write Protect mode."));
        timeStart = micros();
        prom.disableSoftwareWriteProtect();
        timeEnd = micros();
        Serial.print("Unlock command time in uSec=");
        Serial.println(timeEnd - timeStart);
        break;

    case 'p':
        pokeBytes(line+1);
        break;

    case 's':
        if ((arg = getHex32(cursor, noValue)) != noValue)
        {
            Serial.println();
            dumpBlock(arg, arg + 63);
        }
        else
            cmdError = true;
        break;

    case 'z':
        if ((arg = getHex32(cursor, noValue)) != noValue)
        {
            zapTest(arg);
        }
        else
            cmdError = true;
        break;

    default:
        unknownCmd = true;
        break;
    }

    if (cmdError) {
        Serial.println(F("Missing or invalid command argument"));
    } else if (unknownCmd) {
        Serial.print(F("Hardware Verifier - "));
        Serial.println(prom.getName());
        Serial.println();
        Serial.println(F("Valid commands are:"));
        Serial.println(F("  Axxxx          - Set address bus to xxxx"));
        Serial.println(F("  Dxx            - Set Data bus to xx"));
        Serial.println(F("  Cs             - Set Chip enable to state (e=enable, d=disable)"));
        Serial.println(F("  Os             - Set Output enable to state (e=enable, d=disable)"));
        Serial.println(F("  Ws             - Set Write enable to state (e=enable, d=disable)"));
        Serial.println(F("  R              - Read and print the value on the data bus"));
        Serial.println(F("  L              - Send Lock sequence to enable device Software Data Protection"));
        Serial.println(F("  U              - Send Unlock sequence to disable device Software Data Protection"));
        Serial.println("");
        Serial.println(F("  Pxxxx dd dd... - Poke (write) values to EEPROM (up to 32 values)"));
        Serial.println(F("  Sxxxx          - Show (dump) bytes from EEPROM to terminal"));
        Serial.println(F("  Zxxxx          - Zap (burn) a 32 byte test pattern"));
    }
}
