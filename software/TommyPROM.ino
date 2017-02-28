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


// Uncomment only one of these to match the Arduino model

//#define ARDUINO_IS_MICRO
//#define ARDUINO_IS_UNO
#define ARDUINO_IS_NANO

// Comment this out to remove extra debugging commands and code
#define ENABLE_DEBUG_COMMANDS


/*****************************************************************************/
/*****************************************************************************/
/**
 * Command Status class
 *
 * Creates an object that holds an error message and a number of
 * optional numeric values.  Each value has a format (hex,
 * decimal) and a label.  The printStatus call formats all of
 * the data in the command status and prints it out to the
 * serial port.
 */
class CmdStatus
{
  public:
    CmdStatus();
    void clear();
    void info(char * msg);
    void error(char * msg);
    void setValueDec(int index, char * label, long value);
    void setValueHex(int index, char * label, long value);

    bool isClear();
    void printStatus();

private:
    enum
    {
        MAX_VALUES = 3
    };
    enum StatusLevel { SL_NONE, SL_INFO, SL_ERROR };
    enum ValueType { VT_NONE, VT_DEC, VT_HEX };

    struct StatusValue
    {
        const char * label;
        ValueType valType;
        long      value;
    };

    StatusLevel level;
    const char * message;
    StatusValue values[MAX_VALUES];

    void setLongValue(int index, char * label, long value, ValueType vt);
};

CmdStatus::CmdStatus()
{
    clear();
}

void CmdStatus::clear()
{
    level = SL_NONE;
    message = "OK";
    for (int ix = 0; ix < MAX_VALUES; ix++)
    {
        values[ix].valType = VT_NONE;
    }
}

void CmdStatus::info(char * msg)
{
    level = SL_INFO;
    message = msg;
}

void CmdStatus::error(char * msg)
{
    level = SL_ERROR;
    message = msg;
}

void CmdStatus::setValueDec(int index, char * label, long value)
{
    setLongValue(index, label, value, VT_DEC);
}

void CmdStatus::setValueHex(int index, char * label, long value)
{
    setLongValue(index, label, value, VT_HEX);
}

void CmdStatus::setLongValue(int index, char * label, long value, ValueType vt)
{
    if ((vt >= 0) && (vt < MAX_VALUES))
    {
        values[index].label = label;
        values[index].value = value;
        values[index].valType = vt;
    }
}

bool CmdStatus::isClear()
{
    return level == SL_NONE;
}

void CmdStatus::printStatus()
{
    if (level == SL_NONE)
    {
        Serial.println("OK");
        return;
    }
    else if (level == SL_INFO)
    {
        Serial.print("INFO: ");
    }
    else {
        Serial.print("ERROR: ");
    }
    Serial.print(message);
    for (int ix = 0; ix < MAX_VALUES; ix++)
    {
        if (values[ix].valType != VT_NONE)
        {
            Serial.print("  ");
            Serial.print(values[ix].label);
            Serial.print("=");
            if (values[ix].valType == VT_DEC)
            {
                Serial.print(values[ix].value, DEC);
            }
            else
            {
                Serial.print("0x");
                Serial.print(values[ix].value, HEX);
            }
        }
    }
    Serial.println("");
}


// Global status
CmdStatus cmdStatus;



/*****************************************************************************/
/*****************************************************************************/
/**
 * PromDevice class
 *
 * Provides the interface to read and write data from a parallel PROM using the
 * Arduino.
 *
 * Block writes are supported on compatible devices by specifying a blockSize
 * in the constructor.  Use zero for byte writes.
 */
class PromDevice
{
  public:
    PromDevice(unsigned long size, word blockSize, unsigned maxWriteTime, bool polling);
    void begin();
    byte readByte(word address);
    bool writeData(byte data[], word len, word address);
    void disableSoftwareWriteProtect();

  private:
    unsigned int mSize;             // Size of the device, in bytes
    unsigned int mBlockSize;        // Block size for page writes, zero if N/A
    unsigned int mMaxWriteTime;     // Max time (in ms) to wait for write cycle to complete
    bool mSupportsDataPoll;         // End of write detected by data polling

    void enableChip();
    void disableChip();
    void enableOutput();
    void disableOutput();
    void enableWrite();
    void disableWrite();

    void setDataBusMode(uint8_t mode);
    byte readDataBus();
    void writeDataBus(byte data);
    void setAddress(word address);
    void setAddressRegister(uint8_t clkPin, byte addr);
    bool burnByte(byte value, word address);
    bool burnBlock(byte data[], word len, word address);
    bool waitForWriteCycleEnd(byte lastValue);
    void setByte(byte value, word address);
};

// Define a device for a 28C256 EEPROM with the following parameters:
//   32K byte device capacity
//   64 byte block writes
//   10ms max write time
//   Data polling supported
PromDevice  prom(32 * 1024, 64, 10, true);

// IO lines for the EEPROM device control
// Pins D2..D9 are used for the data bus.
#define WE              A0
#define CE              A1
#define OE              A2
#define ADDR_CLK_HI     A3
#define ADDR_CLK_LO     A4
#define ADDR_DATA       A5


PromDevice::PromDevice(unsigned long size, word blockSize, unsigned maxWriteTime, bool polling)
{
    mSize = size;
    mBlockSize = blockSize;
    mMaxWriteTime = maxWriteTime;
    mSupportsDataPoll = polling;
}


void PromDevice::begin()
{
    // Define the data bus as input initially so that it does not put out a
    // signal that could collide with output on the data pins of the EEPROM.
    setDataBusMode(INPUT);

    // Define the EEPROM control pins as output, making sure they are all
    // in the disabled state.
    digitalWrite(OE, HIGH);
    pinMode(OE, OUTPUT);
    digitalWrite(CE, HIGH);
    pinMode(CE, OUTPUT);
    digitalWrite(WE, HIGH);
    pinMode(WE, OUTPUT);

    // The address control pins are always outputs.
    pinMode(ADDR_DATA, OUTPUT);
    pinMode(ADDR_CLK_LO, OUTPUT);
    pinMode(ADDR_CLK_HI, OUTPUT);
    digitalWrite(ADDR_DATA, LOW);
    digitalWrite(ADDR_CLK_LO, LOW);
    digitalWrite(ADDR_CLK_HI, LOW);

    // To save time, the setAddress only writes the hi byte if it has changed.
    // The value used to detect the change is initialized to a non-zero value,
    // so set an initial address to avoid the the case where the first address
    // written is the 'magic' initial address.
    setAddress(0x0000);
}


// Read a byte from a given address
byte PromDevice::readByte(word address)
{
    byte data = 0;
    setAddress(address);
    setDataBusMode(INPUT);
    disableOutput();
    disableWrite();
    enableChip();
    enableOutput();
    data = readDataBus();
    disableOutput();
    disableChip();

    return data;
}


// Write a block of data to the device.  If the device supports block writes,
// the data will be broken into chunks and written using the block mode.
// Otherwise, each byte will be individually written and verified.
bool PromDevice::writeData(byte data[], word len, word address)
{
    bool status = true;

    if (mBlockSize == 0)
    {
        // Device does not support block writes.
        for (word ix = 0; (ix < len); ix++)
        {
            if (burnByte(data[ix], address + ix) == false)
            {
                status = false;
                break;
            }
        }
    }
    else
    {
        word offset = 0;
        word chunkSize;
        if (address & (mBlockSize - 1))
        {
            // Address does not start on a block boundary.  Adjust the size of
            // the first block to fit within a single block.
            chunkSize = mBlockSize - (address & (mBlockSize - 1));
            chunkSize = (chunkSize > len) ? len : chunkSize;
            if (burnBlock(data, chunkSize, address) == false)
            {
                return false;
            }
            offset += chunkSize;
            len -= chunkSize;
        }

        // All writes are now aligned to block boundaries, so write full blocks
        // or remaining length, whichever is smaller.
        while (len > 0)
        {
            chunkSize = (len > mBlockSize) ? mBlockSize : len;
            if (burnBlock(data + offset, chunkSize, address + offset) == false)
            {
                status = false;
                break;
            }
            offset += chunkSize;
            len -= chunkSize;
        }
    }

    return status;
}


// Write the special six-byte code to turn off Software Data Protection.
void PromDevice::disableSoftwareWriteProtect()
{
    disableOutput();
    disableWrite();
    enableChip();
    setDataBusMode(OUTPUT);

    setByte(0xaa, 0x5555);
    setByte(0x55, 0x2aaa);
    setByte(0x80, 0x5555);
    setByte(0xaa, 0x5555);
    setByte(0x55, 0x2aaa);
    setByte(0x20, 0x5555);

    setDataBusMode(INPUT);
    disableChip();
}


// BEGIN PRIVATE METHODS
//
// Set the status of the device control pins
void PromDevice::enableChip()       { digitalWrite(CE, LOW); }
void PromDevice::disableChip()      { digitalWrite(CE, HIGH);}
void PromDevice::enableOutput()     { digitalWrite(OE, LOW); }
void PromDevice::disableOutput()    { digitalWrite(OE, HIGH);}
void PromDevice::enableWrite()      { digitalWrite(WE, LOW); }
void PromDevice::disableWrite()     { digitalWrite(WE, HIGH);}


// Set the I/O state of the data bus.
// The first two bits of port D are used for serial, so the 8 bits data bus are
// on pins D2..D9.
void PromDevice::setDataBusMode(uint8_t mode)
{
#if defined(AUDUINO_IS_UNO) || defined(ARDUINO_IS_NANO)
    // On the Uno and Nano, D2..D9 maps to the upper 6 bits of port D and the
    // lower 2 bits of port B.
    if (mode == OUTPUT)
    {
        DDRB |= 0x03;
        DDRD |= 0xfc;
    }
        else
    {
        DDRB &= 0xfc;
        DDRD &= 0x03;
    }
#endif
#ifdef ARDUINO_IS_MICRO
    // On the Micro, D2..D9 maps to the upper 7 bits of port B and the
    // lower bit of port D.
    if (mode == OUTPUT)
    {
        DDRB |= 0xfe;
        DDRD |= 0x01;
    }
        else
    {
        DDRB &= 0x01;
        DDRD &= 0xfe;
    }
#endif
}


// Read a byte from the data bus.  The caller must set the bus to input_mode
// before calling this or no useful data will be returned.
byte PromDevice::readDataBus()
{
#if defined(AUDUINO_IS_UNO) || defined(ARDUINO_IS_NANO)
    return (PINB << 6) | (PIND >> 2);
#endif
#ifdef ARDUINO_IS_MICRO
    return (PINB & 0xfe) | (PIND & 0x01);
#endif
}


// Write a byte to the data bus.  The caller must set the bus to output_mode
// before calling this or no data will be written.
void PromDevice::writeDataBus(byte data)
{
#if defined(AUDUINO_IS_UNO) || defined(ARDUINO_IS_NANO)
     PORTB = (PORTB & 0xfc) | (data >> 6);
     PORTD = (PORTD & 0x03) | (data << 2);
#endif
#ifdef ARDUINO_IS_MICRO
    PORTB = (PORTB & 0x01) | (data & 0xfe);
    PORTD = (PORTD & 0xfe) | (data & 0x01);
#endif
}


// Set a 16 bit address in the two address shift registers.
void PromDevice::setAddress(word address)
{
    static byte lastHi = 0xca;
    byte hi = address >> 8;
    byte lo = address & 0xff;

    if (hi != lastHi)
    {
        setAddressRegister(ADDR_CLK_HI, hi);
        lastHi = hi;
    }
    setAddressRegister(ADDR_CLK_LO, lo);
}


// Shift an 8-bit value into one of the address shift registers.  Note that
// the data pins are tied together, selecting the high or low address register
// is a matter of using the correct clock pin to shift the data in.
void PromDevice::setAddressRegister(uint8_t clkPin, byte addr)
{
    // Make sure the clock is low to start.
    digitalWrite(clkPin, LOW);

    // Shift 8 bits in, starting with the MSB.
    for (int ix = 0; (ix < 8); ix++)
    {
        // Set the data bit
        if (addr & 0x80)
        {
            digitalWrite(ADDR_DATA, HIGH);
        }
        else
        {
            digitalWrite(ADDR_DATA, LOW);
        }

        digitalWrite(clkPin, HIGH); // Clock in a bit
        digitalWrite(clkPin, LOW);  // Reset the clock pin
        addr <<= 1;
    }
}


// Burn a byte to the chip and verify that it was written.
bool PromDevice::burnByte(byte value, word address)
{
    bool status = false;

    disableOutput();
    disableWrite();

    setAddress(address);
    setDataBusMode(OUTPUT);
    writeDataBus(value);

    enableChip();
    delayMicroseconds(1);
    enableWrite();
    delayMicroseconds(1);
    disableWrite();

    status = waitForWriteCycleEnd(value);

    disableChip();

    return status;
}


bool PromDevice::burnBlock(byte data[], word len, word address)
{
    bool status = false;

    if (len == 0)  return true;

    disableOutput();
    disableWrite();
    enableChip();

    // Write all of the bytes in the block out to the chip.  The chip will
    // program them all at once as long as they are written fast enough.
    setDataBusMode(OUTPUT);
    for (word ix = 0; (ix < len); ix++)
    {
        setAddress(address + ix);
        writeDataBus(data[ix]);

        delayMicroseconds(1);
        enableWrite();
        delayMicroseconds(1);
        disableWrite();
    }

    status = waitForWriteCycleEnd(data[len - 1]);
    disableChip();

    return status;
}


bool PromDevice::waitForWriteCycleEnd(byte lastValue)
{
    if (mSupportsDataPoll)
    {
        // Verify programming complete by reading the last value back until it matches the
        // value written twice in a row.  The D7 bit will read the inverse of last written
        // data and the D6 bit will toggle on each read while in programming mode.
        //
        // Note that the max readcount is set to the device's maxReadTime (in uSecs)
        // divided by two because there are two 1 uSec delays in the loop.  In reality,
        // the loop could run for longer because this does not account for the time needed
        // to run all of the loop code.  In actual practice, the loop will terminate much
        // earlier because it will detect the end of the write well before the max time.
        setDataBusMode(INPUT);
        delayMicroseconds(1);
        for (int readCount = mMaxWriteTime * 1000 / 2; (readCount > 0); readCount--)
        {
            enableOutput();
            delayMicroseconds(1);
            byte b1 = readDataBus();
            disableOutput();
            enableOutput();
            delayMicroseconds(1);
            byte b2 = readDataBus();
            disableOutput();
            if ((b1 == b2) && (b1 == lastValue))
            {
                return true;
            }
        }

        return false;
    }
    else
    {
        // No way to detect success.  Just wait the max write time.
        delayMicroseconds(mMaxWriteTime * 1000L);
        return true;
    }
}


// Set an address and data value and toggle the write control.  This is used
// to write control sequences, like the software write protect.  This is not a
// complete byte write function because it does not set the chip enable or the
// mode of the data bus.
void PromDevice::setByte(byte value, word address)
{
    setAddress(address);
    writeDataBus(value);

    delayMicroseconds(1);
    enableWrite();
    delayMicroseconds(1);
    disableWrite();
}



/*****************************************************************************/
/*****************************************************************************/
/**
 *
 * XMODEM CRC Communication
 *
 * Simple implementation of read and write using XMODEM CRC.  This is tied
 * directly to the PROM code, so the receive function writes the data to the
 * PROM device as each packet is received.  The complete file is not kept
 * in memory.
 */
class XModem
{
  public:
    XModem(PromDevice * pd) : pProm(pd) {}
    uint32_t ReceiveFile(uint16_t address);
    boolean SendFile(uint16_t address, uint32_t fileSize);
    void Cancel();

  private:
    enum
    {
        // XMODEM control characters.
        XMDM_SOH = 0x01,
        XMDM_EOT = 0x04,
        XMDM_ACK = 0x06,
        XMDM_NAK = 0x15,
        XMDM_CAN = 0x18,
        XMDM_ESC = 0x1b,
        XMDM_CRC = 'C'
    };
    enum
    {
        // Misc constants for XMODEM.
        PKTLEN = 128
    };

    PromDevice * pProm;

    int GetChar(int msWaitTime = 3000);
    uint16_t UpdateCrc(uint16_t crc, uint8_t data);
    boolean StartReceive();
    boolean ReceivePacket(uint8_t buffer[], unsigned bufferSize, uint8_t seq, uint16_t destAddr);
    void SendPacket(uint16_t address, uint8_t seq);
};

XModem xmodem(&prom);

uint32_t XModem::ReceiveFile(uint16_t address)
{
    uint8_t buffer[PKTLEN];
    int c;
    uint8_t seq = 1;
    uint32_t numBytes = 0;
    bool complete = false;

    if (!StartReceive())
    {
        cmdStatus.error("Timeout waiting for transfer to start.");
        return 0;
    }

    while (!complete)
    {
        if ((c = GetChar()) < 0)
        {
            cmdStatus.error("Timeout waiting for start of next packet.");
            cmdStatus.setValueDec(0, "seq", seq);
            return 0;
        }

        switch (c)
        {
            case XMDM_SOH:
            // Start of a packet
            if (ReceivePacket(buffer, PKTLEN, seq++, address))
            {
                numBytes += PKTLEN;
                address += PKTLEN;
            }
            else
            {
                return 0;
            }
            break;

            case XMDM_EOT:
            // End of transfer
            Serial.write(XMDM_ACK);
            complete = true;
            break;

            case XMDM_CAN:
            case XMDM_ESC:
                // Cancel from sender
                cmdStatus.error("Transfer canceled by sender.");
                cmdStatus.setValueDec(0, "seq", seq);
                return 0;
                break;

            default:
            // Fail the transfer on anything else
            cmdStatus.error("Unexpected character received waiting for next packet.");
            cmdStatus.setValueDec(0, "char", c);
            return 0;
            break;
        }
    }

    return numBytes;
}


// This method it not very tolerant of communication errors.  If the receiver
// does not send a positive ACK for each packet or does not ACK the packet
// within one second then the transfer will fail.  Unlike in the dial-up
// days of old, this is designed to be run on a 3 foot cable betwee two fast
// hosts, so communication errors or timeouts are extremely unlikely.
boolean XModem::SendFile(uint16_t address, uint32_t fileSize)
{
    uint8_t seq = 1;
    int rxChar = -1;
    uint32_t bytesSent = 0;

    while (rxChar == -1)
    {
        rxChar = GetChar();
    }
    if (rxChar != XMDM_CRC)
    {
        cmdStatus.error("Expected XModem CRC start char.");
        cmdStatus.setValueDec(0, "char", rxChar);
        return false;
    }

    while (bytesSent < fileSize)
    {
        SendPacket(address, seq++);
        address += PKTLEN;

        rxChar = GetChar(5000);
        if (rxChar != XMDM_ACK)
        {
            cmdStatus.error("Expected XModem ACK.");
            cmdStatus.setValueDec(0, "char", rxChar);
            return false;
        }
        bytesSent += PKTLEN;
    }

    Serial.write(XMDM_EOT);
    return true;
}


void XModem::Cancel()
{
    // Send a cancel and then eat input until the line is quiet for 3 seconds.
    Serial.write(XMDM_CAN);
    while (GetChar(3000) != -1)
    {}
}


// Private functions
int XModem::GetChar(int msWaitTime)
{
    do
    {
        if (Serial.available() > 0)
        {
            return Serial.read();
        }
        delay(1);
    } while (msWaitTime--);

    return -1;
}


uint16_t XModem::UpdateCrc(uint16_t crc, uint8_t data)
{
    crc = crc ^ ((uint16_t)data << 8);
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

    return crc;
}


boolean XModem::StartReceive()
{
    for (int retries = 20; (retries); --retries)
    {
        // Send the 'C' character, indicating a CRC16 XMODEM transfer, until the sender
        // of the file responds with something.  The start character will be sent once a
        // second for a number of seconds.  If nothing is received in that time then
        // return false to indicate that the transfer did not start.
        Serial.write('C');
        for (int ms = 1000; (ms); --ms)
        {
            if (Serial.available() > 0)
            {
                return true;
            }
            delay(1);
        }
    }

    return false;
}


boolean XModem::ReceivePacket(uint8_t buffer[], unsigned bufferSize, uint8_t seq, uint16_t destAddr)
{
    int c;
    uint8_t rxSeq1, rxSeq2;
    uint16_t calcCrc = 0;
    uint16_t rxCrc;

    rxSeq1 = (uint8_t) GetChar();
    rxSeq2 = (uint8_t) GetChar();

    for (unsigned ix = 0; (ix < bufferSize); ix++)
    {
        if ((c = GetChar()) < 0)
        {
            // If the read times out then fail this packet.  Note that this check isn't
            // done for the sequence and CRC.  If they timeout then the values won't match
            // so there is not point in the extra code to check for the error.  The worst
            // that will happen is that the transfer will need to wait 3 timeouts before
            // realizing that something is wrong.
            cmdStatus.error("Timeout waiting for next packet char.");
            cmdStatus.setValueDec(0, "seq", seq);
            Serial.write(XMDM_CAN);
            return false;
        }
        buffer[ix] = (uint8_t) c;
        calcCrc = UpdateCrc(calcCrc, buffer[ix]);
    }

    rxCrc  = ((uint16_t) GetChar()) << 8;
    rxCrc |= GetChar();

    if ((calcCrc != rxCrc) || (rxSeq1 != seq) || ((rxSeq1 ^ rxSeq2) != 0xff))
    {
        // Fail if the CRC or sequence number is not correct or if the two received
        // sequence numbers are not the complement of one another.
        cmdStatus.error("Bad CRC or sequence number.");
        cmdStatus.setValueDec(0, "seq", seq);
        Serial.write(XMDM_CAN);
        return false;
    }
    else
    {
        // The data is good.  Process the packet then ACK it to the sender.
        pinMode(13, OUTPUT);
        digitalWrite(13, HIGH);
        if (!pProm->writeData(buffer, bufferSize, destAddr))
        {
            cmdStatus.error("Write failed");
            cmdStatus.setValueHex(0, "address", destAddr);
            return false;
        }
        digitalWrite(13, LOW);

        Serial.write(XMDM_ACK);
    }

    return true;
}


void XModem::SendPacket(uint16_t address, uint8_t seq)
{
    uint16_t crc = 0;

    Serial.write(XMDM_SOH);
    Serial.write(seq);
    Serial.write(~seq);
    for (int ix = 0; (ix < PKTLEN); ix++)
    {
        byte c = pProm->readByte(address++);
        Serial.write(c);
        crc = UpdateCrc(crc, c);
    }
    Serial.write(crc >> 8);
    Serial.write(crc & 0xff);
}



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
        word w = prom.readByte(addr);
        w <<= 8;
        w |= prom.readByte(addr + 1);
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

        byte data = prom.readByte(addr);
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
        values[0] = prom.readByte(addr);
        bool fail = false;
        for (int ix = 1; (ix < SCAN_TESTS); ix++)
        {
            values[ix] = prom.readByte(addr);
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
    byte firstValue = prom.readByte(addr);
    for (int ix = 1; (ix < NUM_TESTS); ix++)
    {
        value = prom.readByte(addr);
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

    delayMicroseconds(10000);
    for (int ix = 0; ix < sizeof(testData); ix++)
    {
        byte val = prom.readByte(start + ix);
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
        Serial.println(F("TommyPROM 1.3\n"));
        Serial.println(F("Valid commands are:"));
        Serial.println(F("  Cssss eeee    - Compute checksum from device"));
        Serial.println(F("  Dssss eeee    - Dump bytes from device to terminal"));
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

