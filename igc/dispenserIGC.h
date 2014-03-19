/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	dispenserIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CdispenserIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// dispenserIGC.h : Declaration of the CdispenserIGC

#ifndef __DISPENSERIGC_H_
#define __DISPENSERIGC_H_

/////////////////////////////////////////////////////////////////////////////
// CdispenserIGC
class CdispenserIGC : public IdispenserIGC
{
    public:
        CdispenserIGC(void);
        ~CdispenserIGC(void);

// IbaseIGC
        virtual HRESULT         Initialize(ImissionIGC* pMission, Time now, const void* data, unsigned int dataSize);
        virtual void            Terminate(void);
        virtual void            Update(Time now);

        virtual ObjectType      GetObjectType(void) const
        {
            return OT_dispenser;
        }

        virtual ImissionIGC*    GetMission(void) const
        {
            return m_pMission;
        }

// IpartIGC
        virtual EquipmentType    GetEquipmentType(void) const
        {
            return m_expendableType->GetEquipmentType();
        }

        virtual IpartTypeIGC*    GetPartType(void) const
        {
            return m_partType;
        }

        virtual IshipIGC*        GetShip(void) const
        {
            return m_ship;
        }
        virtual void             SetShip(IshipIGC* newVal, Mount mount);

        virtual Mount            GetMountID(void) const
        {
            return m_mountID;
        }
        virtual void             SetMountID(Mount newVal);

        virtual bool             fActive(void) const
        {
            return m_mountID == 0;
        }

        virtual void             Activate(void)
        {
            m_ship->ChangeSignature(m_expendableType->GetLauncherDef()->signature);
#ifdef WIN
            m_timeLoaded = m_ship->GetLastUpdate() + ((1.0f/m_pMission->GetFloatConstant(c_fcidMountRate)) + m_expendableType->GetLoadTime());
#else
            m_timeLoaded = m_ship->GetLastUpdate() + Seconds(1.0f/m_pMission->GetFloatConstant(c_fcidMountRate) + m_expendableType->GetLoadTime());
#endif
        }
        virtual void             Deactivate(void)
        {
            m_ship->ChangeSignature(-m_expendableType->GetLauncherDef()->signature);
        }
        virtual float               GetMass(void) const
        {
            return m_amount * m_expendableType->GetMass();
        }

        virtual Money            GetPrice(void) const
        {
            return ((Money)m_amount) * m_expendableType->GetLauncherDef()->price;
        }

        virtual float           GetMountedFraction(void) const
        {
            return m_mountedFraction;
        }

        virtual void           SetMountedFraction(float f)
        {
            m_mountedFraction = f;
        }

        virtual void            Arm(void)
        {
            m_mountedFraction = 1.0f;
            m_timeLoaded = m_ship->GetLastUpdate();
        }

    // IlauncherIGC
        virtual void                SetAmount(short a)
        {
            {
                short   maxAmount = m_partType->GetAmount(m_ship);
                if (a > maxAmount)
                    a = maxAmount;
            }

            if (a != m_amount)
            {
                //Adjust the mass of the ship (if there is one) to account for the change in the number of mines
                if (m_ship)
                {
                    m_ship->SetMass(m_ship->GetMass() +
                                    (a - m_amount) * m_expendableType->GetMass());

                    m_pMission->GetIgcSite()->LoadoutChangeEvent(m_ship, this, c_lcQuantityChange);
                }

                m_amount = a;
            }
        }
        virtual short               GetAmount(void) const
        {
            return m_amount;
        }

        virtual void                SetTimeFired(Time   now)
        {
#ifdef WIN
            m_timeLoaded = now + m_expendableType->GetLoadTime();
#else
            m_timeLoaded = now + Seconds(m_expendableType->GetLoadTime());
#endif
        }

        virtual Time            GetTimeLoaded(void) const
        {
            return m_timeLoaded;
        }
        virtual void            SetTimeLoaded(Time  tl)
        {
            m_timeLoaded = tl;
        }
        virtual void            ResetTimeLoaded(void)
        {
            assert (m_ship);
#ifdef WIN
            m_timeLoaded = m_ship->GetLastUpdate() + ((1.0f/m_pMission->GetFloatConstant(c_fcidMountRate)) + m_expendableType->GetLoadTime());
#else
            m_timeLoaded = m_ship->GetLastUpdate() + Seconds((1.0f/m_pMission->GetFloatConstant(c_fcidMountRate)) + m_expendableType->GetLoadTime());
#endif
        }

        virtual const Vector&        GetEmissionPt(void) const
        {
            return m_emissionPt;
        }


    // IdispenserIGC
        virtual IexpendableTypeIGC*    GetExpendableType(void) const
        {
            return m_expendableType;
        }
        virtual float               GetArmedFraction(void) 
        {
            if (GetMountedFraction() < 1.0f)
            {
                return 0;
            }
            else
            {
#ifdef WIN
                return 1.0f - max(0.0f, (m_timeLoaded - Time::Now()) / m_expendableType->GetLoadTime());
#else
                return 1.0f - std::max(0.0f, Seconds(m_timeLoaded - Clock::now()).count() / m_expendableType->GetLoadTime());
#endif
            }
        }

    private:
        ImissionIGC*                    m_pMission;
        IlauncherTypeIGC*               m_partType;
        IexpendableTypeIGC*             m_expendableType;
        IshipIGC*                       m_ship;

        Vector                          m_emissionPt;
        float                           m_mountedFraction;

        Time                            m_timeLoaded;

        short                           m_amount;

        Mount                           m_mountID;
};

#endif //__DISPENSERIGC_H_
