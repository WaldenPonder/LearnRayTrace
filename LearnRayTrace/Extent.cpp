#include "Extent.h"
#include "stdafx.h"

Extent::Extent( MeshObject* mesh ) : mesh_( mesh ) {
    mesh->init_polytope_boundingbox( *this );
}

Extent::Extent( const Extent& pbb ) : mesh_( pbb.mesh_ ) {
    for ( int i = 0; i < 7; i++ ) {
        slabs_[ i ][ 0 ] = pbb.slabs_[ i ][ 0 ];
        slabs_[ i ][ 1 ] = pbb.slabs_[ i ][ 1 ];
    }
}

Extent::Extent() {}

Extent::~Extent() {}

bool Extent::intersect( const Ray& ray ) {
    float precomputedNumerator[ 7 ];
    float precomputedDenominator[ 7 ];
    for ( uint8_t i = 0; i < 7; ++i ) {
        precomputedNumerator[ i ]   = g::planeSetNormals[ i ] * ray.orig;  //  dot(planeSetNormals[i], orig);
        precomputedDenominator[ i ] = g::planeSetNormals[ i ] * ray.dir;   // dot(planeSetNormals[i], dir);
    }

    uint8_t planeIndex;

    float tNear = 0, tFar = FLT_MAX;  // tNear, tFar for the intersected extents

    for ( uint8_t i = 0; i < 7; ++i ) {
        float tNearExtents = ( slabs_[ i ][ 0 ] - precomputedNumerator[ i ] ) / precomputedDenominator[ i ];
        float tFarExtents  = ( slabs_[ i ][ 1 ] - precomputedNumerator[ i ] ) / precomputedDenominator[ i ];
        if ( precomputedDenominator[ i ] < 0 )
            std::swap( tNearExtents, tFarExtents );
        if ( tNearExtents > tNear )
            tNear = tNearExtents, planeIndex = i;
        if ( tFarExtents < tFar )
            tFar = tFarExtents;
        if ( tNear > tFar )
            return false;
    }

    return true;
}

Vec3 Extent::center() const {
    return Vec3( slabs_[ 0 ][ 0 ] + slabs_[ 0 ][ 1 ] * 0.5, slabs_[ 1 ][ 0 ] + slabs_[ 1 ][ 1 ] * 0.5, slabs_[ 2 ][ 0 ] + slabs_[ 2 ][ 1 ] * 0.5 );
}

void Extent::extendBy( const Extent& pbb ) {
    for ( uint8_t i = 0; i < 7; ++i ) {
        if ( pbb.slabs_[ i ][ 0 ] < slabs_[ i ][ 0 ] )
            slabs_[ i ][ 0 ] = pbb.slabs_[ i ][ 0 ];
        if ( pbb.slabs_[ i ][ 1 ] > slabs_[ i ][ 1 ] )
            slabs_[ i ][ 1 ] = pbb.slabs_[ i ][ 1 ];
    }
}
