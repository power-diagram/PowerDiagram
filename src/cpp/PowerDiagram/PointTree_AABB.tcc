#include "PointTree_AABB.h"
#include "support/TODO.h"
#include "support/P.h"

#define DTP template<class Scalar,int nb_dims>
#define UTP PointTree_AABB<Scalar,nb_dims>

DTP UTP::PointTree_AABB( const PointTreeCtorParms &cp, Span<Point> points, Span<Scalar> weights, Span<PI> indices, PointTree<Scalar,nb_dims> *parent ) : PointTree<Scalar,nb_dims>( points, weights, indices, parent ) {
    init_bounds( cp );
    init_children( cp );
}

#ifndef AVOID_DISPLAY
DTP DisplayItem *UTP::display( DisplayItemFactory &ds ) const {
    const auto &children = this->children;
    if ( children.size() )
        return DS_OBJECT( PointTree_AABB, min_pos, max_pos, children );

    const auto &weights = this->weights;
    const auto &points = this->points;
    return DS_OBJECT( PointTree_AABB, min_pos, max_pos, points, weights );
}
#endif

DTP void UTP::init_children( const PointTreeCtorParms &cp ) {
    const PI n = this->points.size();
    if ( n <= cp.max_nb_points ) {
        // if we have cells sharing the same point, we keep the one with the largest weight
        PI k = 0;
        for( PI i = 0; i < this->indices.size(); ++i ) {
            for( PI j = 0; ; ++j ) {
                // no similar point
                if ( j == i ) {
                    if ( k != i ) {
                        this->indices[ k ] = this->indices[ i ];
                        this->weights[ k ] = this->weights[ i ];
                        this->points[ k ] = this->points[ i ];
                    }
                    ++k;
                    break;
                }
                // similar point
                if ( all( this->points[ j ] == this->points[ i ] ) ) {
                    if ( this->weights[ j ] < this->weights[ i ] ) {
                        this->indices[ j ] = this->indices[ i ];
                        this->weights[ j ] = this->weights[ i ];
                    }
                    break;
                }
            }        
        }

        this->indices.resize( k );
        this->weights.resize( k );
        this->points.resize( k );

        return;
    }

    TODO;
}

DTP void UTP::init_bounds( const PointTreeCtorParms &cp ) {
    const PI n = this->points.size();
    if ( n == 0 )
        return;

    min_offset_weights = this->weights[ 0 ];
    max_offset_weights = this->weights[ 0 ];
    coeff_weights = { FromItemValue(), 0 };

    min_pos = this->points[ 0 ];
    max_pos = this->points[ 0 ];
    for( PI i = 1; i < n; ++i ) {
        for( PI d = 0; d < nb_dims; ++d ) {
            min_pos[ d ] = min( min_pos[ d ], this->points[ i ][ d ] );
            max_pos[ d ] = max( max_pos[ d ], this->points[ i ][ d ] );
        }

        min_offset_weights = min( min_offset_weights, this->weights[ i ] );
        max_offset_weights = max( max_offset_weights, this->weights[ i ] );
    }
}

DTP Str UTP::type_name() {
    return "PointTree_AABB";
}

#undef DTP
#undef UTP
