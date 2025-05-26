#include "Jobs.h"

Eigen::VectorXd SparseSolve(const Eigen::SparseMatrix<double>& mat_, const Eigen::VectorXd& vec_)
{
	// i. Ref wrapper to remove copy of mat_ inside solver
	// ii. AMD preconditioner as diag etc not feasible here
	Eigen::ConjugateGradient<Eigen::Ref<Eigen::SparseMatrix<double>>,
		Eigen::Lower | Eigen::Upper, Eigen::AMDOrdering<int>> cg_solver(mat_);

	// Random matrix so default guess of 0 will give best overall chance of convergence
	return cg_solver.solve(vec_);
}
