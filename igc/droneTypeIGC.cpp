/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	droneTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CdroneTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// droneTypeIGC.cpp : Implementation of CdroneTypeIGC
#include "pch.h"
#include "droneTypeIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CdroneTypeIGC
HRESULT         CdroneTypeIGC::Initialize(ImissionIGC*  pMission,
                                         Time           now,
                                         const void*    data,
                                         unsigned int dataSize)
{
    assert (pMission);
    m_pMission = pMission;

	ZRetailAssert (data && (dataSize == sizeof(DataDroneTypeIGC)));

    memcpy(&m_data, data, dataSize);
#ifdef WIN
    m_petLaid = (m_data.etidLaid == NA) ? NULL : pMission->GetExpendableType(m_data.etidLaid);
#else 
    m_petLaid.reset( (m_data.etidLaid == NA) ? nullptr : pMission->GetExpendableType(m_data.etidLaid) );
#endif

	// KGJV - fix for debug mode - to remove later
    //assert ((m_petLaid != NULL) == (m_data.pilotType == c_ptLayer));

    pMission->AddDroneType(this);

    return S_OK;
}

void            CdroneTypeIGC::Terminate(void)
{
    assert (m_pMission);

    m_pMission->DeleteDroneType(this);
}

int             CdroneTypeIGC::Export(void*  data) const
{
    if (data)
        *((DataDroneTypeIGC*)data) = m_data;

    return sizeof(DataDroneTypeIGC);
}

