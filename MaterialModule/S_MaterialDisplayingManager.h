#pragma once
#include "stdafx1.h"

#include "Source/ProjectModule/Undo/ExtendPart/C_UndoDataBase.h"
#include "Source/ProjectModule/Undo/ExtendPart/C_UndoDataGlobalBase.h"

/*
-----==========================================================-----
		类：		材料模块 - 模型显示管理.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		此类定义该模块数据所需要显示的模型。
					【只管理 数据容器 显示模型，编辑窗口的临时模型数据不管】
					（详细见cpp）
-----==========================================================-----
*/

class S_MaterialDisplayingManager : public QObject
{
	Q_OBJECT

	public:
		S_MaterialDisplayingManager();
		~S_MaterialDisplayingManager();
		static S_MaterialDisplayingManager* cur_manager;			//单例
		static S_MaterialDisplayingManager* getInstance();		//单例，获取自己
		

	//-----------------------------------
	//----模型标记（数据id - 模型id，一对一，此数据不存，打开工程时模型全部重建）
	public:
									//模型标记 - 模型重建
		void rebuildAll();


	//-----------------------------------
	//----材料对应 模型显示/透明
	public slots:
									//材料对应显示 - 刷新显示
		void refreshSelected_Material(QString material_id);
		void refreshSelected_MaterialNoSignal(QString material_id);
		void refreshSelected_MaterialAll();
	public:
	signals:
									//材料对应显示 - 显示变化时（信号）
		void signal_selectedChanged_Material();
	protected:
		QStringList m_customUnselectedList_Material;	//隐藏列表（内部用）
	public slots:
									//材料对应显示 - 设置隐藏列表（内部用）
									//		【说明】：内部程序专用的隐藏列表，填入数据的id。
									//		【说明】：很多显示/隐藏条件都被叠加在一起，此隐藏列表只是 多个条件 的一种。
		void setCustomUnselectedList_Material(QStringList material_id_list);
		QStringList getCustomUnselectedList_Material();


	//-----------------------------------
	//----数据容器
	public slots:
										//数据容器 - 模型变化 - 某数据 被添加
		void containerDataAdded(C_UndoDataBasePtr ptr, int index);
										//数据容器 - 模型变化 - 某数据 被修改
		void containerDataModifyed(C_UndoDataBasePtr ptr, int index);
										//数据容器 - 模型变化 - 某数据 被删除
		void containerDataDeleted(C_UndoDataBasePtr ptr, int index);
										//数据容器 - 模型变化 - 某全局数据 被修改
		void containerGlobalDataModifyed(C_UndoDataGlobalBasePtr ptr);
										//数据容器 - 模型变化 - 存储数据 被读取
		void containerDataAllReloaded();

};

