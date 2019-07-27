#include "stdafx.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox() {
	reset();
}

BoundingBox::~BoundingBox() {}

void BoundingBox::reset() {
    _minPt = Vec3( FLT_MAX );
    _maxPt = Vec3( -FLT_MAX );
}

bool BoundingBox::valid() const {
    return _minPt.x() < _maxPt.x() && _minPt.y() < _maxPt.y() && _minPt.z() < _maxPt.z();
}

void BoundingBox::expandBy( const Vec3& pt ) {
    _minPt.x() = min( _minPt.x(), pt.x() );
    _minPt.y() = min( _minPt.y(), pt.y() );
    _minPt.z() = min( _minPt.z(), pt.z() );

    _maxPt.x() = max( _maxPt.x(), pt.x() );
    _maxPt.y() = max( _maxPt.y(), pt.y() );
    _maxPt.z() = max( _maxPt.z(), pt.z() );
}

Vec3 BoundingBox::center() const
{
	return (_minPt + _maxPt) / 2.f;
}

bool BoundingBox::is_intersect( const Ray& r ) {
    float tmin = ( _minPt.x() - r.orig.x() ) / r.dir.x();
    float tmax = ( _maxPt.x() - r.orig.x() ) / r.dir.x();

    if ( tmin > tmax )
        std::swap( tmin, tmax );

    float tymin = ( _minPt.y() - r.orig.y() ) / r.dir.y();
    float tymax = ( _maxPt.y() - r.orig.y() ) / r.dir.y();

    if ( tymin > tymax )
        std::swap( tymin, tymax );

    if ( ( tmin > tymax ) || ( tymin > tmax ) )
        return false;

    if ( tymin > tmin )
        tmin = tymin;

    if ( tymax < tmax )
        tmax = tymax;

    float tzmin = ( _minPt.z() - r.orig.z() ) / r.dir.z();
    float tzmax = ( _maxPt.z() - r.orig.z() ) / r.dir.z();

    if ( tzmin > tzmax )
        std::swap( tzmin, tzmax );

    if ( ( tmin > tzmax ) || ( tzmin > tmax ) )
        return false;

    if ( tzmin > tmin )
        tmin = tzmin;

    if ( tzmax < tmax )
        tmax = tzmax;

    return true;
}

float BoundingBox::max_len() const
{
	return max({ fabs(_minPt.x() - _maxPt.x()), fabs(_minPt.y() - _maxPt.y()),  fabs(_minPt.z() - _maxPt.z()) });
}

float BoundingBox::min_len() const
{
	return min({ fabs(_minPt.x() - _maxPt.x()), fabs(_minPt.y() - _maxPt.y()),  fabs(_minPt.z() - _maxPt.z()) });
}
