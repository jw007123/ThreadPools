#pragma once

#include <queue>

#include "Eigen/Sparse"

#include "Literals.h"

/// Solves Mx = b as x = M^-1b
Eigen::VectorXd SparseSolve(const Eigen::SparseMatrix<double>& mat_, const Eigen::VectorXd& vec_);

