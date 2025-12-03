/**
* Read and write parallel EEPROMS with an interctive command-line interface.
* Modules are available for ATMEL 28C series EEPROMs and Intel 8755A EPROMS.
* Many other parallel EPROM/EEPROMs can be read, but not written, using the
* 28C code.
*
* The 28C module supports block writes for better performance and
* Software Data Protection (SDP) unlocking.
*
* ROM images are moved to and from a host computer using XMODEM.  This is
* available in a number of terminal programs, such as TeraTerm and Minicom.
*
* The default hardware uses two 74LS164 shift registers as the low and
* high address registers.
**/

#include "Configure.h"
#include "CmdStatus.h"
#include "XModem.h"

static const char * MY_VERSION = "3.7";

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
//PromDevice28C  prom(8 * 1024L, 0, 10, true);  // 28C64 with no page writes
//PromDevice28C  prom(2 * 1024L, 0, 10, true);  // 28C16 with no page writes

#elif defined(PROM_IS_27)
// Define a device for a 2764 EPROM with the following parameters:
//   8K byte device capacity
//   Program using dedicated WR pin
//   1000us (1ms) write pulse
//   Max 15 write attempts
//   4x overwrite pulse (4 * writePulseLength * numberOfPulsesWritten)
//   (true) verify data byte after writing
//PromDevice27  prom(8 * 1024L, E27C_PGM_WE, 1000L, 15, 4);  // 2764 with SEEQ intelligent programming
//PromDevice27  prom(32 * 1024L, E27C_PGM_WE, 1000L, 25, 3); // 27C256 with SEEQ intelligent programming
PromDevice27  prom(32 * 1024L, E27C_PGM_CE, 100L, 25, 0); // M27C256 with Presto II intelligent programming
//PromDevice27  prom(32 * 1024L, E27C_PGM_CE, 1000L, 25, 3); // M27256 with fast programming
//PromDevice27  prom(2 * 1024L, E27C_PGM_WE, 50000L, 1, 0);  // 2716 with single 50ms write
//PromDevice27  prom(512 * 1024L, E27C_PGM_CE, 100L, 11, 0); // 27C040 with Atmel rapid programming, CE connected to CE#/PGM#
//PromDevice27  prom(32 * 1024L, E27C_PGM_CE, 100L, 25, 0);  // W27C257/W27E257 with 100uS program pulse on CE
//PromDevice27  prom(64 * 1024L, E27C_PGM_CE, 100L, 1, 0, false);  // W27C512 with single 100uS program pulse on CE, no verify
//PromDevice27  prom(256 * 1024L, E27C_PGM_WE, 20L, 1, 0, false); // SST27SF020 with single 20us write, no verify

#elif defined(PROM_IS_SST39SF)
// Define a device for anSST39SF Flash with the following parameters:
//   512K byte device capacity
//   10us max write time
//   Data polling supported
//PromDeviceSST39SF  prom(128 * 1024L, 10, true);  // SST39SF010
//PromDeviceSST39SF  prom(256 * 1024L, 10, true);  // SST39SF020
PromDeviceSST39SF  prom(512 * 1024L, 10, true);  // SST39SF040

#elif defined(PROM_IS_SST28SF)
// Define a device for anSST28SF Flash with the following parameters:
//   512K byte device capacity
//   40us max write time
//   Data polling supported
PromDeviceSST28SF  prom(512 * 1024L, 40, true);

#elif defined(PROM_IS_8755A)
// Define a device for an Intel 8755A with a fixed size of 2K and no other parameters.
PromDevice8755A  prom(2 * 1024L);

#elif defined(PROM_IS_23)
PromDevice23 prom(2 * 1024L);   // 2316

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
const uint32_t unspec = ~0;
inline uint32_t if_unspec(uint32_t val, uint32_t repl) { return val == unspec? repl: val; }

enum {
    // CLI Commands
    CMD_INVALID,
    CMD_BLANK,
    CMD_CHECKSUM,
    CMD_DUMP,
    CMD_ERASE,
    CMD_FILL,
    CMD_INFO,
    CMD_LOCK,
    CMD_POKE,
    CMD_READ,
    CMD_UNLOCK,
    CMD_WRITE,
    CMD_ZAP,

    CMD_SCAN,
    CMD_TEST,
    CMD_PATTERN_FILL,
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
    } while ((c != '\n') && (c != '\r') && (ix < len));

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
        case 'b':  cmd = CMD_BLANK;     break;
        case 'c':  cmd = CMD_CHECKSUM;  break;
        case 'd':  cmd = CMD_DUMP;      break;
        case 'e':  cmd = CMD_ERASE;     break;
        case 'f':  cmd = CMD_FILL;      break;
        case 'i':  cmd = CMD_INFO;      break;
        case 'l':  cmd = CMD_LOCK;      break;
        case 'p':  cmd = CMD_POKE;      break;
        case 'r':  cmd = CMD_READ;      break;
        case 'u':  cmd = CMD_UNLOCK;    break;
        case 'w':  cmd = CMD_WRITE;     break;
        case 'z':  cmd = CMD_ZAP;       break;

        case 's':  cmd = CMD_SCAN;      break;
        case 't':  cmd = CMD_TEST;      break;
        case '!':  cmd = CMD_PATTERN_FILL;break;
        case '/':  cmd = CMD_LAST_STATUS; break;
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
uint32_t getHex32(char *& pData, uint32_t defaultValue=unspec)
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


/*
* Prints a 32 bit value as a hex.
*
* Note that no values over 5 digits are used in
* this appication, so only 5 digits are printed.*/
void printHex32(uint32_t u32)
{
    char line[6];

    line[0] = hex[(u32 >> 16) & 0x0f];
    line[1] = hex[(u32 >> 12) & 0x0f];
    line[2] = hex[(u32 >>  8) & 0x0f];
    line[3] = hex[(u32 >>  4) & 0x0f];
    line[4] = hex[(u32)       & 0x0f];
    line[5] = '\0';

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
        {}
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
word checksumBlock(uint32_t start, uint32_t end)
{
    word checksum = 0;
#if 1
    for (uint32_t addr = start; (addr <= end); addr += 2)
    {
        word w = prom.readData(addr);
        w <<= 8;
        w |= prom.readData(addr + 1);
        checksum += w;
    }
#else
    uint16_t crc = 0xffff;
    for (uint32_t addr = start; (addr <= end); addr++)
    {
        crc = crc ^ (uint16_t(prom.readData(addr)) << 8);
        for (int ix = 0; (ix < 8); ix++)
        {
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    checksum = crc;
#endif

    return checksum;
}


/**
* Read data from the device and dump it in hex and ascii.
**/
uint32_t dumpBlock(uint32_t start, uint32_t end)
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
            if (checkForBreak())
            {
                return addr;
            }
            memset(line, ' ', sizeof(line));
            count = 0;
        }
    }

    if (count)
    {
        Serial.println();
    }

    return end+1;
}


/**
 * Fill a block of PROM data with a single value.
 *
 * @param start - start address
 * @param end - end address
 * @param val - data byte to write to all addresses
 */
void fillBlock(uint32_t start, uint32_t end, byte val)
{
    enum { BLOCK_SIZE = 32 };
    byte block[BLOCK_SIZE];

    for (int ix = 0; ix < BLOCK_SIZE; ix++)
    {
        block[ix] = val;
    }

    for (uint32_t addr = start; (addr <= end); addr += BLOCK_SIZE)
    {
        uint32_t writeLen = ((end - addr + 1) < BLOCK_SIZE) ? (end - addr + 1) : uint32_t(BLOCK_SIZE);
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
void erasedBlockCheck(uint32_t start, uint32_t end)
{
    for (uint32_t addr = start; (addr <= end); addr ++)
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


/**
 * Write a series of bytes from the command line to the PROM.
 *
 * @param cursor - pointer to command line text
 */
void pokeBytes(char * pCursor)
{
    uint32_t val;
    uint32_t start;
    unsigned byteCtr = 0;

    enum { BLOCK_SIZE = 32 };
    byte data[BLOCK_SIZE];

    //first value returned is the starting address
    start = getHex32(pCursor, 0);

    while (((val = getHex32(pCursor)) != unspec) && (byteCtr < BLOCK_SIZE))
    {
        data[byteCtr++] = byte(val);
    }

    if (byteCtr > 0)
    {
        if (!prom.writeData(data, byteCtr, start))
        {
            cmdStatus.error("Write failed");
            return;
        }
    }
    else
    {
        cmdStatus.error("Missing address or data");
        return;
    }
    delay(100);

    if (!prom.is_readback_safe()) {
        // This chip uses the CE line for write control, so don't do the read because it
        // could cause a write operation that would corrupt the data.
        cmdStatus.info("Poke complete");
        return;
    }

    for (unsigned ix = 0; ix < byteCtr ; ix++)
    {
        byte val = prom.readData(start + ix);
        if (val != data[ix])
        {
            cmdStatus.error("Verify failed");
            cmdStatus.setValueHex(0, "addr", start + ix);
            cmdStatus.setValueHex(1, "read", val);
            cmdStatus.setValueHex(2, "expected", data[ix]);
            return;
        }
    }
    cmdStatus.info("Poke successful");
}


/**
 * Write a 32 byte test pattern to the PROM device and verify it
 * by reading back.  The pattern includes a walking 1 and a
 * walking zero, which may help to detect pins that are tied
 * together or swapped.
 *
 * @param start - start address
 */
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
        cmdStatus.error("Write failed");
        return;
    }

    delay(100);

    if (!prom.is_readback_safe()) {
        // This chip uses the CE line for write control, so don't do the read because it
        // could cause a write operation that would corrupt the data.
        cmdStatus.info("Write test complete");
        return;
    }

    for (unsigned ix = 0; ix < sizeof(testData); ix++)
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


void printRetStatus(ERET status)
{
    switch (status) {
    case RET_OK:          Serial.println(F("OK"));              break;
    case RET_FAIL:        Serial.println(F("FAILED"));          break;
    case RET_NOT_SUPPORT: Serial.println(F("NOT SUPPORTED"));   break;
    }
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
void scanBlock(uint32_t start, uint32_t end)
{
    enum { SCAN_TESTS = 10 };

    for (uint32_t addr = start; (addr <= end); addr++)
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
            printHex32(addr);
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
void testAddr(uint32_t addr)
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
 * Fill a PROM with a test pattern
 *
 * Each 8-byte block contains two bytes of 256-byte block number
 * followed by six bytes of byte number.
 *   00000  00 00 02 03 04 05 06 07  00 00 0a 0b 0c 0d 0e 0f
 *   00010  00 00 12 13 14 15 16 17  00 00 1a 1b 1c 1d 1e 1f
 *   00020  00 00 22 23 24 25 26 27  00 00 2a 2b 2c 2d 2e 2f
 *   ...
 *   000f0  00 00 f2 f3 f4 f5 f6 f7  00 00 fa fb fc fd fe ff
 *   00100  00 01 02 03 04 05 06 07  00 01 0a 0b 0c 0d 0e 0f
 *   00010  00 01 12 13 14 15 16 17  00 01 1a 1b 1c 1d 1e 1f
 *   ...
 *   3fff0  03 ff f2 f3 f4 f5 f6 f7  03 ff fa fb fc fd fe ff
 */
void patternFill()
{
    enum { BLOCK_SIZE = 32 };
    byte block[BLOCK_SIZE];

    Serial.print("Filling with pattern from 0 to ");
    printHex32(prom.end());
    Serial.print("...");

    for (uint32_t addr = 0; (addr <= prom.end()); addr += BLOCK_SIZE)
    {
        for (unsigned ix = 0; (ix < BLOCK_SIZE); ix+= 2)
        {
            if ((ix & 7) == 0)
            {
                block[ix]   = (addr >> 16) & 0xff;
                block[ix+1] = (addr >> 8)  & 0xff;
            }
            else
            {
                block[ix]   = (addr + ix)     & 0xff;
                block[ix+1] = (addr + ix + 1) & 0xff;
            }
        }

        if (!prom.writeData(block, BLOCK_SIZE, addr))
        {
            cmdStatus.error("Write failed");
            return;
        }
    }
}
#endif /* ENABLE_DEBUG_COMMANDS */


/************************************************
* MAIN
*************************************************/
uint32_t addr = 0;

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

char line[120];

void loop()
{
    uint32_t w;
    uint32_t numBytes;

    Serial.print("\n>");
    Serial.flush();
    readLine(line, sizeof(line));
    Serial.println();
    byte cmd = parseCommand(line[0]);
    char * pCursor = line+1;
    uint32_t start = getHex32(pCursor);
    uint32_t end = getHex32(pCursor);
    uint32_t val = getHex32(pCursor);
    static uint32_t dump_next = 0;

    if ((cmd != CMD_LAST_STATUS) && (cmd != CMD_INVALID))
    {
        cmdStatus.clear();
    }

    switch (cmd)
    {
    case CMD_BLANK:
        erasedBlockCheck(if_unspec(start, 0), if_unspec(end, prom.end()));
        break;

    case CMD_CHECKSUM:
        start = if_unspec(start, 0);
        end = if_unspec(end, prom.end());
        w = checksumBlock(start, end);
        Serial.print(F("Checksum "));
        printWord(start);
        Serial.print(F("-"));
        printWord(end);
        Serial.print(F(" = "));
        printWord(w);
        Serial.println();
        break;

    case CMD_DUMP:
        start = if_unspec(start, dump_next);
        dump_next = dumpBlock(start, if_unspec(end, start + 0xff));
        break;

    case CMD_ERASE:
        if (start == unspec || end == unspec)
        {
            Serial.println(F("Erase requires explicit start, end"));
        }
        else
        {
            printRetStatus(prom.erase(start, end));
        }
        break;

    case CMD_FILL:
        if (start == unspec || end == unspec || val == unspec)
        {
            Serial.println(F("Fill requires explicit start, end and value"));
        }
        else
        {
            prom.resetDebugStats();
            fillBlock(start, end, (byte)val);
        }
        break;

    case CMD_INFO:
        prom.printDebugStats();
        break;

    case CMD_LOCK:
        Serial.print(F("Writing the lock code to enable Software Write Protect mode: "));
        printRetStatus(prom.enableSoftwareWriteProtect());
        break;

    case CMD_POKE:
        prom.resetDebugStats();
        pokeBytes(line+1);
        break;

    case CMD_READ:
        start = if_unspec(start, 0);
        end = if_unspec(end, prom.end());
        if (xmodem.SendFile(start, end - start + 1))
        {
            cmdStatus.info("Send complete.");
            cmdStatus.setValueDec(0, "NumBytes", end - start + 1);
        }
        break;

    case CMD_UNLOCK:
        Serial.print(F("Writing the unlock code to disable Software Write Protect mode: "));
        printRetStatus(prom.disableSoftwareWriteProtect());
        break;

    case CMD_WRITE:
        prom.resetDebugStats();
        start = if_unspec(start, 0);
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

    case CMD_ZAP:
        prom.resetDebugStats();
        zapTest(if_unspec(start, 0));
        break;

#ifdef ENABLE_DEBUG_COMMANDS
    case CMD_SCAN:
        scanBlock(if_unspec(start, 0), if_unspec(end, prom.end()));
        break;

    case CMD_TEST:
        testAddr(if_unspec(start, 0));
        break;

    case CMD_PATTERN_FILL:
        patternFill();
        break;
#endif /* ENABLE_DEBUG_COMMANDS */

    case CMD_LAST_STATUS:
        Serial.println(F("Status of last command:"));
        break;

    default:
        Serial.print(F("TommyPROM "));
        Serial.print(MY_VERSION);
        Serial.print(F(" - "));
        Serial.println(prom.getName());
        Serial.println();
        Serial.println(F("Valid commands are:"));
        Serial.println(F("  Bsssss eeeee    - Check to see if device range is Blank/erased (all FF)"));
        Serial.println(F("  Csssss eeeee    - Compute Checksum from device"));
        Serial.println(F("  Dsssss eeeee    - Dump bytes from device to terminal"));
        Serial.println(F("  Esssss eeeee    - Erase address range on device (needed for some Flash)"));
        Serial.println(F("  Fsssss eeeee dd - Fill block on device with fixed value"));
        Serial.println(F("  I               - Print debug Info"));
        Serial.println(F("  L               - Lock (enable) device Software Data Protection"));
        Serial.println(F("  Psssss dd dd... - Poke (write) values to device (up to 32 values)"));
        Serial.println(F("  Rsssss eeeee    - Read from device and save to XMODEM CRC file"));
        Serial.println(F("  U               - Unlock (disable) device Software Data Protection"));
        Serial.println(F("  Wsssss          - Write to device from XMODEM CRC file"));
        Serial.println(F("  Zsssss          - Zap (burn) a 32 byte test pattern"));
#ifdef ENABLE_DEBUG_COMMANDS
        Serial.println();
        Serial.println(F("  Ssssss eeeee    - Scan addresses (read each 10x)"));
        Serial.println(F("  Tsssss          - Test read address (read 100x)"));
        Serial.println(F("  !               - Fill entire device with a test pattern"));
#endif /* ENABLE_DEBUG_COMMANDS */
        break;
    }

    if (!cmdStatus.isClear() || (cmd == CMD_LAST_STATUS))
    {
        Serial.println();
        cmdStatus.printStatus();
    }
}
