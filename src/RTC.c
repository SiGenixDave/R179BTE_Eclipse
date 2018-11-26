/*
 * RTC.c
 *
 *  Created on: Nov 21, 2018
 *      Author: David Smail
 */
#include "Types.h"

typedef struct
{
    const UINT_16 addrOffset;
    const UINT_8 value;
    BOOLEAN readEnable;
    BOOLEAN writeEnable;
} UpdateTable;

static UpdateTable m_Table[] =
{
 { 0x00, 0x00, TRUE, TRUE },  { 0x00, 0x01, TRUE, TRUE },  { 0x00, 0x02, TRUE, TRUE },  { 0x00, 0x04, TRUE, TRUE },
 { 0x00, 0x08, TRUE, TRUE },  { 0x00, 0x10, TRUE, TRUE },  { 0x00, 0x20, TRUE, TRUE },  { 0x00, 0x40, TRUE, TRUE },
 { 0x00, 0x80, TRUE, TRUE },  { 0x00, 0xFF, TRUE, TRUE },  { 0x00, 0xFE, TRUE, TRUE },  { 0x00, 0xFD, TRUE, TRUE },
 { 0x00, 0xFB, TRUE, TRUE },  { 0x00, 0xF7, TRUE, TRUE },  { 0x00, 0xEF, TRUE, TRUE },  { 0x00, 0xDF, TRUE, TRUE },
 { 0x00, 0xBF, TRUE, TRUE },  { 0x00, 0x7F, TRUE, TRUE },

};

static const UINT_16 TABLE_SIZE = sizeof(m_Table)/sizeof(UpdateTable);
static UINT_16 m_ReadValue;


#ifdef _WIN32
static UINT_8 debugBaseArray[100];
static UINT_8 *m_BaseAddress = debugBaseArray;
#else
static UINT_8 *m_BaseAddress = (UINT_16 *)0x301000;
#endif


void RTCService (void)
{
    UINT_16 tableIndex;
    for (tableIndex = 0; tableIndex < TABLE_SIZE; tableIndex++)
    {
        if (m_Table[tableIndex].writeEnable)
        {
            m_BaseAddress[m_Table[tableIndex].addrOffset] = m_Table[tableIndex].value;
        }
        if (m_Table[tableIndex].readEnable)
        {
            m_ReadValue = m_BaseAddress[m_Table[tableIndex].addrOffset];
        }
    }
}

BOOLEAN RTCTableUpdate(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable)
{
    BOOLEAN valid = FALSE;

    if (tableIndex < TABLE_SIZE)
    {
        m_Table[tableIndex].readEnable = readEnable;
        m_Table[tableIndex].writeEnable = writeEnable;
        valid = TRUE;
    }

    return (valid);

}


BOOLEAN RTCTableUpdateAll(BOOLEAN readEnable, BOOLEAN writeEnable)
{
    UINT_16 tableIndex;

    for (tableIndex = 0; tableIndex < TABLE_SIZE; tableIndex++)
    {
        m_Table[tableIndex].readEnable = readEnable;
        m_Table[tableIndex].writeEnable = writeEnable;
    }

    return (TRUE);
}


