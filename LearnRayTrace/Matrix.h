/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2004 Robert Osfield
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

#ifndef OSG_MATRIXD
#define OSG_MATRIXD 1

#include "Vec3.h"
#include "Vec4.h"
#include "Quat.h"

class  Matrix
{
    public:

        typedef float value_type;
        typedef float other_value_type;

        inline Matrix() { makeIdentity(); }
        inline Matrix( const Matrix& mat) { set(mat.ptr()); }
        
        inline explicit Matrix( float const * const ptr ) { set(ptr); }
        inline explicit Matrix( const Quat& quat ) { makeRotate(quat); }

        Matrix(value_type a00, value_type a01, value_type a02, value_type a03,
                value_type a10, value_type a11, value_type a12, value_type a13,
                value_type a20, value_type a21, value_type a22, value_type a23,
                value_type a30, value_type a31, value_type a32, value_type a33);

        ~Matrix() {}

        int compare(const Matrix& m) const;

        bool operator < (const Matrix& m) const { return compare(m)<0; }
        bool operator == (const Matrix& m) const { return compare(m)==0; }
        bool operator != (const Matrix& m) const { return compare(m)!=0; }

        inline value_type& operator()(int row, int col) { return _mat[row][col]; }
        inline value_type operator()(int row, int col) const { return _mat[row][col]; }

		bool isNaN(float v) { return std::isnan(v); }

        inline bool valid() const { return !isNaN(); }

        inline bool isNaN() const { return std::isnan(_mat[0][0]) || std::isnan(_mat[0][1]) || std::isnan(_mat[0][2]) || std::isnan(_mat[0][3]) ||
                                                 std::isnan(_mat[1][0]) || std::isnan(_mat[1][1]) || std::isnan(_mat[1][2]) || std::isnan(_mat[1][3]) ||
                                                 std::isnan(_mat[2][0]) || std::isnan(_mat[2][1]) || std::isnan(_mat[2][2]) || std::isnan(_mat[2][3]) ||
                                                 std::isnan(_mat[3][0]) || std::isnan(_mat[3][1]) || std::isnan(_mat[3][2]) || std::isnan(_mat[3][3]); }

        inline Matrix& operator = (const Matrix& rhs)
        {
            if( &rhs == this ) return *this;
            set(rhs.ptr());
            return *this;
        }

      

        inline void set(const Matrix& rhs) { set(rhs.ptr()); }

     
        inline void set(float const * const ptr)
        {
            value_type* local_ptr = (value_type*)_mat;
            for(int i=0;i<16;++i) local_ptr[i]=(value_type)ptr[i];
        }

        void set(value_type a00, value_type a01, value_type a02,value_type a03,
                 value_type a10, value_type a11, value_type a12,value_type a13,
                 value_type a20, value_type a21, value_type a22,value_type a23,
                 value_type a30, value_type a31, value_type a32,value_type a33);

        value_type * ptr() { return (value_type*)_mat; }
        const value_type * ptr() const { return (const value_type *)_mat; }

        bool isIdentity() const
        {
            return _mat[0][0]==1.0 && _mat[0][1]==0.0 && _mat[0][2]==0.0 &&  _mat[0][3]==0.0 &&
                   _mat[1][0]==0.0 && _mat[1][1]==1.0 && _mat[1][2]==0.0 &&  _mat[1][3]==0.0 &&
                   _mat[2][0]==0.0 && _mat[2][1]==0.0 && _mat[2][2]==1.0 &&  _mat[2][3]==0.0 &&
                   _mat[3][0]==0.0 && _mat[3][1]==0.0 && _mat[3][2]==0.0 &&  _mat[3][3]==1.0;
        }

        void makeIdentity();

        void makeScale( const Vec3& );
        void makeScale( value_type, value_type, value_type );

        void makeTranslate( const Vec3& );
        void makeTranslate( value_type, value_type, value_type );

        void makeRotate( const Vec3& from, const Vec3& to );

        void makeRotate( value_type angle, const Vec3& axis );

        void makeRotate( value_type angle, value_type x, value_type y, value_type z );
        void makeRotate( const Quat& );
        void makeRotate( value_type angle1, const Vec3& axis1,
                         value_type angle2, const Vec3& axis2,
                         value_type angle3, const Vec3& axis3);


        /** decompose the matrix into translation, rotation, scale and scale orientation.*/
        void decompose( Vec3& translation,
                        Quat& rotation,
                        Vec3& scale,
                        Quat& so ) const;


        /** Set to an orthographic projection.
         * See glOrtho for further details.
        */
        void makeOrtho(float left,   float right,
                       float bottom, float top,
                       float zNear,  float zFar);

        /** Get the orthographic settings of the orthographic projection matrix.
          * Note, if matrix is not an orthographic matrix then invalid values
          * will be returned.
        */
        bool getOrtho(float& left,   float& right,
                      float& bottom, float& top,
                      float& zNear,  float& zFar) const;


        /** Set to a 2D orthographic projection.
          * See glOrtho2D for further details.
        */
        inline void makeOrtho2D(float left,   float right,
                                float bottom, float top)
        {
            makeOrtho(left,right,bottom,top,-1.0,1.0);
        }


        /** Set to a perspective projection.
          * See glFrustum for further details.
        */
        void makeFrustum(float left,   float right,
                         float bottom, float top,
                         float zNear,  float zFar);

        /** Get the frustum settings of a perspective projection matrix.
          * Note, if matrix is not a perspective matrix then invalid values
          * will be returned.
        */
        bool getFrustum(float& left,   float& right,
                        float& bottom, float& top,
                        float& zNear,  float& zFar) const;


        /** Set to a symmetrical perspective projection.
          * See gluPerspective for further details.
          * Aspect ratio is defined as width/height.
        */
        void makePerspective(float fovy,  float aspectRatio,
                             float zNear, float zFar);

        /** Get the frustum settings of a symmetric perspective projection
          * matrix.
          * Return false if matrix is not a perspective matrix,
          * where parameter values are undefined.
          * Note, if matrix is not a symmetric perspective matrix then the
          * shear will be lost.
          * Asymmetric matrices occur when stereo, power walls, caves and
          * reality center display are used.
          * In these configuration one should use the AsFrustum method instead.
        */
        bool getPerspective(float& fovy,  float& aspectRatio,
                            float& zNear, float& zFar) const;

        /** Set the position and orientation to be a view matrix,
          * using the same convention as gluLookAt.
        */
        void makeLookAt(const Vec3& eye,const Vec3& center,const Vec3& up);

        /** Get to the position and orientation of a modelview matrix,
          * using the same convention as gluLookAt.
        */
        void getLookAt(Vec3& eye,Vec3& center,Vec3& up,
                       value_type lookDistance=1.0f) const;


        /** invert the matrix rhs, automatically select invert_4x3 or invert_4x4. */
        inline bool invert( const Matrix& rhs)
        {
            bool is_4x3 = (rhs._mat[0][3]==0.0 && rhs._mat[1][3]==0.0 &&  rhs._mat[2][3]==0.0 && rhs._mat[3][3]==1.0);
            return is_4x3 ? invert_4x3(rhs) :  invert_4x4(rhs);
        }

        /** 4x3 matrix invert, not right hand column is assumed to be 0,0,0,1. */
        bool invert_4x3( const Matrix& rhs);

        /** full 4x4 matrix invert. */
        bool invert_4x4( const Matrix& rhs);

        /** ortho-normalize the 3x3 rotation & scale matrix */
        void orthoNormalize(const Matrix& rhs);

        // basic utility functions to create new matrices
        inline static Matrix identity( void );
        inline static Matrix scale( const Vec3& sv);
        inline static Matrix scale( value_type sx, value_type sy, value_type sz);
        inline static Matrix translate( const Vec3& dv);

        inline static Matrix translate( value_type x, value_type y, value_type z);
        inline static Matrix rotate( const Vec3& from, const Vec3& to);

        inline static Matrix rotate( value_type angle, value_type x, value_type y, value_type z);
        inline static Matrix rotate( value_type angle, const Vec3& axis);
 
        inline static Matrix rotate( value_type angle1, const Vec3& axis1,
                                      value_type angle2, const Vec3& axis2,
                                      value_type angle3, const Vec3& axis3);
        inline static Matrix rotate( const Quat& quat);
        inline static Matrix inverse( const Matrix& matrix);
        inline static Matrix orthoNormal(const Matrix& matrix);
        /** Create an orthographic projection matrix.
          * See glOrtho for further details.
        */
        inline static Matrix ortho(float left,   float right,
                                    float bottom, float top,
                                    float zNear,  float zFar);

        /** Create a 2D orthographic projection.
          * See glOrtho for further details.
        */
        inline static Matrix ortho2D(float left,   float right,
                                      float bottom, float top);

        /** Create a perspective projection.
          * See glFrustum for further details.
        */
        inline static Matrix frustum(float left,   float right,
                                      float bottom, float top,
                                      float zNear,  float zFar);

        /** Create a symmetrical perspective projection.
          * See gluPerspective for further details.
          * Aspect ratio is defined as width/height.
        */
        inline static Matrix perspective(float fovy,  float aspectRatio,
                                          float zNear, float zFar);

        /** Create the position and orientation as per a camera,
          * using the same convention as gluLookAt.
        */
        inline static Matrix lookAt(const Vec3& eye,
                                     const Vec3& center,
                                     const Vec3& up);

        inline Vec3 preMult( const Vec3& v ) const;
        inline Vec3 postMult( const Vec3& v ) const;
        inline Vec3 operator* ( const Vec3& v ) const;
        inline Vec4 preMult( const Vec4& v ) const;
        inline Vec4 postMult( const Vec4& v ) const;
        inline Vec4 operator* ( const Vec4& v ) const;

        void setRotate(const Quat& q);
        /** Get the matrix rotation as a Quat. Note that this function
          * assumes a non-scaled matrix and will return incorrect results
          * for scaled matrixces. Consider decompose() instead.
          */
        Quat getRotate() const;

        void setTrans( value_type tx, value_type ty, value_type tz );
        void setTrans( const Vec3& v );

        inline Vec3 getTrans() const { return Vec3(_mat[3][0],_mat[3][1],_mat[3][2]); }

        inline Vec3 getScale() const {
          Vec3 x_vec(_mat[0][0],_mat[1][0],_mat[2][0]);
          Vec3 y_vec(_mat[0][1],_mat[1][1],_mat[2][1]);
          Vec3 z_vec(_mat[0][2],_mat[1][2],_mat[2][2]);
          return Vec3(x_vec.length(), y_vec.length(), z_vec.length());
        }

        /** apply a 3x3 transform of v*M[0..2,0..2]. */
        inline static Vec3 transform3x3(const Vec3& v,const Matrix& m);

        /** apply a 3x3 transform of M[0..2,0..2]*v. */
        inline static Vec3 transform3x3(const Matrix& m,const Vec3& v);

        // basic Matrixd multiplication, our workhorse methods.
        void mult( const Matrix&, const Matrix& );
        void preMult( const Matrix& );
        void postMult( const Matrix& );

        /** Optimized version of preMult(translate(v)); */
        inline void preMultTranslate( const Vec3& v );

        /** Optimized version of postMult(translate(v)); */
        inline void postMultTranslate( const Vec3& v );


        /** Optimized version of preMult(scale(v)); */
        inline void preMultScale( const Vec3& v );

        /** Optimized version of postMult(scale(v)); */
		inline void postMultScale(const Vec3& v);

        /** Optimized version of preMult(rotate(q)); */
        inline void preMultRotate( const Quat& q );
        /** Optimized version of postMult(rotate(q)); */
        inline void postMultRotate( const Quat& q );

        inline void operator *= ( const Matrix& other )
        {    if( this == &other ) {
                Matrix temp(other);
                postMult( temp );
            }
            else postMult( other );
        }

        inline Matrix operator * ( const Matrix &m ) const
        {
            Matrix r;
            r.mult(*this,m);
            return  r;
        }

    protected:
        value_type _mat[4][4];

};

// static utility methods
inline Matrix Matrix::identity(void)
{
    Matrix m;
    m.makeIdentity();
    return m;
}

inline Matrix Matrix::scale(value_type sx, value_type sy, value_type sz)
{
    Matrix m;
    m.makeScale(sx,sy,sz);
    return m;
}

inline Matrix Matrix::scale(const Vec3& v )
{
    return scale(v.x(), v.y(), v.z() );
}

inline Matrix Matrix::translate(value_type tx, value_type ty, value_type tz)
{
    Matrix m;
    m.makeTranslate(tx,ty,tz);
    return m;
}

inline Matrix Matrix::translate(const Vec3& v )
{
    return translate(v.x(), v.y(), v.z() );
}

inline Matrix Matrix::rotate( const Quat& q )
{
    return Matrix(q);
}
inline Matrix Matrix::rotate(value_type angle, value_type x, value_type y, value_type z )
{
    Matrix m;
    m.makeRotate(angle,x,y,z);
    return m;
}
inline Matrix Matrix::rotate(value_type angle, const Vec3& axis )
{
    Matrix m;
    m.makeRotate(angle,axis);
    return m;
}

inline Matrix Matrix::rotate( value_type angle1, const Vec3& axis1,
                                value_type angle2, const Vec3& axis2,
                                value_type angle3, const Vec3& axis3)
{
    Matrix m;
    m.makeRotate(angle1,axis1,angle2,axis2,angle3,axis3);
    return m;
}

inline Matrix Matrix::rotate(const Vec3& from, const Vec3& to )
{
    Matrix m;
    m.makeRotate(from,to);
    return m;
}

inline Matrix Matrix::inverse( const Matrix& matrix)
{
    Matrix m;
    m.invert(matrix);
    return m;
}

inline Matrix Matrix::orthoNormal(const Matrix& matrix)
{
  Matrix m;
  m.orthoNormalize(matrix);
  return m;
}

inline Matrix Matrix::ortho(float left,   float right,
                              float bottom, float top,
                              float zNear,  float zFar)
{
    Matrix m;
    m.makeOrtho(left,right,bottom,top,zNear,zFar);
    return m;
}

inline Matrix Matrix::ortho2D(float left,   float right,
                                float bottom, float top)
{
    Matrix m;
    m.makeOrtho2D(left,right,bottom,top);
    return m;
}

inline Matrix Matrix::frustum(float left,   float right,
                                float bottom, float top,
                                float zNear,  float zFar)
{
    Matrix m;
    m.makeFrustum(left,right,bottom,top,zNear,zFar);
    return m;
}

inline Matrix Matrix::perspective(float fovy,  float aspectRatio,
                                    float zNear, float zFar)
{
    Matrix m;
    m.makePerspective(fovy,aspectRatio,zNear,zFar);
    return m;
}

inline Matrix Matrix::lookAt(const Vec3& eye,
                               const Vec3& center,
                               const Vec3& up)
{
    Matrix m;
    m.makeLookAt(eye,center,up);
    return m;
}


inline Vec3 Matrix::postMult( const Vec3& v ) const
{
    value_type d = 1.0f/(_mat[3][0]*v.x()+_mat[3][1]*v.y()+_mat[3][2]*v.z()+_mat[3][3]) ;
    return Vec3( (_mat[0][0]*v.x() + _mat[0][1]*v.y() + _mat[0][2]*v.z() + _mat[0][3])*d,
        (_mat[1][0]*v.x() + _mat[1][1]*v.y() + _mat[1][2]*v.z() + _mat[1][3])*d,
        (_mat[2][0]*v.x() + _mat[2][1]*v.y() + _mat[2][2]*v.z() + _mat[2][3])*d) ;
}


inline Vec3 Matrix::preMult( const Vec3& v ) const
{
    value_type d = 1.0f/(_mat[0][3]*v.x()+_mat[1][3]*v.y()+_mat[2][3]*v.z()+_mat[3][3]) ;
    return Vec3( (_mat[0][0]*v.x() + _mat[1][0]*v.y() + _mat[2][0]*v.z() + _mat[3][0])*d,
        (_mat[0][1]*v.x() + _mat[1][1]*v.y() + _mat[2][1]*v.z() + _mat[3][1])*d,
        (_mat[0][2]*v.x() + _mat[1][2]*v.y() + _mat[2][2]*v.z() + _mat[3][2])*d);
}

inline Vec4 Matrix::postMult( const Vec4& v ) const
{
    return Vec4( (_mat[0][0]*v.x() + _mat[0][1]*v.y() + _mat[0][2]*v.z() + _mat[0][3]*v.w()),
        (_mat[1][0]*v.x() + _mat[1][1]*v.y() + _mat[1][2]*v.z() + _mat[1][3]*v.w()),
        (_mat[2][0]*v.x() + _mat[2][1]*v.y() + _mat[2][2]*v.z() + _mat[2][3]*v.w()),
        (_mat[3][0]*v.x() + _mat[3][1]*v.y() + _mat[3][2]*v.z() + _mat[3][3]*v.w())) ;
}

inline Vec4 Matrix::preMult( const Vec4& v ) const
{
    return Vec4( (_mat[0][0]*v.x() + _mat[1][0]*v.y() + _mat[2][0]*v.z() + _mat[3][0]*v.w()),
        (_mat[0][1]*v.x() + _mat[1][1]*v.y() + _mat[2][1]*v.z() + _mat[3][1]*v.w()),
        (_mat[0][2]*v.x() + _mat[1][2]*v.y() + _mat[2][2]*v.z() + _mat[3][2]*v.w()),
        (_mat[0][3]*v.x() + _mat[1][3]*v.y() + _mat[2][3]*v.z() + _mat[3][3]*v.w()));
}

inline Vec3 Matrix::transform3x3(const Vec3& v,const Matrix& m)
{
    return Vec3( (m._mat[0][0]*v.x() + m._mat[1][0]*v.y() + m._mat[2][0]*v.z()),
                 (m._mat[0][1]*v.x() + m._mat[1][1]*v.y() + m._mat[2][1]*v.z()),
                 (m._mat[0][2]*v.x() + m._mat[1][2]*v.y() + m._mat[2][2]*v.z()));
}

inline Vec3 Matrix::transform3x3(const Matrix& m,const Vec3& v)
{
    return Vec3( (m._mat[0][0]*v.x() + m._mat[0][1]*v.y() + m._mat[0][2]*v.z()),
                 (m._mat[1][0]*v.x() + m._mat[1][1]*v.y() + m._mat[1][2]*v.z()),
                 (m._mat[2][0]*v.x() + m._mat[2][1]*v.y() + m._mat[2][2]*v.z()) ) ;
}

inline void Matrix::preMultTranslate( const Vec3& v )
{
    for (unsigned i = 0; i < 3; ++i)
    {
        float tmp = v[i];
        if (tmp == 0)
            continue;
        _mat[3][0] += tmp*_mat[i][0];
        _mat[3][1] += tmp*_mat[i][1];
        _mat[3][2] += tmp*_mat[i][2];
        _mat[3][3] += tmp*_mat[i][3];
    }
}

inline void Matrix::postMultTranslate( const Vec3& v )
{
    for (unsigned i = 0; i < 3; ++i)
    {
        float tmp = v[i];
        if (tmp == 0)
            continue;
        _mat[0][i] += tmp*_mat[0][3];
        _mat[1][i] += tmp*_mat[1][3];
        _mat[2][i] += tmp*_mat[2][3];
        _mat[3][i] += tmp*_mat[3][3];
    }
}

inline void Matrix::preMultScale( const Vec3& v )
{
    _mat[0][0] *= v[0]; _mat[0][1] *= v[0]; _mat[0][2] *= v[0]; _mat[0][3] *= v[0];
    _mat[1][0] *= v[1]; _mat[1][1] *= v[1]; _mat[1][2] *= v[1]; _mat[1][3] *= v[1];
    _mat[2][0] *= v[2]; _mat[2][1] *= v[2]; _mat[2][2] *= v[2]; _mat[2][3] *= v[2];
}

inline void Matrix::postMultScale( const Vec3& v )
{
    _mat[0][0] *= v[0]; _mat[1][0] *= v[0]; _mat[2][0] *= v[0]; _mat[3][0] *= v[0];
    _mat[0][1] *= v[1]; _mat[1][1] *= v[1]; _mat[2][1] *= v[1]; _mat[3][1] *= v[1];
    _mat[0][2] *= v[2]; _mat[1][2] *= v[2]; _mat[2][2] *= v[2]; _mat[3][2] *= v[2];
}

inline void Matrix::preMultRotate( const Quat& q )
{
    if (q.zeroRotation())
        return;
    Matrix r;
    r.setRotate(q);
    preMult(r);
}

inline void Matrix::postMultRotate( const Quat& q )
{
    if (q.zeroRotation())
        return;
    Matrix r;
    r.setRotate(q);
    postMult(r);
}

inline Vec3 operator* (const Vec3& v, const Matrix& m )
{
    return m.preMult(v);
}


inline Vec4 operator* (const Vec4& v, const Matrix& m )
{
    return m.preMult(v);
}

inline Vec3 Matrix::operator* (const Vec3& v) const
{
    return postMult(v);
}

inline Vec4 Matrix::operator* (const Vec4& v) const
{
    return postMult(v);
}

#endif
