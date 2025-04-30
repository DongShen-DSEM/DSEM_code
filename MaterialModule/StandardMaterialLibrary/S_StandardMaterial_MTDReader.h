#pragma once
#include "stdafx1.h"

#include "../StandardMaterial/C_StandardMaterial.h"

/*
-----==========================================================-----
		类：		材料库文件 读取器.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		读取指定文件夹下的 材料文件，并返回材料数据类对象列表。
-----==========================================================-----
*/

class S_StandardMaterial_MTDReader : public QObject
{
	Q_OBJECT

	public:
		S_StandardMaterial_MTDReader();
		~S_StandardMaterial_MTDReader();
		static S_StandardMaterial_MTDReader* cur_manager;		//单例
		static S_StandardMaterial_MTDReader* getInstance();		//单例，获取自己
		
		
	//-----------------------------------
	//----读取器
	public:	
													//读取器 - 读取材料（单个）
		C_StandardMaterial readFromFile_Single(QFileInfo mtd_file);
													//读取器 - 读取材料（多个）
		QList<C_StandardMaterial> readFromFile_Batch(QList<QFileInfo> mtd_file_list);
													//读取器 - 读取材料（全部）
		QList<C_StandardMaterial> readFromDir(QDir mtd_dir);
	protected:
													//读取器 - 解析文本
		C_StandardMaterial readFromText(QString context, QString material_name, QString material_location);
		
};

