#pragma once
#include "stdafx1.h"

#include "../StandardMaterial/C_StandardMaterial.h"
#include "S_StandardMaterial_MTDReader.h"
#include "S_StandardMaterial_MTDWriter.h"

/*
-----==========================================================-----
		类：		材料库容器.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料库的数据容器，用于管理材料库中的全部数据与文件。
					【此容器 不对外 开放，只在执行材料库操作时工作】
-----==========================================================-----
*/

class S_StandardMaterialLibrary : public QObject
{
	Q_OBJECT

	public:
		S_StandardMaterialLibrary();
		~S_StandardMaterialLibrary();
		static S_StandardMaterialLibrary* cur_manager;			//单例
		static S_StandardMaterialLibrary* getInstance();		//单例，获取自己
		
		
	//-----------------------------------
	//----材料库容器
	protected:
		QString m_dirPath;
		QList<C_StandardMaterial> m_materialList;
	public:
										//容器 - 获取路径
		QString getDirPath();
										//容器 - 重刷材料内容
		void refreshAllMaterial();

										//容器 - 是否含材料
										//		【说明】：文件名即材料名称。
		bool hasMaterial(QString material_name);
										//容器 - 获取材料（根据名称）
		C_StandardMaterial getMaterial_ByName(QString material_name);
										//容器 - 获取材料（全部）
		QList<C_StandardMaterial> getMaterial_All();

										//容器 - 获取材料
		C_StandardMaterial updateMaterial(QString material_name);
	protected:
										//容器 - 初始化
		void initMaterialLibrary();
		
};

