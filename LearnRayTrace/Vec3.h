/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#ifndef OSG_VEC3F
#define OSG_VEC3F 1

#include "Vec2.h"
#include "Math.h"
#include <string>
#include <algorithm>

using std::string;

class Vec3
{
    public:

        /** Data type of vector components.*/
        typedef float value_type;

        /** Number of vector components. */
        enum { num_components = 3 };

        value_type _v[3];

        /** Constructor that sets all components of the vector to zero */
        Vec3() { _v[0]=0.0f; _v[1]=0.0f; _v[2]=0.0f;}
        Vec3(value_type x,value_type y,value_type z) { _v[0]=x; _v[1]=y; _v[2]=z; }
		explicit Vec3(value_type v) { _v[0] = v; _v[1] = v; _v[2] = v; }

        Vec3(const Point2D& v2,value_type zz)
        {
            _v[0] = v2[0];
            _v[1] = v2[1];
            _v[2] = zz;
        }


        inline bool operator == (const Vec3& v) const { return _v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2]; }

        inline bool operator != (const Vec3& v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2]; }

        inline bool operator <  (const Vec3& v) const
        {
            if (_v[0]<v._v[0]) return true;
            else if (_v[0]>v._v[0]) return false;
            else if (_v[1]<v._v[1]) return true;
            else if (_v[1]>v._v[1]) return false;
            else return (_v[2]<v._v[2]);
        }

        inline value_type* ptr() { return _v; }
        inline const value_type* ptr() const { return _v; }

        inline void set( value_type x, value_type y, value_type z)
        {
            _v[0]=x; _v[1]=y; _v[2]=z;
        }

        inline void set( const Vec3& rhs)
        {
            _v[0]=rhs._v[0]; _v[1]=rhs._v[1]; _v[2]=rhs._v[2];
        }

        inline value_type& operator [] (int i) { return _v[i]; }
        inline value_type operator [] (int i) const { return _v[i]; }

        inline value_type& x() { return _v[0]; }
        inline value_type& y() { return _v[1]; }
        inline value_type& z() { return _v[2]; }

        inline value_type x() const { return _v[0]; }
        inline value_type y() const { return _v[1]; }
        inline value_type z() const { return _v[2]; }

		inline value_type& r() { return _v[0]; }
		inline value_type& g() { return _v[1]; }
		inline value_type& b() { return _v[2]; }

		inline value_type r() const { return _v[0]; }
		inline value_type g() const { return _v[1]; }
		inline value_type b() const { return _v[2]; }

        /** Returns true if all components have values that are not NaN. */
        inline bool valid() const { return !isNaN(); }
        /** Returns true if at least one component has value NaN. */
        inline bool isNaN() const { return util::isNaN(_v[0]) || util::isNaN(_v[1]) || util::isNaN(_v[2]); }

        /** Dot product. */
        inline value_type operator * (const Vec3& rhs) const  
        {
            return _v[0]*rhs._v[0]+_v[1]*rhs._v[1]+_v[2]*rhs._v[2];
        }

        /** Cross product. */
        inline const Vec3 operator ^ (const Vec3& rhs) const
        {
            return Vec3(_v[1]*rhs._v[2]-_v[2]*rhs._v[1],
                         _v[2]*rhs._v[0]-_v[0]*rhs._v[2] ,
                         _v[0]*rhs._v[1]-_v[1]*rhs._v[0]);
        }

        /** Multiply by scalar. */
        inline const Vec3 operator * (value_type rhs) const
        {
            return Vec3(_v[0]*rhs, _v[1]*rhs, _v[2]*rhs);
        }

        /** Unary multiply by scalar. */
        inline Vec3& operator *= (value_type rhs)
        {
            _v[0]*=rhs;
            _v[1]*=rhs;
            _v[2]*=rhs;
            return *this;
        }

        /** Divide by scalar. */
        inline const Vec3 operator / (value_type rhs) const
        {
            return Vec3(_v[0]/rhs, _v[1]/rhs, _v[2]/rhs);
        }

        /** Unary divide by scalar. */
        inline Vec3& operator /= (value_type rhs)
        {
            _v[0]/=rhs;
            _v[1]/=rhs;
            _v[2]/=rhs;
            return *this;
        }

        /** Binary vector add. */
        inline const Vec3 operator + (const Vec3& rhs) const
        {
            return Vec3(_v[0]+rhs._v[0], _v[1]+rhs._v[1], _v[2]+rhs._v[2]);
        }

        /** Unary vector add. Slightly more efficient because no temporary
          * intermediate object.
        */
        inline Vec3& operator += (const Vec3& rhs)
        {
            _v[0] += rhs._v[0];
            _v[1] += rhs._v[1];
            _v[2] += rhs._v[2];
            return *this;
        }

        /** Binary vector subtract. */
        inline const Vec3 operator - (const Vec3& rhs) const
        {
            return Vec3(_v[0]-rhs._v[0], _v[1]-rhs._v[1], _v[2]-rhs._v[2]);
        }

        /** Unary vector subtract. */
        inline Vec3& operator -= (const Vec3& rhs)
        {
            _v[0]-=rhs._v[0];
            _v[1]-=rhs._v[1];
            _v[2]-=rhs._v[2];
            return *this;
        }

        /** Negation operator. Returns the negative of the Vec3. */
        inline const Vec3 operator - () const
        {
            return Vec3 (-_v[0], -_v[1], -_v[2]);
        }

        /** Length of the vector = sqrt( vec . vec ) */
        inline value_type length() const
        {
            return sqrtf( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] );
        }

        /** Length squared of the vector = vec . vec */
        inline value_type length2() const
        {
            return _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2];
        }

        /** Normalize the vector so that it has length unity.
          * Returns the previous length of the vector.
        */
        inline value_type normalize()
        {
            value_type norm = Vec3::length();
            if (norm>0.0)
            {
                value_type inv = 1.0f/norm;
                _v[0] *= inv;
                _v[1] *= inv;
                _v[2] *= inv;
            }
            return( norm );
        }

		inline Vec3 clamp(float lo, float hi)
		{
			_v[0] = util::clampTo(_v[0], lo, hi);
			_v[1] = util::clampTo(_v[1], lo, hi);
			_v[2] = util::clampTo(_v[2], lo, hi);

			return Vec3(_v[0], _v[1], _v[2]);
		}

};    // end of class Vec3

/** multiply by vector components. */
inline Vec3 componentMultiply(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs[0]*rhs[0], lhs[1]*rhs[1], lhs[2]*rhs[2]);
}

/** divide rhs components by rhs vector components. */
inline Vec3 componentDivide(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs[0]/rhs[0], lhs[1]/rhs[1], lhs[2]/rhs[2]);
}

inline Vec3 operator*(float f, const Vec3& v)
{
	return v * f;
}

inline  std::ostream& operator<< (std::ostream& stm, const Vec3& v)
{
	stm << std::string("  [") << v[0] << " , " << v[1] << " , " << v[2]  << "] ";

	return stm;
}

const Vec3 X_AXIS(1.0,0.0,0.0);
const Vec3 Y_AXIS(0.0,1.0,0.0);
const Vec3 Z_AXIS(0.0,0.0,1.0);

using Color = Vec3;
using Point = Vec3;
using Vec3f = Vec3;

namespace g{
const Color  White(255 / 255.f, 255 / 255.f, 255 / 255.f);
const Color  Red(255 / 255.f, 0 / 255.f, 0 / 255.f);
const Color  Green(0 / 255.f, 255 / 255.f, 0 / 255.f);
const Color  Blue(0 / 255.f, 0 / 255.f, 255 / 255.f);
const Color  Magenta(255 / 255.f, 0 / 255.f, 255 / 255.f);
const Color  Cyan(0 / 255.f, 255 / 255.f, 255 / 255.f);
const Color  Yellow(255 / 255.f, 255 / 255.f, 0 / 255.f);
const Color  Black(0 / 255.f, 0 / 255.f, 0 / 255.f);
const Color  Aquamarine(112 / 255.f, 219 / 255.f, 147 / 255.f);
const Color  BakerChocolate(92 / 255.f, 51 / 255.f, 23 / 255.f);
const Color  BlueViolet(159 / 255.f, 95 / 255.f, 159 / 255.f);
const Color  Brass(181 / 255.f, 166 / 255.f, 66 / 255.f);
const Color  BrightGold(217 / 255.f, 217 / 255.f, 25 / 255.f);
const Color  Brown(166 / 255.f, 42 / 255.f, 42 / 255.f);
const Color  Bronze(140 / 255.f, 120 / 255.f, 83 / 255.f);
const Color  BronzeII(166 / 255.f, 125 / 255.f, 61 / 255.f);
const Color  CadetBlue(95 / 255.f, 159 / 255.f, 159 / 255.f);
const Color  CoolCopper(217 / 255.f, 135 / 255.f, 25 / 255.f);
const Color  Copper(184 / 255.f, 115 / 255.f, 51 / 255.f);
const Color  Coral(255 / 255.f, 127 / 255.f, 0 / 255.f);
const Color  CornFlowerBlue(66 / 255.f, 66 / 255.f, 111 / 255.f);
const Color  DarkBrown(92 / 255.f, 64 / 255.f, 51 / 255.f);
const Color  DarkGreen(47 / 255.f, 79 / 255.f, 47 / 255.f);
const Color  DarkGreenCopper(74 / 255.f, 118 / 255.f, 110 / 255.f);
const Color  DarkOliveGreen(79 / 255.f, 79 / 255.f, 47 / 255.f);
const Color  DarkOrchid(153 / 255.f, 50 / 255.f, 205 / 255.f);
const Color  DarkPurple(135 / 255.f, 31 / 255.f, 120 / 255.f);
const Color  DarkSlateBlue(107 / 255.f, 35 / 255.f, 142 / 255.f);
const Color  DarkSlateGrey(47 / 255.f, 79 / 255.f, 79 / 255.f);
const Color  DarkTan(151 / 255.f, 105 / 255.f, 79 / 255.f);
const Color  DarkTurquoise(112 / 255.f, 147 / 255.f, 219 / 255.f);
const Color  DarkWood(133 / 255.f, 94 / 255.f, 66 / 255.f);
const Color  DimGrey(84 / 255.f, 84 / 255.f, 84 / 255.f);
const Color  DustyRose(133 / 255.f, 99 / 255.f, 99 / 255.f);
const Color  Feldspar(209 / 255.f, 146 / 255.f, 117 / 255.f);
const Color  Firebrick(142 / 255.f, 35 / 255.f, 35 / 255.f);
const Color  ForestGreen(35 / 255.f, 142 / 255.f, 35 / 255.f);
const Color  Gold(205 / 255.f, 127 / 255.f, 50 / 255.f);
const Color  Goldenrod(219 / 255.f, 219 / 255.f, 112 / 255.f);
const Color  Grey(192 / 255.f, 192 / 255.f, 192 / 255.f);
const Color  GreenCopper(82 / 255.f, 127 / 255.f, 118 / 255.f);
const Color  GreenYellow(147 / 255.f, 219 / 255.f, 112 / 255.f);
const Color  HunterGreen(33 / 255.f, 94 / 255.f, 33 / 255.f);
const Color  IndianRed(78 / 255.f, 47 / 255.f, 47 / 255.f);
const Color  Khaki(159 / 255.f, 159 / 255.f, 95 / 255.f);
const Color  LightBlue(192 / 255.f, 217 / 255.f, 217 / 255.f);
const Color  LightGrey(168 / 255.f, 168 / 255.f, 168 / 255.f);
const Color  LightSteelBlue(143 / 255.f, 143 / 255.f, 189 / 255.f);
const Color  LightWood(233 / 255.f, 194 / 255.f, 166 / 255.f);
const Color  LimeGreen(50 / 255.f, 205 / 255.f, 50 / 255.f);
const Color  MandarianOrange(228 / 255.f, 120 / 255.f, 51 / 255.f);
const Color  Maroon(142 / 255.f, 35 / 255.f, 107 / 255.f);
const Color  MediumAquamarine(50 / 255.f, 205 / 255.f, 153 / 255.f);
const Color  MediumBlue(50 / 255.f, 50 / 255.f, 205 / 255.f);
const Color  MediumForestGreen(107 / 255.f, 142 / 255.f, 35 / 255.f);
const Color  MediumGoldenrod(234 / 255.f, 234 / 255.f, 174 / 255.f);
const Color  MediumOrchid(147 / 255.f, 112 / 255.f, 219 / 255.f);
const Color  MediumSeaGreen(66 / 255.f, 111 / 255.f, 66 / 255.f);
const Color  MediumSlateBlue(127 / 255.f, 0 / 255.f, 255 / 255.f);
const Color  MediumSpringGreen(127 / 255.f, 255 / 255.f, 0 / 255.f);
const Color  MediumTurquoise(112 / 255.f, 219 / 255.f, 219 / 255.f);
const Color  MediumVioletRed(219 / 255.f, 112 / 255.f, 147 / 255.f);
const Color  MediumWood(166 / 255.f, 128 / 255.f, 100 / 255.f);
const Color  MidnightBlue(47 / 255.f, 47 / 255.f, 79 / 255.f);
const Color  NavyBlue(35 / 255.f, 35 / 255.f, 142 / 255.f);
const Color  NeonBlue(77 / 255.f, 77 / 255.f, 255 / 255.f);
const Color  NeonPink(255 / 255.f, 110 / 255.f, 199 / 255.f);
const Color  NewMidnightBlue(0 / 255.f, 0 / 255.f, 156 / 255.f);
const Color  NewTan(235 / 255.f, 199 / 255.f, 158 / 255.f);
const Color  OldGold(207 / 255.f, 181 / 255.f, 59 / 255.f);
const Color  Orange(255 / 255.f, 127 / 255.f, 0 / 255.f);
const Color  OrangeRed(255 / 255.f, 36 / 255.f, 0 / 255.f);
const Color  Orchid(219 / 255.f, 112 / 255.f, 219 / 255.f);
const Color  PaleGreen(143 / 255.f, 188 / 255.f, 143 / 255.f);
const Color  Pink(188 / 255.f, 143 / 255.f, 143 / 255.f);
const Color  Plum(234 / 255.f, 173 / 255.f, 234 / 255.f);
const Color  Quartz(217 / 255.f, 217 / 255.f, 243 / 255.f);
const Color  RichBlue(89 / 255.f, 89 / 255.f, 171 / 255.f);
const Color  Salmon(111 / 255.f, 66 / 255.f, 66 / 255.f);
const Color  Scarlet(140 / 255.f, 23 / 255.f, 23 / 255.f);
const Color  SeaGreen(35 / 255.f, 142 / 255.f, 104 / 255.f);
const Color  SemiSweetChocolate(107 / 255.f, 66 / 255.f, 38 / 255.f);
const Color  Sienna(142 / 255.f, 107 / 255.f, 35 / 255.f);
const Color  Silver(230 / 255.f, 232 / 255.f, 250 / 255.f);
const Color  SkyBlue(50 / 255.f, 153 / 255.f, 204 / 255.f);
const Color  SlateBlue(0 / 255.f, 127 / 255.f, 255 / 255.f);
const Color  SpicyPink(255 / 255.f, 28 / 255.f, 174 / 255.f);
const Color  SpringGreen(0 / 255.f, 255 / 255.f, 127 / 255.f);
const Color  SteelBlue(35 / 255.f, 107 / 255.f, 142 / 255.f);
const Color  SummerSky(56 / 255.f, 176 / 255.f, 222 / 255.f);
const Color  Tan(219 / 255.f, 147 / 255.f, 112 / 255.f);
const Color  Thistle(216 / 255.f, 191 / 255.f, 216 / 255.f);
const Color  Turquoise(173 / 255.f, 234 / 255.f, 234 / 255.f);
const Color  VeryDarkBrown(92 / 255.f, 64 / 255.f, 51 / 255.f);
const Color  VeryLightGrey(205 / 255.f, 205 / 255.f, 205 / 255.f);
const Color  Violet(79 / 255.f, 47 / 255.f, 79 / 255.f);
const Color  VioletRed(204 / 255.f, 50 / 255.f, 153 / 255.f);
const Color  Wheat(216 / 255.f, 216 / 255.f, 191 / 255.f);
const Color  YellowGreen(153 / 255.f, 204 / 255.f, 50 / 255.f);
}
#endif

