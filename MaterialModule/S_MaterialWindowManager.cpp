#include "stdafx.h"
#include "S_MaterialWindowManager.h"
#include "S_MaterialDataContainer.h"
#include "S_MaterialTreeManager.h"
#include "StandardMaterialLibrary/S_StandardMaterial_MTDWriter.h"
#include "Source/MaterialModule/STD_StandardMaterial/S_STD_StandardMaterialReader.h"

#include "Source/ProjectModule/S_ProjectManager.h"
#include "DSEMUI.h"

/*
-----==========================================================-----
		类：		材料模块 - 窗口管理.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		对打开、编辑窗口进行操作，包括响应相关控制。
					【窗口】
					
					对窗口的数据进行相关操作。
-----==========================================================-----
*/

S_MaterialWindowManager::S_MaterialWindowManager(){
}
S_MaterialWindowManager::~S_MaterialWindowManager() {
}

/* --------------------------------------------------------------
---------- 单例
*/
S_MaterialWindowManager* S_MaterialWindowManager::cur_manager = NULL;
S_MaterialWindowManager* S_MaterialWindowManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_MaterialWindowManager();
	}
	return cur_manager;
}

/* --------------------------------------------------------------
		材料设置 - 添加
*/
void S_MaterialWindowManager::openAddWindowStandardMaterial() {
	this->m_W_StandardMaterial = new W_StandardMaterial(DSEMUI::getInstance());
	connect(this->m_W_StandardMaterial, &W_StandardMaterial::acceptDataSignal, this, &S_MaterialWindowManager::acceptDataStandardMaterial);
	this->m_W_StandardMaterial->setDataInAddMode();

	this->m_W_StandardMaterial->resize(1,1);
	this->m_W_StandardMaterial->show();	//（关闭后自动销毁）
}

/* --------------------------------------------------------------
		材料设置 - 编辑
*/
void S_MaterialWindowManager::openModifyWindowStandardMaterial(QString id) {
	C_StandardMaterial data = S_MaterialDataContainer::getInstance()->getDataStandardMaterialById(id);
	if (data.isNull()) { return; }

	this->m_W_StandardMaterial = new W_StandardMaterial(DSEMUI::getInstance());
	connect(this->m_W_StandardMaterial, &W_StandardMaterial::acceptDataSignal, this, &S_MaterialWindowManager::acceptDataStandardMaterial);
	this->m_W_StandardMaterial->setDataInModifyMode(data);

	this->m_W_StandardMaterial->resize(1,1);
	this->m_W_StandardMaterial->show();	//（关闭后自动销毁）
}

/* --------------------------------------------------------------
		材料设置 - 编辑（action）
*/
void S_MaterialWindowManager::openModifyWindowStandardMaterialInAction() {
	QAction* cur_action = qobject_cast<QAction*>(this->sender());		//（从action里面取出数据指针）
	QString id = cur_action->data().value<QString>();
	this->openModifyWindowStandardMaterial(id);
}

/* --------------------------------------------------------------
		材料设置 - 导出（action）
*/
void S_MaterialWindowManager::export_MaterialInAction() {
	QAction* cur_action = qobject_cast<QAction*>(this->sender());		//（从action里面取出数据指针）
	QString id_list_str = cur_action->data().value<QString>();
	QStringList id_list = id_list_str.split("___");

	for (int i = 0; i < id_list.count(); i++) {
		QString id = id_list.at(i);

		// > 获取材料
		C_StandardMaterial c_m = S_MaterialDataContainer::getInstance()->getDataStandardMaterialById(id);
		if (c_m.isNull()) { return; }

		// > 选择文件
		QFileDialog fd;
		fd.setWindowTitle(tr("Export Material File"));
		fd.setAcceptMode(QFileDialog::AcceptSave);		//对话框类型（打开/保存）（保存会有文件覆盖提示）
		fd.setNameFilters(QStringList()					//文件格式
			<< tr("Material File(*.mtd)")
		);
		fd.setViewMode(QFileDialog::Detail);
		fd.selectFile(c_m.getName() + ".mtd");
		if (fd.exec() == QDialog::Accepted) {
			if (fd.selectedFiles().empty()) { return; }

			// > 写入材料文件
			QString file_path = fd.selectedFiles().at(0);
			S_StandardMaterial_MTDWriter::getInstance()->writeToFile_Single(c_m, file_path);
		}
	}
}
/* --------------------------------------------------------------
		材料设置 - 导出到材料库（action）
*/
void S_MaterialWindowManager::export_MaterialToLibraryInAction() {
	QAction* cur_action = qobject_cast<QAction*>(this->sender());		//（从action里面取出数据指针）
	QString id = cur_action->data().value<QString>();

	// > 获取材料
	C_StandardMaterial c_m = S_MaterialDataContainer::getInstance()->getDataStandardMaterialById(id);
	if (c_m.isNull()) { return; }

	// > 写入材料文件
	QString file_path = S_StandardMaterialLibrary::getInstance()->getDirPath() + "/" + c_m.getMinName() + ".mtd";
	S_StandardMaterial_MTDWriter::getInstance()->writeToFile_Single(c_m, file_path);

	// > 重刷材料内容
	S_StandardMaterialLibrary::getInstance()->refreshAllMaterial();
}
/* --------------------------------------------------------------
		材料设置 - 提交数据
*/
void S_MaterialWindowManager::acceptDataStandardMaterial(bool isAddMode, C_StandardMaterial data){
	S_ProjectManager::getInstance()->setDirty();
	if (isAddMode == true) {
		S_MaterialDataContainer::getInstance()->add(data);
	}else {
		S_MaterialDataContainer::getInstance()->modify(data);
	}
}


/* --------------------------------------------------------------
		默认材料 - 编辑
*/
void S_MaterialWindowManager::openModifyWindowDefaultMaterial(QString id) {
	C_StandardMaterial data = S_MaterialDataContainer::getInstance()->getDataDefaultMaterialById(id);
	if (data.isNull()) { return; }
	this->m_W_StandardMaterial = new W_StandardMaterial(DSEMUI::getInstance());
	connect(this->m_W_StandardMaterial, &W_StandardMaterial::acceptDataSignal, this, &S_MaterialWindowManager::acceptDataDefaultMaterial);
	this->m_W_StandardMaterial->setDefaultDataInModifyMode(data);

	this->m_W_StandardMaterial->resize(1,1);
	this->m_W_StandardMaterial->show();	//（关闭后自动销毁）
}
/* --------------------------------------------------------------
		默认材料 - 编辑（action）
*/
void S_MaterialWindowManager::openModifyWindowDefaultMaterialInAction() {
	QAction* cur_action = qobject_cast<QAction*>(this->sender());		//（从action里面取出数据指针）
	QString id = cur_action->data().value<QString>();
	this->openModifyWindowDefaultMaterial(id);
}
/* --------------------------------------------------------------
		默认材料 - 提交数据
*/
void S_MaterialWindowManager::acceptDataDefaultMaterial(bool isAddMode, C_StandardMaterial data){
	S_ProjectManager::getInstance()->setDirty();
	S_MaterialDataContainer::getInstance()->modifyDefaultMaterial(data);
}


/* --------------------------------------------------------------
		全材料 - 编辑
*/
void S_MaterialWindowManager::openModifyWindow_AllMaterial(QString id) {
	if (S_MaterialDataContainer::getInstance()->isDefaultMaterialId(id)) {
		this->openModifyWindowDefaultMaterial(id);
	}else {
		this->openModifyWindowStandardMaterial(id);
	}
}

/* --------------------------------------------------------------
		全材料 - 编辑（action）
*/
void S_MaterialWindowManager::openModifyWindow_AllMaterialInAction() {
	QAction* cur_action = qobject_cast<QAction*>(this->sender());		//（从action里面取出数据指针）
	QString id = cur_action->data().value<QString>();
	this->openModifyWindow_AllMaterial(id);
}

/* --------------------------------------------------------------
		全材料 - 编辑
*/
void S_MaterialWindowManager::openModifyWindow_CurrentMaterial() {
	QStringList id_list = S_MaterialTreeManager::getInstance()->getSelectedMaterialId();
	if (id_list.count() > 0)
	{
		this->openModifyWindow_AllMaterial(id_list.first());
	}
}

/* --------------------------------------------------------------
----------编辑 - 材料库导入
*/
void S_MaterialWindowManager::openModifyWindowStandardMaterialLibLoading(){
	this->m_W_StandardMaterialLibraryLoading = new W_StandardMaterialLibraryLoading(DSEMUI::getInstance());
	connect(this->m_W_StandardMaterialLibraryLoading, &W_StandardMaterialLibraryLoading::acceptDataSignal, this, &S_MaterialWindowManager::acceptDataStandardMaterialLibLoading);
	this->m_W_StandardMaterialLibraryLoading->show();	//（关闭后自动销毁）
}
void S_MaterialWindowManager::acceptDataStandardMaterialLibLoading(C_StandardMaterial data){
	if (data.isNull()) { return; }
	S_ProjectManager::getInstance()->setDirty();
	
	// > 没有，则添加新的
	C_StandardMaterial c_data = S_MaterialDataContainer::getInstance()->getDataStandardMaterialByName(data.getName());
	if (c_data.isNull()) {
		S_MaterialDataContainer::getInstance()->add(data);

	// > 已有，则只编辑
	}else {
		c_data.copyFrom(&data);
		S_MaterialDataContainer::getInstance()->modify(c_data);
	}
}


/*-----------------------------------
		材料映射表 - 导入映射表（action）
*/
void S_MaterialWindowManager::action_importMappingMaterialFile() {
	QString file_path;
	QFileDialog fd;
	fd.setWindowTitle( "导入文件" );
	fd.setAcceptMode(QFileDialog::AcceptOpen);		//对话框类型（打开/保存）（保存会有文件覆盖提示）
	fd.setDirectory(".");							//默认目录
	fd.setNameFilters(QStringList() << "json文件(*.json)" << "所有文件(*.*)");
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) {
			return;
		}
		file_path = fd.selectedFiles().at(0);
	}else {
		return;
	}

	// > 读取文件
	S_STD_StandardMaterialReader::getInstance()->readFromJsonFile(file_path.toStdString());
}
/*-----------------------------------
		材料映射表 - 导出映射表（action）
*/
void S_MaterialWindowManager::action_exportMappingMaterialFile() {
	QString file_path;
	QFileDialog fd;
	fd.setWindowTitle( "导出文件" );
	fd.setAcceptMode(QFileDialog::AcceptSave);		//对话框类型（打开/保存）（保存会有文件覆盖提示）
	fd.setDirectory(".");							//默认目录
	fd.setNameFilters(QStringList() << "json文件(*.json)" << "所有文件(*.*)");
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) {
			return;
		}
		file_path = fd.selectedFiles().at(0);
	}else {
		return;
	}

	this->exportMappingMaterialFile(QFileInfo(file_path));
}
/*-----------------------------------
		材料映射表 - 导出映射表
*/
void S_MaterialWindowManager::exportMappingMaterialFile(QFileInfo file_path, QString code) {
	QJsonObject obj_AllMaterial = QJsonObject();
	obj_AllMaterial.insert("type", "StandardMaterial");
	QJsonArray arr_StandardMaterial = QJsonArray();

	// > 刷新材料映射
	S_MaterialDataContainer::getInstance()->refreshStandardMaterialMappingId();

	// > 加入默认材料
	arr_StandardMaterial.append(S_MaterialDataContainer::getInstance()->getDataDefaultMaterial_PEC().getJsonObject());
	arr_StandardMaterial.append(S_MaterialDataContainer::getInstance()->getDataDefaultMaterial_Vacuum().getJsonObject());

	// > 加入材料
	QList<C_StandardMaterial> list_StandardMaterial = S_MaterialDataContainer::getInstance()->getDataStandardMaterialList();
	for (int i = 0; i < list_StandardMaterial.size(); i++){
		C_StandardMaterial c = list_StandardMaterial.at(i);
		arr_StandardMaterial.append(c.getJsonObject());
	}
	obj_AllMaterial.insert("data_list", arr_StandardMaterial);

	QString file_data = QJsonDocument(obj_AllMaterial).toJson();


	// > 覆盖文件
	QFile file_to(file_path.absoluteFilePath());
	if (file_to.exists()) { file_to.remove(); }

	// > 写入文件
	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return; }
	QByteArray ba;
	if (code == "toLocal8Bit") {			//windows本地字符
		ba = file_data.toLocal8Bit();
	}else if (code == "toLatin1") {			//ISO-8859-1编码，只有ASCII，不支持汉字
		ba = file_data.toLatin1();
	}else {
		ba = file_data.toUtf8();
	}
	file_to.write(ba);
	file_to.close();
}
