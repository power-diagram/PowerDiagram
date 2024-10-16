#pragma once

#include "VertexRefIds.h"
#include "SimdTensor.h"
#include "VtkOutput.h"
#include "Cut.h"

namespace power_diagram {

/**
 * @brief
 *
 */
class PD_NAME( Cell ) { STD_TL_TYPE_INFO( Cell, "" ) //
public:
    using                        VertexCoords              = SimdTensor<TF,nb_dims>;
    using                        VertexRefs                = PD_NAME( VertexRefs );
    using                        PavingItem                = PD_NAME( PavingItem );
    using                        Cell                      = PD_NAME( Cell );
    using                        Cut                       = PD_NAME( Cut );
    
    /**/                         PD_NAME( Cell )           ();

    void                         init_from                 ( const Cell &that, const Pt &p0, TF w0, PI i0 );

    void                         cut_boundary              ( const Pt &dir, TF off, PI num_boundary );
    void                         cut_dirac                 ( const Pt &p1, TF w1, PI i1, PavingItem *paving_item, PI32 num_in_paving_item );

    void                         memory_compaction         ();
 
    TF                           for_each_cut_with_measure ( const std::function<void( const Cut &cut, TF measure )> &f ) const;
    void                         for_each_edge             ( const std::function<void( const Vec<PI32,nb_dims-1> &num_cuts, Span<PI32,2> vertices )> &f ) const;
    void                         for_each_face             ( const std::function<void( const Vec<PI32,nb_dims-2> &num_cuts, Span<PI32> vertices )> &f ) const;
 
    void                         display_vtk               ( VtkOutput &vo, const std::function<Vec<VtkOutput::TF,3>( const Pt &pt )> &coord_change ) const; ///<
    void                         display_vtk               ( VtkOutput &vo ) const; ///<

    PI                           nb_vertices               () const { return vertex_cuts.size(); }
    PI                           capa_cuts                 () const { return cuts.size(); }
 
    void                         get_prev_cut_info         ( PrevCutInfo<Config> &pci );

    bool                         contains                  ( const Pt &x ) const;
    TF                           measure                   () const;
    bool                         is_inf                    () const;
    TF                           height                    ( const Pt &point ) const;
    bool                         empty                     () const;

    VertexCoords                 vertex_coords;
    Vec<VertexCut>               vertex_cuts;
    Vec<Cut>                     cuts;                     ///< some of them may be inactive

    Pt                           min_pos;                  ///<
    Pt                           max_pos;                  ///<
    Pt                           p0;                       ///<
    TF                           w0;                       ///<
    PI                           i0;                       ///<

private:
    template<int i> class        MapOfNumCuts              { public: MapOfUniquePISortedArray<i,PI32,PI> map; };
    using                        NumCutMap                 = RangeOfClasses<MapOfNumCuts,0,nb_dims>;

    void                         add_measure_rec           ( auto &res, auto &M, const auto &num_cuts, PI32 prev_vertex, PI op_id, Vec<TF> &measure_for_each_cut ) const;
    void                         add_measure_rec           ( auto &res, auto &M, const auto &num_cuts, PI32 prev_vertex, PI op_id ) const;
    PI                           new_cut_oid               ( PI s = 0 ) const;
 
    Pt                           compute_pos               ( const Pt &p0, const Pt &p1, TF s0, TF s1 ) const;
    Pt                           compute_pos               ( Vec<PI,nb_dims> num_cuts ) const;

    void                         _remove_ext_vertices      ( PI old_nb_vertices ); ///< return new size
    void                         _add_cut_vertices         ( const Pt &dir, TF off, PI32 new_cut );
    bool                         _all_inside               ( const Pt &dir, TF off );
    void                         _get_sps                  ( const Pt &dir, TF off );
    void                         _cut                      ( CutType type, const Pt &dir, TF off, const Pt &p1, TF w1, PI i1, Ptree *ptr, PI32 num_in_ptr );

    // intermediate data
    mutable NumCutMap            num_cut_map;              ///<
    mutable PI                   num_cut_oid;              ///< curr op id for num_cut_map
    AlignedTFVec                 sps;                      ///< scalar products for each vertex
};

} // namespace power_diagram
