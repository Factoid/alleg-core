//////////////////////////////////////////////////////////////////////////////
//
// 2D Points
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _Point_H_
#define _Point_H_

//////////////////////////////////////////////////////////////////////////////
//
// floating point Points
//
//////////////////////////////////////////////////////////////////////////////
class PointCore {
protected:
    typedef float Number;

private:
    Number m_x;
    Number m_y;

public:
    PointCore() {}

    PointCore(Number x, Number y) :
        m_x(x),
        m_y(y)
    {}

    Number X() const { return m_x; }
    Number Y() const { return m_y; }

    void SetX(Number x) { m_x = x; }
    void SetY(Number y) { m_y = y; }

    operator POINT()
    {
        POINT pt;

        pt.x = (int)m_x;
        pt.y = (int)m_y;

        return pt;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Template Points
//
//////////////////////////////////////////////////////////////////////////////

template <class PointType>
class TPoint : public PointType {
public:
    TPoint() {}

    TPoint(VSNET_TNFIX PointType::Number x, VSNET_TNFIX PointType::Number y) :
        PointType(x, y)
    {}

    static TPoint GetZero() { return TPoint(0, 0); }

    template<class Type>
    static TPoint Cast(const TPoint<Type>& point)
    {
        return
            TPoint(
                (PointType::Number)point.X(),
                (PointType::Number)point.Y()
            );
    }

    TPoint& operator+=(const TPoint& pt) {
        SetX(X() + pt.X());
        SetY(Y() + pt.Y());
        return *this;
    }

    //  , there should really be a Vector2 class

    VSNET_TNFIX PointType::Number LengthSquared() const
    {
        return X() * X() + Y() * Y();
    }

    VSNET_TNFIX PointType::Number Length() const
    {
        if (LengthSquared() == 1) {
            return 1;
        } else {
            return (PointType::Number)sqrt((float)LengthSquared());
        }
    }

    ZString GetString() const
    {
        return ZString("(") + ZString(X()) + ", " + ZString(Y()) + ")";
    }

    friend TPoint operator*(VSNET_TNFIX PointType::Number value, const TPoint& p1)
    {
        return TPoint(p1.X() * value, p1.Y() * value);
    }

    friend TPoint operator*(const TPoint& p1, VSNET_TNFIX PointType::Number value)
    {
        return TPoint(p1.X() * value, p1.Y() * value);
    }

    friend TPoint operator/(const TPoint& p1, VSNET_TNFIX PointType::Number value)
    {
        return TPoint(p1.X() / value, p1.Y() / value);
    }

    friend TPoint operator+(const TPoint& p1, const TPoint& p2)
    {
        return TPoint(p1.X() + p2.X(), p1.Y() + p2.Y());
    }

    friend TPoint operator-(const TPoint& p1)
    {
        return TPoint(-p1.X(), -p1.Y());
    }

    friend TPoint operator-(const TPoint& p1, const TPoint& p2)
    {
        return TPoint(p1.X() - p2.X(), p1.Y() - p2.Y());
    }

    friend bool operator==(const TPoint& p1, const TPoint& p2)
    {
        return p1.X() == p2.X() && p1.Y() == p2.Y();
    }

    friend bool operator!=(const TPoint& p1, const TPoint& p2)
    {
        return p1.X() != p2.X() || p1.Y() != p2.Y();
    }
};

typedef TPoint<PointCore> Point;

inline ZString GetString(const Point& point)
{
    return "(" + ZString(point.X()) + ", " + ZString(point.Y()) + ")";
}

//////////////////////////////////////////////////////////////////////////////
//
// Windows Points
//
//////////////////////////////////////////////////////////////////////////////

class WinPointCore : public POINT {
protected:
    typedef int Number;

public:

    WinPointCore() {}
    WinPointCore(Number xArg, Number yArg)
    {
        x = xArg;
        y = yArg;
    }

    Number X() const { return x; }
    Number Y() const { return y; }

    void SetX(Number xArg) { x = xArg; }
    void SetY(Number yArg) { y = yArg; }
};

typedef TPoint<WinPointCore> WinPoint;

inline ZString GetString(const WinPoint& point)
{
    return "(" + ZString(point.X()) + ", " + ZString(point.Y()) + ")";
}

#endif
