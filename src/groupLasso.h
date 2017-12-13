#ifndef groupLasso_h
#define groupLasso_h

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>
#include <set>
#include <algorithm>
#include <Rcpp.h>

using namespace Eigen;

template <class TX>
class groupLassoFit
{
protected:
    //Input needed
    TX X;// with intercept, N by p matrix, p = 1+k1+..+k(J-1)
    VectorXd y;// size N
    ArrayXd gsize;// size J, first group = intercept
    ArrayXd pen; // size J, first element = 0;
    ArrayXd lambdaseq;//size K, default 100
    bool isUserLambdaseq;
    int pathLength;
    double lambdaMinRatio;
    int maxit;
    double tol;
    bool verbose;
    
    //
    int iter;// current iterations
    TX Xcentered;
    VectorXd resid;
    bool converged_CD; // convergence in activeset
    bool converged_KKT; // KKT condition satisfied in inactiveSet
    
    //Dimension Information
    int N;
    int J;
    int p;
    int K;
    
    //Definition Inside
    ArrayXi grpSIdx;//size J
    ArrayXi iters;
    MatrixXd coefficients; //size p*k
    MatrixXd std_coefficients;
    VectorXd Xcenter;
    std::vector<MatrixXd> Rinvs;
    VectorXd beta;// size p
    std::vector<VectorXd> g;
    ArrayXd default_lambdaseq;
    
    
    //bool intercept_set;
    std::set<int> activeSet;
    std::set<int> inactiveSet;
    std::set<int> inactiveSet1;// strong and inactive set
    std::set<int> inactiveSet2;// not strong and not active
    ArrayXi convFlag;
   
    //These constructors will be called only from derived classes
    groupLassoFit(const TX & X_, VectorXd & y_, VectorXd & icoef_, ArrayXd & gsize_,ArrayXd & pen_,
                  ArrayXd & lambdaseq_,bool isUserLambdaseq_,  int pathLength_,
                  double lambdaMinRatio_,int maxit_, double tol_, bool verbose_);
    
    void Rinvs_X();
    VectorXd linpred(const VectorXd & beta);
    VectorXd linpred_update(const VectorXd & new_resid, const VectorXd & old_resid, const VectorXd & old_lpred);

    
    //CD/KKT
    bool quadraticBCD(VectorXd & resid, const ArrayXd & lambda_k,double tol);
    //Active Set
    void blockCoordinateDescent(VectorXd & resid, const ArrayXd & lambda_k,double tol);
    void coordinateDescent_0(VectorXd & resid);
    void D_coordinateDescent_j(int j, VectorXd & resid, const ArrayXd & lambda_k);
    double B_coordinateDescent_j(int j, VectorXd & resid, const ArrayXd & lambda_k);
    double S_coordinateDescent_j(int j, VectorXd & resid, const ArrayXd & lambda_k);
    
    
    //Inactive set
    void checkKKT(const VectorXd & resid, const ArrayXd & lambda_k);
    bool KKT(const VectorXd & resid, const ArrayXd & lambda_k, int setidx);
    bool checkKKT_j(int j,const VectorXd & resid, const ArrayXd & lambda_k);
    
public:
    //getters
    MatrixXd getCoefficients();
    MatrixXd getStdCoefficients();
    ArrayXi getIters();
    ArrayXd getLambdaSequence();
    ArrayXi getconvFlag();
    
    //Misc functions
    VectorXd back_to_org(const VectorXd & beta);
    VectorXd org_to_std(const VectorXd & coef);
    ArrayXd computeLambdaSequence(const VectorXd & resp);
    void checkDesignMatrix(const TX & X);
};

#endif /* groupLasso_h */