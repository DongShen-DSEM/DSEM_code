#include "stdafx.h"
#include "C_STD_StandardMaterial.h"

/*
-----==========================================================-----
		类：		材料 数据类（STD）.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/

C_STD_StandardMaterial::C_STD_StandardMaterial() {

	// > 属性
	this->name = "";
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

	this->ElAlpha0 = 0.0;					//（General 1st/2st order）
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
	this->Rho = 0;

	
	// > 映射表
	this->m_mappingId = 0;

	// > 描述
	this->m_description = "";

	// > UI
	//	（无）

}
C_STD_StandardMaterial::~C_STD_StandardMaterial(){
}


/*-------------------------------------------------
		属性 - 设置名称
*/
void C_STD_StandardMaterial::setName(string name){

	//（不处理，直接获取作为名称）

	this->name = name;
}
string C_STD_StandardMaterial::getName(){
	return this->name;
}
/*-------------------------------------------------
		属性 - 设置类型
*/
void C_STD_StandardMaterial::setType(string type){
	this->type = type;
}
string C_STD_StandardMaterial::getType(){
	return this->type;
}
vector<string> C_STD_StandardMaterial::getAllType_Material() {
	vector<string> result;
	result.push_back("PEC");		//（固定的名词）
	result.push_back("Normal");
	//result.push_back("Anisotropic");
	//result.push_back("Lossy metal");
	return result;
}
vector<string> C_STD_StandardMaterial::getAllType_DielectricDispersion() {
	vector<string> result;
	result.push_back("None");		//（固定的名词）
	result.push_back("Debye 1st order");
	result.push_back("Debye 2st order");
	result.push_back("Drude");
	result.push_back("Lorentz");
	result.push_back("Gyrotropic");
	result.push_back("General 1st order");
	result.push_back("General 2nd order");
	result.push_back("Nonlinear 2nd order");
	result.push_back("Nonlinear 3rd order");
	result.push_back("Nonlinear Kerr");
	result.push_back("Nonlinear Raman");
	result.push_back("User");
	return result;
}
vector<string> C_STD_StandardMaterial::getAllType_ElectricConductivity() {
	vector<string> result;
	result.push_back("None");		//（固定的名词）
	result.push_back("El.conductivity");
	result.push_back("Tangent delta el");
	return result;
}
vector<string> C_STD_StandardMaterial::getAllType_MagneticDispersion() {
	vector<string> result;
	result.push_back("None");		//（固定的名词）
	result.push_back("Debye 1st order");
	result.push_back("Debye 2st order");
	result.push_back("Drude");
	result.push_back("Lorentz");
	result.push_back("Gyrotropic");
	result.push_back("General 1st order");
	result.push_back("General 2nd order");
	result.push_back("Nonlinear 2nd order");
	result.push_back("Nonlinear 3rd order");
	result.push_back("Nonlinear Kerr");
	result.push_back("Nonlinear Raman");
	result.push_back("User");
	return result;
}
vector<string> C_STD_StandardMaterial::getAllType_MagneticConductivity() {
	vector<string> result;
	result.push_back("None");		//（固定的名词）
	result.push_back("Mag.conductivity");
	result.push_back("Tangent delta mag");
	return result;
}
/*-------------------------------------------------
		属性 - 设置频率类型
*/
void C_STD_StandardMaterial::setFrqType(string frqType) {
	this->frqType = frqType;
}
string C_STD_StandardMaterial::getFrqType() {
	return this->frqType;
}


/*-------------------------------------------------
		映射表 - 设置映射ID
*/
void C_STD_StandardMaterial::setMappingId(int mapping_id) {
	this->m_mappingId = mapping_id;
}
/*-------------------------------------------------
		映射表 - 获取映射ID
*/
int C_STD_StandardMaterial::getMappingId() {
	return this->m_mappingId;
}
/*-------------------------------------------------
		映射表 - 是否为默认材料
*/
bool C_STD_StandardMaterial::isDefaultMaterial() {
	return this->m_mappingId < 0;
}
bool C_STD_StandardMaterial::isDefaultMaterial_PEC() {
	return this->m_mappingId == -1;
}
bool C_STD_StandardMaterial::isDefaultMaterial_Vacuum() {
	return this->m_mappingId == -2;
}

/*-------------------------------------------------
		描述 - 设置描述
*/
void C_STD_StandardMaterial::setDescription(string context){
	this->m_description = context;
}
string C_STD_StandardMaterial::getDescription(){
	return this->m_description;
}
/*-------------------------------------------------
		描述 - 设置文件路径
*/
void C_STD_StandardMaterial::setLocation(string location){
	this->m_location = location;
}
string C_STD_StandardMaterial::getLocation(){
	return this->m_location;
}


/*-------------------------------------------------
		访问器 - 获取子类类名
*/
string C_STD_StandardMaterial::get__CLASS_NAME__(){
	return "C_STD_StandardMaterial";
}
/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
string C_STD_StandardMaterial::get__CLASS_NAME__Chinese(){
	return "材料";
}

/*-------------------------------------------------
		运算符重载
*/
const bool C_STD_StandardMaterial::operator== (const C_STD_StandardMaterial& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		空判断
*/
bool C_STD_StandardMaterial::isNull(){
	return this->name == "";
}
/*-------------------------------------------------
		JsonObject -> 实体类
*/
void C_STD_StandardMaterial::setJsonObject(const Value& obj) {

	// > 属性
	this->name = obj["name"].GetString();
	this->type = obj["type"].GetString();
	this->frqType = obj["frqType"].GetString();


	// > 参数（基本参数）
	this->Epsilon = obj["Epsilon"].GetDouble();
	this->Mu = obj["Mu"].GetDouble();
	if (this->Epsilon == 0) { this->Epsilon = 1; }
	if (this->Mu == 0) { this->Mu = 1; }

	// > 参数（有耗金属基本参数）
	// this->EleConductivity = obj["EleConductivity"].GetDouble();
	this->MuLossyMetal = obj["MuLossyMetal"].GetDouble();
	this->SurfaceRoughness = obj["SurfaceRoughness"].GetDouble();

	// > 参数（各项异性 Anisotropic）
	this->EpsilonX = obj["EpsilonX"].GetDouble();
	this->EpsilonY = obj["EpsilonY"].GetDouble();
	this->EpsilonZ = obj["EpsilonZ"].GetDouble();
	this->MuX = obj["MuX"].GetDouble();
	this->MuY = obj["MuY"].GetDouble();
	this->MuZ = obj["MuZ"].GetDouble();

	// > 参数（电导率/磁导率）
	this->ElType = obj["ElType"].GetString();								//电导率类型
	this->MaType = obj["MaType"].GetString();								//磁导率类型

	this->ElConductivity = obj["ElConductivity"].GetDouble();				//（Electric conductivity）
	this->ElDeltaEl = obj["ElDeltaEl"].GetDouble();
	this->ElDeltaElAtFrequency = obj["ElDeltaElAtFrequency"].GetDouble();
	this->MaConductivity = obj["MaConductivity"].GetDouble();
	this->MaDeltaMag = obj["MaDeltaMag"].GetDouble();
	this->MaDeltaMagAtFrequency = obj["MaDeltaMagAtFrequency"].GetDouble();

	this->ElEpsilonInfinity = obj["ElEpsilonInfinity"].GetDouble();			//（Debye 1st/2st order）
	this->ElEpsilonStatic1 = obj["ElEpsilonStatic1"].GetDouble();
	this->ElEpsilonStatic2 = obj["ElEpsilonStatic2"].GetDouble();
	this->ElRelaxationTime1 = obj["ElRelaxationTime1"].GetDouble();
	this->ElRelaxationTime2 = obj["ElRelaxationTime2"].GetDouble();
	this->MaMuInfinity = obj["MaMuInfinity"].GetDouble();
	this->MaMuStatic1 = obj["MaMuStatic1"].GetDouble();
	this->MaMuStatic2 = obj["MaMuStatic2"].GetDouble();
	this->MaRelaxationTime1 = obj["MaRelaxationTime1"].GetDouble();
	this->MaRelaxationTime2 = obj["MaRelaxationTime2"].GetDouble();

	this->ElPlasmaFrequency = obj["ElPlasmaFrequency"].GetDouble();			//（Drude）
	this->ElCollisionFrequency = obj["ElCollisionFrequency"].GetDouble();
	this->ElFieldBreakdown = obj["ElFieldBreakdown"].GetDouble();
	this->ElPlasmaMaintainFrequency = obj["ElPlasmaMaintainFrequency"].GetDouble();
	this->MaPlasmaFrequency = obj["MaPlasmaFrequency"].GetDouble();
	this->MaCollisionFrequency = obj["MaCollisionFrequency"].GetDouble();

	this->ElResonanceFrequency = obj["ElResonanceFrequency"].GetDouble();	//（Lorentz）
	this->ElDampingFrequency = obj["ElDampingFrequency"].GetDouble();
	this->MaResonanceFrequency = obj["MaResonanceFrequency"].GetDouble();
	this->MaDampingFrequency = obj["MaDampingFrequency"].GetDouble();

	this->ElCyclotronFrequency = obj["ElCyclotronFrequency"].GetDouble();	//（Gyrotropic）
	this->ElBiasingDirectionX = obj["ElBiasingDirectionX"].GetDouble();
	this->ElBiasingDirectionY = obj["ElBiasingDirectionY"].GetDouble();
	this->ElBiasingDirectionZ = obj["ElBiasingDirectionZ"].GetDouble();
	this->MaLandFactor = obj["MaLandFactor"].GetDouble();
	this->MaSatMagnetization = obj["MaSatMagnetization"].GetDouble();
	this->MaResonanceLineWidth = obj["MaResonanceLineWidth"].GetDouble();
	this->MaFieldVectorX = obj["MaFieldVectorX"].GetDouble();
	this->MaFieldVectorY = obj["MaFieldVectorY"].GetDouble();
	this->MaFieldVectorZ = obj["MaFieldVectorZ"].GetDouble();

	this->ElAlpha0 = obj["ElAlpha0"].GetDouble();							//（General 1st/2st order）
	this->ElBeta0 = obj["ElBeta0"].GetDouble();
	this->ElAlpha1 = obj["ElAlpha1"].GetDouble();
	this->ElBeta1 = obj["ElBeta1"].GetDouble();
	this->MaAlpha0 = obj["MaAlpha0"].GetDouble();
	this->MaBeta0 = obj["MaBeta0"].GetDouble();
	this->MaAlpha1 = obj["MaAlpha1"].GetDouble();
	this->MaBeta1 = obj["MaBeta1"].GetDouble();

	this->ElChi2 = obj["ElChi2"].GetDouble();								//（Nonlinear 2nd/3rd order）
	this->ElChi3 = obj["ElChi3"].GetDouble();
	this->MaChi2 = obj["MaChi2"].GetDouble();
	this->MaChi3 = obj["MaChi3"].GetDouble();

	this->ElChi3Infinity = obj["ElChi3Infinity"].GetDouble();				//（Nonlinear Kerr）
	this->ElChi3Static = obj["ElChi3Static"].GetDouble();
	this->MaChi3Infinity = obj["MaChi3Infinity"].GetDouble();
	this->MaChi3Static = obj["MaChi3Static"].GetDouble();

	//（重复参数）															//（Nonlinear Raman）

	this->dielectricFit.setJsonObject(obj["dielectricFit"]);				//（User）
	this->magneticFit.setJsonObject(obj["magneticFit"]);


	// > 参数（热传导）
	this->ThermalType = obj["ThermalType"].GetString();
	this->ThermalConductivity = obj["ThermalConductivity"].GetDouble();
	this->DynamicViscosity = obj["DynamicViscosity"].GetDouble();
	this->HeatCapacity = obj["HeatCapacity"].GetDouble();

	// > 参数（密度）
	this->Rho = obj["Rho"].GetDouble();


	// > 映射表
	this->m_mappingId = obj["mappingId"].GetInt();


	// > 描述
	this->m_description = obj["description"].GetString();
	this->m_location = obj["location"].GetString();

	// > 分类
	//	（无）
	
	// > UI
	//	（无）

	// > 读取用户自定义色散数据弹窗的数据
	if (this->ElType == "User") {
		this->m_C_DispersionUser.setJsonObject(obj["dispersionUser"]);
	}
}