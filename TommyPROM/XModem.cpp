#include "XModem.h"
#include "CmdStatus.h"

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
bool XModem::SendFile(uint16_t address, uint32_t fileSize)
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


bool XModem::StartReceive()
{
    for (int retries = 30; (retries); --retries)
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


bool XModem::ReceivePacket(uint8_t buffer[], unsigned bufferSize, uint8_t seq, uint16_t destAddr)
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
        if (!promDevice.writeData(buffer, bufferSize, destAddr))
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
        byte c = promDevice.readData(address++);
        Serial.write(c);
        crc = UpdateCrc(crc, c);
    }
    Serial.write(crc >> 8);
    Serial.write(crc & 0xff);
}



