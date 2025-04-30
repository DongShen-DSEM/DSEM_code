#include "stdafx.h"
#include "C_MaterialDispersionFit_Magnetic.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		材料属性-离散拟合-磁导率 数据类.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料的数据类对象。
-----==========================================================-----
*/

C_MaterialDispersionFit_Magnetic::C_MaterialDispersionFit_Magnetic() : C_ContainerDataBase(){

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

C_MaterialDispersionFit_Magnetic::~C_MaterialDispersionFit_Magnetic(){
}


/*-------------------------------------------------
		参数 - 获取装配方案
*/
QStringList C_MaterialDispersionFit_Magnetic::getTypeList_FittingScheme() {
	return QStringList() << QObject::tr("Conductivity") << QObject::tr("nth order");
}
/*-------------------------------------------------
		参数 - 获取数据类型
*/
QStringList C_MaterialDispersionFit_Magnetic::getTypeList_DataFormat() {
	return QStringList() << QObject::tr("(Real,Imag)") << QObject::tr("(Real,Tand)");
}

/*-------------------------------------------------
		参数 - 获取属性描述
*/
QString C_MaterialDispersionFit_Magnetic::getAttributesDesc() {

	// > 装配方案（0:Conductivity, 1:nth order）
	if (this->fittingScheme == 0) {
		QString text;
		text.append(QObject::tr("Conductivity model"));
		text.append(QObject::tr("(fit)"));
		return text;
	}
	if (this->fittingScheme == 1) {
		QString text;
		text.append(QObject::tr("Nth order model"));
		text.append(",");
		text.append("N = ");
		text.append(QString::number(this->usedOrder));
		text.append(QObject::tr("(fit)"));
		return text;
	}
	return "";
}


/*-------------------------------------------------
		访问器 - 获取子类类名
*/
QString C_MaterialDispersionFit_Magnetic::get__CLASS_NAME__(){
	return "C_MaterialDispersionFit_Magnetic";
}
/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
QString C_MaterialDispersionFit_Magnetic::get__CLASS_NAME__Chinese(){
	return "材料属性-离散拟合-磁导率";
}
/*-------------------------------------------------
		访问器 - 获取所有继承的类名
*/
QStringList C_MaterialDispersionFit_Magnetic::get__CLASS_INHERITED_LIST__(){
	return C_ContainerDataBase::get__CLASS_INHERITED_LIST__() << "C_MaterialDispersionFit_Magnetic";
}
/*-------------------------------------------------
		访问器 - 是否继承了某类
*/
bool C_MaterialDispersionFit_Magnetic::get__CLASS_IS_INSTANCE_OF__(QString class_name){
	return this->get__CLASS_INHERITED_LIST__().contains(class_name);
}
/*-------------------------------------------------
		访问器 - 原型复制对象
*/
C_MaterialDispersionFit_Magnetic C_MaterialDispersionFit_Magnetic::clone(){
	return C_MaterialDispersionFit_Magnetic(*this);
}
QSharedPointer<C_MaterialDispersionFit_Magnetic> C_MaterialDispersionFit_Magnetic::clonePtr(){
	return QSharedPointer<C_MaterialDispersionFit_Magnetic>(new C_MaterialDispersionFit_Magnetic(*this));
}

/*-------------------------------------------------
		复制对象
*/
void C_MaterialDispersionFit_Magnetic::copyFrom(C_MaterialDispersionFit_Magnetic* a){

	// > 参数（基本参数）
	this->fittingScheme = a->fittingScheme;			//装配方案（0:Conductivity, 1:nth order）
	this->maxOrder = a->maxOrder;
	this->errorLimit = a->errorLimit;
	this->useFreqRange = a->useFreqRange;

	this->usedOrder = a->usedOrder;
	this->error = a->error;

	// > 参数（表格参数）
	this->data_format = a->data_format;				//（0:(Real,Imag), 1:(Real,Tand)）
	this->data_list = a->data_list;

}
/*-------------------------------------------------
		运算符重载
*/
const bool C_MaterialDispersionFit_Magnetic::operator== (const C_MaterialDispersionFit_Magnetic& a)const {
	return C_ContainerDataBase::operator==(a);
}
/*-------------------------------------------------
		空判断
*/
bool C_MaterialDispersionFit_Magnetic::isNull(){
	return C_ContainerDataBase::isNull();
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_MaterialDispersionFit_Magnetic::getJsonObject_childData(){
	QJsonObject obj;

	// > 参数（基本参数）
	obj.insert("fittingScheme", this->fittingScheme);
	obj.insert("maxOrder", this->maxOrder);
	obj.insert("errorLimit", this->fittingScheme);
	obj.insert("useFreqRange", this->useFreqRange);

	obj.insert("usedOrder", this->usedOrder);
	obj.insert("error", this->error);

	// > 参数（表格参数）
	obj.insert("data_format", this->data_format);
	QJsonArray arr;
	for (int i = 0; i < this->data_list.size(); i++) {
		C_MaterialDispersionFit_MagneticCell c = this->data_list.at(i);
		arr.append(c.getJsonObject());
	}
	obj.insert("data_list", arr);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_MaterialDispersionFit_Magnetic::setJsonObject_childData(QJsonObject obj){

	// > 参数（基本参数）
	this->fittingScheme = obj.value("fittingScheme").toInt();
	this->maxOrder = obj.value("maxOrder").toInt();
	this->errorLimit = obj.value("errorLimit").toDouble();
	this->useFreqRange = obj.value("useFreqRange").toBool();

	this->usedOrder = obj.value("usedOrder").toInt();
	this->error = obj.value("error").toDouble();

	// > 参数（表格参数）
	this->data_format = obj.value("data_format").toInt();
	this->data_list.clear();
	QJsonArray arr = obj.value("data_list").toArray();
	for (int i = 0; i < arr.size(); i++) {
		C_MaterialDispersionFit_MagneticCell c;
		c.setJsonObject(arr.at(i).toObject());
		this->data_list.append(c);
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
C_MaterialDispersionFit_MagneticCell::C_MaterialDispersionFit_MagneticCell(){

	// > 参数
	this->freq = 0;
	this->value_real = 0;
	this->value_imag = 0;
	this->value_tand = 0;
	this->weight = 1;
}
C_MaterialDispersionFit_MagneticCell::~C_MaterialDispersionFit_MagneticCell(){
}

/*-------------------------------------------------
		参数 - 计算并覆盖tand值
*/
void C_MaterialDispersionFit_MagneticCell::calculateTand() {
	this->value_tand = this->value_imag / this->value_real;
}
/*-------------------------------------------------
		参数 - 计算并覆盖imag值
*/
void C_MaterialDispersionFit_MagneticCell::calculateImag() {
	this->value_imag = this->value_real * this->value_tand;
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_MaterialDispersionFit_MagneticCell::getJsonObject() {
	QJsonObject obj;

	obj.insert("freq", this->freq);
	obj.insert("value_real", this->value_real);
	obj.insert("value_imag", this->value_imag);
	obj.insert("value_tand", this->value_tand);
	obj.insert("weight", this->weight);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_MaterialDispersionFit_MagneticCell::setJsonObject(QJsonObject obj) {

	this->freq = obj.value("freq").toDouble();
	this->value_real = obj.value("value_real").toDouble();
	this->value_imag = obj.value("value_imag").toDouble();
	this->value_tand = obj.value("value_tand").toDouble();
	this->weight = obj.value("weight").toDouble();

}