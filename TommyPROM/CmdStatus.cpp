#include "Arduino.h"
#include "CmdStatus.h"

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

void CmdStatus::info(const char * msg)
{
    level = SL_INFO;
    message = msg;
}

void CmdStatus::error(const char * msg)
{
    level = SL_ERROR;
    message = msg;
}

void CmdStatus::setValueDec(int index, const char * label, long value)
{
    setLongValue(index, label, value, VT_DEC);
}

void CmdStatus::setValueHex(int index, const char * label, long value)
{
    setLongValue(index, label, value, VT_HEX);
}

void CmdStatus::setLongValue(int index, const char * label, long value, ValueType vt)
{
    if ((index >= 0) && (index < MAX_VALUES))
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


