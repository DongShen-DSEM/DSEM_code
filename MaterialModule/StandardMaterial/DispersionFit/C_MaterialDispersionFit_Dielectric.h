#pragma once
#include "stdafx1.h"
#include <QList>
#include <QMap>

#include "Source/ProjectModule/Container/C_ContainerDataBase.h"

/*
-----==========================================================-----
		类：		材料属性-离散拟合-电导率 数据类.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/

class C_MaterialDispersionFit_DielectricCell;
class C_MaterialDispersionFit_Dielectric : public C_ContainerDataBase{

	public:
		C_MaterialDispersionFit_Dielectric();
		~C_MaterialDispersionFit_Dielectric();
		
	//-----------------------------------
	//----属性
	protected:
										//ID（基类数据）


	//-----------------------------------
	//----参数（基本参数）
	public:
		int fittingScheme;				//装配方案（0:Conductivity, 1:nth order）
		int maxOrder;
		double errorLimit;
		bool useFreqRange;

		int usedOrder;
		double error;
	public:
										//参数 - 获取装配方案
		static QStringList getTypeList_FittingScheme();
										//参数 - 获取属性描述
		QString getAttributesDesc();
		
	//-----------------------------------
	//----参数（表格参数）
	public:
		int data_format;				//（0:(Real,Imag), 1:(Real,Tand)）
		QList<C_MaterialDispersionFit_DielectricCell> data_list;
	public:
										//参数 - 获取数据类型
		static QStringList getTypeList_DataFormat();

		
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
		C_MaterialDispersionFit_Dielectric clone();
		QSharedPointer<C_MaterialDispersionFit_Dielectric> clonePtr();

	//-----------------------------------
	//----类属性
	public:
												//复制对象
		void copyFrom(C_MaterialDispersionFit_Dielectric* a);
												//运算符重载
		const bool operator== (const C_MaterialDispersionFit_Dielectric& a)const;
												//空判断
		bool isNull();
												//实体类 -> QJsonObject
		QJsonObject getJsonObject_childData() override;
												//QJsonObject -> 实体类
		void setJsonObject_childData(QJsonObject obj) override;

};

/*
-----==========================================================-----
		类：		材料属性-离散拟合-电导率 单条数据.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/
class C_MaterialDispersionFit_DielectricCell{

	public:
		C_MaterialDispersionFit_DielectricCell();
		~C_MaterialDispersionFit_DielectricCell();
		
	//-----------------------------------
	//----参数
	public:
		double freq;
		double value_real;
		double value_imag;
		double value_tand;
		double weight;
	public:
												//参数 - 计算并覆盖tand值
		void calculateTand();
												//参数 - 计算并覆盖imag值
		void calculateImag();


	//-----------------------------------
	//----类属性
	public:
												//实体类 -> QJsonObject
		QJsonObject getJsonObject();
												//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);

};