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

const Rotation c_rotationZero(0.0f, 0.0f, 1.0f, 0.0f);

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

