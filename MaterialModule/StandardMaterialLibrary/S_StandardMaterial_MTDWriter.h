#pragma once
#include "stdafx1.h"

#include "../StandardMaterial/C_StandardMaterial.h"

/*
-----==========================================================-----
		类：		材料库文件 写入器.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		在指定文件夹下的 材料文件 进行覆写。
-----==========================================================-----
*/

class S_StandardMaterial_MTDWriter : public QObject
{
	Q_OBJECT

	public:
		S_StandardMaterial_MTDWriter();
		~S_StandardMaterial_MTDWriter();
		static S_StandardMaterial_MTDWriter* cur_manager;		//单例
		static S_StandardMaterial_MTDWriter* getInstance();		//单例，获取自己
		
		
	//-----------------------------------
	//----写入器
	public:
													//写入器 - 写入到文件（单个）
													//		【说明】：注意，此操作会直接覆盖文件，已存在的文件也会被覆盖。
		void writeToFile_Single(C_StandardMaterial material, QFileInfo mtd_file, QString code = "toUtf8");
													//写入器 - 写入到文件（多个）
													//		【说明】：注意，此操作会直接覆盖文件，已存在的文件也会被覆盖。
		void writeToFile_Batch(QList<C_StandardMaterial> material_list, QDir mtd_dir, QString code = "toUtf8");
	protected:
													//写入器 - 输出文本
		QString writeMaterial(C_StandardMaterial material);
		
};

