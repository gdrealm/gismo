/** @file gsSinglePatchPreconditioners.h

    @brief Provides robust preconditioners for single patch geometries.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): S. Takacs, C. Hofreither
*/
#pragma once

#include <gsAssembler/gsAssembler.h>
#include <gsSolver/gsLinearOperator.h>


namespace gismo
{

/// @brief Provides robust preconditioners for single patch geometries.
///
/// This class provides efficient preconditioners for single patch geometries,
/// assuming to have \a gsTensorBasis.
///
/// @ingroup Solver
template<typename T>
class gsSinglePatchPreconditioners
{
    typedef typename gsLinearOperator<T>::uPtr OpUPtr;
    typedef typename gsLinearOperator<T>::Ptr  OpPtr;
public:

    /// Constructor taking \a gsBasis, \a gsBoundaryConditions and the
    /// \a dirichtet::strategy
    ///
    /// By setting up \a gsSinglePatchPreconditioners using this constructor,
    /// the geometry transformation is approximated by the identity mapping.
    gsSinglePatchPreconditioners(
        const gsBasis<T>& _basis,
        const gsBoundaryConditions<T>& _bc,
        dirichlet::strategy _dirichlet = dirichlet::elimination
    )
    : m_basis(_basis), m_bc(_bc), m_options(gsGenericAssembler<T>::defaultOptions())
    { m_options.setInt("DirichletStrategy", (index_t) _dirichlet ); }

    /// Constructor taking \a gsBasis, \a gsBoundaryConditions and
    /// \a gsOptionList object providing the Dirichlet strategy.
    ///
    /// By setting up \a gsSinglePatchPreconditioners using this constructor,
    /// the geometry transformation is approximated by the identity mapping.
    gsSinglePatchPreconditioners(
        const gsBasis<T>& _basis,
        const gsBoundaryConditions<T>& _bc,
        const gsOptionList& _opt
    )
    : m_basis(_basis), m_bc(_bc), m_options(_opt)
    { }

    // TODO: In followup pull requests, also some rank-1 geometry approximation
    // will be provided

    /// Provieds the mass matrix
    gsSparseMatrix<T> getMassMatrix()                      const;

    /// Provides \a gsLinearOperator representing the mass matrix (in a matrix-free way)
    OpUPtr            getMassMatrixOp()                    const;

    /// Provides \a gsLinearOperator representing the inverse of the mass matrix (in a matrix-free way)
    OpUPtr            getMassMatrixInvOp()                 const;

    /// Provieds stiffness matrix
    ///
    /// The stiffness matrix represents \f$ -\Delta u + a u \f$
    gsSparseMatrix<T> getStiffnessMatrix(T a=0)            const;

    /// Provides \a gsLinearOperator representing the stiffness matrix (in a matrix-free way)
    ///
    /// The stiffness matrix represents \f$ -\Delta u + a u \f$
    OpUPtr            getStiffnessMatrixOp(T a=0)          const;

    /// Provides \a gsLinearOperator representing the inverse stiffness matrix
    /// based on the fast diagonalization approach
    /// (SIAM J. Sci. Comput., 38 (6), p. A3644 - A3671, 2016)
    ///
    /// The stiffness matrix represents \f$ -\Delta u + a u \f$
    OpUPtr            getFastDiagonalizationOp(T a=0)      const;

    // TODO: Will be provided in a followup pull request:
    // Provides \a gsLinearOperator representing the subspace corrected mass smoother
    // (SIAM J. on Numerical Analysis. 55 (4). p. 2004 - 2024, 2017)
    //
    // This operator is spectrally equivalent to the inverse of
    // \f$ - \Delta u + h^{-2} u \f$
    //OpUPtr            getSubspaceCorrectedMassSmootherOp() const;

private:

    const gsBasis<T> &      m_basis;
    gsBoundaryConditions<T> m_bc;
    gsOptionList            m_options;
};

} // namespace gismo
