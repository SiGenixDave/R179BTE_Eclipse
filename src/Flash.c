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
    BOOLEAN readEnable;
} UpdateTable;

static UpdateTable m_Table[] =
{
 { 0x00, TRUE },  { 0x01, TRUE },  { 0x02, TRUE },  { 0x03, TRUE },
 { 0x04, TRUE },  { 0x05, TRUE },  { 0x06, TRUE },  { 0x07, TRUE },
 { 0x08, TRUE },  { 0x09, TRUE },  { 0x0A, TRUE },  { 0x0B, TRUE },
 { 0x0C, TRUE },  { 0x0D, TRUE },  { 0x0E, TRUE },  { 0x0F, TRUE },
 { 0x10, TRUE },  { 0x11, TRUE },  { 0x12, TRUE },  { 0x13, TRUE },
 { 0x14, TRUE },  { 0x15, TRUE },  { 0x16, TRUE },  { 0x17, TRUE },
 { 0x18, TRUE },  { 0x19, TRUE },  { 0x1A, TRUE },  { 0x1B, TRUE },
 { 0x1C, TRUE },  { 0x1D, TRUE },  { 0x1E, TRUE },  { 0x1F, TRUE },
 { 0x20, TRUE },  { 0x21, TRUE },
};

static const UINT_16 TABLE_SIZE = sizeof(m_Table)/sizeof(UpdateTable);
static UINT_16 m_FlashReadValue;


#ifdef _WIN32
static UINT_16 debugBaseArray[100];
static UINT_16 *m_BaseAddress = debugBaseArray;
#else
static UINT_16 *m_BaseAddress = (UINT_16 *)0x120000;
#endif


void FlashService (void)
{
    UINT_16 index;
    for (index = 0; index < TABLE_SIZE; index++)
    {
        if (m_Table[index].readEnable)
        {
            m_FlashReadValue = m_BaseAddress[m_Table[index].addrOffset];
        }
    }
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
