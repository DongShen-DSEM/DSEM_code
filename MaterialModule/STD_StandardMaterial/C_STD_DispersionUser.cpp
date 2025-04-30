#include "stdafx.h"
#include "C_STD_DispersionUser.h"


C_STD_DispersionUser::C_STD_DispersionUser(){
}
C_STD_DispersionUser::~C_STD_DispersionUser() {
}

/*-------------------------------------------------
		参数 - 设置材料名称
*/
void C_STD_DispersionUser::setName(std::string name)
{
	m_Name = name;
}

/*-------------------------------------------------
		参数 - 获取材料名称
*/
std::string C_STD_DispersionUser::getName()
{
	return m_Name;
}

/*-------------------------------------------------
		参数 - 设置FreqPole1st
*/
void C_STD_DispersionUser::setFreqPole1st(std::vector<double> list)
{
	m_FreqPole1st = list;
}

/*-------------------------------------------------
		参数 - 获取FreqPole1st
*/
std::vector<double> C_STD_DispersionUser::getFreqPole1st()
{
	return m_FreqPole1st;
}

/*-------------------------------------------------
		参数 - 设置FreqPole2nd
*/
void C_STD_DispersionUser::setFreqPole2nd(std::vector<double> list)
{
	m_FreqPole2nd = list;
}

/*-------------------------------------------------
		参数 - 获取FreqPole2nd
*/
std::vector<double> C_STD_DispersionUser::getFreqPole2nd()
{
	return m_FreqPole2nd;
}

/*-------------------------------------------------
		参数 - 设置Q
*/
void C_STD_DispersionUser::setQ(std::vector<double> list)
{
	m_Q = list;
}

/*-------------------------------------------------
		参数 - 获取Q
*/
std::vector<double> C_STD_DispersionUser::getQ()
{
	return m_Q;
}

/*-------------------------------------------------
		参数 - 设置FreqZero
*/
void C_STD_DispersionUser::setFreqZero(std::vector<double> list)
{
	m_FreqZero = list;
}

/*-------------------------------------------------
		参数 - 获取FreqZero
*/
std::vector<double> C_STD_DispersionUser::getFreqZero()
{
	return m_FreqZero;
}

/*-------------------------------------------------
		参数 - 设置Gain1
*/
void C_STD_DispersionUser::setGain1(std::vector<double> list)
{
	m_Gain1 = list;
}

/*-------------------------------------------------
		参数 - 获取Gain1
*/
std::vector<double> C_STD_DispersionUser::getGain1()
{
	return m_Gain1;
}

/*-------------------------------------------------
		参数 - 设置Gain2
*/
void C_STD_DispersionUser::setGain2(std::vector<double> list)
{
	m_Gain2 = list;
}

/*-------------------------------------------------
		参数 - 获取Gain2
*/
std::vector<double> C_STD_DispersionUser::getGain2()
{
	return m_Gain2;
}

/*-------------------------------------------------
		参数 - 设置EpsilonInfinity
*/
void C_STD_DispersionUser::setEpsilonInfinity(double value)
{
	m_EpsilonInfinity = value;
}

/*-------------------------------------------------
		参数 - 获取EpsilonInfinity
*/
double& C_STD_DispersionUser::getEpsilonInfinity()
{
	return m_EpsilonInfinity;
}

/*-------------------------------------------------
		参数 - 设置DetailsError
*/
void C_STD_DispersionUser::setDetailsError(double value)
{
	m_DetailsError = value;
}

/*-------------------------------------------------
		参数 - 获取DetailsError
*/
double& C_STD_DispersionUser::getDetailsError()
{
	return m_DetailsError;
}

/*-------------------------------------------------
		参数 - 设置Alpha0
*/
void C_STD_DispersionUser::setAlpha0(std::vector<double> list)
{
	m_Alpha0 = list;
}

/*-------------------------------------------------
		参数 - 获取Alpha0
*/
std::vector<double> C_STD_DispersionUser::getAlpha0()
{
	return m_Alpha0;
}

/*-------------------------------------------------
		参数 - 设置Belta0
*/
void C_STD_DispersionUser::setBelta0(std::vector<double> list)
{
	m_Belta0 = list;
}

/*-------------------------------------------------
		参数 - 获取Belta0
*/
std::vector<double> C_STD_DispersionUser::getBelta0()
{
	return m_Belta0;
}

/*-------------------------------------------------
		参数 - 设置Gamma0
*/
void C_STD_DispersionUser::setGamma0(std::vector<double> list)
{
	m_Gamma0 = list;
}

/*-------------------------------------------------
		参数 - 获取Gamma0
*/
std::vector<double> C_STD_DispersionUser::getGamma0()
{
	return m_Gamma0;
}

/*-------------------------------------------------
		参数 - 设置Gamma1
*/
void C_STD_DispersionUser::setGamma1(std::vector<double> list)
{
	m_Gamma1 = list;
}

/*-------------------------------------------------
		参数 - 获取Gamma1
*/
std::vector<double> C_STD_DispersionUser::getGamma1()
{
	return m_Gamma1;
}

/*-------------------------------------------------
		参数 - 设置Delta0
*/
void C_STD_DispersionUser::setDelta0(std::vector<double> list)
{
	m_Delta0 = list;
}

/*-------------------------------------------------
		参数 - 获取Delta0
*/
std::vector<double> C_STD_DispersionUser::getDelta0()
{
	return m_Delta0;
}

/*-------------------------------------------------
		参数 - 设置Delta1
*/
void C_STD_DispersionUser::setDelta1(std::vector<double> list)
{
	m_Delta1 = list;
}

/*-------------------------------------------------
		参数 - 获取Delta1
*/
std::vector<double> C_STD_DispersionUser::getDelta1()
{
	return m_Delta1;
}

/*-------------------------------------------------
		参数 - 设置EpsRealData
*/
void C_STD_DispersionUser::setEpsRealData(vector<std::pair<double, double>> list)
{
	m_EpsRealData = list;
}

/*-------------------------------------------------
		参数 - 获取EpsRealData
*/
vector<std::pair<double, double>> C_STD_DispersionUser::getEpsRealData()
{
	return m_EpsRealData;
}

/*-------------------------------------------------
		参数 - 设置EpsImagData
*/
void C_STD_DispersionUser::setEpsImagData(vector<std::pair<double, double>> list)
{
	m_EpsImagData = list;
}

/*-------------------------------------------------
		参数 - 获取EpsImagData
*/
vector<std::pair<double, double>> C_STD_DispersionUser::getEpsImagData()
{
	return m_EpsImagData;
}

/*-------------------------------------------------
		参数 - 设置EpsRealFit
*/
void C_STD_DispersionUser::setEpsRealFit(vector<std::pair<double, double>> list)
{
	m_EpsRealFit = list;
}

/*-------------------------------------------------
		参数 - 获取EpsRealFit
*/
vector<std::pair<double, double>> C_STD_DispersionUser::getEpsRealFit()
{
	return m_EpsRealFit;
}

/*-------------------------------------------------
		参数 - 设置EpsImagFit
*/
void C_STD_DispersionUser::setEpsImagFit(vector<std::pair<double, double>> list)
{
	m_EpsImagFit = list;
}

/*-------------------------------------------------
		参数 - 获取EpsImagFit
*/
vector<std::pair<double, double>> C_STD_DispersionUser::getEpsImagFit()
{
	return m_EpsImagFit;
}

/*-------------------------------------------------
		访问器 - 获取子类类名
*/
std::string C_STD_DispersionUser::get__CLASS_NAME__() {
	return "C_STD_DispersionUser";
}

/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
std::string C_STD_DispersionUser::get__CLASS_NAME__Chinese() {
	return "no chinese";
}

/*-------------------------------------------------
		运算符重载
*/
const bool C_STD_DispersionUser::operator== (const C_STD_DispersionUser& a)const {
	return this->m_Name == a.m_Name;
}

/*-------------------------------------------------
		空判断
*/
bool C_STD_DispersionUser::isNull() {
	return this->m_Name == "";
}

/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_STD_DispersionUser::setJsonObject(const Value& obj) {
	// > 材料名称
	m_Name = obj["name"].GetString();

	// > 求解器相关
	m_EpsilonInfinity = obj["epsilonInfinity"].GetDouble();
	m_DetailsError = obj["detailsError"].GetDouble();

	const Value& freqPole1stArray = obj["freqPole1st"];
	for (SizeType i = 0; i < freqPole1stArray.Size(); ++i)
	{
		if (freqPole1stArray[i].IsNumber())
		{
			m_FreqPole1st.push_back(freqPole1stArray[i].GetDouble());
		}
	}

	auto& freqPole2ndArray = obj["freqPole2nd"];
	for (SizeType i = 0; i < freqPole2ndArray.Size(); ++i)
	{
		if (freqPole2ndArray[i].IsNumber())
		{
			m_FreqPole2nd.push_back(freqPole2ndArray[i].GetDouble());
		}
	}

	auto& QArray = obj["Q"];
	for (SizeType i = 0; i < QArray.Size(); ++i)
	{
		if (QArray[i].IsNumber())
		{
			m_Q.push_back(QArray[i].GetDouble());
		}
	}

	auto& FreqZeroArray = obj["freqZero"];
	for (SizeType i = 0; i < FreqZeroArray.Size(); ++i)
	{
		if (FreqZeroArray[i].IsNumber())
		{
			m_FreqZero.push_back(FreqZeroArray[i].GetDouble());
		}
	}

	auto& Gain1Array = obj["gain1"];
	for (SizeType i = 0; i < Gain1Array.Size(); ++i)
	{
		if (Gain1Array[i].IsNumber())
		{
			m_Gain1.push_back(Gain1Array[i].GetDouble());
		}
	}

	auto& Gain2Array = obj["gain2"];
	for (SizeType i = 0; i < Gain2Array.Size(); ++i)
	{
		if (Gain2Array[i].IsNumber())
		{
			m_Gain2.push_back(Gain2Array[i].GetDouble());
		}
	}

	auto& alpha0Array = obj["alpha0"];
	for (SizeType i = 0; i < alpha0Array.Size(); ++i)
	{
		if (alpha0Array[i].IsNumber())
		{
			m_Alpha0.push_back(alpha0Array[i].GetDouble());
		}
	}

	auto& belta0Array = obj["belta0"];
	for (SizeType i = 0; i < belta0Array.Size(); ++i)
	{
		if (belta0Array[i].IsNumber())
		{
			m_Belta0.push_back(belta0Array[i].GetDouble());
		}
	}

	auto& gamma0Array = obj["gamma0"];
	for (SizeType i = 0; i < gamma0Array.Size(); ++i)
	{
		if (gamma0Array[i].IsNumber())
		{
			m_Gamma0.push_back(gamma0Array[i].GetDouble());
		}
	}

	auto& gamma1Array = obj["gamma1"];
	for (SizeType i = 0; i < gamma1Array.Size(); ++i)
	{
		if (gamma1Array[i].IsNumber())
		{
			m_Gamma1.push_back(gamma1Array[i].GetDouble());
		}
	}

	auto& delta0Array = obj["delta0"];
	for (SizeType i = 0; i < delta0Array.Size(); ++i)
	{
		if (delta0Array[i].IsNumber())
		{
			m_Delta0.push_back(delta0Array[i].GetDouble());
		}
	}

	auto& delta1Array = obj["delta1"];
	for (SizeType i = 0; i < delta1Array.Size(); ++i)
	{
		if (delta1Array[i].IsNumber())
		{
			m_Delta1.push_back(delta1Array[i].GetDouble());
		}
	}
}