#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "Types.h"
#include "Flash.h"

typedef enum
{
    WAIT_FOR_OPEN_BRACE,
    WAIT_FOR_ENTIRE_CMD,
    WAIT_FOR_CLOSE_BRACE,

} SerialInputState;

typedef enum
{
    FLASH,
    RAM,
    NVRAM,
    RTC

} DeviceType;


typedef void (*TableUpdate)(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable);
typedef void (*TableUpdateAll)(BOOLEAN readEnable, BOOLEAN writeEnable);

typedef struct
{
    DeviceType device;
    const char *str;
    TableUpdate tableUpdateFn;
    TableUpdateAll tableUpdateAllFn;
} CmdUpdate;

const CmdUpdate m_CmdUpdate[] =
{
   {FLASH, "FL", FlashTableUpdate, FlashTableUpdateAll},
};


SerialInputState m_SerInState;
char m_CmdString[10];
UINT_16 m_CmdIndex;

static const UINT_16 CMD_SIZE = 8;
static const UINT_16 NUM_MEMORY_DEVICES = sizeof(m_CmdUpdate)/sizeof(CmdUpdate);


static void ParseValidCommand(void);

void ResetStateMachine(void)
{
    m_SerInState = WAIT_FOR_OPEN_BRACE;
    m_CmdIndex = 0;
    memset(m_CmdString, 0, sizeof(m_CmdString));
}

void ProcessSerialInputChar (char ch)
{
    /* TODO putchar(ch); */

    switch (m_SerInState)
    {
        case WAIT_FOR_OPEN_BRACE:
        default:
            if (ch == '<')
            {
                m_SerInState = WAIT_FOR_ENTIRE_CMD;
            }
            else
            {
                m_CmdIndex = 0;
                ResetStateMachine();
                /* TODO Send Invalid command */
            }
            break;

        case WAIT_FOR_ENTIRE_CMD:
            m_CmdString[m_CmdIndex++] = toupper(ch);
            if (m_CmdIndex >= CMD_SIZE)
            {
                m_SerInState = WAIT_FOR_CLOSE_BRACE;
            }
            break;

        case WAIT_FOR_CLOSE_BRACE:
            if (ch == '>')
            {
                ParseValidCommand();
            }
            else
            {
                /* TODO Send Invalid command */
            }
            ResetStateMachine();
            break;

    }

}

static void ParseValidCommand(void)
{
    UINT_16 offset = 0xFFFF;
    BOOLEAN readEnable = TRUE;
    BOOLEAN writeEnable = TRUE;

    /* Get the on board device to change behavior */
    UINT_16 index;
    for (index = 0; index < NUM_MEMORY_DEVICES; index++)
    {
        if (!strncmp(&m_CmdString[0],  m_CmdUpdate[index].str, 2))
        {
            break;
        }
    }

    if (index >= NUM_MEMORY_DEVICES)
    {
        /* TODO Valid device not found... abort */
        return;
    }

    /* Get the offset */
    if (!strncmp(&m_CmdString[2], "XX", 2))
    {
        /* All are requested */
    }
    else
    {
        char hexNumberStr[3];
        strncpy(hexNumberStr, &m_CmdString[2], 2);
        offset = strtol(hexNumberStr, NULL, 16);
    }

    /* Verify R and W are in the correct spots */
    if (m_CmdString[4] == 'R')
    {
        readEnable = (m_CmdString[5] == '+');
    }
    else
    {
        /* TODO return invalid command */
    }

    if (m_CmdString[6] == 'W')
    {
        writeEnable = (m_CmdString[7] == '+');
    }
    else
    {
        /* TODO return invalid command */
    }

    if (offset == 0xFFFF)
    {
        m_CmdUpdate[index].tableUpdateAllFn(readEnable, writeEnable);
    }
    else
    {
        m_CmdUpdate[index].tableUpdateFn(offset, readEnable, writeEnable);
    }


}
