#pragma once

#include "Eigen/Sparse"
#include "Eigen/Dense"
#include "Eigen/Core"

/*
*	Various jobs ordered by time from slow -> fast.
*/

/// Solves Mx = b as x = M^-1b
Eigen::VectorXd SparseSolve(const Eigen::SparseMatrix<double>& mat_, const Eigen::VectorXd& vec_);

/// Computes M^-1 from M
Eigen::Matrix4d DenseInverse(const Eigen::Matrix4d& mat_);

/// For some angle x in radians, verifies that sin^2(x) + cos^2(x) does indeed equal 1
bool TrigIdentity(const double angle_);

/// Increments a scalar by 1
size_t IncrementScalar(const size_t scalar_);
