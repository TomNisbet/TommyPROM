/**
* Read and write ATMEL 28C series EEPROMs.  Support block writes for better
* performance.  Read-only is supported for most parallel EPROM/EEPROMs.
*
* ROM images are moved to and from a host computer using XMODEM.
* This is available in a number of terminal programs, such as
* TeraTerm and Minicom.
*
* The hardware uses two 74LS164 shift registers as the low and
* high address registers.
**/

#include "Configure.h"
#include "CmdStatus.h"
#include "XModem.h"


// Global status
CmdStatus cmdStatus;


// Declare a global PROM device depending on the device type that is
// defined in Configure.h
#if defined(PROM_IS_28C)
// Define a device for a 28C256 EEPROM with the following parameters:
//   32K byte device capacity
//   64 byte block writes
//   10ms max write time
//   Data polling supported
PromDevice28C  prom(32 * 1024L, 64, 10, true);

#elif defined(PROM_IS_27)
// Define a device for a 2764 EPROM with the following parameters:
//   8K byte device capacity
//   1000us (1ms) write pulse
//   15 write attempts
//   4x overwrite pulse
PromDevice27  prom(8 * 1024L, 1000L, 15, 4); // 2764 with SEEQ intelligent programming
//PromDevice27  prom(32 * 1024L, 1000L, 25, 3); // 27C256 with SEEQ intelligent programming
//PromDevice27  prom(2 * 1024L, 50000L, 1, 0); // 2716 with single 50ms write
//PromDevice27  prom(64 * 1024L, 100L, 11, 0); // 27C040 with Atmel rapid programming

#elif defined(PROM_IS_8755A)
// Define a device for an 8755A.  This has a fixed size of 2K and no
// other parameters.
PromDevice8755A  prom(2 * 1024L);

// Additional device-specific code goes here...
//#elif defined(PROM_IS...

#else
#error "Must define a PROM type in Configure.h"
#endif

// Global XModem driver
XModem xmodem(prom, cmdStatus);


/*****************************************************************************/
/*****************************************************************************/

/**
 * CLI parse functions
 */
const char hex[] = "0123456789abcdef";

enum {
    // CLI Commands
    CMD_INVALID,
    CMD_CHECKSUM,
    CMD_DUMP,
    CMD_ERASED,
    CMD_FILL,
    CMD_READ,
    CMD_UNLOCK,
    CMD_WRITE,

    CMD_SCAN,
    CMD_TEST,
    CMD_ZAP,
    CMD_LAST_STATUS
};


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


byte parseCommand(char c)
{
    byte cmd = CMD_INVALID;

    // Convert the command to lowercase.
    if ((c >= 'A') && (c <= 'Z')) {
        c |= 0x20;
    }

    switch (c)
    {
        case 'c':  cmd = CMD_CHECKSUM;  break;
        case 'd':  cmd = CMD_DUMP;      break;
        case 'e':  cmd = CMD_ERASED;    break;
        case 'f':  cmd = CMD_FILL;      break;
        case 'r':  cmd = CMD_READ;      break;
        case 'u':  cmd = CMD_UNLOCK;    break;
        case 'w':  cmd = CMD_WRITE;     break;

        case 's':  cmd = CMD_SCAN;      break;
        case 't':  cmd = CMD_TEST;      break;
        case 'z':  cmd = CMD_ZAP;       break;
        case '/':  cmd = CMD_LAST_STATUS;break;
        default:   cmd = CMD_INVALID;   break;
    }

    return cmd;
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


// If the user presses a key then pause until they press another.  Return true if
// Ctrl-C is pressed.
bool checkForBreak()
{
    if (Serial.available())
    {
        if (Serial.read() == 0x03)
        {
            return true;
        }
        while (!Serial.available())
        {;}
        if (Serial.read() == 0x03)
        {
            return true;
        }
    }

    return false;
}



/*****************************************************************************/
/*****************************************************************************/
/**
 * Command implementations
 */


/**
 * Compute a 16 bit checksum from PROM data
 *
 * Note that this always reads an even number of bytes from the
 * device and will read one byte beyond the specified end
 * address if an odd number of bytes is specified by start and
 * end.
 */
word checksumBlock(word start, word end)
{
    word checksum = 0;
    for (word addr = start; (addr <= end); addr += 2)
    {
        word w = prom.readData(addr);
        w <<= 8;
        w |= prom.readData(addr + 1);
        checksum += w;

        if (addr >= 0xfffe)
        {
            // This is a really kludgy check to make sure the counter doesn't wrap
            // around to zero.  Could replace addr and end with longs to fix this,
            // but that might not be any faster.
            break;
        }
    }

    return checksum;
}


/**
* Read data from the device and dump it in hex and ascii.
**/
void dumpBlock(word start, word end)
{
    char line[81];
//  01234567891 234567892 234567893 234567894 234567895 234567896 234567897 23456789
//  1234: 01 23 45 67  89 ab cf ef  01 23 45 67  89 ab cd ef  1.2.3.4. 5.6.7.8.
    int count = 0;

    memset(line, ' ', sizeof(line));

    char * pHex = line;
    char * pChar = line + 58;
    for (word addr = start; (addr <= end); addr++)
    {
        if (count == 0)
        {
            //print out the address at the beginning of the line
            pHex = line;
            pChar = line + 58;
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
            if (checkForBreak())
            {
                return;
            }
            memset(line, ' ', sizeof(line));
            count = 0;
        }
    }

    if (count)
    {
        Serial.println();
    }
}


/**
 * Fill a block of PROM data with a single value.
 *
 * @param start - start address
 * @param end - end address
 * @param val - data byte to write to all addresses
 */
void fillBlock(word start, word end, byte val)
{
    enum { BLOCK_SIZE = 32 };
    byte block[BLOCK_SIZE];

    for (int ix = 0; ix < BLOCK_SIZE; ix++)
    {
        block[ix] = val;
    }

    for (word addr = start; (addr <= end); addr += BLOCK_SIZE)
    {
        unsigned writeLen = ((end - addr + 1) < BLOCK_SIZE) ? (end - addr + 1) : BLOCK_SIZE;
        if (!prom.writeData(block, writeLen, addr))
        {
            cmdStatus.error("Write failed");
            return;
        }
    }
}


/**
 * Verify that a block of PROM contains the all FF erased value.
 *
 * @param start - start address
 * @param end - end address
 */
void erasedBlockCheck(word start, word end)
{
    for (word addr = start; (addr <= end); addr ++)
    {
        byte val = prom.readData(addr);
        if (val != 0xff)
        {
            cmdStatus.error("Block is not erased");
            cmdStatus.setValueHex(0, "addr", addr);
            cmdStatus.setValueHex(1, "value", val);
            return;
        }
    }
    cmdStatus.info("Block is erased");
}


#ifdef ENABLE_DEBUG_COMMANDS
/**
 * Runs through a range of addresses, reading a single address
 * multiple times.  Fails if all of the reads for an address do
 * not produce that same value.
 *
 * @param start - start address
 * @param end - end address
 */
void scanBlock(word start, word end)
{
    enum { SCAN_TESTS = 10 };

    for (word addr = start; (addr <= end); addr++)
    {
        byte values[SCAN_TESTS];
        values[0] = prom.readData(addr);
        bool fail = false;
        for (int ix = 1; (ix < SCAN_TESTS); ix++)
        {
            values[ix] = prom.readData(addr);
            if (values[ix] != values[0])
            {
                fail = true;
            }
        }
        if (fail)
        {
            printWord(addr);
            Serial.print(": ");
            for (int ix = 0; (ix < SCAN_TESTS); ix++)
            {
                printByte(values[ix]);
                Serial.print(" ");
            }
            Serial.println();
            cmdStatus.error("Repeated reads returned different values");
            cmdStatus.setValueHex(0, "addr", addr);
            break;
        }

        if (addr == 0xffff)  break;
    }
}


/**
 * Reads a single address in the PROM multiple times and fails
 * if all of the reads do not produce the same value.
 *
 * @param addr - address to test
 */
void testAddr(word addr)
{
    enum { NUM_TESTS = 100 };

    bool fail = false;
    byte value;
    byte firstValue = prom.readData(addr);
    for (int ix = 1; (ix < NUM_TESTS); ix++)
    {
        value = prom.readData(addr);
        if (value != firstValue)
        {
            fail = true;
        }
    }
    if (fail)
    {
        cmdStatus.error("Repeated reads returned different values");
        cmdStatus.setValueHex(0, "addr", addr);
        cmdStatus.setValueHex(1, "first read", firstValue);
        cmdStatus.setValueHex(2, "last read", value);
    }
    else
    {
        cmdStatus.info("Read test passed");
    }
}


/**
 * Write a 32 byte test pattern to the PROM device and verify it
 * by reading back.  The pattern includes a walking 1 and a
 * walking zero, which may help to detect pins that are tied
 * together or swapped.
 *
 * @param start - start address
 */
void zapTest(word start)
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
        cmdStatus.error("Write failed");
        return;
    }

    delay(100);
    for (int ix = 0; ix < sizeof(testData); ix++)
    {
        byte val = prom.readData(start + ix);
        if (val != testData[ix])
        {
            cmdStatus.error("Verify failed");
            cmdStatus.setValueHex(0, "addr", start + ix);
            cmdStatus.setValueHex(1, "read", val);
            cmdStatus.setValueHex(2, "expected", testData[ix]);
            return;
        }
    }
    cmdStatus.info("Write test successful");
}
#endif /* ENABLE_DEBUG_COMMANDS */


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


/**
* main loop that runs infinite times, parsing a given command and
* executing read or write requestes.
**/

/* 8085 Test programs
byte ledTest[] =
{
    0xc3, 0x03, 0x80, 0x3e, 0xc0, 0x30, 0x3e, 0xff,
    0x47, 0x3d, 0x05, 0xc2, 0x0a, 0x80, 0xfe, 0x00,
    0xc2, 0x09, 0x80, 0x3e, 0x40, 0x30, 0x3e, 0xff,
    0x47, 0x3d, 0x05, 0xc2, 0x1a, 0x80, 0xfe, 0x00,
    0xc2, 0x19, 0x80, 0xc3, 0x03, 0x80
};
byte charTest[] =
{
    0xc3, 0x03, 0x80, 0x0e, 0x55, 0xf3, 0x06, 0x0b, 0xaf, 0x3e, 0x80, 0x1f,
    0x3f, 0x30, 0x21, 0x19, 0x00, 0x2d, 0xc2, 0x11, 0x80, 0x25, 0xc2, 0x11,
    0x80, 0x37, 0x79, 0x1f, 0x4f, 0x05, 0xc2, 0x09, 0x80, 0x3e, 0xc0, 0x30,
    0x3e, 0x40, 0x30, 0x3e, 0xc0, 0x30, 0x3e, 0x40, 0x30, 0x21, 0xff, 0xff,
    0x2d, 0xc2, 0x30, 0x80, 0x25, 0xc2, 0x30, 0x80, 0xc3, 0x03, 0x80
};
*/

word start = 0;
word end = 0xff;
byte val = 0xff;

void loop()
{
    word w;
    char line[20];
    uint32_t numBytes;

    Serial.print("\n>");
    Serial.flush();
    readLine(line, sizeof(line));
    byte cmd = parseCommand(line[0]);
    if (hexDigit(line[1]) <= 15)
        start = hexWord(line + 1);
    if (hexDigit(line[6]) <= 15)
        end = hexWord(line + 6);
    if (hexDigit(line[6]) <= 11)
        val = hexByte(line + 11);

    if ((cmd != CMD_LAST_STATUS) && (cmd != CMD_INVALID))
    {
        cmdStatus.clear();
    }

    switch (cmd)
    {
    case CMD_CHECKSUM:
        w = checksumBlock(start, end);
        Serial.print("Checksum ");
        printWord(start);
        Serial.print("-");
        printWord(end);
        Serial.print(" = ");
        printWord(w);
        Serial.println();
        break;

    case CMD_DUMP:
        dumpBlock(start, end);
        break;

    case CMD_ERASED:
        erasedBlockCheck(start, end);
        break;

    case CMD_FILL:
        fillBlock(start, end, val);
        break;

    case CMD_READ:
        Serial.println(F("Set the terminal to receive XMODEM CRC"));
        if (xmodem.SendFile(start, uint32_t(end) - start + 1))
        {
            cmdStatus.info("Send complete.");
            cmdStatus.setValueDec(0, "NumBytes", uint32_t(end) - start + 1);
        }
        break;

    case CMD_UNLOCK:
        Serial.println(F("Writing the unlock code to disable Software Write Protect mode."));
        prom.disableSoftwareWriteProtect();
        break;

    case CMD_WRITE:
        Serial.println(F("Send the image file using XMODEM CRC"));
        numBytes = xmodem.ReceiveFile(start);
        if (numBytes)
        {
            cmdStatus.info("Success writing to EEPROM device.");
            cmdStatus.setValueDec(0, "NumBytes", numBytes);
        }
        else
        {
            xmodem.Cancel();
        }
        break;

#ifdef ENABLE_DEBUG_COMMANDS
    case CMD_SCAN:
        scanBlock(start, end);
        break;

    case CMD_TEST:
        testAddr(start);
        break;

    case CMD_ZAP:
        zapTest(start);
        break;
#endif /* ENABLE_DEBUG_COMMANDS */

    case CMD_LAST_STATUS:
        Serial.println(F("Status of last command:"));
        break;

    default:
        Serial.println(F("TommyPROM 1.6\n"));
        Serial.println(F("Valid commands are:"));
        Serial.println(F("  Cssss eeee    - Compute checksum from device"));
        Serial.println(F("  Dssss eeee    - Dump bytes from device to terminal"));
        Serial.println(F("  Essss eeee    - Check to see if device range is Erased (all FF)"));
        Serial.println(F("  Fssss eeee dd - Fill block on device with fixed value"));
        Serial.println(F("  Rssss eeee    - Read from device and save to XMODEM CRC file"));
        Serial.println(F("  U             - Unlock device Software Data Protection"));
        Serial.println(F("  Wssss         - Write to device from XMODEM CRC file"));
#ifdef ENABLE_DEBUG_COMMANDS
        Serial.println();
        Serial.println(F("  Sssss eeee    - Scan addresses (read each 10x)"));
        Serial.println(F("  Tssss         - Test read address (read 100x)"));
        Serial.println(F("  Zssss         - Zap (burn) a 32 byte test pattern"));
#endif /* ENABLE_DEBUG_COMMANDS */
        break;
    }

    if (!cmdStatus.isClear() || (cmd == CMD_LAST_STATUS))
    {
        Serial.println();
        cmdStatus.printStatus();
    }
}

