#pragma once
#include "stdafx.h"

#include "C_STD_DispersionUser.h"
#include "Source/DllModule/rapidjson/document.h"
#include "DispersionFit/C_STD_MaterialDispersionFit_Dielectric.h"
#include "DispersionFit/C_STD_MaterialDispersionFit_Magnetic.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

/*
-----==========================================================-----
		类：		材料 数据类（STD）.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/

using namespace std;
using namespace rapidjson;

class C_STD_StandardMaterial{

	public:
		C_STD_StandardMaterial();
		~C_STD_StandardMaterial();
		
	//-----------------------------------
	//----属性
	protected:
										//ID（无）
		string name;					//名称

		string type;					//类型（PEC/Normal/Anisotropic/Lossy metal）
		string frqType;					//频率类型（all默认/static直流）
										//（选择高频计算工程的时候，会自动定义频率的类型，从而过滤掉直流情况）
	public:
										//属性 - 设置名称
										//		【说明】：名称格式为"aaa/bbb/name"。
		void setName(string name);
		string getName();
		//string getMinName();			//（不实现）
										//属性 - 设置类型
		void setType(string type);
		string getType();
		static vector<string> getAllType_Material();
										//属性 - 设置频率类型
		void setFrqType(string frqType);
		string getFrqType();
		
	//-----------------------------------
	//----色散属性
	//	（无）

	//-----------------------------------
	//----参数（基本参数）
	public:
		double Epsilon;
		double Mu;

	//-----------------------------------
	//----参数（有耗金属基本参数）
	public:
		// double EleConductivity;
		double MuLossyMetal;
		double SurfaceRoughness;
		
	//-----------------------------------
	//----参数（各项异性 Anisotropic）
	public:
		double EpsilonX;
		double EpsilonY;
		double EpsilonZ;
		double MuX;
		double MuY;
		double MuZ;
		
	//-----------------------------------
	//----参数（电导率/磁导率）
	public:
		string ElType;						//电导率类型（getAllType_DielectricDispersion和getAllType_ElectricConductivity中的一种）
		string MaType;						//磁导率类型（getAllType_MagneticDispersion和getAllType_MagneticConductivity中的一种）

		double ElConductivity;				//（Electric conductivity）
		double ElDeltaEl;
		double ElDeltaElAtFrequency;

		double MaConductivity;				//（Magnetic conductivity）
		double MaDeltaMag;
		double MaDeltaMagAtFrequency;

		double ElEpsilonInfinity;			//（Debye 1st/2st order）
		double ElEpsilonStatic1;
		double ElEpsilonStatic2;
		double ElRelaxationTime1;
		double ElRelaxationTime2;
		double MaMuInfinity;
		double MaMuStatic1;
		double MaMuStatic2;
		double MaRelaxationTime1;
		double MaRelaxationTime2;

		//double ElEpsilonInfinity;(重复)	//（Drude）
		double ElPlasmaFrequency;
		double ElCollisionFrequency;
		double ElFieldBreakdown;
		double ElPlasmaMaintainFrequency;
		//double MaMuInfinity;(重复)
		double MaPlasmaFrequency;
		double MaCollisionFrequency;

		//double ElEpsilonInfinity;(重复)	//（Lorentz）
		//double ElEpsilonStatic1;(重复)
		double ElResonanceFrequency;
		double ElDampingFrequency;
		//double MaMuInfinity;(重复)
		//double MaMuStatic1;(重复)
		double MaResonanceFrequency;
		double MaDampingFrequency;

		//double ElEpsilonInfinity;(重复)	//（Gyrotropic）
		//double ElPlasmaFrequency;(重复)
		//double ElCollisionFrequency;(重复)
		double ElCyclotronFrequency;
		double ElBiasingDirectionX;
		double ElBiasingDirectionY;
		double ElBiasingDirectionZ;
		//double MaMuInfinity;(重复)
		double MaLandFactor;
		double MaSatMagnetization;
		double MaResonanceLineWidth;
		double MaFieldVectorX;
		double MaFieldVectorY;
		double MaFieldVectorZ;

		//double ElEpsilonInfinity;(重复)	//（General 1st/2nd order）
		double ElAlpha0;
		double ElBeta0;
		double ElAlpha1;
		double ElBeta1;
		//double MaMuInfinity;(重复)
		double MaAlpha0;
		double MaBeta0;
		double MaAlpha1;
		double MaBeta1;

		double ElChi2;						//（Nonlinear 2nd/3rd order）
		double ElChi3;
		double MaChi2;
		double MaChi3;

		double ElChi3Infinity;				//（Nonlinear Kerr）
		double ElChi3Static;
		//double ElRelaxationTime1;(重复)
		double MaChi3Infinity;
		double MaChi3Static;
		//double MaRelaxationTime1;(重复)

		//double ElChi3Infinity;(重复)		//（Nonlinear Raman）
		//double ElChi3Static;(重复)
		//double ElResonanceFrequency;(重复)
		//double ElDampingFrequency;(重复)
		//double MaChi3Infinity;(重复)
		//double MaChi3Static;(重复)
		//double MaResonanceFrequency;(重复)
		//double MaDampingFrequency;(重复)

		C_STD_MaterialDispersionFit_Dielectric dielectricFit;	//（User）
		C_STD_MaterialDispersionFit_Magnetic magneticFit;
		
											//单选是否使用用户材料设置 2023-12-27 杨骁
		int m_DielectricUserOption;			//（0:Disp. model，1:User）
		int m_MagneticUserOption;			//（0:Disp. model，1:User）
	public:
		static vector<string> getAllType_DielectricDispersion();
		static vector<string> getAllType_ElectricConductivity();
		static vector<string> getAllType_MagneticDispersion();
		static vector<string> getAllType_MagneticConductivity();

	//-----------------------------------
	//----色散属性
	public:
										//色散属性 - 用于自定义色散列表 2024-1-4 杨骁
		C_STD_DispersionUser m_C_DispersionUser;
		
	//-----------------------------------
	//----参数（热传导）
	public:
		string ThermalType;
		double ThermalConductivity;
		double DynamicViscosity;
		double HeatCapacity;

	//-----------------------------------
	//----参数（密度）
	public:
		double Rho;


	//-----------------------------------
	//----映射表
	protected:
		int m_mappingId;
	public:
										//映射表 - 设置映射ID
		void setMappingId(int mapping_id);
										//映射表 - 获取映射ID
										//		【说明】：映射ID为0表示未定义，从1开始计数自定义的材料，-1表示PEC，-2表示真空。
		int getMappingId();
										//映射表 - 是否为默认材料
										//		【说明】：默认材料只能改颜色，不能改参数。
		bool isDefaultMaterial();
										//映射表 - 是否为默认材料 - PEC
		bool isDefaultMaterial_PEC();
										//映射表 - 是否为默认材料 - 真空
		bool isDefaultMaterial_Vacuum();


	//-----------------------------------
	//----描述
	protected:
		string m_description;			//描述
		string m_location;				//文件路径
	public:
										//描述 - 设置描述
		void setDescription(string context);
		string getDescription();
										//描述 - 获取属性描述
		//string getAttributesDesc();	//（不实现）
										//描述 - 设置文件路径
		void setLocation(string location);
		string getLocation();

	//-----------------------------------
	//----分类
	public:
										//分类 - 获取分类
		//string getFolder();			//（不实现）
		

	//-----------------------------------
	//----UI
	//	（无）

		
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
												//运算符重载
		const bool operator== (const C_STD_StandardMaterial& a)const;
												//空判断
		bool isNull();
												//JsonObject -> 实体类
		void setJsonObject(const Value& obj);

};
