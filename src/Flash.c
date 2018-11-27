/*
 * Flash.c
 *
 *  Created on: Nov 21, 2018
 *      Author: David Smail
 */
#include "Types.h"

typedef struct
{
    const UINT_16 addrOffset;
    const UINT_16 expectedValue;
    BOOLEAN readEnable;
} UpdateTable;

static UpdateTable m_Table[] =
{
 { 0x00, 0x0000, FALSE },  { 0x01, 0x0001, FALSE },  { 0x02, 0x0002, FALSE },  { 0x03, 0x0004, FALSE },
 { 0x04, 0x0008, FALSE },  { 0x05, 0x0010, FALSE },  { 0x06, 0x0020, FALSE },  { 0x07, 0x0040, FALSE },
 { 0x08, 0x0080, FALSE },  { 0x09, 0x0100, FALSE },  { 0x0A, 0x0200, FALSE },  { 0x0B, 0x0400, FALSE },
 { 0x0C, 0x0800, FALSE },  { 0x0D, 0x1000, FALSE },  { 0x0E, 0x2000, FALSE },  { 0x0F, 0x4000, FALSE },
 { 0x10, 0x8000, FALSE },  { 0x11, 0xFFFF, FALSE },  { 0x12, 0xFFFE, FALSE },  { 0x13, 0xFFFD, FALSE },
 { 0x14, 0xFFFB, FALSE },  { 0x15, 0xFFF7, FALSE },  { 0x16, 0xFFEF, FALSE },  { 0x17, 0xFFDF, FALSE },
 { 0x18, 0xFFBF, FALSE },  { 0x19, 0xFF7F, FALSE },  { 0x1A, 0xFEFF, FALSE },  { 0x1B, 0xFDFF, FALSE },
 { 0x1C, 0xFBFF, FALSE },  { 0x1D, 0xF7FF, FALSE },  { 0x1E, 0xEFFF, FALSE },  { 0x1F, 0xDFFF, FALSE },
 { 0x20, 0xBFFF, FALSE },  { 0x21, 0x7FFF, FALSE },
};

static const UINT_16 TABLE_SIZE = sizeof(m_Table)/sizeof(UpdateTable);
static UINT_16 m_FlashReadValue;


#ifdef _WIN32
static UINT_16 debugBaseArray[100];
static UINT_16 *m_BaseAddress = debugBaseArray;
#else
static UINT_16 *m_BaseAddress = (UINT_16 *)0x120000;
#endif


BOOLEAN FlashService (INT_32 *expectedValue, INT_32 *actualValue)
{
    UINT_16 index;
    for (index = 0; index < TABLE_SIZE; index++)
    {
        if (m_Table[index].readEnable)
        {
            m_Table[index].readEnable = FALSE;
            m_FlashReadValue = m_BaseAddress[m_Table[index].addrOffset];
            if (m_FlashReadValue != m_Table[index].expectedValue)
            {
                *expectedValue = m_Table[index].expectedValue;
                *actualValue = m_FlashReadValue;
                return (FALSE);
            }
        }
    }

    return (TRUE);
}

BOOLEAN FlashTableUpdate(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable)
{
    BOOLEAN valid = FALSE;

    if (tableIndex < TABLE_SIZE)
    {
        m_Table[tableIndex].readEnable = readEnable;
        valid = TRUE;
    }

    return (valid);
}

BOOLEAN FlashTableUpdateAll(BOOLEAN readEnable, BOOLEAN writeEnable)
{
    UINT_16 index;
    for (index = 0; index < TABLE_SIZE; index++)
    {
        m_Table[index].readEnable = readEnable;
    }

    return (TRUE);
}
