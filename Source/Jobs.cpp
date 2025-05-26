#include "Jobs.h"

Eigen::VectorXd SparseSolve(const Eigen::SparseMatrix<double>& mat_, const Eigen::VectorXd& vec_)
{
	// Iterative solver for symmetric input matrix. Avoid AMD preconditioners due to
	// MPL2 license and, uh, bugs in their code
	Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Lower | Eigen::Upper,
		Eigen::IncompleteCholesky<double, Eigen::Lower | Eigen::Upper,
		Eigen::NaturalOrdering<int>>> cg_solver(mat_);

	// Random matrix so default guess of 0 will give best overall chance of convergence
	return cg_solver.solve(vec_);
}

Eigen::Matrix4d DenseInverse(const Eigen::Matrix4d& mat_)
{
	Eigen::Matrix4d inv;
	bool was_inv;

	// Run-up some more time by requiring a determinant check
	mat_.computeInverseWithCheck(inv, was_inv, 1e-6);
	return was_inv ? inv : mat_;
}

bool TrigIdentity(const double angle_)
{
	const double s = std::sin(angle_);
	const double c = std::cos(angle_);
	const double v = s * s + c * c;

	// Tolerance due to fp error accumulating
	return std::abs(v) < 1e-4;
}

size_t IncrementScalar(const size_t scalar_)
{
	return scalar_ + 1;
}
