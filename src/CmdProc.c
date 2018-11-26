#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "Types.h"
#include "SerComm.h"
#include "Flash.h"
#include "Ram.h"
#include "NVRam.h"
#include "RTC.h"

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


typedef BOOLEAN (*TableUpdate)(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable);
typedef BOOLEAN (*TableUpdateAll)(BOOLEAN readEnable, BOOLEAN writeEnable);

typedef struct
{
    DeviceType device;
    const char *str;
    TableUpdate tableUpdateFn;
    TableUpdateAll tableUpdateAllFn;
} CmdUpdate;

const CmdUpdate m_CmdUpdate[] =
{
   {FLASH, "FL", FlashTableUpdate, FlashTableUpdateAll },
   {RAM,   "RA", RamTableUpdate,   RamTableUpdateAll   },
   {NVRAM, "NV", NVRamTableUpdate, NVRamTableUpdateAll },
   {RTC,   "RT", RTCTableUpdate,   RTCTableUpdateAll   },
};


SerialInputState m_SerInState;
char m_CmdString[10];
UINT_16 m_CmdIndex;

static const UINT_16 CMD_SIZE = 8;
static const UINT_16 NUM_MEMORY_DEVICES = sizeof(m_CmdUpdate)/sizeof(CmdUpdate);

static const char *SOFTWARE_VERSION = "0.0";


static void SendCommandResponse(BOOLEAN validCmdReceived);
static void ParseValidCommand(void);

void ResetStateMachine(void)
{
    m_SerInState = WAIT_FOR_OPEN_BRACE;
    m_CmdIndex = 0;
    memset(m_CmdString, 0, sizeof(m_CmdString));
}

void ProcessSerialInputChar (char ch)
{
    SC_PutChar(ch);

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
                SendCommandResponse(FALSE);
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
                SendCommandResponse(FALSE);
            }
            ResetStateMachine();
            break;

    }

}

static void SendCommandResponse(BOOLEAN validCmdReceived)
{
    char response[10];

    if (validCmdReceived)
    {
        strcpy(response, "<OK");
    }
    else
    {
        strcpy(response, "<INV");
    }
    strcat(response, SOFTWARE_VERSION);
    strcat(response, ">");

    SC_Puts(response);
}



static void ParseValidCommand(void)
{
    UINT_16 offset = 0xFFFF;
    BOOLEAN readEnable = TRUE;
    BOOLEAN writeEnable = TRUE;
    BOOLEAN valid;

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
        SendCommandResponse(FALSE);
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
        SendCommandResponse(FALSE);
        return;
    }

    if (m_CmdString[6] == 'W')
    {
        writeEnable = (m_CmdString[7] == '+');
    }
    else
    {
        SendCommandResponse(FALSE);
        return;
    }

    if (offset == 0xFFFF)
    {
        valid = m_CmdUpdate[index].tableUpdateAllFn(readEnable, writeEnable);
    }
    else
    {
        valid = m_CmdUpdate[index].tableUpdateFn(offset, readEnable, writeEnable);
    }

    SendCommandResponse(valid);


}
