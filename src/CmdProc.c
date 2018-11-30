#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "Types.h"
#include "CmdProc.h"
#include "SerComm.h"
#include "Flash.h"
#include "Ram.h"
#include "NVRam.h"
#include "RTC.h"
#include "PeekPoke.h"

#define MAX_PARAMS          5
#define MAX_CMD_SIZE        55

typedef enum
{
	WAIT_FOR_OPEN_BRACE, WAIT_FOR_ENTIRE_CMD
} SerialInputState;

typedef BOOLEAN(*CmdUpdateFnPtr) (char cmdPtr[][MAX_PARAM_LENGTH]);
typedef void(*ServiceFnPtr) (const char *str);

typedef struct
{
	const char *str;
	const CmdUpdateFnPtr cmdUpdateFnPtr;
	const ServiceFnPtr serviceFnPtr;
} CmdUpdate;

const CmdUpdate m_CmdUpdate[] =
{
	{ "FLA", FlashTableUpdate, FlashService },
	  { "RAM", RamTableUpdate, RamService },
	  { "NVR", NVRamTableUpdate, NVRamService },
	  { "RTC", RTCTableUpdate, RTCService },
      { "PES", PeekSingle, PeekSingleService },
      { "POS", PokeSingle, PokeSingleService },
//      { "PEC", PeekContinuous, NULL},
//      { "PEC", PokeContinuous, NULL},
//      { "PEK", PeekKill, NULL},
//      { "PEK", PokeKill, NULL},

};

SerialInputState m_SerInState;
UINT_16 m_CmdIndex;

static const char *SOFTWARE_VERSION = "0.0";
static const UINT_16 NUM_VALID_COMMANDS = sizeof(m_CmdUpdate) / sizeof(CmdUpdate);

static char m_CmdString[MAX_CMD_SIZE];
static BOOLEAN m_ContinuousPeekOrPokeEnabled = FALSE;

static void ProcessSerialInputChar(char ch);
static void SendCommandResponse(BOOLEAN validCmdReceived);
static void ParseValidCommand(void);
static char *GetDataString(UINT_32 data, eDataWidth dataWidth);

void ApplicationService(void)
{
	INT_16 debugInChar;

	/* Read serial port to determine if any new characters available */
	debugInChar = SC_GetChar();

	if (debugInChar != EOF)
	{
		ProcessSerialInputChar((char)debugInChar);
	}

	if (!m_ContinuousPeekOrPokeEnabled)
	{
		UINT_16 index;
		for (index = 0; index < NUM_VALID_COMMANDS; index++)
		{
			m_CmdUpdate[index].serviceFnPtr(m_CmdUpdate[index].str);
		}
	}
}

BOOLEAN HexStringToValue(char *ptr, UINT_32 *value)
{
	/* Error detection info received from:
	 * https://stackoverflow.com/questions/26080829/detecting-strtol-failure */

	UINT_32 number = 0;
	char *endptr = NULL;
	number = (UINT_32)strtol(ptr, &endptr, 16);

	if ((ptr == endptr) || (errno == ERANGE && number == LONG_MIN) || (errno == ERANGE && number == LONG_MAX)
		|| (errno == EINVAL) || (errno != 0 && number == 0))
	{
		return (FALSE);
	}

	*value = number;
	return (TRUE);
}

void ResetStateMachine(void)
{
	m_SerInState = WAIT_FOR_OPEN_BRACE;
	m_CmdIndex = 0;
	memset(m_CmdString, 0, sizeof(m_CmdString));
}

void SendTestPassed(const char *str, UINT_32 expectedValue, eDataWidth dataWidth)
{
    char response[50];

    strcpy(response,"<");
    strcat(response, str);
    strcat(response, ",PASS,");

    strcat(response, GetDataString(expectedValue, dataWidth));
    strcat(response, ">");

    SC_PutsAlways(response);
}

void SendMismatchError(const char *str, UINT_32 expectedValue, UINT_32 actualValue, eDataWidth dataWidth)
{
	char response[50];

	strcpy(response,"<");
    strcat(response, str);
    strcat(response, ",FAIL,");

    strcat(response, GetDataString(expectedValue, dataWidth));
    strcat(response, ",");
    strcat(response, GetDataString(actualValue, dataWidth));
    strcat(response, ">");

	SC_PutsAlways(response);
}

void SendAddressDataResponse (const char *str, UINT_32 address, UINT_32 data, eDataWidth dataWidth)
{
    char response[50];

    strcpy(response,"<");
    strcat(response, str);
    strcat(response, ",");
    strcat(response, GetDataString(address, BIT_WIDTH_24));
    strcat(response, ",");
    strcat(response, GetDataString(data, dataWidth));
    strcat(response, ">");

    SC_PutsAlways(response);

}

static void ProcessSerialInputChar(char ch)
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
		if (m_CmdIndex >= MAX_CMD_SIZE)
		{
			SendCommandResponse(FALSE);
			ResetStateMachine();
		}
		else if (ch == '>')
		{
			m_CmdString[m_CmdIndex] = '\0';
			ParseValidCommand();
			ResetStateMachine();
		}
		else
		{
			m_CmdString[m_CmdIndex] = toupper(ch);
			m_CmdIndex++;
		}
		break;
	}
}

static void SendCommandResponse(BOOLEAN validCmdReceived)
{
	char response[10];

	if (validCmdReceived)
	{
		strcpy(response, "<OK,");
	}
	else
	{
		strcpy(response, "<INV,");
	}
	strcat(response, SOFTWARE_VERSION);
	strcat(response, ">");

	SC_Puts(response);
}

static void ParseValidCommand(void)
{
	BOOLEAN valid = FALSE;

	UINT_16 cmd = 0;
	UINT_16 cmdIndex = 0;
	UINT_16 index = 0;

	static char parsedCommands[MAX_PARAMS][MAX_PARAM_LENGTH];

	/* Save all of the command parameter info */
	while (m_CmdString[index] != '\0')
	{
		if (m_CmdString[index] == ',')
		{
			parsedCommands[cmd][cmdIndex] = 0;
			cmdIndex = 0;
			cmd++;
			if (cmd >= MAX_PARAMS)
			{
				/* TODO Issue invalid command; too many params */
				return;
			}
		}
		else
		{
			if (cmdIndex >= MAX_PARAM_LENGTH)
			{
				/* TODO Issue invalid command; param length too long */
				return;
			}
			else
			{
				parsedCommands[cmd][cmdIndex] = m_CmdString[index];
				cmdIndex++;
			}
		}
		index++;
	}

	parsedCommands[cmd][cmdIndex] = '\0';

	/* Get the on board device to change behavior */
	for (index = 0; index < NUM_VALID_COMMANDS; index++)
	{
		if (!strcmp(parsedCommands[0], m_CmdUpdate[index].str))
		{
			break;
		}
	}

	if (index >= NUM_VALID_COMMANDS)
	{
		SendCommandResponse(FALSE);
		return;
	}

	valid = m_CmdUpdate[index].cmdUpdateFnPtr(&parsedCommands[0]);

	SendCommandResponse(valid);
}

/* Needed because sprintf doesn't work on the C167 */
static char *GetDataString(UINT_32 data, eDataWidth dataWidth)
{
    static char value[10];
    UINT_16 i;
    UINT_16 shift;

    /* Clear all of memory so that string will be NULL terminated */
    memset (value, 0, sizeof (value));

    switch (dataWidth)
    {
        case BIT_WIDTH_8:
            shift = 8;
            break;

        case BIT_WIDTH_16:
            shift = 16;
            break;

        case BIT_WIDTH_24:
            shift = 24;
            break;

        case BIT_WIDTH_32:
            shift = 32;
            break;

        default:
            shift = 0;
            break;
    }

    i = 0;
    while (shift != 0)
    {
        shift -= 4;
        /* Convert each nibble, starting with the MS Nibble to ASCII */
        value[i] = '0' + ((data >> shift) & 0xf);
        /* Convert values from A-F */
        if (value[i] > '9')
        {
            value[i] = value[i] + 7;
        }
        i++;
    }

    return (value);
}
