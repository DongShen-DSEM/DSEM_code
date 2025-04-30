#pragma once
#include "stdafx1.h"
#include <QList>
#include <QMap>

#include "Source/ProjectModule/Undo/ExtendPart/C_UndoDataBase.h"
#include "DispersionFit/C_MaterialDispersionFit_Dielectric.h"
#include "DispersionFit/C_MaterialDispersionFit_Magnetic.h"
#include "./DispersionUser/C_DispersionUser.h"

/*
-----==========================================================-----
		类：		材料 数据类.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/

class C_StandardMaterial : public C_UndoDataBase{

	public:
		C_StandardMaterial();
		~C_StandardMaterial();
		
	//-----------------------------------
	//----属性
	protected:
										//ID（撤回基类数据）
										//名称（撤回基类数据）

		QString type;					//类型（PEC/Normal/Anisotropic/Lossy metal）
		QString frqType;				//频率类型（all默认/static直流）
										//（选择高频计算工程的时候，会自动定义频率的类型，从而过滤掉直流情况）
	public:
										//属性 - 设置名称
										//		【说明】：名称格式为"aaa/bbb/name"。
		void setName(QString name);
		QString getName();
		QString getMinName();
										//属性 - 设置类型
		void setType(QString type);
		QString getType();
		static QStringList getAllType_Material();
										//属性 - 设置频率类型
		void setFrqType(QString frqType);
		QString getFrqType();

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
		QString ElType;						//电导率类型（getAllType_DielectricDispersion和getAllType_ElectricConductivity中的一种）
		QString MaType;						//磁导率类型（getAllType_MagneticDispersion和getAllType_MagneticConductivity中的一种）

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

		C_MaterialDispersionFit_Dielectric dielectricFit;	//（User）
		C_MaterialDispersionFit_Magnetic magneticFit;

											//单选是否使用用户材料设置 2023-12-27 杨骁
		int m_DielectricUserOption;			//（0:Disp. model，1:User）
		int m_MagneticUserOption;			//（0:Disp. model，1:User）
	public:
		static QStringList getAllType_DielectricDispersion();
		static QStringList getAllType_ElectricConductivity();
		static QStringList getAllType_MagneticDispersion();
		static QStringList getAllType_MagneticConductivity();

	//-----------------------------------
	//----色散属性
	public:
										//色散属性 - 用于自定义色散列表 2024-1-4 杨骁
		C_DispersionUser m_C_DispersionUser;
										//色散属性 - 如果设置了色散选项，就要生成对应的色散曲线 2024-9-4 杨骁
		void genDispersionTxt(bool);
										//色散属性 - 从cst的色散类型转换到dsem的 2025-2-28 杨骁
		QString getElTypeFromCST(QString);

	//-----------------------------------
	//----参数（热传导）
	public:
		QString ThermalType;
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
		QString m_description;			//描述
		QString m_location;				//文件路径
	public:
										//描述 - 设置描述
		void setDescription(QString context);
		QString getDescription();
										//描述 - 获取属性描述
		QString getAttributesDesc();
										//描述 - 设置文件路径
		void setLocation(QString location);
		QString getLocation();

	//-----------------------------------
	//----分类
	public:
										//分类 - 获取分类
		QString getFolder();
	

	//-----------------------------------
	//----UI - 材料库数据
	public:
										//材料库数据 - 主要分类项
		static QStringList getLibParam_TypeOptions();
										//材料库数据 - 属性分类项
		static QStringList getLibParam_AttrOptions();
										//材料库数据 - 类型描述
		QString getLibParam_MaterialSets();
										//材料库数据 - 类型图标
		QIcon getLibParam_TypeIcon();
	private:
										//材料库数据 - 类型图标缓存
		QMap<QString, QIcon> m_IconCache;

	//-----------------------------------
	//----UI - 颜色
	protected:
		QColor m_color;					//颜色
		double m_transparency;			//透明度（0至1）
	public:
										//颜色 - 设置颜色
		void setColor(QColor color);
		QColor getColor();
		QString getMtdDescription_Color();
										//颜色 - 获取颜色图标
		QIcon getIcon_Color();
										//颜色 - 设置透明度
		void setTransparency(double transparency);
		double getTransparency();
										//颜色 - 设置透明度（百分比int值）
		void setTransparency_Per(int transparency_per);
		int getTransparency_Per();
		QString getMtdDescription_Transparency();
		
		
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
		C_StandardMaterial clone();
		QSharedPointer<C_StandardMaterial> clonePtr();
		C_UndoDataBasePtr cloneParentPtr() override;

	//-----------------------------------
	//----类属性
	public:
		double getDoubleFromObject(const QJsonObject &obj, const QString &key, double defaultValue = 0.0) const;
		QString getStringFromObject(const QJsonObject &obj, const QString &key, const QString &defaultValue = "") const;
												//复制对象
		void copyFrom(C_StandardMaterial* a);
												//运算符重载
		const bool operator== (const C_StandardMaterial& a)const;
												//空判断
		bool isNull();
												//实体类 -> QJsonObject
		QJsonObject getJsonObject_undoChildData() override;
												//QJsonObject -> 实体类
		void setJsonObject_undoChildData(QJsonObject obj) override;
};
