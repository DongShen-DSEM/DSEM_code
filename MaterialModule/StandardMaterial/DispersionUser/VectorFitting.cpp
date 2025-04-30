#include "stdafx.h"
#include "VectorFitting.h"

VectorFitting::VectorFitting()
{

}

VectorFitting::~VectorFitting()
{

}

void VectorFitting::CalcResults(const vector<vector<double>>& UserList, int max_order, double error_limit)
{
	N = max_order;
	SetTransferFunction(UserList); //读取用户输入数据
	Create_starting_pole(); //创建初始极点

	int n = 0; //迭代次数，限制在100以内
//	double rmserr_old = 0., rmserr = 1.;
//	while (abs(rmserr - rmserr_old) > 0.0001 && n < 100)
	rmserr = 1.;
	while (abs(rmserr) > error_limit && n < 20)
	{
//		rmserr_old = rmserr;
		rmserr = UpdateVectFit(); //更新矢量拟合结果
		SERAStart = SERA;
		// std::cout << "目标复矩阵SERAStart：" << std::endl;
		// std::cout << SERAStart << std::endl;
		n++;
	}
}

void VectorFitting::GetFittingData(vector <double>& fp1, vector <double>& fp2, vector <double>& Q, vector <double>& fz, vector <double>& gain1, vector <double>& gain2, double& epsilon_infinity, double& error)
{
	vector <double> alpha, belta;
	vector <double> A0, A1, B0, B1;
	/* 实极点为general一阶，复共轭极点组合成general二阶 */
	for (int m = 0; m < SERA.cols(); m++)
	{
		for (int n = 0; n < SERA.rows(); n++)
		{
			bool order2nd = SERA(n, m).imag();
			if (order2nd)
			{
				B0.push_back(-SERA(n, m).real()); B1.push_back(-SERA(n, m).imag());
				A0.push_back(SERC(n, m).real()); A1.push_back(SERC(n, m).imag());

			}
			else
			{
				alpha.push_back(-SERA(n, m).real());
				belta.push_back(SERC(n, m).real());
			}
		}
	}
	/* 将极点留数转换成CST上显示的参数 */
	double PI = 3.1415926;
	for (int n = 0; n < alpha.size(); n++)
	{
		double fp1n = alpha[n] / (2 * PI);
		double gain1n = belta[n] / alpha[n];
		fp1.push_back(fp1n);
		gain1.push_back(gain1n);
	}
	for (int n = 0; n < A0.size() / 2.; n++)
	{
		int n1 = 2 * n;
		double fp2n = sqrt(pow(B0[n1], 2) + pow(B1[n1], 2)) / (2 * PI);
		double Q2n = sqrt(pow(B0[n1], 2) + pow(B1[n1], 2)) / (2 * B0[n1]);
		double fz2n = (A0[n1] * B0[n1] + A1[n1] * B1[n1]) / (2 * PI * A0[n1]);
		double gain2n = 2 * (A0[n1] * B0[n1] + A1[n1] * B1[n1]) / (pow(B0[n1], 2) + pow(B1[n1], 2));

		fp2.push_back(fp2n);
		Q.push_back(Q2n);
		fz.push_back(fz2n);
		gain2.push_back(gain2n);
	}
	epsilon_infinity = SERD(0, 0);
	error = rmserr;
}

/* 处理用户输入的数据，分类存储 */
void VectorFitting::SetTransferFunction(const vector<vector<double>>& UserList)
{
	Ns = UserList.size();
	Nc = 1; //只需要求解1个元素


	/* 初始化 */
	S.resize(Ns, 1);
	F.resize(Nc, Ns);
	weight.resize(Ns, 1);

	/* 赋值 */
	/* 频率矩阵 */
	for (int i = 0; i < Ns; i++) {
		double real = 0.0;
		double imag = UserList[i][0];
		S(i, 0) = std::complex<double>(real, imag);
	}
	/* 函数矩阵 */
	for (int i = 0; i < Ns; i++) {
		for (int j = 0; j < Nc; j++) {
			int num = i * Nc + j;
			double real = UserList[num][1];
			double imag = UserList[num][2];
			F(j, i) = std::complex<double>(real, imag);
		}
	}
	/* 加权矩阵 */
	for (int i = 0; i < Ns; i++) {
			weight(i, 0) = UserList[i][3];
	}
}


/* 创建初始复极点 */
void VectorFitting::Create_starting_pole()
{
	/* 计算bet，即正实轴上的极点 */
	std::vector<double> bet(N / 2);
	/* 线性插值加权向量 */
	for (int n = 0; n < N / 2; n++) {
		bet[n] = S(0, 0).imag() + (n * ((S(Ns - 1, 0).imag() - S(0, 0).imag()) / (N / 2 - 1)));
	}

	SERAStart.resize(N, 1);
	for (int n = 0; n < bet.size(); n++) {
		double alf = -bet[n] * 1e-2;
		std::complex<double> pole1(alf, -bet[n]);
		std::complex<double> pole2(alf, bet[n]);
		SERAStart(2 * n, 0) = pole1;
		SERAStart((2 * n) + 1, 0) = pole2;

	}

	// std::cout << "目标复矩阵SERAStart：" << std::endl;
	// std::cout << SERAStart << std::endl;
}


/* -------------------------- 矢量拟合 -------------------------------------------

   kill=0 --> unstable poles are kept unchanged
   kill=1 --> unstable poles are deleted
   kill=2 --> unstable poles are 'flipped' into the left half plane不稳定的极点被“翻转”到左半平面
   (kill=2 is the recommended choice)%(建议选择kill=2)
   
   asympflag=1 --> order(numerator)=order(denominator)-1 ('Strictly proper')阶(分子)=阶(分母)-1(“严格正确”)
   asympflag=2 --> order(numerator)=order(denominator)   ('Proper')
   asympflag=3 --> order(numerator)=order(denominator)+1 ('Improper') 

   -------------------------------------------------------------------------------*/
double VectorFitting::UpdateVectFit()
{
	/* 初始化矩阵 */	
	SERC.resize(Nc, N); SERC.setZero();
	SERD.resize(Nc, 1); SERD.setZero();  //constant term 常数
	Eigen::MatrixXd SERE(Nc, 1); SERE.setZero();  //proportional term 比例项
	double rmserr;  //root-mean-square error 均方根误差
	Eigen::MatrixXd B(N, 1); B.setOnes();  // 初始化列向量矩阵B为全1
	Eigen::MatrixXd I(N, N); I.setIdentity(); // 初始化单位矩阵I为对角线为1的矩阵
	/* 声明N x N的复数矩阵LAMBD */
	Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> LAMBD(N, N);
	LAMBD.setZero();
	LAMBD.diagonal() = SERAStart; // 将SERAstart复数矩阵放入LAMBD的对角线中


	/* 判断s的第一个元素是否等于0且SERAstart的第一个元素是否等于0 */
	if (S(0, 0) == std::complex<double>(0.0, 0.0) && SERAStart(0, 0) == std::complex<double>(0.0, 0.0)) {
		SERAStart(0, 0) = std::complex<double>(1.0, 0.0); // 将SERAstart的第一个元素设为1
	}
	/* 判断s的第一个元素是否等于0且SERAstart的第二个元素是否等于0 */
	if (SERAStart.size() > 1) {

		if (S(0, 0) == std::complex<double>(0.0, 0.0) && SERAStart(1, 0) == std::complex<double>(0.0, 0.0)) {
			SERAStart(1, 0) = std::complex<double>(1.0, 0.0); // 将SERAstart的第二个元素设为1
		}
	}


	ComplexMatrix A, b;//后续可能需要修改数据类型？
				  // 根据asympflag的不同情况，调整系数矩阵A和向量b的大小
	int Ncol = 0;
	int Nrow = Ns * Nc;
	if (asympflag == 1)
		Ncol = N * Nc + N;
	else if (asympflag == 2)
		Ncol = (N + 1) * Nc + N;
	else if (asympflag == 3)
		Ncol = (N + 2) * Nc + N;
	else
		assert(1);
	A.resize(Nrow, Ncol); A.setZero();
	b.resize(Nrow, 1);

	Eigen::MatrixXd b1 = Eigen::MatrixXd::Zero(2 * Nrow, 1);
	Eigen::MatrixXd A1 = Eigen::MatrixXd::Zero(2 * Nrow, Ncol);
	Eigen::VectorXd x(Ncol); // 定义x向量来存储解向量

	Eigen::RowVectorXd cindex = find_complexpoles(LAMBD);

	Building_system_matrix(Ns, F, S, cindex, A, b, A1, b1, Nrow, Ncol, LAMBD); //构建实虚部分开的A1、b1矩阵
	solveEquation(A1, b1, x); //求解超定线性方程组
	Eigen::MatrixXd CCC;  // Output matrices
	ComplexMatrix C(Nc + 1, N);
	calculate_residue(x, CCC, C, cindex, SERD, SERE);
	ComplexMatrix roetter(1, N);
	calculate_new_poles(LAMBD, C, B, roetter, SERA);

	Eigen::MatrixXd I1(N, 1);
	Eigen::MatrixXd SERB(N, 1);	SERB.setZero();

	int order; //拟合阶数

	if (!skip) {
		// 清空变量
		LAMBD.setZero();
		A.setZero();
		A1.setZero();
		b.setZero();
		C.setZero();
		B.setZero();

		ComplexMatrix LAMBD2(1, N);
		LAMBD2 = roetter;
		Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> LAMBD21(N, N); // 声明N x N的复数矩阵LAMBD
		LAMBD21.setZero();
		// 将SERAstart复数矩阵放入LAMBD的对角线中
		LAMBD21.diagonal() = LAMBD2.transpose();

		Eigen::MatrixXd B2(N, 1);
		B2.setOnes();// 初始化列向量矩阵B为全1，等价于B.setConstant(1.0);
		Eigen::RowVectorXd cindex2 = find_complexpoles(LAMBD21);


		int Ncol2;				   // 根据asympflag的不同情况，调整系数矩阵A和向量b的大小
		if (asympflag == 1) {
			// asympflag为1，d=0;e=0
			Ncol2 = N;
		}
		else if (asympflag == 2) {
			Ncol2 = N + 1;
		}
		else {
			Ncol2 = N + 2;
		}

		ComplexMatrix A2(Ns, Ncol2), b2(Ns, Nc);
		A2.setZero(); // 全部置零
		b2.setZero();
		// Initialize A1 and b1 matrices
		Eigen::MatrixXd A21 = Eigen::MatrixXd::Zero(2 * Ns, Ncol2);
		Eigen::MatrixXd b21 = Eigen::MatrixXd::Zero(2 * Ns, Nc);

		Building_system_matrix_2(Ns, F, S, cindex2, A2, b2, A21, b21, Ncol2, LAMBD21);
		//至此，第二阶段实虚部分开的A21, b21矩阵构建完成
		// pass A, BB, and Nc to the solveLinearSystem function
		Eigen::MatrixXd X2 = solveEquation2(A21, b21);
		// Clear A
		A21.setZero();
		// Transpose X
		X2.transposeInPlace();

		// Extract C
		Eigen::MatrixXd C2 = X2.leftCols(N);
		Eigen::VectorXd SERD2;
		SERD2.setZero();
		Eigen::VectorXd SERE2;
		SERE2.setZero();
		if (asympflag == 2) {
			// Extract SERD
			SERD2 = X2.col(N);
		}
		else if (asympflag == 3) {
			// Extract SERE and SERD
			SERE2 = X2.col(N + 2);
			SERD2 = X2.col(N + 1);

		}
		//std::cout << "第二阶段C2:\n" << C2 << std::endl;
		ComplexMatrix C21(Nc, N);
		C21.setZero();
		for (int m = 0; m < N; m++) {
			if (cindex2(m) == 0) {
				for (int n = 0; n < Nc; n++) {
					C21(n, m) = C2(n, m);
				}
			}
			if (cindex2(m) == 1) {
				for (int n = 0; n < Nc; n++) {
					double real = C2(n, m);
					double imag = C2(n, m + 1);
					C21(n, m) = std::complex<double>(real, imag);
					C21(n, m + 1) = std::complex<double>(real, -imag);

				}
			}
		}

		//std::cout << "第二阶段C21:\n" << C21 << std::endl;
		B.setOnes();
		I1.setOnes();//对应matlab代码中的I，暂时没啥用
		SERA = LAMBD2.transpose();
		SERB = B;
		SERC = C21.transpose();
		SERD = SERD2;
		SERE = SERE2;
		order = N; //拟合阶数

	}

	ComplexMatrix Dk(Ns, N), RES1(Ns, Nc);
	Dk.setZero(), RES1.setZero();
	for (int m = 0; m < N; m++) {
		for (int n = 0; n < Ns; n++) {
			Dk(n, m) = 1. / (S(n, 0) - SERA(m));
		}
	}
	RES1 = (Dk.matrix() * SERC.matrix()).array();
	for (int n = 0; n < Nc; n++)
	{
		for (int m = 0; m < Ns; m++)
		{
			if (asympflag == 2)
				RES1(m, n) = RES1(m, n) + SERD(n);
			else if (asympflag == 3)
				RES1(m, n) = RES1(m, n) + SERD(n) + S(m,n) * SERE(n);
		}
	}
	ComplexMatrix Diff = RES1 - F.transpose();
	double error = 0.;
	for (int n = 0; n < Nc; n++)
	{
		for (int m = 0; m < Ns; m++)
		{
			error += pow(abs(Diff(m, n)),2) / (Nc * Ns);
		}
	}
	rmserr = sqrt(error);
	return rmserr;
}

Eigen::RowVectorXd VectorFitting::find_complexpoles(const Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& LAMBD) 
{
	Eigen::RowVectorXd cindex(N); // cindex用于表征对应极点是否为复数
	cindex.setZero(); // 初始化cindex为零

	for (int m = 0; m < N; m++) {
		if (std::imag(LAMBD(m, m)) != 0) {
			if (m == 0) {
				cindex(m) = 1;
			}
			else {
				if (cindex(m - 1) == 0 || cindex(m - 1) == 2) {
					cindex(m) = 1;
					cindex(m + 1) = 2;
				}
				else {
					cindex(m) = 2;
				}
			}
		}
	}
	return cindex;
}


void VectorFitting::Building_system_matrix(int Ns, ComplexMatrix f, ComplexMatrix s, Eigen::RowVectorXd cindex, ComplexMatrix A, ComplexMatrix b, Eigen::MatrixXd& A1, Eigen::MatrixXd& b1, int Nrow, int Ncol,
	const Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& LAMBD)
{
	ComplexMatrix Dk(Ns, N); // Dk用于保存每个元素部分和多项式带入各频率点对应方程组中的真分数项矩阵
	Dk.setZero(); // 初始化Dk为零矩阵
	ComplexMatrix LAMBDDD(Ns, N);
	LAMBDDD.setZero();
	ComplexMatrix Bblokk(Ns, 1);
	Bblokk.setZero();
	Eigen::MatrixXcd H(Ns, 1);
	H.setConstant(std::complex<double>(0, 1));
	for (int m = 0; m < N; m++) {
		for (int i = 0; i < Ns; i++) {
			LAMBDDD(i, m) = LAMBD(m, m); // LAMBD(m, m)
		}

		if (cindex(m) == 0) {
			Dk.col(m) = weight.array() / (S.array() - LAMBDDD.col(m).array());
		}
		else if (cindex(m) == 1) {
			Dk.col(m) = weight.array() / (s.array() - LAMBDDD.col(m).array()) + weight.array() / (s.array() - LAMBDDD.col(m).conjugate().array());
			Dk.col(m + 1) = (H.array() * weight.array()) / (s.array() - LAMBDDD.col(m).array()) - (H.array() * weight.array()) / (s.array() - LAMBDDD.col(m).conjugate().array());
		}
	}


	for (int m = 0; m < Nc; m++) {
		Bblokk = weight.array() * f.row(m).transpose().array();
		ComplexMatrix Ablokk;
		if (asympflag == 1) {
			Ablokk.resize(Ns, (N * Nc + N));
			Ablokk.setZero();
			int ind1 = m * N + 1;
			int ind2 = (m + 1) * N;
			//将矩阵A的子矩阵块赋值给矩阵B的相应位置
			//B.block(startRow, startCol, numRows, numCols) = A.block(startRow, startCol, numRows, numCols);
			Ablokk.block(0, ind1 - 1, Ns, ind2 - ind1 + 1) = Dk.block(0, 0, Ns, Dk.cols());
			for (int col = 0; col < N; col++) {
				Ablokk.block(0, Nc * N + col, Ns, 1) = -(Dk.block(0, col, Ns, 1).array() * f.block(m, 0, 1, Ns).transpose().array());
			}
		}
		else if (asympflag == 2) {
			Ablokk.resize(Ns, (N + 1) * Nc + N);
			Ablokk.setZero();
			int ind1 = m * (N + 1) + 1;
			int ind2 = (m + 1) * (N + 1);

			Ablokk.block(0, ind1 - 1, Ns, ind2 - ind1) = Dk.block(0, 0, Ns, Dk.cols());
			Ablokk.block(0, ind2 - 1, Ns, 1) = weight.array() * Eigen::ArrayXd::Ones(Ns).array();

			for (int col = 0; col < N; col++) {
				Ablokk.block(0, Nc * (N + 1) + col, Ns, 1) = -(Dk.block(0, col, Ns, 1).array() * f.block(m, 0, 1, Ns).transpose().array());
			}
		}
		else if (asympflag == 3) {
			Ablokk.resize(Ns, (N + 2) * Nc + N);
			Ablokk.setZero();
			int ind1 = m * (N + 2) + 1;
			int ind2 = (m + 1) * (N + 2);

			Ablokk.block(0, ind1 - 1, Ns, ind2 - ind1 - 1) = Dk.block(0, 0, Ns, Dk.cols());
			Ablokk.block(0, ind2 - 2, Ns, 1) = weight.array() * Eigen::ArrayXd::Ones(Ns).array();
			Ablokk.block(0, ind2 - 1, Ns, 1) = weight.array() * s.array();

			for (int col = 0; col < N; col++) {
				Ablokk.block(0, Nc * (N + 2) + col, Ns, 1) = -(Dk.block(0, col, Ns, 1).array() * f.block(m, 0, 1, Ns).transpose().array());
			}
		}
		else {
			std::cout << "ERROR1!!!!!" << std::endl;
		}

		int ind1 = m * Ns + 1;
		int ind2 = (m + 1) * Ns;

		A.block(ind1 - 1, 0, ind2 - ind1 + 1, A.cols()) = Ablokk;
		b.block(ind1 - 1, 0, ind2 - ind1 + 1, b.cols()) = Bblokk;
		//std::cout << "第m个元素" << std::endl << m << std::endl;
		//std::cout << "Ablokk:" << std::endl << Ablokk << std::endl;
	}


	// Extract real and imaginary parts of A
	A1.block(Nrow, 0, Nrow, Ncol) = A.block(0, 0, Nrow, Ncol).imag();
	A1.block(0, 0, Nrow, Ncol) = A.block(0, 0, Nrow, Ncol).real();

	// Assign values to b1
	b1.block(Nrow, 0, Nrow, 1) = b.block(0, 0, Nrow, 1).imag();
	b1.block(0, 0, Nrow, 1) = b.block(0, 0, Nrow, 1).real();

	//std::cout << "内部A1:" << std::endl << A1 << std::endl;
	//std::cout << "内部b1:" << std::endl << b1 << std::endl;

	std::ofstream outputFileF("E:/VF_C++_2023_12_18/F.txt");
	if (outputFileF.is_open()) {
		for (int i = 0; i < f.rows(); i++) {
			for (int j = 0; j < f.cols(); j++) {
				outputFileF << f(i, j) << " ";
			}
			outputFileF << "\n";
		}
		outputFileF.close();
	}
	std::ofstream outputFileS("E:/VF_C++_2023_12_18/S.txt");
	if (outputFileS.is_open()) {
		for (int i = 0; i < s.rows(); i++) {
			for (int j = 0; j < s.cols(); j++) {
				outputFileS << s(i, j) << " ";
			}
			outputFileS << "\n";
		}
		outputFileS.close();
	}
}


void VectorFitting::solveEquation(Eigen::MatrixXd& A1, Eigen::MatrixXd& b1, Eigen::VectorXd& x) {
	Eigen::VectorXd Escale(A1.cols());
	for (int col = 0; col < A1.cols(); col++) {
		Escale(col) = A1.col(col).norm(); // Escale为行向量，计算向量A的2范数
		A1.col(col) /= Escale(col);
	}

	if (A1.cols() > 3) {
		Eigen::SparseMatrix<double> sparseA1 = A1.sparseView(); // 将矩阵A转化为稀疏矩阵的形式
		A1 = sparseA1;
	}

	//x = A1.colPivHouseholderQr().solve(b1); // 求解线性方程组

	//x.array() /= Escale.array().transpose(); // 将x除以Escale的转置
	x = A1.colPivHouseholderQr().solve(b1); // 求解线性方程组

	x.array() /= Escale.array().transpose(); // 将x除以Escale的转置
	//std::cout << "内部矩阵x的行数：" << x.rows() << std::endl;										 // 打印解向量x
	//std::cout << "内部x:\n" << x << std::endl;

	std::ofstream outputFileA1("E:/VF_C++_2023_12_18/A1.txt");
	if (outputFileA1.is_open()) {
		for (int i = 0; i < A1.rows(); i++) {
			for (int j = 0; j < A1.cols(); j++) {
				outputFileA1 << A1(i, j) << " ";
			}
			outputFileA1 << "\n";
		}
		outputFileA1.close();
	}

	std::ofstream outputFileb1("E:/VF_C++_2023_12_18/b1.txt");
	if (outputFileb1.is_open()) {
		for (int i = 0; i < b1.rows(); i++) {
			for (int j = 0; j < b1.cols(); j++) {
				outputFileb1 << b1(i, j) << " ";
			}
			outputFileb1 << "\n";
		}
		outputFileb1.close();
	}
}


void VectorFitting::calculate_residue(Eigen::VectorXd& x, Eigen::MatrixXd& CCC, ComplexMatrix& C, Eigen::RowVectorXd cindex, Eigen::MatrixXd& SERD, Eigen::MatrixXd& SERE)
{
	CCC.resize(N, Nc + 1);
	SERD.resize(Nc, 1);
	SERE.resize(Nc, 1);

	if (asympflag == 1) {
		for (int j = 0; j < Nc; j++) {
			CCC.col(j) = x.block((j * N), 0, N, 1);
		}
		CCC.col(Nc) = x.block(Nc * N, 0, N, 1);
	}
	else if (asympflag == 2) {
		for (int j = 0; j < Nc; j++) {
			CCC.col(j) = x.block((j * (N + 1)), 0, N, 1);
			SERD(j, 0) = x(j * (N + 1) + N);
		}
		CCC.col(Nc) = x.block(Nc * (N + 1), 0, N, 1);
	}
	else {
		for (int j = 0; j < Nc; j++) {
			CCC.col(j) = x.block((j * (N + 2)), 0, N, 1);
			SERD(j, 0) = x(j * (N + 2) + N);
			SERE(j, 0) = x(j * (N + 2) + N + 1);
		}
		CCC.col(Nc) = x.block(Nc * (N + 2), 0, N, 1);
	}
	CCC.transposeInPlace();
	for (int m = 0; m < N; m++) {
		if (cindex(m) == 0) {
			for (int n = 0; n < Nc + 1; n++) {
				C(n, m) = CCC(n, m);
			}
		}
		if (cindex(m) == 1) {
			for (int n = 0; n < Nc + 1; n++) {
				double real = CCC(n, m);
				double imag = CCC(n, m + 1);
				C(n, m) = std::complex<double>(real, imag);
				C(n, m + 1) = std::complex<double>(real, -imag);

			}
		}
	}
}

void VectorFitting::calculate_new_poles(const Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& LAMBD, Eigen::Array<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& C, Eigen::MatrixXd& B, ComplexMatrix& roetter, ComplexMatrix& SERA)
{

	int m = -1;
	Eigen::MatrixXd LAMBDDD(N, N);
	LAMBDDD.setZero();
	Eigen::MatrixXd CCC(1, N);//存储matlab中C(Nc+1,:)
	CCC.setZero();
	for (int n = 0; n < N; n++)
	{
		m = m + 1;
		if (m < N)
		{
			if (std::abs(LAMBD(m, m)) > std::abs(std::real(LAMBD(m, m))))
			{
				LAMBDDD(m + 1, m) = -std::imag(LAMBD(m, m));
				LAMBDDD(m, m + 1) = std::imag(LAMBD(m, m));
				LAMBDDD(m, m) = std::real(LAMBD(m, m));
				LAMBDDD(m + 1, m + 1) = LAMBDDD(m, m);
				B(m, 0) = 2.0;
				B(m + 1, 0) = 0.0;
				std::complex<double> koko = C(Nc, m);
				CCC(0, m) = std::real(koko);
				CCC(0, m + 1) = std::imag(koko);
				m = m + 1;
			}
			else {
				LAMBDDD(m, m) = std::real(LAMBD(m, m));
				std::complex<double> koko = C(Nc, m);
				CCC(0, m) = std::real(koko);
			}

			//std::cout << "std::abs(std::real(LAMBD(m, m)))：" << std::abs(std::real(LAMBD(m, m))) << std::endl;
		}

	}
	//std::cout << " LAMBDDD：" << LAMBDDD << std::endl;
	//std::cout << " B：" << B << std::endl;
	//std::cout << " CCC：" << CCC << std::endl;
	Eigen::MatrixXd ZER = LAMBDDD - B * CCC / 1.0;
	roetter = ZER.eigenvalues().eval().transpose();


	Eigen::Array<bool, Eigen::Dynamic, Eigen::Dynamic> unstables = roetter.real() > 0;

	if (kill == 1) {
		std::vector<std::complex<double>> newRoetter;
		for (int i = 0; i < roetter.size(); i++) {
			if (!unstables(i)) {
				newRoetter.push_back(roetter(i));
			}
		}
		Eigen::Map<Eigen::Array<std::complex<double>, 1, Eigen::Dynamic>> newRoetterMap(newRoetter.data(), 1, newRoetter.size());
		roetter = newRoetterMap;
	}
	else if (kill == 2) {
		for (int i = 0; i < roetter.size(); i++) {
			if (unstables(i)) {
				roetter(i) = std::complex<double>(roetter(i).real() - 2 * roetter(i).real(), roetter(i).imag());
			}
		}
	}
	std::sort(roetter.data(), roetter.data() + roetter.size(), [](auto& lhs, auto& rhs) {
		return std::norm(lhs) < std::norm(rhs);
		});

	N = roetter.size();
	//std::cout << "N" << N << std::endl;
	//std::cout << "内部roetter2:" << roetter << std::endl;
	// 将零点中的实数零点移到前面
	for (int n = 0; n < N; n++) {
		for (int m = n + 1; m < N; m++) {
			if (imag(roetter(0, m)) == 0 && imag(roetter(0, n)) != 0) {
				std::complex<double> trans = roetter(0, n);
				roetter(0, n) = roetter(0, m);
				roetter(0, m) = trans;
			}
		}
	}
	//std::cout << "内部roetter1:" << roetter << std::endl;
	int N1 = 0;
	for (int m = 0; m < N; m++) {
		if (imag(roetter(0, m)) == 0)
			N1 = m + 1;
	}


	if (N1 < N) {
		Eigen::Array<std::complex<double>, 1, Eigen::Dynamic> sortedRoetter = roetter.block(0, N1, 1, N - N1);

		std::sort(sortedRoetter.data(), sortedRoetter.data() + sortedRoetter.size(), [](const auto& c1, const auto& c2) {
			double abs1 = std::abs(c1);
			double abs2 = std::abs(c2);
			if (abs1 == abs2) {
				if (c1.imag() < 0 && c2.imag() >= 0)
					return true;
				if (c1.imag() >= 0 && c2.imag() < 0)
					return false;
			}
			return abs1 < abs2;
			});

		roetter.block(0, N1, 1, N - N1) = sortedRoetter;
	}

	//std::cout << "N1：" << N1 << std::endl;
	//std::cout << "内部roetter:" << roetter << std::endl;
	int N2 = N - N1;
	Eigen::MatrixXcd H(1, N);
	H.setConstant(std::complex<double>(0, 1));
	roetter = roetter - 2.0 * H.array() * roetter.imag();
	SERA = roetter.transpose();

	//至此，新的极点已经确定
}

void VectorFitting::Building_system_matrix_2(int Ns, ComplexMatrix f, ComplexMatrix s, Eigen::RowVectorXd cindex, ComplexMatrix A, ComplexMatrix b, Eigen::MatrixXd& A1, Eigen::MatrixXd& b1, int Ncol2,
	const Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>& LAMBD)
{
	ComplexMatrix Dk(Ns, N); // Dk用于保存每个元素部分和多项式带入各频率点对应方程组中的真分数项矩阵
	Dk.setZero(); // 初始化Dk为零矩阵

	//ComplexMatrix Bblokk(Ns, 1);
	//Bblokk.setZero();

	Eigen::MatrixXcd H(Ns, 1);
	H.setConstant(std::complex<double>(0, 1));

	ComplexMatrix LAMBDDD(Ns, N);
	LAMBDDD.setZero();
	//std::cout << " cindex：" << cindex << std::endl;
	for (int m = 0; m < N; m++) {
		for (int i = 0; i < Ns; i++) {
			LAMBDDD(i, m) = LAMBD(m, m); // LAMBD(m, m)
		}
		if (cindex(m) == 0) {

			Dk.col(m) = weight.array() / (s.array() - LAMBDDD.col(m).array());
		}
		else if (cindex(m) == 1) {

			Dk.col(m) = weight.array() / (s.array() - LAMBDDD.col(m).array()) + weight.array() / (s.array() - LAMBDDD.col(m).conjugate().array());
			Dk.col(m + 1) = (H.array() * weight.array()) / (s.array() - LAMBDDD.col(m).array()) - (H.array() * weight.array()) / (s.array() - LAMBDDD.col(m).conjugate().array());
		}
	}
	//std::cout << " LAMBDDD：" << LAMBDDD << std::endl;


	if (asympflag == 1) {
		// asympflag为1，d=0;e=0
		A.block(0, 0, Ns, N) = Dk.block(0, 0, Ns, N);
	}
	else if (asympflag == 2) {
		A.block(0, 0, Ns, N) = Dk.block(0, 0, Ns, N);
		A.block(0, N, Ns, 1) = weight;
	}
	else {
		A.block(0, 0, Ns, N) = Dk.block(0, 0, Ns, N);
		A.block(0, N, Ns, 1) = weight;
		A.block(0, N + 1, Ns, 1) = weight.array() * s.array();

	}

	for (int m = 0; m < Nc; m++) {
		b.block(0, m, Ns, 1) = weight.array() * f.row(m).transpose().array();
	}


	// Extract real and imaginary parts of A
	A1.block(Ns, 0, Ns, Ncol2) = A.block(0, 0, Ns, Ncol2).imag();
	A1.block(0, 0, Ns, Ncol2) = A.block(0, 0, Ns, Ncol2).real();

	// Assign values to b1
	b1.block(Ns, 0, Ns, Nc) = b.block(0, 0, Ns, Nc).imag();
	b1.block(0, 0, Ns, Nc) = b.block(0, 0, Ns, Nc).real();
	//std::cout << " A1：" << A1 << std::endl;
	//std::cout << "b1：" << b1 << std::endl;
	//至此，实虚部分开的A1、b1矩阵构建完成


}

Eigen::MatrixXd VectorFitting::solveEquation2(Eigen::MatrixXd& A, const Eigen::MatrixXd& BB)
{
	Eigen::VectorXd Escale(A.cols());

	for (int col = 0; col < A.cols(); ++col) {
		Escale(col) = A.col(col).norm(); // calculate the norm of each column
		A.col(col) /= Escale(col); // divide the column by its norm
	}
	//std::cout << "第二阶段A:\n" << A << std::endl;
	//std::cout << "第二阶段BB:\n" << BB << std::endl;
	Eigen::MatrixXd X = A.colPivHouseholderQr().solve(BB);
	//Eigen::MatrixXd X = A.fullPivLu().solve(BB); // solve the linear system
	for (int n = 0; n < Nc; ++n) {
		X.col(n).array() /= Escale.array().transpose(); // divide each column by the transpose of Escale

	}
	return X;
}