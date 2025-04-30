#include "stdafx.h"
#include "C_StandardMaterial.h"

#include "Source/Utils/Common/TTool.h"
#include "Source/SettingModule/S_SettingDataContainer.h"
#include "Source/ResultModule/S_ResultDataContainer.h"
#include "Source/Utils/Common/Tool4Sean.h"
#include "Source/ProjectModule/File/S_TempFileManager.h"
#include <complex>
#include <iostream>
#include <cmath>

/*
-----==========================================================-----
		类：		材料 数据类.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/

C_StandardMaterial::C_StandardMaterial() : C_UndoDataBase(){

	// > 属性
	this->type = "Normal";
	this->frqType = "all";
	

	// > 参数（基本参数）
	this->Epsilon = 1.0;
	this->Mu = 1.0;

	// > 参数（有耗金属基本参数）
	// this->EleConductivity = 0;
	this->MuLossyMetal = 1;
	this->SurfaceRoughness = 0;

	// > 参数（各项异性 Anisotropic）
	this->EpsilonX = 1.0;
	this->EpsilonY = 1.0;
	this->EpsilonZ = 1.0;
	this->MuX = 1.0;
	this->MuY = 1.0;
	this->MuZ = 1.0;


	// > 参数（电导率/磁导率）
	this->ElType = "None";					//电导率类型（getAllType_DielectricDispersion和getAllType_ElectricConductivity中的一种）
	this->MaType = "None";					//磁导率类型（getAllType_MagneticDispersion和getAllType_MagneticConductivity中的一种）

	this->ElConductivity = 0.0;				//（Electric conductivity）
	this->ElDeltaEl = 0.0;
	this->ElDeltaElAtFrequency = 0.0;

	this->MaConductivity = 0.0;				//（Magnetic conductivity）
	this->MaDeltaMag = 0.0;
	this->MaDeltaMagAtFrequency = 0.0;

	this->ElEpsilonInfinity = 1.0;			//（Debye 1st/2st order）
	this->ElEpsilonStatic1 = 2.0;
	this->ElEpsilonStatic2 = 2.0;
	this->ElRelaxationTime1 = 0.0;
	this->ElRelaxationTime2 = 0.0;
	this->MaMuInfinity = 1.0;
	this->MaMuStatic1 = 2.0;
	this->MaMuStatic2 = 2.0;
	this->MaRelaxationTime1 = 0.0;
	this->MaRelaxationTime2 = 0.0;

	this->ElPlasmaFrequency = 0.0;			//（Drude）
	this->ElCollisionFrequency = 0.0;
	this->ElFieldBreakdown = 0.0;
	this->ElPlasmaMaintainFrequency = 0.0;
	this->MaPlasmaFrequency = 0.0;
	this->MaCollisionFrequency = 0.0;

	this->ElResonanceFrequency = 0.0;		//（Lorentz）
	this->ElDampingFrequency = 0.0;
	this->MaResonanceFrequency = 0.0;
	this->MaDampingFrequency = 0.0;

	this->ElCyclotronFrequency = 0.0;		//（Gyrotropic）
	this->ElBiasingDirectionX = 0.0;
	this->ElBiasingDirectionY = 0.0;
	this->ElBiasingDirectionZ = 0.0;
	this->MaLandFactor = 0.0;
	this->MaSatMagnetization = 0.0;
	this->MaResonanceLineWidth = 0.0;
	this->MaFieldVectorX = 0.0;
	this->MaFieldVectorY = 0.0;
	this->MaFieldVectorZ = 0.0;

	this->ElAlpha0 = 0.0;					//（General 1st/2nd order）
	this->ElBeta0 = 0.0;
	this->ElAlpha1 = 0.0;
	this->ElBeta1 = 0.0;
	this->MaAlpha0 = 0.0;
	this->MaBeta0 = 0.0;
	this->MaAlpha1 = 0.0;
	this->MaBeta1 = 0.0;

	this->ElChi2 = 0.0;						//（Nonlinear 2nd/3rd order）
	this->ElChi3 = 0.0;
	this->MaChi2 = 0.0;
	this->MaChi3 = 0.0;

	this->ElChi3Infinity = 0.0;				//（Nonlinear Kerr）
	this->ElChi3Static = 0.0;
	this->MaChi3Infinity = 0.0;
	this->MaChi3Static = 0.0;

	//（重复参数）							//（Nonlinear Raman）

	this->dielectricFit;					//（User）
	this->magneticFit;

	// > 参数（单选是否使用用户材料设置）
	this->m_DielectricUserOption = 0;		//（0:Disp. model，1:User）
	this->m_MagneticUserOption = 0;			//（0:Disp. model，1:User）

	// > 参数（热传导）
	this->ThermalType = "Normal";
	this->ThermalConductivity = 0;
	this->DynamicViscosity = 0;
	this->HeatCapacity = 0;
	
	// > 参数（密度）
	this->Rho = 1.0;

	
	// > 映射表
	this->m_mappingId = 0;

	// > 描述
	this->m_description = "";

	// > UI - 颜色	
	qsrand(47 + QTime::currentTime().msec());
	int r = qrand() % 256;
	qsrand(147 + QTime::currentTime().msec());
	int g = qrand() % 256;
	qsrand(247 + QTime::currentTime().msec());
	int b = qrand() % 256;
	this->m_color = QColor(r, g, b);	//随机颜色
	this->m_transparency = 0;

}
C_StandardMaterial::~C_StandardMaterial(){
}


/*-------------------------------------------------
		属性 - 设置名称
*/
void C_StandardMaterial::setName(QString name){
	
	// > 将目录统一为"/"
	QStringList str_list = name.split(QRegExp("[_/]{3}"));

	this->name = str_list.join("/");
}
QString C_StandardMaterial::getName(){
	return this->name;
}
QString C_StandardMaterial::getMinName() {
	return this->name.split("/").last();
}
/*-------------------------------------------------
		属性 - 设置类型
*/
void C_StandardMaterial::setType(QString type){
	this->type = type;
}
QString C_StandardMaterial::getType(){
	return this->type;
}
QStringList C_StandardMaterial::getAllType_Material() {
	// return QStringList() << "PEC" << "Normal";		//（固定的名词）
	return QStringList() << "PEC" << "Normal" << "Anisotropic" << "Lossy metal";
}
QStringList C_StandardMaterial::getAllType_DielectricDispersion() {
	// return QStringList() << "None" << "Debye 1st order" << "Debye 2nd order" << "Drude" 	//（固定的名词）
	// 	<< "Lorentz" << "Gyrotropic" << "General 1st order" << "General 2nd order"
	// 	<< "Nonlinear 2nd order" << "Nonlinear 3rd order" << "Nonlinear Kerr" << "Nonlinear Raman"
	// 	<< "User";
	return QStringList() << "None" << "Debye 1st order" << "Debye 2nd order" << "Drude"
		<< "Lorentz" << "General 1st order" << "General 2nd order";
}

QString C_StandardMaterial::getElTypeFromCST(QString cstStr)
{
	QMap<QString, QString> typeMap;
	typeMap.insert("Debye1st", "Debye 1st order");
	typeMap.insert("Debye2nd", "Debye 2nd order");
	typeMap.insert("Drude", "Drude");
	typeMap.insert("Lorentz", "Lorentz");
	typeMap.insert("General1st", "General 1st order");
	typeMap.insert("General2nd", "General 2nd order");

	if (typeMap.contains(cstStr))
	{
		return typeMap[cstStr];
	}
	else
	{
		return "None";
	}
}

QStringList C_StandardMaterial::getAllType_ElectricConductivity() {
	return QStringList() << "None" << "El.conductivity" << "Tangent delta el";
}
QStringList C_StandardMaterial::getAllType_MagneticDispersion() {
	// return QStringList() << "None" << "Debye 1st order" << "Debye 2nd order" << "Drude"
	// 	<< "Lorentz" << "Gyrotropic" << "General 1st order" << "General 2nd order"
	// 	<< "Nonlinear 2nd order" << "Nonlinear 3rd order" << "Nonlinear Kerr" << "Nonlinear Raman"
	// 	<< "User";
	return QStringList() << "None";
}
QStringList C_StandardMaterial::getAllType_MagneticConductivity() {
	return QStringList() << "None" << "Mag.conductivity" << "Tangent delta mag";
}

/*-------------------------------------------------
		色散属性 - 如果设置了色散选项，就要生成对应的色散曲线
*/
void C_StandardMaterial::genDispersionTxt(bool update)
{
	double freqMin = S_SettingDataContainer::getInstance()->getDataFrequency().getFreqMin_Hz();
	double freqMax = S_SettingDataContainer::getInstance()->getDataFrequency().getFreqMax_Hz();
	if (freqMax > 0 && freqMax > freqMin)
	{
		int pointCount = 2000;

		QVector<std::pair<double, double>> realList;
		QVector<std::pair<double, double>> imagList;

		double diff = freqMax - freqMin;
		C_SettingUnits units = S_SettingDataContainer::getInstance()->getDataUnits();
		double per = C_SettingUnits::getConvertedMagnification_Frequency(0, units.getCurrentIndex_frequency());

		if (ElType == "Debye 1st order")
		{
			// debye 1st
			for (int i = 0; i <= pointCount; i++)
			{
				double tmpF = (freqMin + (diff / pointCount * i));
				std::complex<double> denominator(1.0, 2 * M_PI * tmpF * ElRelaxationTime1);
				std::complex<double> result = ElEpsilonInfinity + (ElEpsilonStatic1 - ElEpsilonInfinity) / denominator;

				realList.append(std::make_pair(tmpF * per, result.real()));
				imagList.append(std::make_pair(tmpF * per, -result.imag()));
			}
		}

		if (ElType == "Debye 2nd order")
		{
			// debye 2nd
			for (int i = 0; i <= pointCount; i++)
			{
				double tmpF = (freqMin + (diff / pointCount * i));
				std::complex<double> denominator(1.0, 2 * M_PI * tmpF * ElRelaxationTime1);
				std::complex<double> result = ElEpsilonInfinity + (ElEpsilonStatic1 - ElEpsilonInfinity) / denominator;

				std::complex<double> denominator2(1.0, 2 * M_PI * tmpF * ElRelaxationTime2);
				std::complex<double> result2 = ElEpsilonInfinity + (ElEpsilonStatic2 - ElEpsilonInfinity) / denominator2;

				realList.append(std::make_pair(tmpF * per, result.real() + result2.real()));
				imagList.append(std::make_pair(tmpF * per, -result.imag() - result2.imag()));
			}
		}

		if (ElType == "Drude")
		{
			// drude
			for (int i = 0; i <= pointCount; i++)
			{
				double tmpF = (freqMin + (diff / pointCount * i));
				std::complex<double> denominator(std::pow(2 * M_PI * tmpF, 2), -ElCollisionFrequency * 2 * M_PI * tmpF);
				std::complex<double> result = ElEpsilonInfinity - std::pow(ElPlasmaFrequency, 2) / denominator;

				realList.append(std::make_pair(tmpF * per, result.real()));
				imagList.append(std::make_pair(tmpF * per, -result.imag()));
			}
		}

		if (ElType == "Lorentz")
		{
			// lorentz
			for (int i = 0; i <= pointCount; i++)
			{
				double tmpF = (freqMin + (diff / pointCount * i));
				std::complex<double> denominator(std::pow(ElResonanceFrequency, 2) - std::pow(2 * M_PI * tmpF, 2),
												 ElDampingFrequency * 2 * M_PI * tmpF);
				std::complex<double> result = ElEpsilonInfinity + std::pow(ElResonanceFrequency, 2) * (ElEpsilonStatic1 - ElEpsilonInfinity) / denominator;

				realList.append(std::make_pair(tmpF * per, result.real()));
				imagList.append(std::make_pair(tmpF * per, -result.imag()));
			}
		}

		if (ElType == "General 1st order")
		{
			// general 1st
			for (int i = 0; i <= pointCount; i++)
			{
				double tmpF = (freqMin + (diff / pointCount * i));
				std::complex<double> denominator(ElAlpha0, 2 * M_PI * tmpF);
				std::complex<double> result = ElEpsilonInfinity + ElBeta0 / denominator;

				realList.append(std::make_pair(tmpF * per, result.real()));
				imagList.append(std::make_pair(tmpF * per, -result.imag()));
			}
		}

		if (ElType == "General 2nd order")
		{
			// general 2nd
			for (int i = 0; i <= pointCount; i++)
			{
				double tmpF = (freqMin + (diff / pointCount * i));
				std::complex<double> denominator(ElAlpha0 - std::pow(2 * M_PI * tmpF, 2), 2 * M_PI * tmpF * ElAlpha1);
				std::complex<double> denominator2(ElBeta0, ElBeta1 * 2 * M_PI * tmpF);
				std::complex<double> result = ElEpsilonInfinity + denominator2 / denominator;

				realList.append(std::make_pair(tmpF * per, result.real()));
				imagList.append(std::make_pair(tmpF * per, -result.imag()));
			}
		}

		if (realList.count() > 0 && imagList.count() > 0)
		{
			// 单位字符串传递给x轴
			QString unitStr = units.getUnitAllType_frequency().at(units.getCurrentIndex_frequency());

			// 写入到txt文件
			QString materialPath = QString("OutputResults/1D_Results/Materials/%1/Dispersive/").arg(name);
			QString txtDir = QDir(S_TempFileManager::getInstance()->getTempFileUrl()).filePath(materialPath);
			QString realPath = QDir(txtDir).filePath("Eps'.txt");
			QString imagPath = QDir(txtDir).filePath("Eps''.txt");

			Tool4Sean::writeDoubleListToTxt(realPath, realList, unitStr);
			Tool4Sean::writeDoubleListToTxt(imagPath, imagList, unitStr);

			// 加载色散文件到左侧树
            S_ResultDataContainer::getInstance()->loadDispersion_loadResult_1D(this->name, update);
		}
	}
}

/*-------------------------------------------------
		属性 - 设置频率类型
*/
void C_StandardMaterial::setFrqType(QString frqType){
	this->frqType = frqType;
}
QString C_StandardMaterial::getFrqType(){
	return this->frqType;
}


/*-------------------------------------------------
		映射表 - 设置映射ID
*/
void C_StandardMaterial::setMappingId(int mapping_id) {
	this->m_mappingId = mapping_id;
}
/*-------------------------------------------------
		映射表 - 获取映射ID
*/
int C_StandardMaterial::getMappingId() {
	return this->m_mappingId;
}
/*-------------------------------------------------
		映射表 - 是否为默认材料
*/
bool C_StandardMaterial::isDefaultMaterial() {
	return this->m_mappingId < 0;
}
bool C_StandardMaterial::isDefaultMaterial_PEC() {
	return this->m_mappingId == -1;
}
bool C_StandardMaterial::isDefaultMaterial_Vacuum() {
	return this->m_mappingId == -2;
}

/*-------------------------------------------------
		描述 - 设置描述
*/
void C_StandardMaterial::setDescription(QString context){
	this->m_description = context;
}
QString C_StandardMaterial::getDescription(){
	return this->m_description;
}
QString C_StandardMaterial::getAttributesDesc(){
	QString context;

	// > PEC时只输出固定字符串
	if (this->isDefaultMaterial_PEC()){
		context.append(QObject::tr("Type           = PEC"));
		context.append("\n");
		return context;
	}

	// > Vacuum时只输出固定字符串
	if (this->isDefaultMaterial_Vacuum()) {
		context.append(QObject::tr("Type           = "));
		context.append(this->type);
		context.append("\n");
		context.append(QObject::tr("Epsilon        = "));
		context.append(QString::number(this->Epsilon));
		context.append("\n");
		context.append(QObject::tr("Mu             = "));
		context.append(QString::number(this->Mu));
		context.append("\n");
		return context;
	}


	// > 属性
	context.append(QObject::tr("Type           = "));
	context.append(this->type);
	context.append("\n");


	// > 参数（基本参数）
	if ( this->ElType !="None" && this->getAllType_DielectricDispersion().contains(this->ElType) ) {
		//（Dispersion 标签页时，不显示 Epsilon）
	}else {
		context.append(QObject::tr("Epsilon        = "));
		context.append(QString::number(this->Epsilon));
		context.append("\n");
	}
	if ( this->MaType != "None" && this->getAllType_MagneticDispersion().contains(this->MaType) ) {
		//（Dispersion 标签页时，不显示 Mu）
	}else {
		context.append(QObject::tr("Mu             = "));
		context.append(QString::number(this->Mu));
		context.append("\n");
	}


	// > 参数（电导率）
	if (this->ElType == "El.conductivity") {
		context.append(QObject::tr("Electric cond. = "));
		context.append(QString::number(this->ElConductivity));
		context.append("[S/m]");
		context.append("\n");
	}
	if (this->ElType == "Tangent delta el") {
		context.append(QObject::tr("Electric tand  = "));
		context.append(QString::number(this->ElDeltaEl));
		context.append("(Const.fit)");
		context.append("\n");
	}
	if ( this->ElType != "None" && this->getAllType_DielectricDispersion().contains(this->ElType)) {
		context.append(QObject::tr("Dispersive eps = "));
		if (this->ElType == "Debye 1st order") { context.append(QObject::tr("Debye 1st order model")); }
		if (this->ElType == "Debye 2nd order") { context.append(QObject::tr("Debye 2nd order model")); }
		if (this->ElType == "Drude") { context.append(QObject::tr("Drude model")); }
		if (this->ElType == "Lorentz") { context.append(QObject::tr("Lorentz model")); }
		if (this->ElType == "Gyrotropic") { context.append(QObject::tr("Gyrotropic(magnetized plasma)")); }
		if (this->ElType == "General 1st order") { context.append(QObject::tr("General 1st order model")); }
		if (this->ElType == "General 2nd order") { context.append(QObject::tr("General 2nd order model")); }
		if (this->ElType == "Nonlinear 2nd order") { context.append(QObject::tr("Nonlinear 2nd order model")); }
		if (this->ElType == "Nonlinear 3rd order") { context.append(QObject::tr("Nonlinear 3rd order model")); }
		if (this->ElType == "Nonlinear Kerr") { context.append(QObject::tr("Nonlinear Kerr model")); }
		if (this->ElType == "Nonlinear Raman") { context.append(QObject::tr("Nonlinear Raman model")); }
		if (this->ElType == "User") { 
			context.append(this->dielectricFit.getAttributesDesc());
		}
		context.append("\n");
	}

	// > 参数（磁导率）
	if (this->MaType == "Mag.conductivity") {
		context.append(QObject::tr("Magnetic cond. = "));
		context.append(QString::number(this->MaConductivity));
		context.append("[1/(Sm)]");
		context.append("\n");
	}
	if (this->MaType == "Tangent delta mag") {
		context.append(QObject::tr("Magnetic tand  = "));
		context.append(QString::number(this->MaDeltaMag));
		context.append("(Const.fit)");
		context.append("\n");
	}
	if ( this->MaType != "None" && this->getAllType_MagneticDispersion().contains(this->MaType)) {
		context.append(QObject::tr("Dispersive mu  = "));
		if (this->MaType == "Debye 1st order") { context.append(QObject::tr("Debye 1st order model")); }
		if (this->MaType == "Debye 2nd order") { context.append(QObject::tr("Debye 2nd order model")); }
		if (this->MaType == "Drude") { context.append(QObject::tr("Drude model")); }
		if (this->MaType == "Lorentz") { context.append(QObject::tr("Lorentz model")); }
		if (this->MaType == "Gyrotropic") { context.append(QObject::tr("Gyrotropic(biased ferrites)")); }
		if (this->MaType == "General 1st order") { context.append(QObject::tr("General 1st order model")); }
		if (this->MaType == "General 2nd order") { context.append(QObject::tr("General 2nd order model")); }
		if (this->MaType == "Nonlinear 2nd order") { context.append(QObject::tr("Nonlinear 2nd order model")); }
		if (this->MaType == "Nonlinear 3rd order") { context.append(QObject::tr("Nonlinear 3rd order model")); }
		if (this->MaType == "Nonlinear Kerr") { context.append(QObject::tr("Nonlinear Kerr model")); }
		if (this->MaType == "Nonlinear Raman") { context.append(QObject::tr("Nonlinear Raman model")); }
		if (this->MaType == "User") {
			context.append(this->magneticFit.getAttributesDesc());
		}
		context.append("\n");
	}


	// > 参数（热传导）
	if (this->ThermalConductivity != 0) {
		context.append(QObject::tr("Thermal cond.  = "));
		context.append(QString::number(this->ThermalConductivity));
		context.append("\n");
	}
	if (this->DynamicViscosity != 0) {
		context.append(QObject::tr("Dyn. viscosity = "));
		context.append(QString::number(this->DynamicViscosity));
		context.append("\n");
	}
	if (this->HeatCapacity != 0) {
		context.append(QObject::tr("Heat capacity  = "));
		context.append(QString::number(this->HeatCapacity));
		context.append("\n");
	}

	// > 参数（密度）
	if (this->Rho != 0) {
		context.append(QObject::tr("Rho            = "));
		context.append(QString::number(this->Rho));
		context.append("\n");
	}

	return context;
}
/*-------------------------------------------------
		描述 - 设置文件路径
*/
void C_StandardMaterial::setLocation(QString location) {
	this->m_location = location;
}
QString C_StandardMaterial::getLocation() {
	return this->m_location;
}


/*-------------------------------------------------
		分类 - 设置分类
*/
QString C_StandardMaterial::getFolder() {
	QStringList str_list = this->name.split("/");
	str_list.removeLast();
	return str_list.join("/");
}



/*-------------------------------------------------
		材料库数据 - 主要分类项
*/
QStringList C_StandardMaterial::getLibParam_TypeOptions() {
	// 不要翻译，否则无法匹配
	return QStringList() << "<All>" << "Normal" << "Lossy metal" << "PEC";
}
/*-------------------------------------------------
		材料库数据 - 属性分类项
*/
QStringList C_StandardMaterial::getLibParam_AttrOptions() {
	// 不要翻译，否则无法匹配
	return QStringList() << "<All>" << "Diffusivity" << "Dispersive eps"
		<< "Dispersive mu" << "Dyn. viscosity" << "Electric cond."
		<< "Electric tand" << "Epsilon" << "Mu" << "Poisson's ratio"
		<< "Rho" << "Specific heat" << "Thermal cond."
		<< "Thermal expan" << "Young's modulus";
}

/*-------------------------------------------------
		材料库数据 - 类型描述
*/
QString C_StandardMaterial::getLibParam_MaterialSets(){
	if (this->type.contains("Lossy metal")){
		return "Low Frequency, Default";
	}else{
		return "Default";
	}
}

/*-------------------------------------------------
		材料库数据 - 类型图标
*/
QIcon C_StandardMaterial::getLibParam_TypeIcon(){
    QString iconPath = ":/modeling" + QRC_IconPath + "/modeling/" + (this->type.contains("Lossy metal") ? "NewMaterialColor2.png" : "NewMaterialColor.png");

    if (m_IconCache.contains(iconPath)) {
        return m_IconCache[iconPath];
    } else {
        QIcon icon(iconPath);
        m_IconCache.insert(iconPath, icon);
        return icon;
    }
}

/*-------------------------------------------------
		颜色 - 设置颜色
*/
void C_StandardMaterial::setColor(QColor color) {
	this->m_color = color;
}
QColor C_StandardMaterial::getColor() {
	return this->m_color;
}
QString C_StandardMaterial::getMtdDescription_Color() {
	QString result;
	result.append("\"");
	result.append(QString::number((double)this->m_color.red() / 255.0));
	result.append("\"");
	result.append(", ");
	result.append("\"");
	result.append(QString::number((double)this->m_color.green() / 255.0));
	result.append("\"");
	result.append(", ");
	result.append("\"");
	result.append(QString::number((double)this->m_color.blue() / 255.0));
	result.append("\"");
	return result;
}

/*-------------------------------------------------
		颜色 - 获取颜色图标
*/
QIcon C_StandardMaterial::getIcon_Color() {

	// > 材料边框图
	QPixmap pixmap_border = QPixmap(":/modeling" + QRC_IconPath + "/modeling/NewMaterialLine.png");
	pixmap_border = pixmap_border.scaled(64, 64);		//（注意图像太大，要缩放）

	// > 遮罩设置（白色为填充，黑色、灰色、无色全部为不填充）
	QPixmap pixmap_mask = QPixmap(":/modeling" + QRC_IconPath + "/modeling/MaterialMask.png");
	pixmap_mask = pixmap_mask.scaled(64, 64);		//（注意图像太大，要缩放）
	QBitmap bitmap_mask = pixmap_mask.createMaskFromColor(Qt::white, Qt::MaskOutColor);

	// > 遮罩过滤的颜色
	QPixmap pixmap_colorWithMask(64, 64);
	pixmap_colorWithMask.fill(this->m_color);
	pixmap_colorWithMask.setMask(bitmap_mask);

	// > 画板绘制
	QPixmap pixmap_result(64, 64);
	pixmap_result.fill(Qt::transparent);
	QPainter painter(&pixmap_result);
	painter.drawPixmap(0, 0, pixmap_colorWithMask);
	painter.drawPixmap(0, 0, pixmap_border);
	return QIcon(pixmap_result);
}

/*-------------------------------------------------
		颜色 - 设置透明度
*/
void C_StandardMaterial::setTransparency(double transparency) {
	this->m_transparency = transparency;
}
double C_StandardMaterial::getTransparency() {
	return this->m_transparency;
}
/*-------------------------------------------------
		颜色 - 设置透明度（百分比int值）
*/
void C_StandardMaterial::setTransparency_Per(int transparency_per) {
	this->m_transparency = transparency_per*0.01;
}
int C_StandardMaterial::getTransparency_Per() {
	return this->m_transparency * 100;
}
QString C_StandardMaterial::getMtdDescription_Transparency() {
	QString result;
	result.append("\"");
	result.append(QString::number(this->getTransparency_Per()));
	result.append("\"");
	return result;
}


/*-------------------------------------------------
		访问器 - 获取子类类名
*/
QString C_StandardMaterial::get__CLASS_NAME__(){
	return "C_StandardMaterial";
}
/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
QString C_StandardMaterial::get__CLASS_NAME__Chinese(){
	return "材料";
}
/*-------------------------------------------------
		访问器 - 获取所有继承的类名
*/
QStringList C_StandardMaterial::get__CLASS_INHERITED_LIST__(){
	return C_UndoDataBase::get__CLASS_INHERITED_LIST__() << "C_StandardMaterial";
}
/*-------------------------------------------------
		访问器 - 是否继承了某类
*/
bool C_StandardMaterial::get__CLASS_IS_INSTANCE_OF__(QString class_name){
	return this->get__CLASS_INHERITED_LIST__().contains(class_name);
}
/*-------------------------------------------------
		访问器 - 原型复制对象
*/
C_StandardMaterial C_StandardMaterial::clone(){
	return C_StandardMaterial(*this);
}
QSharedPointer<C_StandardMaterial> C_StandardMaterial::clonePtr(){
	return QSharedPointer<C_StandardMaterial>(new C_StandardMaterial(*this));
}
C_UndoDataBasePtr C_StandardMaterial::cloneParentPtr(){
	return this->clonePtr();
}

/*-------------------------------------------------
		复制对象
*/
void C_StandardMaterial::copyFrom(C_StandardMaterial* a){
	this->name = a->name;

	// > 属性
	this->type = a->type;
	this->frqType = a->frqType;

	// > 参数（基本参数）
	this->Epsilon = a->Epsilon;
	this->Mu = a->Mu;

	// > 参数（有耗金属基本参数）
	//this->EleConductivity = a->EleConductivity;
	this->MuLossyMetal = a->MuLossyMetal;
	this->SurfaceRoughness = a->SurfaceRoughness;

	// > 参数（各项异性 Anisotropic）
	this->EpsilonX = a->EpsilonX;
	this->EpsilonY = a->EpsilonY;
	this->EpsilonZ = a->EpsilonZ;
	this->MuX = a->MuX;
	this->MuY = a->MuY;
	this->MuZ = a->MuZ;

	// > 参数（电导率/磁导率）
	this->ElType = a->ElType;								//电导率类型
	this->MaType = a->MaType;								//磁导率类型

	this->ElConductivity = a->ElConductivity;				//（Electric conductivity）
	this->ElDeltaEl = a->ElDeltaEl;
	this->ElDeltaElAtFrequency = a->ElDeltaElAtFrequency;
	this->MaConductivity = a->MaConductivity;
	this->MaDeltaMag = a->MaDeltaMag;
	this->MaDeltaMagAtFrequency = a->MaDeltaMagAtFrequency;

	this->ElEpsilonInfinity = a->ElEpsilonInfinity;			//（Debye 1st/2st order）
	this->ElEpsilonStatic1 = a->ElEpsilonStatic1;
	this->ElEpsilonStatic2 = a->ElEpsilonStatic2;
	this->ElRelaxationTime1 = a->ElRelaxationTime1;
	this->ElRelaxationTime2 = a->ElRelaxationTime2;
	this->MaMuInfinity = a->MaMuInfinity;
	this->MaMuStatic1 = a->MaMuStatic1;
	this->MaMuStatic2 = a->MaMuStatic2;
	this->MaRelaxationTime1 = a->MaRelaxationTime1;
	this->MaRelaxationTime2 = a->MaRelaxationTime2;

	this->ElPlasmaFrequency = a->ElPlasmaFrequency;			//（Drude）
	this->ElCollisionFrequency = a->ElCollisionFrequency;
	this->ElFieldBreakdown = a->ElFieldBreakdown;
	this->ElPlasmaMaintainFrequency = a->ElPlasmaMaintainFrequency;
	this->MaPlasmaFrequency = a->MaPlasmaFrequency;
	this->MaCollisionFrequency = a->MaCollisionFrequency;

	this->ElResonanceFrequency = a->ElResonanceFrequency;	//（Lorentz）
	this->ElDampingFrequency = a->ElDampingFrequency;
	this->MaResonanceFrequency = a->MaResonanceFrequency;
	this->MaDampingFrequency = a->MaDampingFrequency;

	this->ElCyclotronFrequency = a->ElCyclotronFrequency;	//（Gyrotropic）
	this->ElBiasingDirectionX = a->ElBiasingDirectionX;
	this->ElBiasingDirectionY = a->ElBiasingDirectionY;
	this->ElBiasingDirectionZ = a->ElBiasingDirectionZ;
	this->MaLandFactor = a->MaLandFactor;
	this->MaSatMagnetization = a->MaSatMagnetization;
	this->MaResonanceLineWidth = a->MaResonanceLineWidth;
	this->MaFieldVectorX = a->MaFieldVectorX;
	this->MaFieldVectorY = a->MaFieldVectorY;
	this->MaFieldVectorZ = a->MaFieldVectorZ;

	this->ElAlpha0 = a->ElAlpha0;							//（General 1st/2st order）
	this->ElBeta0 = a->ElBeta0;
	this->ElAlpha1 = a->ElAlpha1;
	this->ElBeta1 = a->ElBeta1;
	this->MaAlpha0 = a->MaAlpha0;
	this->MaBeta0 = a->MaBeta0;
	this->MaAlpha1 = a->MaAlpha1;
	this->MaBeta1 = a->MaBeta1;

	this->ElChi2 = a->ElChi2;								//（Nonlinear 2nd/3rd order）
	this->ElChi3 = a->ElChi3;
	this->MaChi2 = a->MaChi2;
	this->MaChi3 = a->MaChi3;

	this->ElChi3Infinity = a->ElChi3Infinity;				//（Nonlinear Kerr）
	this->ElChi3Static = a->ElChi3Static;
	this->MaChi3Infinity = a->MaChi3Infinity;
	this->MaChi3Static = a->MaChi3Static;

	//（重复参数）											//（Nonlinear Raman）

	this->dielectricFit = a->dielectricFit;					//（User）
	this->magneticFit = a->magneticFit;


	// > 参数（热传导）
	this->ThermalType = a->ThermalType;
	this->ThermalConductivity = a->ThermalConductivity;
	this->DynamicViscosity = a->DynamicViscosity;
	this->HeatCapacity = a->HeatCapacity;

	// > 参数（密度）
	this->Rho = a->Rho;


	// > 映射表标记
	//	（映射不能被复制）

	// > 描述
	this->m_description = a->m_description;
	this->m_location = a->m_location;

	// > 分类
	//	（无）

	// > UI - 颜色
	//	（无）
}
/*-------------------------------------------------
		运算符重载
*/
const bool C_StandardMaterial::operator== (const C_StandardMaterial& a)const {
	return C_UndoDataBase::operator==(a);
}
/*-------------------------------------------------
		空判断
*/
bool C_StandardMaterial::isNull(){
	return C_UndoDataBase::isNull();
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_StandardMaterial::getJsonObject_undoChildData(){
	QJsonObject obj;

	// > 属性
	obj.insert("type", this->type);
	obj.insert("frqType", this->frqType);

	// > 参数（基本参数）
	obj.insert("Epsilon", this->Epsilon);
	obj.insert("Mu", this->Mu);

	// > 参数（有耗金属基本参数）
	// obj.insert("EleConductivity", this->EleConductivity);
	obj.insert("MuLossyMetal", this->MuLossyMetal);
	obj.insert("SurfaceRoughness", this->SurfaceRoughness);

	// > 参数（各项异性 Anisotropic）
	obj.insert("EpsilonX", this->EpsilonX);
	obj.insert("EpsilonY", this->EpsilonY);
	obj.insert("EpsilonZ", this->EpsilonZ);
	obj.insert("MuX", this->MuX);
	obj.insert("MuY", this->MuY);
	obj.insert("MuZ", this->MuZ);

	// > 参数（电导率/磁导率）
	obj.insert("DielectricUserOption", m_DielectricUserOption == 0 ? "Disp" : "User");
	obj.insert("MagneticUserOption", m_MagneticUserOption == 0 ? "Disp" : "User");

	// 电导率选择了自定列表文件之后，通过User传递给求解器进行匹配解析
	if (this->m_DielectricUserOption == 1){
		obj.insert("ElType", "User");
	}else{
		obj.insert("ElType", this->ElType); // 电导率类型
	}
	obj.insert("MaType", this->MaType);								//磁导率类型

	obj.insert("ElConductivity", this->ElConductivity);				//（Electric conductivity）
	obj.insert("ElDeltaEl", this->ElDeltaEl);
	obj.insert("ElDeltaElAtFrequency", this->ElDeltaElAtFrequency);
	obj.insert("MaConductivity", this->MaConductivity);
	obj.insert("MaDeltaMag", this->MaDeltaMag);
	obj.insert("MaDeltaMagAtFrequency", this->MaDeltaMagAtFrequency);

	obj.insert("ElEpsilonInfinity", this->ElEpsilonInfinity);		//（Debye 1st/2st order）
	obj.insert("ElEpsilonStatic1", this->ElEpsilonStatic1);
	obj.insert("ElEpsilonStatic2", this->ElEpsilonStatic2);
	obj.insert("ElRelaxationTime1", this->ElRelaxationTime1);
	obj.insert("ElRelaxationTime2", this->ElRelaxationTime2);
	obj.insert("MaMuInfinity", this->MaMuInfinity);
	obj.insert("MaMuStatic1", this->MaMuStatic1);
	obj.insert("MaMuStatic2", this->MaMuStatic2);
	obj.insert("MaRelaxationTime1", this->MaRelaxationTime1);
	obj.insert("MaRelaxationTime2", this->MaRelaxationTime2);

	obj.insert("ElPlasmaFrequency", this->ElPlasmaFrequency);		//（Drude）
	obj.insert("ElCollisionFrequency", this->ElCollisionFrequency);
	obj.insert("ElFieldBreakdown", this->ElFieldBreakdown);
	obj.insert("ElPlasmaMaintainFrequency", this->ElPlasmaMaintainFrequency);
	obj.insert("MaPlasmaFrequency", this->MaPlasmaFrequency);
	obj.insert("MaCollisionFrequency", this->MaCollisionFrequency);

	obj.insert("ElResonanceFrequency", this->ElResonanceFrequency);	//（Lorentz）
	obj.insert("ElDampingFrequency", this->ElDampingFrequency);
	obj.insert("MaResonanceFrequency", this->MaResonanceFrequency);
	obj.insert("MaDampingFrequency", this->MaDampingFrequency);

	obj.insert("ElCyclotronFrequency", this->ElCyclotronFrequency);	//（Gyrotropic）
	obj.insert("ElBiasingDirectionX", this->ElBiasingDirectionX);
	obj.insert("ElBiasingDirectionY", this->ElBiasingDirectionY);
	obj.insert("ElBiasingDirectionZ", this->ElBiasingDirectionZ);
	obj.insert("MaLandFactor", this->MaLandFactor);
	obj.insert("MaSatMagnetization", this->MaSatMagnetization);
	obj.insert("MaResonanceLineWidth", this->MaResonanceLineWidth);
	obj.insert("MaFieldVectorX", this->MaFieldVectorX);
	obj.insert("MaFieldVectorY", this->MaFieldVectorY);
	obj.insert("MaFieldVectorZ", this->MaFieldVectorZ);

	obj.insert("ElAlpha0", this->ElAlpha0);							//（General 1st/2st order）
	obj.insert("ElBeta0", this->ElBeta0);
	obj.insert("ElAlpha1", this->ElAlpha1);
	obj.insert("ElBeta1", this->ElBeta1);
	obj.insert("MaAlpha0", this->MaAlpha0);
	obj.insert("MaBeta0", this->MaBeta0);
	obj.insert("MaAlpha1", this->MaAlpha1);
	obj.insert("MaBeta1", this->MaBeta1);

	obj.insert("ElChi2", this->ElChi2);								//（Nonlinear 2nd/3rd order）
	obj.insert("ElChi3", this->ElChi3);
	obj.insert("MaChi2", this->MaChi2);
	obj.insert("MaChi3", this->MaChi3);

	obj.insert("ElChi3Infinity", this->ElChi3Infinity);				//（Nonlinear Kerr）
	obj.insert("ElChi3Static", this->ElChi3Static);
	obj.insert("MaChi3Infinity", this->MaChi3Infinity);
	obj.insert("MaChi3Static", this->MaChi3Static);

	//（重复参数）													//（Nonlinear Raman）

	obj.insert("dielectricFit", this->dielectricFit.getJsonObject());	//（User）
	obj.insert("magneticFit", this->magneticFit.getJsonObject());


	// > 参数（热传导）
	obj.insert("ThermalType", this->ThermalType);
	obj.insert("ThermalConductivity", this->ThermalConductivity);
	obj.insert("DynamicViscosity", this->DynamicViscosity);
	obj.insert("HeatCapacity", this->HeatCapacity);

	// > 参数（密度）
	obj.insert("Rho", this->Rho);


	// > 映射表
	obj.insert("mappingId", this->m_mappingId);

	// > 描述
	obj.insert("description", this->m_description);
	obj.insert("location", this->m_location);

	// > 分类
	//	（无）


	// > UI - 颜色
	obj.insert("color", TTool::_to_HtmlColor_(this->m_color));

	// > 写入用户自定义色散数据弹窗的数据
	if (this->m_DielectricUserOption == 1){
		obj.insert("dispersionUser", this->m_C_DispersionUser.getJsonObject_undoChildData());
	}

	return obj;
}

double C_StandardMaterial::getDoubleFromObject(const QJsonObject &obj, const QString &key, double defaultValue) const
{
	if (obj.contains(key))
	{
		return obj.value(key).toDouble(defaultValue);
	}
	return defaultValue;
}

QString C_StandardMaterial::getStringFromObject(const QJsonObject &obj, const QString &key, const QString &defaultValue) const
{
	if (obj.contains(key))
	{
		return obj.value(key).toString(defaultValue);
	}
	return defaultValue;
}

/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_StandardMaterial::setJsonObject_undoChildData(QJsonObject obj){
	// > 属性
	this->type = getStringFromObject(obj, "type");
	this->frqType = getStringFromObject(obj, "frqType");

	// > 参数（基本参数）
	this->Epsilon = getDoubleFromObject(obj, "Epsilon", 1.0);
	this->Mu = getDoubleFromObject(obj, "Mu", 1.0);

	// > 参数（有耗金属基本参数）
	// this->EleConductivity = getDoubleFromObject(obj, "EleConductivity");
	this->MuLossyMetal = getDoubleFromObject(obj, "MuLossyMetal");
	this->SurfaceRoughness = getDoubleFromObject(obj, "SurfaceRoughness");

	// > 参数（各项异性 Anisotropic）
	this->EpsilonX = getDoubleFromObject(obj, "EpsilonX");
	this->EpsilonY = getDoubleFromObject(obj, "EpsilonY");
	this->EpsilonZ = getDoubleFromObject(obj, "EpsilonZ");
	this->MuX = getDoubleFromObject(obj, "MuX");
	this->MuY = getDoubleFromObject(obj, "MuY");
	this->MuZ = getDoubleFromObject(obj, "MuZ");

	// > 参数（电导率/磁导率）
	this->m_DielectricUserOption = getStringFromObject(obj, "DielectricUserOption") == "Disp" ? 0 : 1;
	this->m_MagneticUserOption = getStringFromObject(obj, "MagneticUserOption") == "Disp" ? 0 : 1;

	this->ElType = getStringFromObject(obj, "ElType");
	this->MaType = getStringFromObject(obj, "MaType");

	this->ElConductivity = getDoubleFromObject(obj, "ElConductivity");
	this->ElDeltaEl = getDoubleFromObject(obj, "ElDeltaEl");
	this->ElDeltaElAtFrequency = getDoubleFromObject(obj, "ElDeltaElAtFrequency");
	this->MaConductivity = getDoubleFromObject(obj, "MaConductivity");
	this->MaDeltaMag = getDoubleFromObject(obj, "MaDeltaMag");
	this->MaDeltaMagAtFrequency = getDoubleFromObject(obj, "MaDeltaMagAtFrequency");

	this->ElEpsilonInfinity = getDoubleFromObject(obj, "ElEpsilonInfinity");
	this->ElEpsilonStatic1 = getDoubleFromObject(obj, "ElEpsilonStatic1");
	this->ElEpsilonStatic2 = getDoubleFromObject(obj, "ElEpsilonStatic2");
	this->ElRelaxationTime1 = getDoubleFromObject(obj, "ElRelaxationTime1");
	this->ElRelaxationTime2 = getDoubleFromObject(obj, "ElRelaxationTime2");
	this->MaMuInfinity = getDoubleFromObject(obj, "MaMuInfinity");
	this->MaMuStatic1 = getDoubleFromObject(obj, "MaMuStatic1");
	this->MaMuStatic2 = getDoubleFromObject(obj, "MaMuStatic2");
	this->MaRelaxationTime1 = getDoubleFromObject(obj, "MaRelaxationTime1");
	this->MaRelaxationTime2 = getDoubleFromObject(obj, "MaRelaxationTime2");

	this->ElPlasmaFrequency = getDoubleFromObject(obj, "ElPlasmaFrequency"); //（Drude）
	this->ElCollisionFrequency = getDoubleFromObject(obj, "ElCollisionFrequency");
	this->ElFieldBreakdown = getDoubleFromObject(obj, "ElFieldBreakdown");
	this->ElPlasmaMaintainFrequency = getDoubleFromObject(obj, "ElPlasmaMaintainFrequency");
	this->MaPlasmaFrequency = getDoubleFromObject(obj, "MaPlasmaFrequency");
	this->MaCollisionFrequency = getDoubleFromObject(obj, "MaCollisionFrequency");

	this->ElResonanceFrequency = getDoubleFromObject(obj, "ElResonanceFrequency"); //（Lorentz）
	this->ElDampingFrequency = getDoubleFromObject(obj, "ElDampingFrequency");
	this->MaResonanceFrequency = getDoubleFromObject(obj, "MaResonanceFrequency");
	this->MaDampingFrequency = getDoubleFromObject(obj, "MaDampingFrequency");

	this->ElCyclotronFrequency = getDoubleFromObject(obj, "ElCyclotronFrequency"); //（Gyrotropic）
	this->ElBiasingDirectionX = getDoubleFromObject(obj, "ElBiasingDirectionX");
	this->ElBiasingDirectionY = getDoubleFromObject(obj, "ElBiasingDirectionY");
	this->ElBiasingDirectionZ = getDoubleFromObject(obj, "ElBiasingDirectionZ");
	this->MaLandFactor = getDoubleFromObject(obj, "MaLandFactor");
	this->MaSatMagnetization = getDoubleFromObject(obj, "MaSatMagnetization");
	this->MaResonanceLineWidth = getDoubleFromObject(obj, "MaResonanceLineWidth");
	this->MaFieldVectorX = getDoubleFromObject(obj, "MaFieldVectorX");
	this->MaFieldVectorY = getDoubleFromObject(obj, "MaFieldVectorY");
	this->MaFieldVectorZ = getDoubleFromObject(obj, "MaFieldVectorZ");

	this->ElAlpha0 = getDoubleFromObject(obj, "ElAlpha0"); //（General 1st/2st order）
	this->ElBeta0 = getDoubleFromObject(obj, "ElBeta0");
	this->ElAlpha1 = getDoubleFromObject(obj, "ElAlpha1");
	this->ElBeta1 = getDoubleFromObject(obj, "ElBeta1");
	this->MaAlpha0 = getDoubleFromObject(obj, "MaAlpha0");
	this->MaBeta0 = getDoubleFromObject(obj, "MaBeta0");
	this->MaAlpha1 = getDoubleFromObject(obj, "MaAlpha1");
	this->MaBeta1 = getDoubleFromObject(obj, "MaBeta1");

	this->ElChi2 = getDoubleFromObject(obj, "ElChi2"); //（Nonlinear 2nd/3rd order）
	this->ElChi3 = getDoubleFromObject(obj, "ElChi3");
	this->MaChi2 = getDoubleFromObject(obj, "MaChi2");
	this->MaChi3 = getDoubleFromObject(obj, "MaChi3");

	this->ElChi3Infinity = getDoubleFromObject(obj, "ElChi3Infinity"); //（Nonlinear Kerr）
	this->ElChi3Static = getDoubleFromObject(obj, "ElChi3Static");
	this->MaChi3Infinity = getDoubleFromObject(obj, "MaChi3Infinity");
	this->MaChi3Static = getDoubleFromObject(obj, "MaChi3Static");

	// （重复参数）
	if (obj.contains("dielectricFit"))
	{
		QJsonObject dielectricFitObj = obj.value("dielectricFit").toObject();
		if (!dielectricFitObj.isEmpty())
		{
			this->dielectricFit.setJsonObject(dielectricFitObj);
		}
	}
	if (obj.contains("magneticFit"))
	{
		QJsonObject magneticFitObj = obj.value("magneticFit").toObject();
		if (!magneticFitObj.isEmpty())
		{
			this->magneticFit.setJsonObject(magneticFitObj);
		}
	}

	// > 参数（热传导）
	this->ThermalType = getStringFromObject(obj, "ThermalType");
	this->ThermalConductivity = getDoubleFromObject(obj, "ThermalConductivity");
	this->DynamicViscosity = getDoubleFromObject(obj, "DynamicViscosity");
	this->HeatCapacity = getDoubleFromObject(obj, "HeatCapacity");

	// > 参数（密度）
	this->Rho = getDoubleFromObject(obj, "Rho");

	// > 映射表
	if (obj.value("mappingId").isUndefined() == false) {
		this->m_mappingId = obj.value("mappingId").toInt();
	}else {
		this->m_mappingId = obj.value("defaultId").toInt();
	}

	// > 描述
	this->m_description = getStringFromObject(obj, "description");
	this->m_location = getStringFromObject(obj, "location");

	// > 分类
	// （无）

	// > UI - 颜色
	if (obj.contains("color"))
	{
		QString colorStr = getStringFromObject(obj, "color");
		if (!colorStr.isEmpty())
		{
			this->m_color = QColor(colorStr);
		}
	}

	// > 读取用户自定义色散数据弹窗的数据
	if (obj.contains("dispersionUser"))
	{
		QJsonObject dispersionUserObj = obj.value("dispersionUser").toObject();
		if (!dispersionUserObj.isEmpty())
		{
			this->m_C_DispersionUser.setJsonObject_undoChildData(dispersionUserObj);
		}
	}
}
