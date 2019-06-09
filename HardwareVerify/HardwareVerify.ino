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
* convert a hex byte (00 - ff) to byte
* @param c-string with the hex value of the byte
* @return byte represented by the digits
************************************************************/
byte hexByte(char * a)
{
    return (hexDigit(a[0]) << 4) | hexDigit(a[1]);
}


/************************************************************
* convert a hex word (0000 - ffff) to unsigned int
* @param c-string with the hex value of the word
* @return unsigned int represented by the digits
************************************************************/
unsigned int hexWord(char * data)
{
    return (hexDigit(data[0]) << 12) |
           (hexDigit(data[1]) <<  8) |
           (hexDigit(data[2]) <<  4) |
           (hexDigit(data[3]));
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



word start = 0;
word end = 0xff;
byte val = 0xff;

void loop()
{
    byte b;
    word w;
    bool error = false;
    char line[20];
    uint32_t numBytes;

    Serial.print("\n#");
    Serial.flush();
    readLine(line, sizeof(line));
    byte c = tolower(line[0]);
    if ((c >= 'A') && (c <= 'Z')) {
        c |= 0x20;
    }

    /*
    * Note that the comamnds here allow for direct writing of the 28C control lines with some exceptions to
    * protect the chip and the host arduino:
    *   1) When the O command is used to enable chip output, the arduino data bus us set to INPUT
    *   2) When the D command is used to write data from the arduino, the chip output is disabled
    *   3) The R command sets to output enable (OE) on the chip (but not the chip enable (CE)) */
    switch (c)
    {
    case 'a':
        if (hexDigit(line[1]) <= 15)
        {
            w = hexWord(line + 1);
            prom.setAddress(w);
        }
        else
            error = true;
        break;
    case 'd':
        if (hexDigit(line[1]) <= 15)
        {
            prom.disableOutput();
            prom.setDataBusMode(OUTPUT);
            b = hexByte(line + 1);
            prom.writeDataBus(b);
        }
        else
            error = true;
        break;
    case 'c':
    case 'o':
    case 'w':
        if ((line[1] == 'd') || (line[1] == 'e')) {
            bool enable = line[1] == 'e';
            if (c == 'c')
                if (enable) prom.enableChip(); else prom.disableChip();
            else if (c == 'w')
                if (enable) prom.enableWrite(); else prom.disableWrite();
            else { // c == 'o'
                if (enable) 
                {
                    // Don't allow the prom and the data bus to output at the same time
                    prom.setDataBusMode(INPUT);
                    prom.enableOutput();
                }
                else prom.disableOutput();
            }
        }
        else
        {
            error = true;
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
        prom.disableSoftwareWriteProtect();
        break;

    default:
        error = true;
        break;
    }

    if (error) {
        Serial.print(F("Hardware Verifier - "));
        Serial.println(prom.getName());
        Serial.println();
        Serial.println(F("Valid commands are:"));
        Serial.println(F("  Axxxx         - Set address bus to xxxx"));
        Serial.println(F("  Dxx           - Set Data bus to xx"));
        Serial.println(F("  Cs            - Set Chip enable to state (e=enable, d=disable)"));
        Serial.println(F("  Os            - Set Output enable to state (e=enable, d=disable)"));
        Serial.println(F("  Ws            - Set Write enable to state (e=enable, d=disable)"));
        Serial.println(F("  R             - Read and print the value on the data bus"));
        Serial.println(F("  L             - Send Lock sequence to enable device Software Data Protection"));
        Serial.println(F("  U             - Send Unlock sequence to disable device Software Data Protection"));
    }
}

