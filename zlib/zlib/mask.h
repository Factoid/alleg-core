//////////////////////////////////////////////////////////////////////////////
//
// Bit masks
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _mask_H_
#define _mask_H_

#include <cstdlib>
#include <climits>

//////////////////////////////////////////////////////////////////////////////
//
// This class is a type safe way of dealing with flag words see surface.h
// for example usage.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type, class WordType>
class TBitMask 
{
private:
    WordType m_word;

public: //KGJV protected:
    TBitMask(WordType word) :
        m_word(word)
    {
    }

public:
    TBitMask() :
        m_word(0)
    {
    }

    TBitMask(const TBitMask& bm) :
        m_word(bm.m_word)
    {
    }

    void operator=(const TBitMask& bm)
    {
        m_word = bm.m_word;
    }

    friend TBitMask operator|(const TBitMask& bm1, const TBitMask& bm2)
    {
        return bm1.m_word | bm2.m_word;
    }

    friend bool operator==(const TBitMask& bm1, const TBitMask& bm2)
    {
        return bm1.m_word == bm2.m_word;
    }

    friend bool operator!=(const TBitMask& bm1, const TBitMask& bm2)
    {
        return bm1.m_word != bm2.m_word;
    }

    TBitMask& Set(const TBitMask& bm)
    {
        m_word |= bm.m_word;

        return *this;
    }

    TBitMask& Clear(const TBitMask& bm)
    {
        m_word &= ~bm.m_word;

        return *this;
    }

    TBitMask& Mask(const TBitMask& bm)
    {
        m_word &= bm.m_word;

        return *this;
    }

    bool Test(const TBitMask& bm) const
    {
        return (m_word & bm.m_word) != 0;
    }

    bool Test(const TBitMask& bm, const TBitMask& bmResult) const
    {
        return (m_word & bm.m_word) == bmResult.m_word;
    }

    //
    // Unsafe accessors needed for interface with Win32
    //

    void SetWord(WordType word)
    {
        m_word = word;
    }

    WordType GetWord() const
    {
        return m_word;
    }
};

class BitMask : public TBitMask<DWORD, DWORD> 
{
public:
    BitMask(DWORD dw) : 
        TBitMask<DWORD, DWORD>(dw)
    {
    }

    BitMask() : 
        TBitMask<DWORD, DWORD>(0)
    {
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// This class implements a flag word with more than 32 bits.
//
//////////////////////////////////////////////////////////////////////////////
#ifdef WIN
template<int nBits> class TLargeBitMask
#else
template<unsigned long int nBits> class TLargeBitMask
#endif
{
    public:
        TLargeBitMask(void)
        {
            //ClearAll();
        }

        TLargeBitMask(const BYTE*    bits)
        {
            Set(bits);
        }

        void    Set(const BYTE* bits)
        {
            memcpy(&(m_bits[0]), bits, sizeof(m_bits));
        }

        void    ClearAll(void)
        {
            memset(&(m_bits[0]), 0, sizeof(m_bits));
        }

        void    SetAll(void)
        {
            memset(&(m_bits[0]), 0xff, sizeof(m_bits));
        }

        void    ClearBit(int    i)
        {
            int byte = i >> 3;
            int bit = i & 0x07;

            m_bits[byte] &= (0xff7f >> bit);    //pad the high byte with 1's
        }

        void    SetBit(int      i)
        {
            int byte = i >> 3;
            int bit = i & 0x07;

            m_bits[byte] |= (0x80 >> bit);
        }

        bool    GetBit(int      i) const
        {
            int byte = i >> 3;
            int bit = i & 0x07;

            return 0 != (m_bits[byte] & (0x80 >> bit));
        }

        bool    GetAllZero(void) const
        {
            for (int i = 0; (i < (nBits / 8)); i++)
            {
                if (m_bits[i] != 0)
                    return false;
            }

            return true;
        }
		// CHECK THIS, IT MIGHT OVERFLOW ! -KGJV
#ifdef WIN
        void ToString(char* pszBytes, int cch) const
#else
        void ToString(char* pszBytes, long unsigned int cch) const
#endif
        {
#ifdef WIN
          int cb = min(cch / 2, sizeof(m_bits));
#else
          long unsigned int cb = std::min( cch / 2, sizeof(m_bits) );
#endif
#ifdef WIN
          for (int i = 0; i < cb; ++i)
#else
          for (long unsigned int i = 0; i < cb; ++i)
#endif
          {
            char szByte[3];
            sprintf(szByte, "%02X", m_bits[i]);
#ifdef WIN
            CopyMemory(pszBytes + (i * 2), szByte, 2);
#else
            memcpy( pszBytes + (i*2), szByte, 2);
#endif
          }
        }

        bool FromString(const char* pszBits)
        {
#ifdef WIN
          int cch = strlen(pszBits);
#else
          unsigned long int cch = strlen(pszBits);
#endif
          if (cch % 2)
            return false;
          BYTE bits[sizeof(m_bits)];
          ZeroMemory(bits, sizeof(bits));
#ifdef WIN
          int cb = min(cch / 2, sizeof(m_bits));
#else
          unsigned long int cb = std::min(cch / 2, sizeof(m_bits));
#endif
#ifdef WIN
          for (int i = 0; i < cb; ++i)
#else
          for( unsigned long int i = 0; i < cb; ++i )
#endif
          {
            char szByte[3];
#ifdef WIN
            CopyMemory(szByte, pszBits + (i * 2), 2);
#else
            memcpy( szByte, pszBits + (i*2), 2 );
#endif
            szByte[2] = '\0';
#ifdef WIN
            long nBits = strtoul(szByte, NULL, 16);
            if ((0 == nBits || ULONG_MAX == nBits) && ERANGE == errno)
              return false;
            bits[i] = (BYTE)nBits;
#else
            unsigned long int mBits = strtoul(szByte, NULL, 16);
            if( (0 == mBits || mBits == ULONG_MAX) && ERANGE == errno )
              return false;
            bits[i] = (BYTE)mBits;
#endif
          }

          Set(bits);
          return true;
        }

        TLargeBitMask&   operator = (const TLargeBitMask& tbm)
        {
            memcpy(m_bits, tbm.m_bits, sizeof(m_bits));

            return *this;
        }

        TLargeBitMask&   operator |= (const TLargeBitMask& tbm)
        {
            for (int i = 0; (i < (nBits / 8)); i++)
                m_bits[i] |= tbm.m_bits[i];

            return *this;
        }

        TLargeBitMask    operator | (const TLargeBitMask& tbm) const
        {
            TLargeBitMask<nBits> r;
            for (int i = 0; (i < (nBits / 8)); i++)
                r.m_bits[i] = m_bits[i] | tbm.m_bits[i];

            return r;
        }

        TLargeBitMask&   operator &= (const TLargeBitMask& tbm)
        {
            for (int i = 0; (i < (nBits / 8)); i++)
                m_bits[i] &= tbm.m_bits[i];

            return *this;
        }

        TLargeBitMask    operator & (const TLargeBitMask& tbm) const
        {
            TLargeBitMask<nBits> r;
            for (int i = 0; (i < (nBits / 8)); i++)
                r.m_bits[i] = m_bits[i] & tbm.m_bits[i];

            return r;
        }

        bool        operator == (const TLargeBitMask&    tbm) const
        {
            bool    rc = true;
            for (int i = 0; (i < (nBits / 8)); i++)
            {
                if (m_bits[i] != tbm.m_bits[i])
                {
                    rc = false;
                    break;
                }
            }

            return rc;
        }

        bool        operator != (const TLargeBitMask&    tbm) const
        {
            bool    rc = true;
            for (int i = 0; (i < (nBits / 8)); i++)
            {
                if (m_bits[i] != tbm.m_bits[i])
                {
                    rc = false;
                    break;
                }
            }

            return !rc;
        }

        //Subset
        bool        operator <= (const TLargeBitMask& tbm) const
        {
            bool    rc = true;
            for (int i = 0; (i < (nBits / 8)); i++)
            {
                if ((m_bits[i] & tbm.m_bits[i]) != m_bits[i])
                {
                    rc = false;
                    break;
                }
            }

            return rc;
        }

        //Superset
        bool        operator >= (const TLargeBitMask& tbm) const
        {
            bool    rc = true;
            for (int i = 0; (i < (nBits / 8)); i++)
            {
                if ((m_bits[i] & tbm.m_bits[i]) != tbm.m_bits[i])
                {
                    rc = false;
                    break;
                }
            }

            return rc;
        }

        //"Difference": A - B == A & ~B
        //e.g. remove all the bits in B from A
        TLargeBitMask&   operator -= (const TLargeBitMask& tbm)
        {
            for (int i = 0; (i < (nBits / 8)); i++)
                m_bits[i] &= ~tbm.m_bits[i];

            return *this;
        }

        TLargeBitMask    operator - (const TLargeBitMask& tbm) const
        {
            TLargeBitMask<nBits> r;
            for (int i = 0; (i < (nBits / 8)); i++)
                r.m_bits[i] = m_bits[i] & ~tbm.m_bits[i];

            return r;
        }

    private:
        BYTE    m_bits[((nBits - 1) / 8) + 1];

};

#endif
