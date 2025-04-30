#include "stdafx.h"
#include "C_DispersionUser.h"
#include "VectorFitting.h"
#include "Source/SettingModule/S_SettingDataContainer.h"
#include "Source/ProjectModule/File/S_TempFileManager.h"
#include "Source/ResultModule/S_ResultDataContainer.h"
#include "Source/Utils/Common/Tool4Sean.h"

C_DispersionUser::C_DispersionUser() : C_UndoDataBase() {
}

C_DispersionUser::~C_DispersionUser() {
}

/*-------------------------------------------------
		参数 - 设置材料名称
*/
void C_DispersionUser::setName(QString name)
{
	m_Name = name;
}

/*-------------------------------------------------
		参数 - 获取材料名称
*/
QString C_DispersionUser::getName()
{
	return m_Name;
}

/*-------------------------------------------------
		访问器 - 获取Fitting scheme下拉框选项
*/
QStringList C_DispersionUser::getFittingSchemeList()
{
	return QStringList() << QObject::tr("Conductivity") << QObject::tr("nth order");
}

/*-------------------------------------------------
		访问器 - 获取Data format下拉框选项
*/
QStringList C_DispersionUser::getDataFormatList()
{
	return QStringList() << QObject::tr("(Real, Imag)") << QObject::tr("(Real, Tand)");
}

/*-------------------------------------------------
		组件 - 下拉框 - Freq unit选项
*/
QStringList C_DispersionUser::getFreqUnitList(){
	return QStringList() << QObject::tr("Hz") << QObject::tr("kHz") 
		<< QObject::tr("MHz") << QObject::tr("GHz") 
		<< QObject::tr("THz") << QObject::tr("PHz");
}

/*-------------------------------------------------
		组件 - 输入框 - 设置Max order输入值
*/
void C_DispersionUser::setMaxOrder(int value)
{
	m_MaxOrder = value;
}

/*-------------------------------------------------
		组件 - 输入框 - 获取Max order输入值
*/
int C_DispersionUser::getMaxOrder()
{
	return m_MaxOrder;
}

/*-------------------------------------------------
		组件 - 输入框 - 设置Error limit输入值
*/
void C_DispersionUser::setErrorLimit(double value)
{
	m_ErrorLimit = value;
}

/*-------------------------------------------------
		组件 - 输入框 - 获取Error limit输入值
*/
double C_DispersionUser::getErrorLimit()
{
	return m_ErrorLimit;
}

/*-------------------------------------------------
		参数 - 输入框 - 设置Used order输入值
*/
void C_DispersionUser::setUsedOrder(int value)
{
	m_UsedOrder = value;
}

/*-------------------------------------------------
		参数 - 输入框 - 获取Used order输入值
*/
int C_DispersionUser::getUsedOrder()
{
	return m_UsedOrder;
}

/*-------------------------------------------------
		组件 - 获取details弹出内容字符串
*/
QString C_DispersionUser::getDetailsStr(){
	QStringList firstStr;
	for (int i = 0; i < m_FreqPole1st.size(); ++i)
	{
		firstStr.append(QString("\t%1) Real pole : freq. = %2 GHz\n"
								"\t     Partial fraction gain = %3")
							.arg(i + 1)
							.arg(m_FreqPole1st.at(i))
							.arg(m_Gain1.at(i)));
	}

	QStringList secondStr;
	for (int i = 0; i < m_FreqPole2nd.size(); ++i)
	{
		secondStr.append(QString("\t%1) Complex pole - freq. = %2 GHz, Q = %3\n"
								 "\t     Real zero : freq.(negative) = %4 GHz\n"
								 "\t     Partial fraction gain = %5")
							 .arg(i + 1)
							 .arg(m_FreqPole2nd.at(i))
							 .arg(m_Q.at(i))
							 .arg(m_FreqZero.at(i))
							 .arg(m_Gain2.at(i)));
	}

	QString content = QString("---------------------------------------\n"
							  "Electric isotropic material [material1]\n"
							  "Nth order dispersion fitting information\n"
							  "---------------------------------------\n\n"
							  "Order %1 (%2 first and %3 second order model) - Error = %4\n"
							  "Epsilon infinity = %5\n\n"
							  "1st order model contribution:\n"
							  "%6\n\n"
							  "2nd order model contribution:\n"
							  "%7\n")
						  .arg(m_FreqPole1st.size() + m_FreqPole2nd.size() * 2)
						  .arg(m_FreqPole1st.size())
						  .arg(m_FreqPole2nd.size() * 2)
						  .arg(m_Error)
						  .arg(m_EpsilonInfinity)
						  .arg(firstStr.join("\n"))
						  .arg(secondStr.join("\n"));

	return content;
}

/*-------------------------------------------------
		组件 - 生成曲线txt文件
*/
void C_DispersionUser::genUserDispersionTxt(bool update)
{
	auto freqData = S_SettingDataContainer::getInstance()->getDataFrequency();
	double freqMin = freqData.getFreqMin();
	double freqMax = freqData.getFreqMax();

	if (freqMax > 0 && freqMax > freqMin)
	{
		// 采样点2000
		int pointCount = 2000;

		// 清理数据，因为求解器算法使用的是push_back
		m_FreqPole1st.clear();
		m_FreqPole2nd.clear();
		m_Q.clear();
		m_FreqZero.clear();
		m_Gain1.clear();
		m_Gain2.clear();

		m_Alpha0.clear();
		m_Belta0.clear();
		m_Gamma0.clear();
		m_Gamma1.clear();
		m_Delta0.clear();
		m_Delta1.clear();

		m_EpsRealData.clear();
		m_EpsImagData.clear();
		m_EpsRealFit.clear();
		m_EpsImagFit.clear();

		// 开始计算
		VectorFitting vf;
		vf.CalcResults(m_ModelData, m_MaxOrder, m_ErrorLimit);
		vf.GetFittingData(m_FreqPole1st, m_FreqPole2nd, m_Q, m_FreqZero, m_Gain1, m_Gain2, m_EpsilonInfinity, m_Error);

		// 从result获取曲线数据，实部和虚部分开
		for (int row = 0; row < m_ModelData.size(); ++row)
		{
			m_EpsRealData.append(std::make_pair(m_ModelData.at(row).at(0), m_ModelData.at(row).at(1)));
			m_EpsImagData.append(std::make_pair(m_ModelData.at(row).at(0), m_ModelData.at(row).at(2)));
		}

		// 一阶
		for (int n = 0; n < m_FreqPole1st.size(); n++)
		{
			m_Alpha0.push_back(2 * M_PI * m_FreqPole1st[n]);
			m_Belta0.push_back(m_Gain1[n] * m_Alpha0[n]);
		}

		// 二阶
		for (int n = 0; n < m_FreqPole2nd.size(); n++)
		{
			m_Delta0.push_back(pow(2 * M_PI * m_FreqPole2nd[n], 2));
			m_Delta1.push_back(2 * M_PI * m_FreqPole2nd[n] / m_Q[n]);
			m_Gamma0.push_back(m_Delta0[n] * m_Gain2[n]);
			m_Gamma1.push_back(m_Gamma0[n] / (2 * M_PI * m_FreqZero[n]));
		}

		// > 【频率值】-给参数赋值（单位固定Hz）
		//		（用于色散计算，色散计算单位为Hz，画对应的波形-这里给出x y坐标对应数据）
		double diff = (freqMax - freqMin) / pointCount;
		for (int n = 0; n < pointCount; n++)
		{
			double freq = freqMin + n * diff; // x坐标
			double omega = freq;
			std::complex<double> epsilon = m_EpsilonInfinity;
			for (int m = 0; m < m_Alpha0.size(); m++)
			{
				epsilon += m_Belta0[m] / std::complex<double>(m_Alpha0[m], omega);
			}
			for (int m = 0; m < m_Delta0.size(); m++)
			{
				epsilon += std::complex<double>(m_Gamma0[m], m_Gamma1[m] * omega) / std::complex<double>(m_Delta0[m] - pow(omega, 2), omega * m_Delta1[m]);
			}

			// 添加到数据列表中
			m_EpsRealFit.append(std::make_pair(freq, epsilon.real()));
			m_EpsImagFit.append(std::make_pair(freq, epsilon.imag()));
		}

		if (m_EpsRealData.count() > 0 && m_EpsImagData.count() > 0 && m_EpsRealFit.count() > 0 && m_EpsImagFit.count() > 0)
		{
			// 单位字符串传递给x轴
			C_SettingUnits units = S_SettingDataContainer::getInstance()->getDataUnits();
			QString unitStr = units.getUnitAllType_frequency().at(units.getCurrentIndex_frequency());

			// 写入到txt文件
			QString bufferPath = S_TempFileManager::getInstance()->getTempFileUrl();
			QString materialPath = QString("OutputResults/1D_Results/Materials/%1/Dispersive/").arg(m_Name);
			QString txtDir = QDir(bufferPath).filePath(materialPath);

			QString dataRealPath = QDir(txtDir).filePath("Eps'(Data list).txt");
			QString dataImagPath = QDir(txtDir).filePath("Eps''(Data list).txt");
			QString fitRealPath = QDir(txtDir).filePath("Eps'(Fit).txt");
			QString fitImagPath = QDir(txtDir).filePath("Eps''(Fit).txt");

			Tool4Sean::writeDoubleListToTxt(dataRealPath, m_EpsRealData, unitStr);
			Tool4Sean::writeDoubleListToTxt(dataImagPath, m_EpsImagData, unitStr);
			Tool4Sean::writeDoubleListToTxt(fitRealPath, m_EpsRealFit, unitStr);
			Tool4Sean::writeDoubleListToTxt(fitImagPath, m_EpsImagFit, unitStr);

			// 记录当前使用阶数
			m_UsedOrder = m_FreqPole1st.size() + m_FreqPole2nd.size() * 2;

			// 加载色散文件到左侧树
			S_ResultDataContainer::getInstance()->loadDispersion_loadResult_1D(m_Name, update);
		}
	}
}

/*-------------------------------------------------
		参数 - 设置FreqPole1st
*/
void C_DispersionUser::setFreqPole1st(std::vector<double> list)
{
	m_FreqPole1st = list;
}

/*-------------------------------------------------
		参数 - 获取FreqPole1st
*/
std::vector<double> C_DispersionUser::getFreqPole1st()
{
	return m_FreqPole1st;
}

/*-------------------------------------------------
		参数 - 设置FreqPole2nd
*/
void C_DispersionUser::setFreqPole2nd(std::vector<double> list)
{
	m_FreqPole2nd = list;
}

/*-------------------------------------------------
		参数 - 获取FreqPole2nd
*/
std::vector<double> C_DispersionUser::getFreqPole2nd()
{
	return m_FreqPole2nd;
}

/*-------------------------------------------------
		参数 - 设置Q
*/
void C_DispersionUser::setQ(std::vector<double> list)
{
	m_Q = list;
}

/*-------------------------------------------------
		参数 - 获取Q
*/
std::vector<double> C_DispersionUser::getQ()
{
	return m_Q;
}

/*-------------------------------------------------
		参数 - 设置FreqZero
*/
void C_DispersionUser::setFreqZero(std::vector<double> list)
{
	m_FreqZero = list;
}

/*-------------------------------------------------
		参数 - 获取FreqZero
*/
std::vector<double> C_DispersionUser::getFreqZero()
{
	return m_FreqZero;
}

/*-------------------------------------------------
		参数 - 设置Gain1
*/
void C_DispersionUser::setGain1(std::vector<double> list)
{
	m_Gain1 = list;
}

/*-------------------------------------------------
		参数 - 获取Gain1
*/
std::vector<double> C_DispersionUser::getGain1()
{
	return m_Gain1;
}

/*-------------------------------------------------
		参数 - 设置Gain2
*/
void C_DispersionUser::setGain2(std::vector<double> list)
{
	m_Gain2 = list;
}

/*-------------------------------------------------
		参数 - 获取Gain2
*/
std::vector<double> C_DispersionUser::getGain2()
{
	return m_Gain2;
}

/*-------------------------------------------------
		参数 - 设置EpsilonInfinity
*/
void C_DispersionUser::setEpsilonInfinity(double value)
{
	m_EpsilonInfinity = value;
}

/*-------------------------------------------------
		参数 - 获取EpsilonInfinity
*/
double C_DispersionUser::getEpsilonInfinity()
{
	return m_EpsilonInfinity;
}

/*-------------------------------------------------
		参数 - 设置DetailsError
*/
void C_DispersionUser::setError(double value)
{
	m_Error = value;
}

/*-------------------------------------------------
		参数 - 获取DetailsError
*/
double C_DispersionUser::getError()
{
	return m_Error;
}

/*-------------------------------------------------
		参数 - 设置Alpha0
*/
void C_DispersionUser::setAlpha0(std::vector<double> list)
{
	m_Alpha0 = list;
}

/*-------------------------------------------------
		参数 - 获取Alpha0
*/
std::vector<double> C_DispersionUser::getAlpha0()
{
	return m_Alpha0;
}

/*-------------------------------------------------
		参数 - 设置Belta0
*/
void C_DispersionUser::setBelta0(std::vector<double> list)
{
	m_Belta0 = list;
}

/*-------------------------------------------------
		参数 - 获取Belta0
*/
std::vector<double> C_DispersionUser::getBelta0()
{
	return m_Belta0;
}

/*-------------------------------------------------
		参数 - 设置Gamma0
*/
void C_DispersionUser::setGamma0(std::vector<double> list)
{
	m_Gamma0 = list;
}

/*-------------------------------------------------
		参数 - 获取Gamma0
*/
std::vector<double> C_DispersionUser::getGamma0()
{
	return m_Gamma0;
}

/*-------------------------------------------------
		参数 - 设置Gamma1
*/
void C_DispersionUser::setGamma1(std::vector<double> list)
{
	m_Gamma1 = list;
}

/*-------------------------------------------------
		参数 - 获取Gamma1
*/
std::vector<double> C_DispersionUser::getGamma1()
{
	return m_Gamma1;
}

/*-------------------------------------------------
		参数 - 设置Delta0
*/
void C_DispersionUser::setDelta0(std::vector<double> list)
{
	m_Delta0 = list;
}

/*-------------------------------------------------
		参数 - 获取Delta0
*/
std::vector<double> C_DispersionUser::getDelta0()
{
	return m_Delta0;
}

/*-------------------------------------------------
		参数 - 设置Delta1
*/
void C_DispersionUser::setDelta1(std::vector<double> list)
{
	m_Delta1 = list;
}

/*-------------------------------------------------
		参数 - 获取Delta1
*/
std::vector<double> C_DispersionUser::getDelta1()
{
	return m_Delta1;
}

/*-------------------------------------------------
		参数 - 设置EpsRealData
*/
void C_DispersionUser::setEpsRealData(QVector<std::pair<double, double>> list)
{
	m_EpsRealData = list;
}

/*-------------------------------------------------
		参数 - 获取EpsRealData
*/
QVector<std::pair<double, double>> C_DispersionUser::getEpsRealData()
{
	return m_EpsRealData;
}

/*-------------------------------------------------
		参数 - 设置EpsImagData
*/
void C_DispersionUser::setEpsImagData(QVector<std::pair<double, double>> list)
{
	m_EpsImagData = list;
}

/*-------------------------------------------------
		参数 - 获取EpsImagData
*/
QVector<std::pair<double, double>> C_DispersionUser::getEpsImagData()
{
	return m_EpsImagData;
}

/*-------------------------------------------------
		参数 - 设置EpsRealFit
*/
void C_DispersionUser::setEpsRealFit(QVector<std::pair<double, double>> list)
{
	m_EpsRealFit = list;
}

/*-------------------------------------------------
		参数 - 获取EpsRealFit
*/
QVector<std::pair<double, double>> C_DispersionUser::getEpsRealFit()
{
	return m_EpsRealFit;
}

/*-------------------------------------------------
		参数 - 设置EpsImagFit
*/
void C_DispersionUser::setEpsImagFit(QVector<std::pair<double, double>> list)
{
	m_EpsImagFit = list;
}

/*-------------------------------------------------
		参数 - 获取EpsImagFit
*/
QVector<std::pair<double, double>> C_DispersionUser::getEpsImagFit()
{
	return m_EpsImagFit;
}

/*-------------------------------------------------
		参数 - 获取存储的model数据
*/
std::vector<std::vector<double>> &C_DispersionUser::getModelData()
{
	return m_ModelData;
}

/*-------------------------------------------------
		参数 - 设置存储的model数据
*/
void C_DispersionUser::setModelData(std::vector<std::vector<double>> result)
{
	m_ModelData = result;
}

/*-------------------------------------------------
		访问器 - 获取子类类名
*/
QString C_DispersionUser::get__CLASS_NAME__() {
	return "C_DispersionUser";
}

/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
QString C_DispersionUser::get__CLASS_NAME__Chinese() {
	return "自定义色散";
}

/*-------------------------------------------------
		访问器 - 获取所有继承的类名
*/
QStringList C_DispersionUser::get__CLASS_INHERITED_LIST__(){
	return C_UndoDataBase::get__CLASS_INHERITED_LIST__() << "C_DispersionUser";
}

/*-------------------------------------------------
		访问器 - 是否继承了某类
*/
bool C_DispersionUser::get__CLASS_IS_INSTANCE_OF__(QString class_name){
	return this->get__CLASS_INHERITED_LIST__().contains(class_name);
}

/*-------------------------------------------------
		访问器 - 原型复制对象
*/
C_DispersionUser C_DispersionUser::clone() {
	return C_DispersionUser(*this);
}

QSharedPointer<C_DispersionUser> C_DispersionUser::clonePtr() {
	return QSharedPointer<C_DispersionUser>(new C_DispersionUser(*this));
}

C_UndoDataBasePtr C_DispersionUser::cloneParentPtr(){
	return this->clonePtr();
}

/*-------------------------------------------------
		复制对象
*/
void C_DispersionUser::copyFrom(C_DispersionUser* a) {
	// > 参数
}

/*-------------------------------------------------
		运算符重载
*/
const bool C_DispersionUser::operator== (const C_DispersionUser& a)const {
	return C_UndoDataBase::operator==(a);
}

/*-------------------------------------------------
		空判断
*/
bool C_DispersionUser::isNull() {
	return C_UndoDataBase::isNull();
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_DispersionUser::getJsonObject_undoChildData() {
	QJsonObject obj;

	// > 下拉框下标
	obj.insert("fittingIndex", m_FittingIndex);
	obj.insert("dataFormatIndex", m_DataFormatIndex);
	obj.insert("freqUnitIndex", m_FreqUnitIndex);

	// > 输入框数据
	obj.insert("maxOrder", m_MaxOrder);
	obj.insert("errorLimit", m_ErrorLimit);
	obj.insert("usedOrder", m_UsedOrder);
	obj.insert("error", m_Error);

	// > 材料名称
	obj.insert("name", m_Name);

	// > 求解器相关
	obj.insert("epsilonInfinity", m_EpsilonInfinity);
	obj.insert("detailsError", m_Error);

	QJsonArray freqPole1stArray;
	for (const auto &value : m_FreqPole1st)
	{
		freqPole1stArray.append(value);
	}
	obj.insert("freqPole1st", freqPole1stArray);

	QJsonArray freqPole2ndArray;
	for (const auto &value : m_FreqPole2nd)
	{
		freqPole2ndArray.append(value);
	}
	obj.insert("freqPole2nd", freqPole2ndArray);

	QJsonArray QArray;
	for (const auto &value : m_Q)
	{
		QArray.append(value);
	}
	obj.insert("Q", QArray);

	QJsonArray FreqZeroArray;
	for (const auto &value : m_FreqZero)
	{
		FreqZeroArray.append(value);
	}
	obj.insert("freqZero", FreqZeroArray);

	QJsonArray Gain1Array;
	for (const auto &value : m_Gain1)
	{
		Gain1Array.append(value);
	}
	obj.insert("gain1", Gain1Array);

	QJsonArray Gain2Array;
	for (const auto &value : m_Gain2)
	{
		Gain2Array.append(value);
	}
	obj.insert("gain2", Gain2Array);

	QJsonArray alpha0Array;
	for (const auto &value : m_Alpha0)
	{
		alpha0Array.append(value);
	}
	obj.insert("alpha0", alpha0Array);

	QJsonArray belta0Array;
	for (const auto &value : m_Belta0)
	{
		belta0Array.append(value);
	}
	obj.insert("belta0", belta0Array);

	QJsonArray gamma0Array;
	for (const auto &value : m_Gamma0)
	{
		gamma0Array.append(value);
	}
	obj.insert("gamma0", gamma0Array);

	QJsonArray gamma1Array;
	for (const auto &value : m_Gamma1)
	{
		gamma1Array.append(value);
	}
	obj.insert("gamma1", gamma1Array);

	QJsonArray delta0Array;
	for (const auto &value : m_Delta0)
	{
		delta0Array.append(value);
	}
	obj.insert("delta0", delta0Array);

	QJsonArray delta1Array;
	for (const auto &value : m_Delta1)
	{
		delta1Array.append(value);
	}
	obj.insert("delta1", delta1Array);

	QJsonArray dataArray;
	for (const auto &innerVector : m_ModelData)
	{
		QJsonArray innerArray;
		for (const auto &value : innerVector)
		{
			innerArray.append(value);
		}
		dataArray.append(innerArray);
	}
	obj.insert("modelData", dataArray);

	return obj;
}

/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_DispersionUser::setJsonObject_undoChildData(QJsonObject obj) {
	// > 下拉框下标
	m_FittingIndex = obj.value("fittingIndex").toInt();
	m_DataFormatIndex = obj.value("dataFormatIndex").toInt();
	m_FreqUnitIndex = obj.value("freqUnitIndex").toInt();

	// > 输入框数据
	m_MaxOrder = obj.value("maxOrder").toInt();
	m_ErrorLimit = obj.value("errorLimit").toDouble();
	m_UsedOrder = obj.value("usedOrder").toInt();
	m_Error = obj.value("error").toDouble();

	// > 材料名称
	m_Name = obj.value("name").toString();

	// > 求解器相关
	m_EpsilonInfinity = obj.value("epsilonInfinity").toDouble();
	m_Error = obj.value("detailsError").toDouble();

	m_FreqPole1st.clear();
	QJsonArray freqPole1stArray = obj.value("freqPole1st").toArray();
	for (const QJsonValue &value : freqPole1stArray)
	{
		m_FreqPole1st.push_back(value.toDouble());
	}

	m_FreqPole2nd.clear();
	QJsonArray freqPole2ndArray = obj.value("freqPole2nd").toArray();
	for (const QJsonValue &value : freqPole2ndArray)
	{
		m_FreqPole2nd.push_back(value.toDouble());
	}

	m_Q.clear();
	QJsonArray QArray = obj.value("Q").toArray();
	for (const QJsonValue &value : QArray)
	{
		m_Q.push_back(value.toDouble());
	}

	m_FreqZero.clear();
	QJsonArray FreqZeroArray = obj.value("freqZero").toArray();
	for (const QJsonValue &value : FreqZeroArray)
	{
		m_FreqZero.push_back(value.toDouble());
	}

	m_Gain1.clear();
	QJsonArray Gain1Array = obj.value("gain1").toArray();
	for (const QJsonValue &value : Gain1Array)
	{
		m_Gain1.push_back(value.toDouble());
	}

	m_Gain2.clear();
	QJsonArray Gain2Array = obj.value("gain2").toArray();
	for (const QJsonValue &value : Gain2Array)
	{
		m_Gain2.push_back(value.toDouble());
	}

	m_Alpha0.clear();
	QJsonArray alpha0Array = obj.value("alpha0").toArray();
	for (const QJsonValue &value : alpha0Array)
	{
		m_Alpha0.push_back(value.toDouble());
	}

	m_Belta0.clear();
	QJsonArray belta0Array = obj.value("belta0").toArray();
	for (const QJsonValue &value : belta0Array)
	{
		m_Belta0.push_back(value.toDouble());
	}

	m_Gamma0.clear();
	QJsonArray gamma0Array = obj.value("gamma0").toArray();
	for (const QJsonValue &value : gamma0Array)
	{
		m_Gamma0.push_back(value.toDouble());
	}

	m_Gamma1.clear();
	QJsonArray gamma1Array = obj.value("gamma1").toArray();
	for (const QJsonValue &value : gamma1Array)
	{
		m_Gamma1.push_back(value.toDouble());
	}

	m_Delta0.clear();
	QJsonArray delta0Array = obj.value("delta0").toArray();
	for (const QJsonValue &value : delta0Array)
	{
		m_Delta0.push_back(value.toDouble());
	}

	m_Delta1.clear();
	QJsonArray delta1Array = obj.value("delta1").toArray();
	for (const QJsonValue &value : delta1Array)
	{
		m_Delta1.push_back(value.toDouble());
	}

	if (obj.contains("modelData"))
	{
		QJsonArray dataArray = obj["modelData"].toArray();
		for (const QJsonValue &innerVal : dataArray)
		{
			std::vector<double> innerVector;

			if (innerVal.isArray())
			{
				QJsonArray innerArray = innerVal.toArray();

				for (const QJsonValue &value : innerArray)
				{
					if (value.isDouble())
					{
						innerVector.push_back(value.toDouble());
					}
				}

				m_ModelData.push_back(innerVector);
			}
		}
	}
}