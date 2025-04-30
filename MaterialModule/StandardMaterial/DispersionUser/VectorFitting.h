#ifndef VECTORFITTING_H
#define VECTORFITTING_H

#include <iostream>
#include <fstream>
#include <complex>
#include <algorithm>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Core>
using namespace std;
typedef Eigen::Array<complex<double>, Eigen::Dynamic, Eigen::Dynamic> ComplexMatrix;

class VectorFitting
{
public:
	VectorFitting();
	~VectorFitting();

	void CalcResults(const vector<vector<double>>& UserList, int max_order, double error_limit);
	void GetFittingData(vector <double>& fp1, vector <double>& fp2, vector <double>& Q, vector <double>& fz, vector <double>& gain1, vector <double>& gain2, double& epsilon_infinity, double& error);


private:
	void SetTransferFunction(const vector<vector<double>>& UserList);
	void Create_starting_pole();
	double UpdateVectFit();
	Eigen::RowVectorXd find_complexpoles(const Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& LAMBD);
	void Building_system_matrix(int Ns, ComplexMatrix f, ComplexMatrix s, Eigen::RowVectorXd cindex, ComplexMatrix A, ComplexMatrix b, Eigen::MatrixXd& A1, Eigen::MatrixXd& b1, int Nrow, int Ncol,
		const Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& LAMBD);
	void solveEquation(Eigen::MatrixXd& A1, Eigen::MatrixXd& b1, Eigen::VectorXd& x);
	void calculate_residue(Eigen::VectorXd& x, Eigen::MatrixXd& CCC, ComplexMatrix& C, Eigen::RowVectorXd cindex, Eigen::MatrixXd& SERD, Eigen::MatrixXd& SERE);
	void calculate_new_poles(const Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& LAMBD, Eigen::Array<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& C, Eigen::MatrixXd& B, ComplexMatrix& roetter, ComplexMatrix& SERA);
	void Building_system_matrix_2(int Ns, ComplexMatrix f, ComplexMatrix s, Eigen::RowVectorXd cindex, ComplexMatrix A, ComplexMatrix b, Eigen::MatrixXd& A1, Eigen::MatrixXd& b1, int Ncol2,
		const Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& LAMBD);
	Eigen::MatrixXd solveEquation2(Eigen::MatrixXd& A, const Eigen::MatrixXd& BB);
	ComplexMatrix DotComplexMatrix(const ComplexMatrix& A, const ComplexMatrix& B);

	int Ns, Nc, N;           //频点个数、求解元素个数、拟合阶数
	ComplexMatrix S;         //频率点向量
	ComplexMatrix F;         //拟合函数/向量
	Eigen::MatrixXd weight;  //系统矩阵的加权矩阵
	ComplexMatrix SERAStart; //初始极点矩阵
	ComplexMatrix SERA1;
	ComplexMatrix SERA;
	ComplexMatrix SERC;
	Eigen::MatrixXd SERD;
	double rmserr;

	/* 默认矢量拟合算法配置 */
	int kill = 1, asympflag = 2, skip = 0;

};

#endif