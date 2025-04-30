#pragma once

#include "C_STD_StandardMaterial.h"
#include "Source/DllModule/rapidjson/document.h"
#include "Source/DllModule/rapidjson/FileReadStream.h"
#include <cstdio>
#include <iostream>

/*
-----==========================================================-----
		类：		材料 读取器（STD）.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		读取指定文件夹下的 材料文件，并返回材料数据类对象列表。
-----==========================================================-----
*/

using namespace std;
using namespace rapidjson;
class S_STD_StandardMaterialReader
{

	public:
		S_STD_StandardMaterialReader();
		~S_STD_StandardMaterialReader();
		static S_STD_StandardMaterialReader* cur_manager;		//单例
		static S_STD_StandardMaterialReader* getInstance();		//单例，获取自己
		
		
	//-----------------------------------
	//----读取器
	protected:
		vector<C_STD_StandardMaterial> m_materialList;
	public:	
													//读取器 - 读取文件
													//		【说明】：此函数执行一次即可，多次执行可以刷掉之前已读取的材料数据。
		void readFromJsonFile(std::string json_filePath);
													//读取器 - 清理全部
		void clearAll();

													//读取器 - 获取全部材料
													//		【说明】：包含自定义材料、PEC和真空默认材料。
		vector<C_STD_StandardMaterial> getMaterialList();
													//读取器 - 获取材料（根据映射表）
													//		【说明】：映射ID为0表示未定义，从1开始计数自定义的材料，-1表示PEC，-2表示真空。
													//		【说明】：如果映射表中没有指定材料，则返回的材料对象为空对象，可以调用 isNull 判别一下。
		C_STD_StandardMaterial getMaterialByMappingId(int id);
		
};

