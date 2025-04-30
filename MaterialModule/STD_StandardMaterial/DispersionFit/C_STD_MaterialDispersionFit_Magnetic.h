#pragma once
#include "stdafx.h"

#include "Source/DllModule/rapidjson/document.h"
#include <cstdio>
#include <vector>
#include <string>

/*
-----==========================================================-----
		类：		材料属性-离散拟合-磁导率 数据类（STD）.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/

using namespace std;
using namespace rapidjson;
class C_STD_MaterialDispersionFit_MagneticCell;
class C_STD_MaterialDispersionFit_Magnetic{

	public:
		C_STD_MaterialDispersionFit_Magnetic();
		~C_STD_MaterialDispersionFit_Magnetic();
		
	//-----------------------------------
	//----属性
	protected:
										//ID（无）


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
		static vector<string> getTypeList_FittingScheme();
		
	//-----------------------------------
	//----参数（表格参数）
	public:
		int data_format;				//（0:(Real,Imag), 1:(Real,Tand)）
		vector<C_STD_MaterialDispersionFit_MagneticCell> data_list;
	public:
										//参数 - 获取数据类型
		static vector<string> getTypeList_DataFormat();

		
	//-----------------------------------
	//----访问器
	public:
												//访问器 - 获取子类类名
		string get__CLASS_NAME__();
												//访问器 - 获取子类类名（中文名）
		string get__CLASS_NAME__Chinese();

	//-----------------------------------
	//----类属性
	public:
												//QJsonObject -> 实体类
		void setJsonObject(const Value& obj);

};

/*
-----==========================================================-----
		类：		材料属性-离散拟合-磁导率 单条数据.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/
class C_STD_MaterialDispersionFit_MagneticCell{

	public:
		C_STD_MaterialDispersionFit_MagneticCell();
		~C_STD_MaterialDispersionFit_MagneticCell();
		
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
												//QJsonObject -> 实体类
		void setJsonObject(const Value& obj);

};