#pragma once
#include "stdafx1.h"

#include "StandardMaterial/W_StandardMaterial.h"
#include "StandardMaterialLibrary/W_StandardMaterialLibraryLoading.h"

/*
-----==========================================================-----
		类：		材料模块 - 窗口管理.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		对打开、编辑窗口进行操作，包括响应相关控制。
					（详细见cpp）
-----==========================================================-----
*/

class S_MaterialWindowManager : public QObject
{
	Q_OBJECT

	public:
		S_MaterialWindowManager();
		~S_MaterialWindowManager();
		static S_MaterialWindowManager* cur_manager;		//单例
		static S_MaterialWindowManager* getInstance();		//单例，获取自己
		
		
	//-----------------------------------
	//----材料设置
	protected:
		W_StandardMaterial* m_W_StandardMaterial = nullptr;		//材料设置
	public slots:
												//材料设置 - 添加
		void openAddWindowStandardMaterial();
												//材料设置 - 编辑 
		void openModifyWindowStandardMaterial(QString id);
												//材料设置 - 编辑（action）
		void openModifyWindowStandardMaterialInAction();
												//材料设置 - 导出材料文件（action）
		void export_MaterialInAction();
												//材料设置 - 导出到材料库（action）
		void export_MaterialToLibraryInAction();
	protected slots:
												//材料设置 - 提交数据
		void acceptDataStandardMaterial(bool isAddMode, C_StandardMaterial data);

	public slots:
												//默认材料 - 编辑
		void openModifyWindowDefaultMaterial(QString id);
												//默认材料 - 编辑（action）
		void openModifyWindowDefaultMaterialInAction();
	protected slots:
												//默认材料 - 提交数据
		void acceptDataDefaultMaterial(bool isAddMode, C_StandardMaterial data);

	public slots:
												//全材料 - 编辑
		void openModifyWindow_AllMaterial(QString id);
												//全材料 - 编辑（action）
		void openModifyWindow_AllMaterialInAction();
												//当前材料 - 编辑
		void openModifyWindow_CurrentMaterial();
		
	//-----------------------------------
	//----材料库
	protected:
		W_StandardMaterialLibraryLoading* m_W_StandardMaterialLibraryLoading = nullptr;		//材料库
	public slots:
												//材料库 - 导入
		void openModifyWindowStandardMaterialLibLoading();
	protected slots:
												//材料库 - 提交数据
		void acceptDataStandardMaterialLibLoading(C_StandardMaterial data);
		
		
	//-----------------------------------
	//----材料映射表
	public slots:
												//材料映射表 - 导入映射表（action）
		void action_importMappingMaterialFile();
												//材料映射表 - 导出映射表（action）
		void action_exportMappingMaterialFile();
												//材料映射表 - 导出映射表
		void exportMappingMaterialFile(QFileInfo file_path, QString code = "toUtf8");


};

