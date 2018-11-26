/*
 * NVRam.c
 *
 *  Created on: Nov 21, 2018
 *      Author: David Smail
 */
#include "Types.h"

typedef struct
{
    const UINT_16 addrOffset;
    const UINT_16 value;
    BOOLEAN readEnable;
    BOOLEAN writeEnable;
} UpdateTable;

static UpdateTable m_Table[] =
{
 { 0x00, 0x0000, TRUE, TRUE },  { 0x00, 0x0001, TRUE, TRUE },  { 0x00, 0x0002, TRUE, TRUE },  { 0x00, 0x0004, TRUE, TRUE },
 { 0x00, 0x0008, TRUE, TRUE },  { 0x00, 0x0010, TRUE, TRUE },  { 0x00, 0x0020, TRUE, TRUE },  { 0x00, 0x0040, TRUE, TRUE },
 { 0x00, 0x0080, TRUE, TRUE },  { 0x00, 0x0100, TRUE, TRUE },  { 0x00, 0x0200, TRUE, TRUE },  { 0x00, 0x0400, TRUE, TRUE },
 { 0x00, 0x0800, TRUE, TRUE },  { 0x00, 0x1000, TRUE, TRUE },  { 0x00, 0x2000, TRUE, TRUE },  { 0x00, 0x4000, TRUE, TRUE },
 { 0x00, 0x8000, TRUE, TRUE },  { 0x00, 0xFFFF, TRUE, TRUE },  { 0x00, 0xFFFE, TRUE, TRUE },  { 0x00, 0xFFFD, TRUE, TRUE },
 { 0x00, 0xFFFB, TRUE, TRUE },  { 0x00, 0xFFF7, TRUE, TRUE },  { 0x00, 0xFFEF, TRUE, TRUE },  { 0x00, 0xFFDF, TRUE, TRUE },
 { 0x00, 0xFFBF, TRUE, TRUE },  { 0x00, 0xFF7F, TRUE, TRUE },  { 0x00, 0xFEFF, TRUE, TRUE },  { 0x00, 0xFDFF, TRUE, TRUE },
 { 0x00, 0xFBFF, TRUE, TRUE },  { 0x00, 0xF7FF, TRUE, TRUE },  { 0x00, 0xEFFF, TRUE, TRUE },  { 0x00, 0xDFFF, TRUE, TRUE },
 { 0x00, 0xBFFF, TRUE, TRUE },  { 0x00, 0x7FFF, TRUE, TRUE },

};

static const UINT_16 TABLE_SIZE = sizeof(m_Table)/sizeof(UpdateTable);
static UINT_16 m_ReadValue;


#ifdef _WIN32
static UINT_16 debugBaseArray[100];
static UINT_16 *m_BaseAddress = debugBaseArray;
#else
static UINT_16 *m_BaseAddress = (UINT_16 *)0x810000;
#endif


void NVRamService (void)
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

BOOLEAN NVRamTableUpdate(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable)
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


BOOLEAN NVRamTableUpdateAll(BOOLEAN readEnable, BOOLEAN writeEnable)
{
    UINT_16 tableIndex;
    for (tableIndex = 0; tableIndex < TABLE_SIZE; tableIndex++)
    {
        m_Table[tableIndex].readEnable = readEnable;
        m_Table[tableIndex].writeEnable = writeEnable;
    }

    return (TRUE);
}
