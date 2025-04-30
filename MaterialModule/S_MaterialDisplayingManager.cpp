#include "stdafx.h"
#include "S_MaterialDataContainer.h"
#include "S_MaterialDisplayingManager.h"
#include "S_MaterialTreeManager.h"

#include "Source/ModelingPreModule/S_ModelingPre_InteractiveManager.h"

#include "Source/SettingModule/S_SettingDataContainer.h"
#include "Source/SolverModule/S_SolverDataContainer.h"

/*
-----==========================================================-----
		类：		材料模块 - 模型显示管理.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		此类定义该模块数据所需要显示的模型。
					【只管理 数据容器 显示模型，编辑窗口的临时模型数据不管】
-----==========================================================-----
*/

S_MaterialDisplayingManager::S_MaterialDisplayingManager(){

	// > 数据变化信号
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerDataAdded, this, &S_MaterialDisplayingManager::containerDataAdded);
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerDataModifyed, this, &S_MaterialDisplayingManager::containerDataModifyed);
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerDataDeleted, this, &S_MaterialDisplayingManager::containerDataDeleted);
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerGlobalDataModifyed, this, &S_MaterialDisplayingManager::containerGlobalDataModifyed);
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerDataAllReloaded, this, &S_MaterialDisplayingManager::containerDataAllReloaded);

}
S_MaterialDisplayingManager::~S_MaterialDisplayingManager() {
}

/* --------------------------------------------------------------
---------- 单例
*/
S_MaterialDisplayingManager* S_MaterialDisplayingManager::cur_manager = NULL;
S_MaterialDisplayingManager* S_MaterialDisplayingManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_MaterialDisplayingManager();
	}
	return cur_manager;
}


/*-----------------------------------
		模型标记 - 模型重建
*/
void S_MaterialDisplayingManager::rebuildAll() {
	
	// > 材料对应
	//	（无模型）

}


/*-----------------------------------
		材料对应显示 - 刷新显示
*/
void S_MaterialDisplayingManager::refreshSelected_Material(QString material_id) {
	this->refreshSelected_MaterialNoSignal(material_id);

	// > 刷新视图+信号
	S_ModelingPre_InteractiveManager::getInstance()->updateViewModeling();
	emit signal_selectedChanged_Material();
}

void S_MaterialDisplayingManager::refreshSelected_MaterialNoSignal(QString material_id) {

	// > 根据材料id获取到模型id
	QStringList model_id_list = S_ModelingPre_InteractiveManager::getInstance()->availableModel_getModelIdListByMaterialId(material_id);
	if (model_id_list.count() == 0) { return; }
	bool selected = true;

	// > 显示 - 内部隐藏列表 有此设置，则不显示
	if (this->m_customUnselectedList_Material.contains(material_id)){
		selected = false;
	}

	// > 显示 - 如果选中了树节点，则根据树节点情况显示
	if (S_MaterialTreeManager::getInstance()->hasItemSelectedID_Material()) {
		QStringList selected_port_id = S_MaterialTreeManager::getInstance()->getItemSelectedID_Material();
		if (selected_port_id.contains(material_id)) {
			selected = true;
		}else {
			selected = false;
		}
	}

	// > 显示 - 执行模型显示/透明
	if (selected == true) {
		for (int i = 0; i < model_id_list.count(); i++) {
			QString model_id = model_id_list.at(i);
			S_ModelingPre_InteractiveManager::getInstance()->model_setSelected(model_id, true, false);
		}
	}else{
		for (int i = 0; i < model_id_list.count(); i++) {
			QString model_id = model_id_list.at(i);
			S_ModelingPre_InteractiveManager::getInstance()->model_setSelected(model_id, false, false);
		}
	}
}
void S_MaterialDisplayingManager::refreshSelected_MaterialAll() {
	QStringList id_list = S_MaterialDataContainer::getInstance()->get_AllMaterial_IdList();
	for (int i = 0; i < id_list.count(); i++) {
		QString id = id_list.at(i);
		this->refreshSelected_MaterialNoSignal(id);
	}

	// > 刷新视图+信号
	S_ModelingPre_InteractiveManager::getInstance()->updateViewModeling();
	emit signal_selectedChanged_Material();
}
/*-----------------------------------
		材料对应显示 - 设置隐藏列表（内部用）
*/
void S_MaterialDisplayingManager::setCustomUnselectedList_Material(QStringList material_id_list) {
	this->m_customUnselectedList_Material = material_id_list;
}
QStringList S_MaterialDisplayingManager::getCustomUnselectedList_Material() {
	return this->m_customUnselectedList_Material;
}


/*-----------------------------------
		数据容器 - 模型变化 - 某数据 被添加
*/
void S_MaterialDisplayingManager::containerDataAdded(C_UndoDataBasePtr ptr, int index) {
	if (ptr->get__CLASS_NAME__() == "C_StandardMaterial") {
		//（无操作）
	}
}
/*-----------------------------------
		数据容器 - 模型变化 - 某数据 被修改
*/
void S_MaterialDisplayingManager::containerDataModifyed(C_UndoDataBasePtr ptr, int index) {
	if (ptr->get__CLASS_NAME__() == "C_StandardMaterial") {
		//（无操作）
	}
}
/*-----------------------------------
		数据容器 - 模型变化 - 某数据 被删除
*/
void S_MaterialDisplayingManager::containerDataDeleted(C_UndoDataBasePtr ptr, int index) {
	if (ptr->get__CLASS_NAME__() == "C_StandardMaterial") {
		//（无操作）
	}
}
/*-----------------------------------
		数据容器 - 模型变化 - 某全局数据 被修改
*/
void S_MaterialDisplayingManager::containerGlobalDataModifyed(C_UndoDataGlobalBasePtr ptr) {
	//...
}
/*-----------------------------------
		数据容器 - 模型变化 - 存储数据 被读取
*/
void S_MaterialDisplayingManager::containerDataAllReloaded() {
	this->rebuildAll();
}