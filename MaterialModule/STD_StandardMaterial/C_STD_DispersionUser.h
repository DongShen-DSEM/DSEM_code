#pragma once
#include "stdafx.h"

#include "Source/DllModule/rapidjson/document.h"
#include <utility>
#include <vector>
#include <string>

using namespace std;
using namespace rapidjson;

class C_STD_DispersionUser{

	public:
		C_STD_DispersionUser();
		~C_STD_DispersionUser();

	//-----------------------------------
	//----参数
	public:
							//参数 - 设置材料名称
		void setName(std::string name);
							//参数 - 获取材料名称
		std::string getName();
							//参数 - 设置FreqPole1st
		void setFreqPole1st(std::vector<double> list);
							//参数 - 获取FreqPole1st
		std::vector<double> getFreqPole1st();
							//参数 - 设置FreqPole2nd
		void setFreqPole2nd(std::vector<double> list);
							//参数 - 获取FreqPole2nd
		std::vector<double> getFreqPole2nd();
							//参数 - 设置Q
		void setQ(std::vector<double> list);
							//参数 - 获取Q
		std::vector<double> getQ();
							//参数 - 设置FreqZero
		void setFreqZero(std::vector<double> list);
							//参数 - 获取FreqZero
		std::vector<double> getFreqZero();
							//参数 - 设置Gain1
		void setGain1(std::vector<double> list);
							//参数 - 获取Gain1
		std::vector<double> getGain1();
							//参数 - 设置Gain2
		void setGain2(std::vector<double> list);
							//参数 - 获取Gain2
		std::vector<double> getGain2();
							//参数 - 设置EpsilonInfinity
		void setEpsilonInfinity(double value);
							//参数 - 获取EpsilonInfinity
		double& getEpsilonInfinity();
							//参数 - 设置DetailsError
		void setDetailsError(double value);
							//参数 - 获取DetailsError
		double& getDetailsError();
							//参数 - 设置Alpha0
		void setAlpha0(std::vector<double> list);
							//参数 - 获取Alpha0
		std::vector<double> getAlpha0();
							//参数 - 设置Belta0
		void setBelta0(std::vector<double> list);
							//参数 - 获取Belta0
		std::vector<double> getBelta0();
							//参数 - 设置Gamma0
		void setGamma0(std::vector<double> list);
							//参数 - 获取Gamma0
		std::vector<double> getGamma0();
							//参数 - 设置Gamma1
		void setGamma1(std::vector<double> list);
							//参数 - 获取Gamma1
		std::vector<double> getGamma1();
							//参数 - 设置Delta0
		void setDelta0(std::vector<double> list);
							//参数 - 获取Delta0
		std::vector<double> getDelta0();
							//参数 - 设置Delta1
		void setDelta1(std::vector<double> list);
							//参数 - 获取Delta1
		std::vector<double> getDelta1();
							//参数 - 设置EpsRealData
		void setEpsRealData(vector<std::pair<double,double>> list);
							//参数 - 获取EpsRealData
		vector<std::pair<double,double>> getEpsRealData();
							//参数 - 设置EpsImagData
		void setEpsImagData(vector<std::pair<double,double>> list);
							//参数 - 获取EpsImagData
		vector<std::pair<double,double>> getEpsImagData();
							//参数 - 设置EpsRealFit
		void setEpsRealFit(vector<std::pair<double,double>> list);
							//参数 - 获取EpsRealFit
		vector<std::pair<double,double>> getEpsRealFit();
							//参数 - 设置EpsImagFit
		void setEpsImagFit(vector<std::pair<double,double>> list);
							//参数 - 获取EpsImagFit
		vector<std::pair<double,double>> getEpsImagFit();

	private:
							//参数 - 材料名称
		std::string m_Name="";
							//参数 - details数据
		std::vector<double> m_FreqPole1st, m_FreqPole2nd, m_Q, m_FreqZero, m_Gain1, m_Gain2;
		double m_EpsilonInfinity;
		double m_DetailsError;
							//参数 - 求解器所需数据
		std::vector<double> m_Alpha0, m_Belta0;
		std::vector<double> m_Gamma0, m_Gamma1, m_Delta0, m_Delta1;
							//参数 - 曲线所需数据
		vector<std::pair<double, double>> m_EpsRealData;
		vector<std::pair<double, double>> m_EpsImagData;
		vector<std::pair<double, double>> m_EpsRealFit;
		vector<std::pair<double, double>> m_EpsImagFit;

	//-----------------------------------
	//----访问器
	public:
							//访问器 - 获取子类类名
		std::string get__CLASS_NAME__();
							//访问器 - 获取子类类名（中文名）
		std::string get__CLASS_NAME__Chinese();

	//-----------------------------------
	//----类属性
	public:
							//运算符重载
		const bool operator== (const C_STD_DispersionUser& a)const;
							//空判断
		bool isNull();
							//Json -> 实体类
		void setJsonObject(const Value& obj);
};