#include "stdafx.h"
#include "C_STD_MaterialDispersionFit_Magnetic.h"


/*
-----==========================================================-----
		类：		材料属性-离散拟合-磁导率 数据类（STD）.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/

C_STD_MaterialDispersionFit_Magnetic::C_STD_MaterialDispersionFit_Magnetic(){

	// > 参数（基本参数）
	this->fittingScheme = 1;			//装配方案（0:Conductivity, 1:nth order）
	this->maxOrder = 10;
	this->errorLimit = 0.1;
	this->useFreqRange = false;

	this->usedOrder = 1;
	this->error = 0;

	// > 参数（表格参数）
	this->data_format = 0;				//（0:(Real,Imag), 1:(Real,Tand)）
	this->data_list.clear();
}

C_STD_MaterialDispersionFit_Magnetic::~C_STD_MaterialDispersionFit_Magnetic(){
}


/*-------------------------------------------------
		参数 - 获取装配方案
*/
vector<string> C_STD_MaterialDispersionFit_Magnetic::getTypeList_FittingScheme() {
	vector<string> result;
	result.push_back("Conductivity");	//（注意不要用此处的名词做判断，因为会被翻译）
	result.push_back("nth order");
	return result;
}


/*-------------------------------------------------
		访问器 - 获取子类类名
*/
string C_STD_MaterialDispersionFit_Magnetic::get__CLASS_NAME__(){
	return "C_STD_MaterialDispersionFit_Magnetic";
}
/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
string C_STD_MaterialDispersionFit_Magnetic::get__CLASS_NAME__Chinese(){
	return "材料属性-离散拟合-磁导率";
}


/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_STD_MaterialDispersionFit_Magnetic::setJsonObject(const Value& obj){

	// > 参数（基本参数）
	this->fittingScheme = obj["fittingScheme"].GetInt();
	this->maxOrder = obj["maxOrder"].GetInt();
	this->errorLimit = obj["errorLimit"].GetDouble();
	this->useFreqRange = obj["useFreqRange"].GetBool();

	this->usedOrder = obj["usedOrder"].GetInt();
	this->error = obj["error"].GetDouble();

	// > 参数（表格参数）
	this->data_format = obj["data_format"].GetInt();
	this->data_list.clear();
	const Value& arr = obj["data_list"].GetArray();
	for (int i = 0; i < arr.Size(); i++) {
		C_STD_MaterialDispersionFit_MagneticCell c;
		c.setJsonObject(arr[i].GetObj());
		this->data_list.push_back(c);
	}
}

/*
-----==========================================================-----
		类：		材料属性-离散拟合-磁导率 数据类.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/
C_STD_MaterialDispersionFit_MagneticCell::C_STD_MaterialDispersionFit_MagneticCell(){

	// > 参数
	this->freq = 0;
	this->value_real = 0;
	this->value_imag = 0;
	this->value_tand = 0;
	this->weight = 1;
}
C_STD_MaterialDispersionFit_MagneticCell::~C_STD_MaterialDispersionFit_MagneticCell(){
}

/*-------------------------------------------------
		参数 - 计算并覆盖tand值
*/
void C_STD_MaterialDispersionFit_MagneticCell::calculateTand() {
	this->value_tand = this->value_imag / this->value_real;
}
/*-------------------------------------------------
		参数 - 计算并覆盖imag值
*/
void C_STD_MaterialDispersionFit_MagneticCell::calculateImag() {
	this->value_imag = this->value_real * this->value_tand;
}

/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_STD_MaterialDispersionFit_MagneticCell::setJsonObject(const Value& obj) {

	this->freq = obj["freq"].GetDouble();
	this->value_real = obj["value_real"].GetDouble();
	this->value_imag = obj["value_imag"].GetDouble();
	this->value_tand = obj["value_tand"].GetDouble();
	this->weight = obj["weight"].GetDouble();

}