#ifndef C_DispersionUser_H
#define C_DispersionUser_H

#include "stdafx1.h"
#include "Source/ProjectModule/Undo/ExtendPart/C_UndoDataBase.h"

class C_DispersionUser : public C_UndoDataBase {
	public:
		C_DispersionUser();
		~C_DispersionUser();

	//-----------------------------------
	//----组件
	public:
							//组件 - 下拉框 - Fitting scheme选项
		static QStringList getFittingSchemeList();
							//组件 - 下拉框 - Data format选项
		static QStringList getDataFormatList();
							//组件 - 下拉框 - Freq unit选项
		static QStringList getFreqUnitList();
							//组件 - 下拉框 - Fitting scheme下标
		int m_FittingIndex = 1;
							//组件 - 下拉框 - Data format下标
		int m_DataFormatIndex= 0;
							//组件 - 下拉框 - Freq Unit下标
		int m_FreqUnitIndex = 0;
							//组件 - 输入框 - 设置Max order输入值
		void setMaxOrder(int value);
							//组件 - 输入框 - 获取Max order输入值
		int getMaxOrder();
							//组件 - 输入框 - 设置Error limit输入值
		void setErrorLimit(double value);
							//组件 - 输入框 - 获取Error limit输入值
		double getErrorLimit();
							//参数 - 输入框 - 设置Used order输入值
		void setUsedOrder(int value);
							//参数 - 输入框 - 获取Used order输入值
		int getUsedOrder();
							//参数 -  - 输入框 - 设置Error
		void setError(double value);
							//参数 -  - 输入框 - 获取Error
		double getError();
	private:
							//组件 - 输入框 - Max order输入值
		int m_MaxOrder = 10;
							//组件 - 输入框 - Error limit输入值
		double m_ErrorLimit = 0.1;
							//组件 - 输入框 - Used order输入值
		int m_UsedOrder = 10;
							//组件 - 输入框 - Error输入值
		double m_Error = 0.1;
	//-----------------------------------
	//----求解器
	public:
							//求解器 - 获取details弹出内容字符串
		QString getDetailsStr();
							//求解器 - 生成曲线txt文件
		void genUserDispersionTxt(bool);
	
	//-----------------------------------
	//----参数
	public:
							//参数 - 设置材料名称
		void setName(QString name);
							//参数 - 获取材料名称
		QString getName();
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
		double getEpsilonInfinity();
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
		void setEpsRealData(QVector<std::pair<double,double>> list);
							//参数 - 获取EpsRealData
		QVector<std::pair<double,double>> getEpsRealData();
							//参数 - 设置EpsImagData
		void setEpsImagData(QVector<std::pair<double,double>> list);
							//参数 - 获取EpsImagData
		QVector<std::pair<double,double>> getEpsImagData();
							//参数 - 设置EpsRealFit
		void setEpsRealFit(QVector<std::pair<double,double>> list);
							//参数 - 获取EpsRealFit
		QVector<std::pair<double,double>> getEpsRealFit();
							//参数 - 设置EpsImagFit
		void setEpsImagFit(QVector<std::pair<double,double>> list);
							//参数 - 获取EpsImagFit
		QVector<std::pair<double,double>> getEpsImagFit();
							//参数 - 获取存储的model数据
		std::vector<std::vector<double>>& getModelData();
							//参数 -设置存储的model数据
		void setModelData(std::vector<std::vector<double>> result);

	private:
							//参数 - 材料名称
		QString m_Name;
							//参数 - details数据
		std::vector<double> m_FreqPole1st, m_FreqPole2nd, m_Q, m_FreqZero, m_Gain1, m_Gain2;
		double m_EpsilonInfinity;
							//参数 - 求解器所需数据
		std::vector<double> m_Alpha0, m_Belta0;
		std::vector<double> m_Gamma0, m_Gamma1, m_Delta0, m_Delta1;
							//参数 - 曲线所需数据
		QVector<std::pair<double, double>> m_EpsRealData;
		QVector<std::pair<double, double>> m_EpsImagData;
		QVector<std::pair<double, double>> m_EpsRealFit;
		QVector<std::pair<double, double>> m_EpsImagFit;
							//参数 - 从文件加载过滤后的数据
		std::vector<std::vector<double>> m_ModelData;

	//-----------------------------------
	//----访问器
	public:
							//访问器 - 获取子类类名
		QString get__CLASS_NAME__() override;
							//访问器 - 获取子类类名（中文名）
		QString get__CLASS_NAME__Chinese() override;
							//访问器 - 获取所有继承的类名
		QStringList get__CLASS_INHERITED_LIST__() override;
							//访问器 - 是否继承了某类
		bool get__CLASS_IS_INSTANCE_OF__(QString class_name) override;
							//访问器 - 原型复制对象
		C_DispersionUser clone();
		QSharedPointer<C_DispersionUser> clonePtr();
		C_UndoDataBasePtr cloneParentPtr() override;

	//-----------------------------------
	//----类属性
	public:
							//复制对象
		void copyFrom(C_DispersionUser* a);
							//运算符重载
		const bool operator== (const C_DispersionUser& a)const;
							//空判断
		bool isNull();
							//实体类 -> QJsonObject
		QJsonObject getJsonObject_undoChildData() override;
							//QJsonObject -> 实体类
		void setJsonObject_undoChildData(QJsonObject obj) override;
};

#endif // C_DispersionUser_H
