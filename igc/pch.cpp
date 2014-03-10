/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	stdafx.cpp
**
**  Author: 
**
**  Description:
**      The standard ATL stdafx file. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "pch.h"
#include <fstream>

char UTL::s_artworkPath[MAX_PATH] = "";
const Rotation c_rotationZero(0.0f, 0.0f, 1.0f, 0.0f);

void ZAssertImpl(bool bSucceeded, const char* psz, const char* pszFile, int line, const char* pszModule)
{
  throw "Not Implemented";
}

Rotation::Rotation(void)
:
    m_angle(0.0f)
{
    m_axis.x = m_axis.y = 0.0f;
    m_axis.z = -1.0;
}

Rotation::Rotation(const Rotation&  r)
:
    m_angle(r.m_angle)
{
    m_axis = r.m_axis;
}

void    Rotation::reset(void)
{
    m_angle = m_axis.x = m_axis.y = 0.0f;
    m_axis.z = -1.0;
}

void    Rotation::set(const Vector&  axis, float angle)
{
    m_axis = axis;
    m_angle = angle;
}

Rotation&   Rotation::operator = (const Rotation& r)
{
    m_axis = r.m_axis;
    m_angle = r.m_angle;
    return *this;
}

void            Rotation::axis(const Vector&    a)
{
    m_axis = a;
}

void            Rotation::angle(float a)
{
    m_angle = a;
}

void            Rotation::x(float t)
{
    m_axis.x = t;
}

void            Rotation::y(float t)
{
    m_axis.y = t;
}

void            Rotation::z(float t)
{
    m_axis.z = t;
}

void UTL::putName(char* name, const char* newVal)
{
    assert (name);

    if (newVal)
    {
        strncpy(name, newVal, c_cbName - 1);
        name[c_cbName - 1] = '\0';
    }
    else
    {
        name[0] = '\0';
    }
}

HRESULT UTL::getFile(    const char*    name,
                         const char*    extension,
                     OUT char*          artwork,
                         bool           downloadF,
                         bool           createF)
{
    HRESULT rc = E_FAIL;
    assert (name && extension && artwork && *s_artworkPath);
    
    strcpy(artwork, s_artworkPath);
    {
        char*       pArtwork = artwork + strlen(artwork);
        const char* pName = name;

        while ((*pName != '\0') && (*pName != ' '))
            *(pArtwork++) = *(pName++);

        strcpy(pArtwork, extension);
    }

#ifdef WIN
    OFSTRUCT filedata;
    filedata.cBytes = sizeof(filedata);
    if (OpenFile(artwork, &filedata, OF_EXIST) != HFILE_ERROR)
        rc = S_OK;
    else
        debugf("Unable to open %s%s\n", name, extension);
#else
    std::ifstream f(artwork);
    if( f.good() ) {
      f.close();
      rc = S_OK;
    }
#endif

    return rc;
}
