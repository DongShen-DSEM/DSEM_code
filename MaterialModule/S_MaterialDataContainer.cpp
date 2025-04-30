#include "stdafx.h"
#include "S_MaterialDataContainer.h"

#include "Source/ProjectModule/Undo/S_UndoStackManager.h"
#include "Source/MainModule/Global/S_ConsoleManager.h"
#include "Source/ResultModule/S_ResultDataContainer.h"
#include "Source/ModelingPreModule/S_ModelingPre_InteractiveManager.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		材料模块 - 数据容器.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		【数据容器 + 增删改查 + 撤回】
					其他类可以通过此容器获取到数据对象，数据操作后也会发出数据变化信号。
-----==========================================================-----
*/

S_MaterialDataContainer::S_MaterialDataContainer(){
	this->initDefaultMaterial();

	connect(this, &S_MaterialDataContainer::signal_deleteMaterialLine, S_ResultDataContainer::getInstance(), &S_ResultDataContainer::onDeleteMaterialLine);
}

S_MaterialDataContainer::~S_MaterialDataContainer() {
}

/* --------------------------------------------------------------
----------SourceManager 单例
*/
S_MaterialDataContainer* S_MaterialDataContainer::project_manager = NULL;
S_MaterialDataContainer* S_MaterialDataContainer::getInstance() {
	if (project_manager == NULL) {
		
		project_manager = new S_MaterialDataContainer();
	}
	return project_manager;
}


/*-----------------------------------
		继承 - 某数据被添加
*/
void S_MaterialDataContainer::undoDataAdded(C_UndoDataBasePtr data, int index){
	emit containerDataAdded(data, index);
}
/*-----------------------------------
		继承 - 某数据被修改
*/
void S_MaterialDataContainer::undoDataModifyed(C_UndoDataBasePtr data, int index){
	emit containerDataModifyed(data, index);
}
/*-----------------------------------
		继承 - 某全局数据被修改
*/
void S_MaterialDataContainer::undoDataGlobalModifyed(C_UndoDataGlobalBasePtr data){
	emit containerGlobalDataModifyed(data);
}
/*-----------------------------------
		继承 - 某数据被删除
*/
void S_MaterialDataContainer::undoDataDeleted(C_UndoDataBasePtr data, int index){
	emit containerDataDeleted(data, index);
}
/*-----------------------------------
		消息 - 显示消息
*/
void S_MaterialDataContainer::u_showNewMessage(QString message) {
	S_ConsoleManager::getInstance()->append_InfoMsg(message);
}


/*-----------------------------------
		添加 - 材料（数组数据）
*/
void S_MaterialDataContainer::add(C_StandardMaterial data) {
	Q_ASSERT(data.isNull() == false);
	S_UndoDataContainer::add(data.clonePtr());
}
/*-----------------------------------
		修改 - 材料（数组数据）
*/
void S_MaterialDataContainer::modify(C_StandardMaterial data) {
	Q_ASSERT(data.isNull() == false);
	S_UndoDataContainer::modify(data.clonePtr());
	emit materialColorChanged(data);
}

/*-----------------------------------
		删除 - 删除数据（根据ID）
*/
void S_MaterialDataContainer::deleteById(QString id){
	C_UndoDataBasePtr data_ptr = this->getDataById(id);
	if (data_ptr == nullptr){ return; }
	Q_ASSERT(data_ptr.isNull() == false);
	S_UndoDataContainer::deleteById(data_ptr->getId());

	if (data_ptr->get__CLASS_NAME__() == "C_StandardMaterial") {
		C_StandardMaterial material = data_ptr.dynamicCast<C_StandardMaterial>()->clone();

		// > 信号 - 给模型模块（删除对应材料的模型）
		emit materialColorDeleted(material);

		// > 信号 - 给曲线数据（传递材料名，清理1D结果中对应的曲线数据 2024-9-3 杨骁）
		emit signal_deleteMaterialLine(material.getName());
	}
}
/*-----------------------------------
		删除 - 删除数据（根据action）
*/
void S_MaterialDataContainer::deleteByAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据指针
	QString material_id = cur_action->data().value<QString>();
	C_StandardMaterial material = this->getDataStandardMaterialById(material_id);

	// > 删除前询问
	QStringList model_id_list = S_ModelingPre_InteractiveManager::getInstance()->availableModel_getModelIdListByMaterialId(material_id);
	if (model_id_list.count() > 0) {
		QString first_name = S_ModelingPre_InteractiveManager::getInstance()->availableModel_getNameById(model_id_list.first());
		QString question_text;
		question_text.append(tr("确定要删除材料\""));
		question_text.append(material.getName());
		question_text.append(tr("\"吗？\n"));
		question_text.append(tr("关联材料的模型也会被删除，比如模型："));
		question_text.append(first_name);
		if (QMessageBox::question(nullptr, tr("提示"), question_text) == QMessageBox::Yes) {
			//（选是则继续）
		}else {
			//（选否则返回）
			return;
		}
	}

	// > 删除数据（根据ID）
	this->deleteById(material_id);
}
/*-----------------------------------
		删除 - 批量删除数据（根据action）
*/
void S_MaterialDataContainer::deleteBatchByAction() {
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据指针
	QString id_list_str = cur_action->data().value<QString>();
	QStringList id_list = id_list_str.split("___");

	// > 删除前询问
	QString question_text;
	question_text.append(tr("确定要删除当前选中的材料吗？\n"));
	question_text.append(tr("关联材料的全部模型也会被删除。"));
	if (QMessageBox::question(nullptr, tr("提示"), question_text) == QMessageBox::Yes) {
		//（选是则继续）
	} else {
		//（选否则返回）
		return;
	}

	// > 删除数据（根据ID）
	for (int i = id_list.count()-1; i >=0; i--) {
		this->deleteById(id_list.at(i));
	}
}

/*-----------------------------------
		复制 - 复制数据（根据ID）
*/
void S_MaterialDataContainer::copyById(QString id){
	C_UndoDataBasePtr data_ptr = this->getDataById(id);
	Q_ASSERT(data_ptr.isNull() == false);
	S_UndoDataContainer::copy(data_ptr);
}
/*-----------------------------------
		复制 - 复制数据（根据action）
*/
void S_MaterialDataContainer::copyByAction(){
	QAction* cur_action = qobject_cast<QAction*>(this->sender());		//从action里面取出数据指针
	QString id = cur_action->data().value<QString>();
	this->copyById(id);
}
/*-----------------------------------
		复制 - 复制数据（根据action）
*/
void S_MaterialDataContainer::copyBatchByAction() {
	QAction* cur_action = qobject_cast<QAction*>(this->sender());		//从action里面取出数据指针
	QString id_list_str = cur_action->data().value<QString>();
	QStringList id_list = id_list_str.split("___");
	for (int i = 0; i < id_list.count(); i++) {
		this->copyById(id_list.at(i));
	}
}


//获取 - 材料
S_UndoDataContainer_COMMON_DATA_GET_Method(S_MaterialDataContainer, C_StandardMaterial, StandardMaterial);

/*-----------------------------------
		获取 - 材料
*/
C_StandardMaterial S_MaterialDataContainer::getDataStandardMaterialByMinName(QString full_name) {
	QList<C_StandardMaterial> data_list = this->getDataStandardMaterialList();
	for (int i = 0; i < data_list.count(); i++) {
		C_StandardMaterial data = data_list.at(i);
		if (data.getMinName() == full_name) {
			return data;
		}
	}
	return C_StandardMaterial();
}
QStringList S_MaterialDataContainer::getDataStandardMaterialAllMinName() {
	QStringList result_list;
	QList<C_StandardMaterial> data_list = this->getDataStandardMaterialList();
	for (int i = 0; i < data_list.count(); i++) {
		C_StandardMaterial data = data_list.at(i);
		result_list.append(data.getMinName());
	}
	return result_list;
}


/*-----------------------------------
		材料 - 刷新材料映射
*/
void S_MaterialDataContainer::refreshStandardMaterialMappingId() {

	// > 默认材料
	this->m_materialPEC.setMappingId(-1);
	this->m_materialVacuum.setMappingId(-2);

	// > 自定义材料
	int cur_id = 1;
	for (int i = 0; i < this->m_undoDataTank.size(); i++) {
		C_UndoDataBasePtr undoData = this->m_undoDataTank.at(i);
		if (undoData->get__CLASS_NAME__() == "C_StandardMaterial") {
			QSharedPointer<C_StandardMaterial> dataPtr = undoData.dynamicCast<C_StandardMaterial>();
			dataPtr->setMappingId(cur_id);		//（第n个自定义材料对应映射值n）
			cur_id += 1;
		}
	}
}


/*-----------------------------------
		默认材料 - 初始化
*/
void S_MaterialDataContainer::initDefaultMaterial() {
	// > 默认材料（PEC）
	this->m_materialPEC.init();
	this->m_materialPEC.setName("PEC");
	this->m_materialPEC.setType("PEC");
	this->m_materialPEC.setColor(QColor(155, 155, 155));	//灰色
	this->m_materialPEC.setMappingId(-1);

	// > 默认材料（真空）
	this->m_materialVacuum.init();
	this->m_materialVacuum.setName("Vacuum");
	this->m_materialVacuum.setColor(QColor(105, 168, 210));	//蓝色
	this->m_materialVacuum.setMappingId(-2);

}
/*-----------------------------------
		默认材料 - 修改默认材料
*/
void S_MaterialDataContainer::modifyDefaultMaterial(C_StandardMaterial default_material) {
	Q_ASSERT(default_material.isNull() == false);
	if (this->m_materialPEC.getId() == default_material.getId()) {
		this->m_materialPEC = default_material;
		emit materialColorChanged(default_material);
		emit containerDataModifyed(default_material.clonePtr(), -1);
	}
	if (this->m_materialVacuum.getId() == default_material.getId()) {
		this->m_materialVacuum = default_material;
		emit materialColorChanged(default_material);
		emit containerDataModifyed(default_material.clonePtr(), -2);
	}
}
/*-----------------------------------
		默认材料 - 获取专有材料名列表
*/
QStringList S_MaterialDataContainer::getDefaultMaterialName() {
	return QStringList() << "PEC" << "Vacuum" << "真空";
}
/*-----------------------------------
		默认材料 - 是否为默认材料ID
*/
bool S_MaterialDataContainer::isDefaultMaterialId(QString id) {
	if (this->m_materialPEC.getId() == id) { return true; }
	if (this->m_materialVacuum.getId() == id) { return true; }
	return false;
}
/*-----------------------------------
		默认材料 - 是否为PEC
*/
bool S_MaterialDataContainer::isDefaultMaterialId_PEC(QString id) {
	return this->m_materialPEC.getId() == id;
}
/*-----------------------------------
		默认材料 - 是否为真空
*/
bool S_MaterialDataContainer::isDefaultMaterialId_Vacuum(QString id) {
	return this->m_materialVacuum.getId() == id;
}
/*-----------------------------------
		默认材料 - 是否为默认材料名
*/
bool S_MaterialDataContainer::isDefaultMaterialName(QString name) {
	return this->getDefaultMaterialName().contains(name);
}
/*-----------------------------------
		默认材料 - 获取材料 - PEC
*/
C_StandardMaterial S_MaterialDataContainer::getDataDefaultMaterial_PEC() {
	return this->m_materialPEC;
}
/*-----------------------------------
		默认材料 - 获取材料 - 真空
*/
C_StandardMaterial S_MaterialDataContainer::getDataDefaultMaterial_Vacuum() {
	return this->m_materialVacuum;
}
/*-----------------------------------
		默认材料 - 获取材料 - PEC
*/
C_StandardMaterial S_MaterialDataContainer::getDataDefaultMaterialById(QString id) {
	if (this->m_materialPEC.getId() == id) { return this->m_materialPEC; }
	if (this->m_materialVacuum.getId() == id) { return this->m_materialVacuum; }
	return C_StandardMaterial();
}


/*-----------------------------------
		全材料 - 获取材料ID
*/
QStringList S_MaterialDataContainer::get_AllMaterial_IdList() {
	return this->getDataStandardMaterialAllId() << this->m_materialPEC.getId() << this->m_materialVacuum.getId();
}
/*-----------------------------------
		全材料 - 获取材料名称
*/
QStringList S_MaterialDataContainer::get_AllMaterial_NameList() {
	return this->getDataStandardMaterialAllName() << this->m_materialPEC.getName() << this->m_materialVacuum.getName();
}
QStringList S_MaterialDataContainer::get_AllMaterial_MinNameList() {
	return this->getDataStandardMaterialAllMinName() << this->m_materialPEC.getMinName() << this->m_materialVacuum.getMinName();
}
/*-----------------------------------
		全材料 - 获取材料颜色
*/
QList<QColor> S_MaterialDataContainer::get_AllMaterial_ColorList() {
	QList<QColor> result_list;
	QList<C_StandardMaterial> material_list = this->getDataStandardMaterialList();
	for (int i = 0; i < material_list.count(); i++) {
		C_StandardMaterial material = material_list.at(i);
		result_list.append(material.getColor());
	}
	result_list.append(this->m_materialPEC.getColor());
	result_list.append(this->m_materialVacuum.getColor());
	return result_list;
}

QColor S_MaterialDataContainer::get_AllMaterial_ColorById(QString id) {
	if (this->m_materialPEC.getId() == id) { return this->m_materialPEC.getColor(); }
	if (this->m_materialVacuum.getId() == id) { return this->m_materialVacuum.getColor(); }
	
	C_StandardMaterial material = this->getDataStandardMaterialById(id);
	if (material.isNull()) {
		return QColor();
	}
	return material.getColor();
}

QColor S_MaterialDataContainer::get_AllMaterial_ColorByName(QString name) {
	if (this->m_materialPEC.getName() == name) { return this->m_materialPEC.getColor(); }
	if (this->m_materialVacuum.getName() == name) { return this->m_materialVacuum.getColor(); }

	C_StandardMaterial material = this->getDataStandardMaterialByName(name);
	if (material.isNull()) {
		return QColor();
	}
	return material.getColor();
}

C_StandardMaterial S_MaterialDataContainer::getMaterialByName(QString name)
{
	if (this->m_materialPEC.getName() == name) { return this->m_materialPEC; }
	if (this->m_materialVacuum.getName() == name) { return this->m_materialVacuum; }

	return this->getDataStandardMaterialByName(name);
}

/*-----------------------------------
		数据 - 清除当前管理器数据
*/
void S_MaterialDataContainer::clearAllData_noSignal() {
	S_UndoDataContainer::clearTank();
}
/*-----------------------------------
		数据 - 清除当前管理器数据
*/
void S_MaterialDataContainer::clearAllData() {
	this->clearAllData_noSignal();
	emit containerDataAllReloaded();
}
/*-----------------------------------
		数据 - 获取存储的名称
*/
QString S_MaterialDataContainer::getSaveName() {
	return "S_MaterialDataContainer";
}
/*-----------------------------------
		数据 - 全部管理器数据 -> QJsonObject
*/
QJsonObject S_MaterialDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	// > 数组数据 - 材料
	QJsonObject obj_StandardMaterial = QJsonObject();
	obj_StandardMaterial.insert("type", "StandardMaterial");
	QJsonArray arr_StandardMaterial = QJsonArray();
	QList<C_StandardMaterial> list_StandardMaterial = this->getDataStandardMaterialList();
	for (int i = 0; i < list_StandardMaterial.size(); i++){
		C_StandardMaterial c = list_StandardMaterial.at(i);
		arr_StandardMaterial.push_back(c.getJsonObject());
	}
	obj_StandardMaterial.insert("data_list", arr_StandardMaterial);

	// > 数组数据 - 材料
	obj_all.insert("StandardMaterial", obj_StandardMaterial);


	// > 其它数据 - PEC
	obj_all.insert("PEC", this->m_materialPEC.getJsonObject());

	// > 其它数据 - 真空
	obj_all.insert("Vacuum", this->m_materialVacuum.getJsonObject());
	
	return obj_all;
}

/*-----------------------------------
		数据 - QJsonObject -> 全部管理器数据
*/
void S_MaterialDataContainer::setAllDataFromJsonObject(QJsonObject obj_all) {
	this->clearAllData_noSignal();

	// > 数组数据 - 材料
	QJsonObject obj_StandardMaterial = obj_all.value("StandardMaterial").toObject();
	QJsonArray arr_StandardMaterial = obj_StandardMaterial.value("data_list").toArray();
	for (int i = 0; i < arr_StandardMaterial.size(); i++){
		C_StandardMaterial c = C_StandardMaterial();
		c.setJsonObject(arr_StandardMaterial.at(i).toObject());
		if (c.isNull()){ c.init(); }
		this->u_add(c.clonePtr());
	}


	// > 其它数据 - PEC
	if (obj_all.value("PEC").isUndefined() == false) { 
		this->m_materialPEC.setJsonObject(obj_all.value("PEC").toObject()); 

	};

	// > 其它数据 - 真空
	if (obj_all.value("Vacuum").isUndefined() == false) { this->m_materialVacuum.setJsonObject(obj_all.value("Vacuum").toObject()); };

	emit containerDataAllReloaded();
}