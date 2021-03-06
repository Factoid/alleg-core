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

#ifdef WIN
char UTL::s_artworkPath[MAX_PATH] = "";
#else
DLL_PUBLIC std::string UTL::s_artworkPath;

const std::string& UTL::artworkPath()
{
	return s_artworkPath;
}
#endif

void ZAssertImpl(bool bSucceeded, const char* psz, const char* pszFile, int line, const char* pszModule)
{
  if( !bSucceeded )
  {
    throw "ZAssert Failed";
  }
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

#ifdef WIN
HRESULT UTL::getFile(    const char*    name,
                         const char*    extension,
                     OUT char*          artwork,
                         bool           downloadF,
                         bool           createF)
{
    HRESULT rc = E_FAIL;
    assert (name && extension && artwork && *s_artworkPath);
#ifdef WIN
    strcpy(artwork, s_artworkPath);
#else
    strcpy(artwork, s_artworkPath.c_str());
#endif
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
#else
bool UTL::getFile( const std::string& name, const std::string& ext, std::string& outPath, bool downloadF, bool createF )
{
  outPath = s_artworkPath + name + ext;
  std::ifstream f(outPath);
  if( f.good() )
  {
    f.close();
    return true;
  }
  return false;
}
#endif

#ifdef WIN
void UTL::SetArtPath(const char* szArtwork)
{

  int cbsz = lstrlen(szArtwork);
  assert(cbsz > 0 && cbsz < MAX_PATH);
  bool fTrailingBS = szArtwork[cbsz - 1] == '\\';
  lstrcpy(s_artworkPath, szArtwork);
  if (!fTrailingBS)
  {
    s_artworkPath[cbsz] = '\\';
    s_artworkPath[cbsz + 1] = 0;
  }
}
#else
void UTL::SetArtPath(const std::string& szArtwork)
{
  UTL::s_artworkPath = szArtwork;
}
#endif

