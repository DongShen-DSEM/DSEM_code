#include "stdafx.h"
#include "S_StandardMaterialLibrary.h"

/*
-----==========================================================-----
		类：		材料库容器.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料库的数据容器，用于管理材料库中的全部数据与文件。
					【此容器 不对外 开放，只在执行材料库操作时工作】
-----==========================================================-----
*/

S_StandardMaterialLibrary::S_StandardMaterialLibrary(){
	this->initMaterialLibrary();
}
S_StandardMaterialLibrary::~S_StandardMaterialLibrary() {
}

/* --------------------------------------------------------------
----------单例
*/
S_StandardMaterialLibrary* S_StandardMaterialLibrary::cur_manager = NULL;
S_StandardMaterialLibrary* S_StandardMaterialLibrary::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_StandardMaterialLibrary();
	}
	return cur_manager;
}

/* --------------------------------------------------------------
		容器 - 初始化
*/
void S_StandardMaterialLibrary::initMaterialLibrary(){

	QString dir_path = QApplication::applicationDirPath() + "/Library/Materials";
	QDir dir(dir_path);
	if (dir.exists() == false) {
		dir_path = "D:/Program Files (x86)/CST STUDIO SUITE 2019/Library/Materials";
		QDir dir2(dir_path);
		if (dir2.exists() == false) {
			
		}
	}

	this->m_dirPath = dir_path;

	// > 创建文件夹
	if (dir.exists() == false) {
		dir.mkpath(dir.path());
	}

	// > 重刷材料内容
	this->refreshAllMaterial();
}

/* --------------------------------------------------------------
		容器 - 获取路径
*/
QString S_StandardMaterialLibrary::getDirPath(){
	return this->m_dirPath;
}
/* --------------------------------------------------------------
		容器 - 重刷材料内容
*/
void S_StandardMaterialLibrary::refreshAllMaterial() {
	this->m_materialList.clear();
	this->m_materialList = S_StandardMaterial_MTDReader::getInstance()->readFromDir(QDir(this->m_dirPath));
}
/* --------------------------------------------------------------
		容器 - 是否含材料
*/
bool S_StandardMaterialLibrary::hasMaterial(QString material_name){
	for (int i = 0; i < this->m_materialList.count(); i++){
		C_StandardMaterial material = this->m_materialList.at(i);
		if (material.getName() == material_name){
			return true;
		}
	}
	return false;
}
/* --------------------------------------------------------------
		容器 - 获取材料（根据名称）
*/
C_StandardMaterial S_StandardMaterialLibrary::getMaterial_ByName(QString material_name){
	for (int i = 0; i < this->m_materialList.count(); i++){
		C_StandardMaterial material = this->m_materialList.at(i);
		if (material.getName() == material_name){
			return material;
		}
	}
	return C_StandardMaterial();
}
/* --------------------------------------------------------------
		容器 - 获取材料（全部）
*/
QList<C_StandardMaterial> S_StandardMaterialLibrary::getMaterial_All(){
	return this->m_materialList;
}

