
#include "XModem.h"
#include "CmdStatus.h"

// The original TommyPROM code used XModem CRC protocol but this requires parameters to be
// changed for the host communication program.  The default is now to use basic XModem
// with the 8-bit checksum instead of the 16-bit CRC error checking.  This shouldn't
// matter because communication errors aren't likely to happen on a 3 foot USB cable like
// they did over the long distance dail-up lines that XModem was designed for.  Uncomment
// the XMODEM_CRC_PROTOCOL line below to restore the original XMODEM CRC support.
//#define XMODEM_CRC_PROTOCOL

enum
{
    // XMODEM control characters.
    XMDM_SOH = 0x01,
    XMDM_EOT = 0x04,
    XMDM_ACK = 0x06,
    XMDM_NAK = 0x15,
    XMDM_CAN = 0x18,
    XMDM_ESC = 0x1b,
    XMDM_CRC = 'C',
#ifdef XMODEM_CRC_PROTOCOL
    XMDM_TRANSFER_START = XMDM_CRC
#else
    XMDM_TRANSFER_START = XMDM_NAK
#endif
};

enum
{
    // Misc constants for XMODEM.
    PKTLEN = 128
};

enum { RX_OK, RX_ERROR, RX_IGNORE };

uint32_t XModem::ReceiveFile(uint32_t address)
{
    uint8_t buffer[PKTLEN];
    int c;
    uint8_t seq = 1;
    uint32_t numBytes = 0;
    bool complete = false;
    int status;

#ifdef XMODEM_CRC_PROTOCOL
    Serial.println(F("Send the image file using XMODEM CRC"));
#else
    Serial.println(F("Send the image file using XMODEM"));
#endif
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
            status = ReceivePacket(buffer, PKTLEN, seq, address);
            if (status == RX_OK) {
                // Good packet - advance the buffer
                numBytes += PKTLEN;
                address += PKTLEN;
                ++seq;
            } else if (status == RX_ERROR) {
                return 0;
            }
            // else ignore the duplicate packet
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
            cmdStatus.setValueDec(1, "seq", seq);
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
bool XModem::SendFile(uint32_t address, uint32_t fileSize)
{
    uint8_t seq = 1;
    int rxChar = -1;
    uint32_t bytesSent = 0;

#ifdef XMODEM_CRC_PROTOCOL
    Serial.println(F("Set the terminal to receive XModem CRC"));
#else
    Serial.println(F("Set the terminal to receive XModem"));
#endif
    while (rxChar == -1)
    {
        rxChar = GetChar();
    }
    if (rxChar != XMDM_TRANSFER_START)
    {
#ifdef XMODEM_CRC_PROTOCOL
        cmdStatus.error("Expected XModem CRC start char");
#else
        cmdStatus.error("Expected XModem NAK char to start");
#endif
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

    // Signal end of transfer and process the ACK
    Serial.write(XMDM_EOT);
    rxChar = GetChar(5000);
    if (rxChar != XMDM_ACK)
    {
        cmdStatus.error("Expected XModem ACK to EOT, but received:");
        cmdStatus.setValueDec(0, "char", rxChar);
        return false;
    }

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
    msWaitTime *= 4;
    do
    {
        if (Serial.available() > 0)
        {
            return Serial.read();
        }
        delayMicroseconds(250);
    } while (msWaitTime--);

    return -1;
}


uint16_t XModem::UpdateCrc(uint16_t crc, uint8_t data)
{
#ifdef XMODEM_CRC_PROTOCOL
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
#else
    crc = (crc + data) & 0xff;
#endif
    return crc;
}


bool XModem::StartReceive()
{
    delay(2000);
    for (int retries = 20; (retries); --retries)
    {
        // Send the XMDM_TRANSFER_START until the sender of the file responds with
        // something.  The start character will be sent once a second for a number of
        // seconds.  If nothing is received in that time then return false to indicate
        // that the transfer did not start.
        ++promDevice.debugRxStarts;
        Serial.write(XMDM_TRANSFER_START);
        for (unsigned ms = 30000; (ms); --ms)
        {
            delayMicroseconds(100);
            if (Serial.available() > 0)
            {
                return true;
            }
        }
    }

    return false;
}


int XModem::ReceivePacket(uint8_t buffer[], unsigned bufferSize, uint8_t seq, uint32_t destAddr)
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
            return RX_ERROR;
        }
        buffer[ix] = (uint8_t) c;
        calcCrc = UpdateCrc(calcCrc, buffer[ix]);
    }

#ifdef XMODEM_CRC_PROTOCOL
    rxCrc  = ((uint16_t) GetChar()) << 8;
    rxCrc |= GetChar();
#else
    rxCrc = GetChar();
#endif

    // At this point in the code, there should not be anything in the receive buffer
    // because the sender has just sent a complete packet and is waiting on a response. If
    // the XModem transfer is not started quickly on the host side, TeraTerm seems to
    // buffer the NAK character that starts the transfer and use it as an indication that
    // the first packet should be sent again.  That can cause the Arduino's 64 byte buffer
    // to overflow and gets the transfer out of sync.  The normal processing will detect
    // the first packet and will process it correctly, but the partial packet in the
    // buffer will cause the processing of the next packet to fail.
    delay(10);
    if (Serial.available()) {
        ++promDevice.debugRxSyncErrors;
        if (seq == 1) {
            // If any characters are in the buffer after the first packet, quietly eat
            // them to get back on a packet boundary and send a NAK to cause a retransmit
            // of the packet.
            while (Serial.available() > 0) {
                delay(1);
                Serial.read();
                ++promDevice.debugExtraChars;
            }
            delay(1);
            Serial.write(XMDM_NAK);
            return RX_IGNORE;
        } else {
            // Sync issues shouldn't happen at any other time so fail the transfer.
            cmdStatus.error("RX sync error.");
            cmdStatus.setValueDec(0, "seq", seq);
            cmdStatus.setValueDec(1, "rxSeq1", rxSeq1);
            cmdStatus.setValueDec(2, "rxSeq2", rxSeq2);
            cmdStatus.setValueHex(3, "calcCrc", calcCrc);
            cmdStatus.setValueHex(4, "rxCrc", rxCrc);
            Serial.write(XMDM_CAN);
            return RX_ERROR;
        }
    }

    if ((calcCrc == rxCrc) && (rxSeq1 == seq - 1) && ((rxSeq1 ^ rxSeq2) == 0xff))
    {
        // Resend of previously processed packet.
        delay(10);
        ++promDevice.debugRxDuplicates;
        Serial.write(XMDM_ACK);
        return RX_IGNORE;
    }
    else if ((calcCrc != rxCrc) || (rxSeq1 != seq) || ((rxSeq1 ^ rxSeq2) != 0xff))
    {
        // Fail if the CRC or sequence number is not correct or if the two received
        // sequence numbers are not the complement of one another.
        cmdStatus.error("Bad CRC or sequence number.");
        cmdStatus.setValueDec(0, "seq", seq);
        cmdStatus.setValueDec(1, "rxSeq1", rxSeq1);
        cmdStatus.setValueDec(2, "rxSeq2", rxSeq2);
        cmdStatus.setValueHex(3, "calcCrc", calcCrc);
        cmdStatus.setValueHex(4, "rxCrc", rxCrc);
        Serial.write(XMDM_CAN);
        return RX_ERROR;
    }
    else
    {
        // The data is good.  Process the packet then ACK it to the sender.
        if (!promDevice.writeData(buffer, bufferSize, destAddr))
        {
            cmdStatus.error("Write failed");
            cmdStatus.setValueHex(0, "address", destAddr);
            return RX_ERROR;
        }
        Serial.write(XMDM_ACK);
    }

    return RX_OK;
}


void XModem::SendPacket(uint32_t address, uint8_t seq)
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

#ifdef XMODEM_CRC_PROTOCOL
    Serial.write(crc >> 8);
#endif
    Serial.write(crc & 0xff);
}
