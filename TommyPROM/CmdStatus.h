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
    void info(const char * msg);
    void error(const char * msg);
    void setValueDec(int index, const char * label, long value);
    void setValueHex(int index, const char * label, long value);

    bool isClear();
    void printStatus();

private:
    enum
    {
        MAX_VALUES = 5
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

    void setLongValue(int index, const char * label, long value, ValueType vt);
};
