#include "stdafx.h"
#include "S_STD_StandardMaterialReader.h"

/*
-----==========================================================-----
		类：		材料 读取器（STD）.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		读取指定文件夹下的 材料文件，并返回材料数据类对象列表。
-----==========================================================-----
*/

S_STD_StandardMaterialReader::S_STD_StandardMaterialReader(){
	this->m_materialList.clear();
}
S_STD_StandardMaterialReader::~S_STD_StandardMaterialReader() {
}

/* --------------------------------------------------------------
----------单例
*/
S_STD_StandardMaterialReader* S_STD_StandardMaterialReader::cur_manager = NULL;
S_STD_StandardMaterialReader* S_STD_StandardMaterialReader::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_STD_StandardMaterialReader();
	}
	return cur_manager;
}


/* --------------------------------------------------------------
		读取器 - 读取文件
*/
void S_STD_StandardMaterialReader::readFromJsonFile(std::string json_file) {
	this->clearAll();

	// > 打开文件
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, json_file.c_str(), "rb");
	if (err != NULL) { return; }
	if (fp == NULL) { return; }

	// > 解析
	char readBuffer[65536];
	FileReadStream stream(fp, readBuffer, sizeof(readBuffer));
	Document document;
	document.ParseStream(stream);

	// > 解析 - 没有对象时退出
	if (document.HasMember("data_list") == false) {
		fclose(fp);
		return;
	}
	// > 解析 - 对象类型错误时退出
	const Value& data_list = document["data_list"];
	if (data_list.IsArray() == false) {
		fclose(fp);
		return;
	}

	for (SizeType i = 0; i < data_list.Size(); i++) {
		const Value& data = data_list[i];
		C_STD_StandardMaterial material;
		material.setJsonObject(data);
		this->m_materialList.push_back(material);
	}

	fclose(fp);
}
/* --------------------------------------------------------------
		读取器 - 清理全部
*/
void S_STD_StandardMaterialReader::clearAll() {
	this->m_materialList.clear();
}


/* --------------------------------------------------------------
		读取器 - 获取全部材料
*/
vector<C_STD_StandardMaterial> S_STD_StandardMaterialReader::getMaterialList() {
	return this->m_materialList;
}
/* --------------------------------------------------------------
		读取器 - 获取材料（根据映射表）
*/
C_STD_StandardMaterial S_STD_StandardMaterialReader::getMaterialByMappingId(int id) {
	for (int i = 0; i < this->m_materialList.size(); i++) {
		C_STD_StandardMaterial material = this->m_materialList.at(i);
		if (material.getMappingId() == id) {
			return material;
		}
	}
	return C_STD_StandardMaterial();
}