#include "stdafx.h"
#include "S_MaterialTreeManager.h"
#include "S_MaterialDataContainer.h"
#include "S_MaterialWindowManager.h"
#include "S_MaterialDisplayingManager.h"

#include "Source/MainModule/Global/S_FloatingTextBlock.h"

#include "Source/Utils/Operater/TreeItemDirOperater/I_TreeItemDirOperater.h"
#include "Source/Utils/Operater/TreeItemExpander/P_TreeItemExpender.h"

/*
-----==========================================================-----
		类：		材料模块 - 树节点管理.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		此类定义该模块所使用的树节点。
					【树结构 + 配置数据】
-----==========================================================-----
*/

S_MaterialTreeManager::S_MaterialTreeManager() {

	// > 数据变化信号
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerDataAdded, this, &S_MaterialTreeManager::containerDataAdded);
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerDataModifyed, this, &S_MaterialTreeManager::containerDataModifyed);
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerDataDeleted, this, &S_MaterialTreeManager::containerDataDeleted);
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerGlobalDataModifyed, this, &S_MaterialTreeManager::containerGlobalDataModifyed);
	connect(S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::containerDataAllReloaded, this, &S_MaterialTreeManager::containerDataAllReloaded);
}
S_MaterialTreeManager::~S_MaterialTreeManager() {
}

/* --------------------------------------------------------------
---------- 单例
*/
S_MaterialTreeManager* S_MaterialTreeManager::cur_manager = NULL;
S_MaterialTreeManager* S_MaterialTreeManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_MaterialTreeManager();
	}
	return cur_manager;
}


/*-----------------------------------
		树根 - 树根初始化
*/
void S_MaterialTreeManager::initRoot_Material(QTreeWidget* treeWidget) {

	// > 树根设置
	this->m_root = new QTreeWidgetItem(treeWidget);
	this->m_root->setText(0, tr("Material"));
	this->m_root->setIcon(0, QIcon(":/others" + QRC_IconPath + "/others/DirModeling.png") );
	this->m_root->setExpanded(false);
	treeWidget->insertTopLevelItem(0, this->m_root);

	// > 刷新树根
	this->refreshRoot_Material();
}

/*-----------------------------------
		树根 - 获取树根
*/
QTreeWidgetItem* S_MaterialTreeManager::getRoot_Material() {
	return this->m_root;
}
/*-----------------------------------
		树根 - 刷新树根
*/
void S_MaterialTreeManager::refreshRoot_Material() {

	this->clearBranch_Material();

	// > 树根
	this->m_root->setText(0, tr("Material"));

	// > 默认材料 - PEC
	C_StandardMaterial material_PEC = S_MaterialDataContainer::getInstance()->getDataDefaultMaterial_PEC();
	this->m_item_PEC = new QTreeWidgetItem(this->m_root);
	this->m_item_PEC->setData(0, Qt::UserRole, material_PEC.getId());						//（放置ID）
	this->m_item_PEC->setData(0, Qt::UserRole + 1, material_PEC.get__CLASS_NAME__());		//（放置数据类型）
	this->m_item_PEC->setText(0, material_PEC.getMinName());
	this->m_item_PEC->setIcon(0, material_PEC.getIcon_Color());
	this->m_root->addChild(this->m_item_PEC);

	// > 默认材料 - 真空
	C_StandardMaterial material_Vacuum = S_MaterialDataContainer::getInstance()->getDataDefaultMaterial_Vacuum();
	this->m_item_Vacuum = new QTreeWidgetItem(this->m_root);
	this->m_item_Vacuum->setData(0, Qt::UserRole, material_Vacuum.getId());					//（放置ID）
	this->m_item_Vacuum->setData(0, Qt::UserRole + 1, material_Vacuum.get__CLASS_NAME__());	//（放置数据类型）
	this->m_item_Vacuum->setText(0, material_Vacuum.getMinName());
	this->m_item_Vacuum->setIcon(0, material_Vacuum.getIcon_Color());
	this->m_root->addChild(this->m_item_Vacuum);


	QList<C_StandardMaterial> material_list = S_MaterialDataContainer::getInstance()->getDataStandardMaterialList();
	for (int i = 0; i < material_list.count(); i++) {
		C_StandardMaterial material = material_list.at(i);

		// > 材料文件夹
		I_TreeItemDirOperater::createBranchByDir(material.getFolder(), this->m_root, QIcon(":/others" + QRC_IconPath + "/others/DirModeling.png"));
		QTreeWidgetItem* cur_item = I_TreeItemDirOperater::getBranch(material.getFolder(), this->m_root);
		if (cur_item == nullptr) {
			cur_item = this->m_root;
		}

		// > 材料文件节点
		QTreeWidgetItem* item = new QTreeWidgetItem(cur_item);
		item->setData(0, Qt::UserRole, material.getId());					//（放置ID）
		item->setData(0, Qt::UserRole + 1, material.get__CLASS_NAME__());	//（放置数据类型）
		item->setText(0, material.getMinName());
		item->setIcon(0, material.getIcon_Color());
		cur_item->addChild(item);
		this->m_item_MaterialList.append(item);
	}

}
/*-----------------------------------
		树根 - 清空树根
*/
void S_MaterialTreeManager::clearRoot_Material() {
	//（目前树根下没有 重要树枝，直接全清）
	this->clearBranch_Material();
}
/*-----------------------------------
		树根 - 清空树枝
*/
void S_MaterialTreeManager::clearBranch_Material() {
	
	// > 树根下没有 重要树枝，直接全清
	this->m_root->takeChildren();

	// > 清除 默认材料
	this->m_item_PEC = nullptr;
	this->m_item_Vacuum = nullptr;

	// > 清除 材料
	this->m_item_MaterialList.clear();
	this->m_item_MaterialFolder.clear();
}
/*-----------------------------------
		树根 - 展开树根
*/
void S_MaterialTreeManager::setRootExpandAll_Material() {
	P_TreeItemExpender p;
	p.expandAll(this->m_root);
}
/*-----------------------------------
		树根 - 折叠树根
*/
void S_MaterialTreeManager::setRootCollapseAll_Material() {
	P_TreeItemExpender p;
	p.collapseAll(this->m_root);
}


/*-----------------------------------
		树节点事件 - 失去焦点时
*/
void S_MaterialTreeManager::itemFocusOut_Material(QList<QTreeWidgetItem*> item_list) {
	
	// > 选中 - 清空
	this->m_item_selected.clear();
	this->m_selectedList_MaterialId.clear();
	emit signal_itemSelected_Material(false);

	// > 显示 - 刷新显示
	S_MaterialDisplayingManager::getInstance()->refreshSelected_MaterialAll();

	// > 清空白框描述
	//	（在 S_SoftwareAreaTreeManager 中控制，此处不管）
}
/*-----------------------------------
		树节点事件 - 单击操作
*/
void S_MaterialTreeManager::itemClicked_Material(QList<QTreeWidgetItem*> item_list) {
	if (item_list.count() == 0) { return; }
	QTreeWidgetItem* last_item = item_list.first();

	//（无）
}
/*-----------------------------------
		树节点事件 - 改变操作
*/
void S_MaterialTreeManager::itemSelectionChanged_Material(QList<QTreeWidgetItem*> item_list) {
	this->m_item_selected = item_list;	//（标记选中的树节点）
	if (item_list.count() == 0) { return; }
	QTreeWidgetItem* last_item = item_list.last();

	
	// > 选中树根
	if (this->m_root == last_item) {

		// > 选中 - 清空
		this->m_selectedList_MaterialId.clear();
		emit signal_itemSelected_Material(false);

		// > 显示 - 刷新透明
		S_MaterialDisplayingManager::getInstance()->refreshSelected_MaterialAll();

		// > 白色参数块
		S_FloatingTextBlock::getInstance()->hideBlock();

		return;
	}

	// > 只要选中了材料文件夹，就都不显示
	bool has_folder = false;
	for (int i = 0; i < item_list.count(); i++) {
		if (this->m_item_MaterialFolder.contains(item_list.at(i))) {
			has_folder = true;
			break;
		}
	}
	if (has_folder) {

		// > 选中 - 清空
		this->m_selectedList_MaterialId.clear();
		emit signal_itemSelected_Material(false);

		// > 显示 - 刷新透明
		S_MaterialDisplayingManager::getInstance()->refreshSelected_MaterialAll();

		// > 白色参数块
		S_FloatingTextBlock::getInstance()->hideBlock();

		return;
	}

	// > 选中单节点
	QString id = last_item->data(0, Qt::UserRole).toString();
	QString class_name = last_item->data(0, Qt::UserRole + 1).toString();

	if (item_list.count() == 1) {

		// > 选中 - 单选
		this->m_selectedList_MaterialId.clear();
		this->m_selectedList_MaterialId.append(id);
		emit signal_itemSelected_Material(true);

		// > 显示 - 刷新透明
		S_MaterialDisplayingManager::getInstance()->refreshSelected_MaterialAll();

		// > 白色参数块
		C_StandardMaterial c_data = S_MaterialDataContainer::getInstance()->getDataDefaultMaterialById(id);
		if (c_data.isNull()) {
			c_data = S_MaterialDataContainer::getInstance()->getDataStandardMaterialById(id);
			if (c_data.isNull()) {
				S_FloatingTextBlock::getInstance()->hideBlock();
				return;
			}
		}
		S_FloatingTextBlock::getInstance()->setText(c_data.getName(), c_data.getAttributesDesc(), "");
		S_FloatingTextBlock::getInstance()->showBlock();
		
	// > 选中多节点
	}else{

		// > 选中 - 多选
		QStringList id_list;
		for (QTreeWidgetItem* item : item_list) {
			id_list.append(item->data(0, Qt::UserRole).toString());
		}
		this->m_selectedList_MaterialId = id_list;
		emit signal_itemSelected_Material(true);

		// > 显示 - 刷新透明
		S_MaterialDisplayingManager::getInstance()->refreshSelected_MaterialAll();

		// > 白色参数块
		S_FloatingTextBlock::getInstance()->hideBlock();
	}
}
/*-----------------------------------
		树节点事件 - 双击操作
*/
void S_MaterialTreeManager::itemDoubleClicked_Material(QList<QTreeWidgetItem*> item_list) {
	if (item_list.count() == 0) { return; }
	QTreeWidgetItem* last_item = item_list.last();

	// > 双击直接编辑
	QString id = last_item->data(0, Qt::UserRole).toString();
	QString class_name = last_item->data(0, Qt::UserRole + 1).toString();

	// > 双击树节点（材料）
	if (class_name == "C_StandardMaterial") {
		S_MaterialWindowManager::getInstance()->openModifyWindow_AllMaterial(id);
	}

}
/*-----------------------------------
		树节点事件 - 右键操作
*/
void S_MaterialTreeManager::itemRightClicked_Material(QList<QTreeWidgetItem*> item_list) {
	if (item_list.count() == 0) { return; }
	QTreeWidgetItem* last_item = item_list.last();

	// > 右键树根
	if (this->m_root == last_item) {
		QMenu *popMenu = new QMenu();
		QAction *action;

		action = new QAction(tr("New Material..."), this);
		// action->setIcon(QIcon(":/modeling" + QRC_IconPath + "/modeling/NewMaterialColor.png"));
		connect(action, &QAction::triggered, S_MaterialWindowManager::getInstance(), &S_MaterialWindowManager::openAddWindowStandardMaterial);
		popMenu->addAction(action);

		action = new QAction(tr("Load from Material Library..."), this);
		connect(action, &QAction::triggered, S_MaterialWindowManager::getInstance(), &S_MaterialWindowManager::openModifyWindowStandardMaterialLibLoading);
		popMenu->addAction(action);

		//	（此功能暂时不考虑）
		//action = new QAction(tr("Update All Properties from Material Library"), this);
		//popMenu->addAction(action);

		popMenu->addSeparator();

		/*  显示/隐藏材料
		action = new QAction(tr("Hide All Dielectrics"), this);
		popMenu->addAction(action);

		action = new QAction(tr("Show All Dielectrics"), this);
		popMenu->addAction(action);

		action = new QAction(tr("Hide All Metals"), this);
		popMenu->addAction(action);

		action = new QAction(tr("Show All Metals"), this);
		popMenu->addAction(action);

		action = new QAction(tr("Show All"), this);
		popMenu->addAction(action);
		
		popMenu->addSeparator();
		*/

		action = new QAction(tr("Expand All"), this);
		connect(action, &QAction::triggered, this, &S_MaterialTreeManager::setRootExpandAll_Material);
		if (this->m_root->childCount() == 0) { action->setDisabled(true); }
		popMenu->addAction(action);

		action = new QAction(tr("Collapse All"), this);
		connect(action, &QAction::triggered, this, &S_MaterialTreeManager::setRootCollapseAll_Material);
		if (this->m_root->childCount() == 0) { action->setDisabled(true); }
		popMenu->addAction(action);

		popMenu->exec(QCursor::pos());
		return;
	}


	// > 右键单树节点
	QString id = last_item->data(0, Qt::UserRole).toString();
	QString class_name = last_item->data(0, Qt::UserRole+1).toString();

	if (item_list.count() == 1) {

		// > 右键单树节点（默认材料）
		if (last_item == this->m_item_PEC) {
			QMenu* popMenu = new QMenu();
			QAction* action;

			action = new QAction(tr("Edit Default Material - PEC"), this);
			// action->setIcon(QIcon(":/others" + QRC_IconPath + "/others/Edit.png"));
			action->setData(id);
			connect(action, &QAction::triggered, S_MaterialWindowManager::getInstance(), &S_MaterialWindowManager::openModifyWindow_AllMaterialInAction);
			popMenu->addAction(action);

			popMenu->exec(QCursor::pos());
			return;
		}
		if (last_item == this->m_item_Vacuum) {
			QMenu* popMenu = new QMenu();
			QAction* action;

			action = new QAction(tr("Edit Default Material - Vacuum"), this);
			// action->setIcon(QIcon(":/others" + QRC_IconPath + "/others/Edit.png"));
			action->setData(id);
			connect(action, &QAction::triggered, S_MaterialWindowManager::getInstance(), &S_MaterialWindowManager::openModifyWindow_AllMaterialInAction);
			popMenu->addAction(action);

			popMenu->exec(QCursor::pos());
			return;
		}

		// > 右键单树节点（材料）
		if (class_name == "C_StandardMaterial") {
			QMenu* popMenu = new QMenu();
			QAction* action;

			action = new QAction(tr("Edit Material"), this);
			// action->setIcon(QIcon(":/others" + QRC_IconPath + "/others/Edit.png"));
			action->setData(id);
			connect(action, &QAction::triggered, S_MaterialWindowManager::getInstance(), &S_MaterialWindowManager::openModifyWindow_AllMaterialInAction);
			popMenu->addAction(action);

			action = new QAction(tr("Copy Material"), this);
			// action->setIcon(QIcon(":/others" + QRC_IconPath + "/others/Copy.png"));
			action->setData(id);
			connect(action, &QAction::triggered, S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::copyByAction);
			popMenu->addAction(action);

			action = new QAction(tr("Delete Material"), this);
			// action->setIcon(QIcon(":/others" + QRC_IconPath + "/others/Delete.png"));
			action->setData(id);
			connect(action, &QAction::triggered, S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::deleteByAction);
			popMenu->addAction(action);

			popMenu->addSeparator();

			action = new QAction(tr("Export Material File"), this);
			// action->setIcon(QIcon(":/others" + QRC_IconPath + "/others/Edit.png"));
			action->setData(id);
			connect(action, &QAction::triggered, S_MaterialWindowManager::getInstance(), &S_MaterialWindowManager::export_MaterialInAction);
			popMenu->addAction(action);

			popMenu->exec(QCursor::pos());
		}

	// > 右键多树节点
	} else {

		QStringList id_list;
		for (int i = 0; i < item_list.count(); i++) {
			QTreeWidgetItem* item = item_list.at(i);
			QString id = item->data(0, Qt::UserRole).toString();
			id_list.append(id);
		}

		// > 右键多树节点（材料）
		if (class_name == "C_StandardMaterial") {
			QMenu* popMenu = new QMenu();
			QAction* action;

			action = new QAction(tr("Copy Batch Materials"), this);
			// action->setIcon(QIcon(":/others" + QRC_IconPath + "/others/Copy.png"));
			action->setData(id_list.join("___"));
			connect(action, &QAction::triggered, S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::copyBatchByAction);
			popMenu->addAction(action);

			action = new QAction(tr("Delete Batch Materials"), this);
			// action->setIcon(QIcon(":/others" + QRC_IconPath + "/others/Delete.png"));
			action->setData(id_list.join("___"));
			connect(action, &QAction::triggered, S_MaterialDataContainer::getInstance(), &S_MaterialDataContainer::deleteBatchByAction);
			popMenu->addAction(action);

			popMenu->addSeparator();

			action = new QAction(tr("Export Batch Material Files"), this);
			// action->setIcon(QIcon(":/others" + QRC_IconPath + "/others/Edit.png"));
			action->setData(id_list.join("___"));
			connect(action, &QAction::triggered, S_MaterialWindowManager::getInstance(), &S_MaterialWindowManager::export_MaterialInAction);
			popMenu->addAction(action);

			popMenu->exec(QCursor::pos());
		}
	}
}


/*-----------------------------------
		树节点（端口） - 是否有选中ID
*/
bool S_MaterialTreeManager::hasItemSelectedID_Material() {
	return this->m_selectedList_MaterialId.count() > 0;
}
/*-----------------------------------
		树节点（端口） - 获取选中ID
*/
QStringList S_MaterialTreeManager::getItemSelectedID_Material() {
	return this->m_selectedList_MaterialId;
}


/*-----------------------------------
		选中 - 被选中
*/
bool S_MaterialTreeManager::isMaterialSelected() {
	return this->m_item_selected.count() > 0;
}
/*-----------------------------------
		选中 - 获取被选中的ID
*/
QStringList S_MaterialTreeManager::getSelectedMaterialId() {
	QStringList result_list;
	for (int i = 0; i < this->m_item_selected.count(); i++) {
		QTreeWidgetItem* item = this->m_item_selected.at(i);
		QString id = item->data(0, Qt::UserRole).toString();		//（获取ID）
		result_list.append(id);
	}
	return result_list;
}



/*-----------------------------------
		数据容器 - 树变化 - 某数据 被添加
*/
void S_MaterialTreeManager::containerDataAdded(C_UndoDataBasePtr ptr, int index) {
	this->refreshRoot_Material();
	this->setRootExpandAll_Material();
}
/*-----------------------------------
		数据容器 - 树变化 - 某数据 被修改
*/
void S_MaterialTreeManager::containerDataModifyed(C_UndoDataBasePtr ptr, int index) {
	this->refreshRoot_Material();
}
/*-----------------------------------
		数据容器 - 树变化 - 某数据 被删除
*/
void S_MaterialTreeManager::containerDataDeleted(C_UndoDataBasePtr ptr, int index) {
	this->refreshRoot_Material();
}
/*-----------------------------------
		数据容器 - 树变化 - 某全局数据 被修改
*/
void S_MaterialTreeManager::containerGlobalDataModifyed(C_UndoDataGlobalBasePtr ptr) {
	
	//（暂无）

}
/*-----------------------------------
		数据容器 - 树变化 - 存储数据 被读取
*/
void S_MaterialTreeManager::containerDataAllReloaded() {
	this->refreshRoot_Material();
}