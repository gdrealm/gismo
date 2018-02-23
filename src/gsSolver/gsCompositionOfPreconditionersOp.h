/** @file gsCompositionOfPreconditionersOp.h

    @brief Allows to represent the composition of preconditioners.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): S. Takacs
*/

#pragma once

#include<gsCore/gsLinearAlgebra.h>
#include<gsSolver/gsLinearOperator.h>

namespace gismo
{

/// @brief This class represents the composition of preconditioners of type \a gsPreconditionerOp.
///
/// If the preconditioners have the iteration matrices \f$ I-P_iA \f$, the overall preconditioner
/// has the iteration matrix \f$ (I-P_nA)\cdots (I-P_1A) \f$
///
/// This should not be confused with \a gsProductOfOperatorsOp, which would yield
///
/// \f$ I - P_n \cdots P_1 A \f$.
///
/// @ingroup Solver
template <class T>
class GISMO_EXPORT gsCompositionOfPreconditionersOp : public gsPreconditionerOp<T>
{
    typedef typename gsPreconditionerOp<T>::Ptr BasePtr;
public:

    /// Shared pointer for gsCompositionOfPreconditionersOp
    typedef memory::shared_ptr<gsCompositionOfPreconditionersOp> Ptr;

    /// Unique pointer for gsCompositionOfPreconditionersOp
    typedef memory::unique_ptr<gsCompositionOfPreconditionersOp> uPtr;

    /// Empty constructor. To be filled with addOperator()
    gsCompositionOfPreconditionersOp() : m_sz(0), m_ops() {}

    /// Constructor taking a vector of preconditioners
    gsCompositionOfPreconditionersOp(const std::vector< BasePtr >& ops)
        : m_sz(ops.size()), m_ops(ops)
    {}

    /// Convenience constructor taking two preconditioners
    gsCompositionOfPreconditionersOp(const BasePtr & op0, const BasePtr & op1)
        : m_sz(2), m_ops(2)
    {
        m_ops[0] = op0; m_ops[1] = op1;
    }

    /// Convenience constructor taking three preconditioners
    gsCompositionOfPreconditionersOp(const BasePtr & op0, const BasePtr & op1, const BasePtr & op2)
        : m_sz(3), m_ops(3)
    {
        m_ops[0] = op0; m_ops[1] = op1; m_ops[2] = op2;
    }

    virtual ~gsCompositionOfPreconditionersOp() { }

    /// Make command returning a smart pointer
    static uPtr make(const std::vector< BasePtr >& ops)
    {
        return uPtr( new gsCompositionOfPreconditionersOp(ops) );
    }

    /// Make command returning a smart pointer
    static uPtr make(const BasePtr & op0, const BasePtr & op1)
    {
        return uPtr( new gsCompositionOfPreconditionersOp(op0, op1) );
    }

    /// Make command returning a smart pointer
    static uPtr make(const BasePtr & op0, const BasePtr & op1, const BasePtr & op2)
    {
        return uPtr( new gsCompositionOfPreconditionersOp(op0, op1, op2) );
    }

    /// Add another operator at the end
    void addOperator(const BasePtr& op)
    {
        m_sz++;
        m_ops.push_back( op );
    }

    /// Apply the smoother for the equation Ax=f and update the current iterate x.
    virtual void step(const gsMatrix<T>& f, gsMatrix<T>& x) const
    {
        for ( index_t i=0; i<m_sz; ++i )
            m_ops[i]->step(f,x);
    }

    /// Apply the transposed smoother for the equation Ax=f and update the current iterate x.
    virtual void stepT(const gsMatrix<T>& f, gsMatrix<T>& x) const
    {
        for ( index_t i=m_sz-1; i>=0; --i )
            m_ops[i]->stepT(f,x);
    }

    typename gsLinearOperator<T>::Ptr underlyingOp() const
    {
        GISMO_ASSERT( m_sz>0, "gsCompositionOfPreconditionersOp::underlyingOp does not work for 0 operators.");
        return m_ops[0]->underlyingOp();
    }

    index_t rows() const
    {
        GISMO_ASSERT( m_sz>0, "gsCompositionOfPreconditionersOp::rows does not work for 0 operators.");
        return m_ops[0]->rows();
    }

    index_t cols() const
    {
        GISMO_ASSERT( m_sz>0, "gsCompositionOfPreconditionersOp::rows does not work for 0 operators.");
        return m_ops[0]->cols();
    }

private:
    index_t m_sz;
    std::vector< BasePtr > m_ops;
};


} // namespace gismo
