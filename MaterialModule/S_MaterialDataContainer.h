#pragma once
#include "stdafx1.h"

#include "Source/ProjectModule/Storage/S_StorageManagerTemplate.h"
#include "Source/ProjectModule/Undo/ExtendPart/S_UndoDataContainer.h"

#include "StandardMaterial/C_StandardMaterial.h"

/*
-----==========================================================-----
		类：		材料模块 - 数据容器.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		【数据容器 + 增删改查 + 撤回】
					（详细见cpp）
-----==========================================================-----
*/

class S_MaterialDataContainer : public QObject, public S_UndoDataContainer, public S_StorageManagerTemplate
{
	Q_OBJECT

	public:
		S_MaterialDataContainer();
		~S_MaterialDataContainer();
		static S_MaterialDataContainer* project_manager;	//单例
		static S_MaterialDataContainer* getInstance();		//单例，获取自己
		
	//-----------------------------------
	//----数据变化（数组数据）
	public:
	signals:
										//信号 - 某数据 被添加
		void containerDataAdded(C_UndoDataBasePtr ptr, int index);
										//信号 - 某数据 被修改
		void containerDataModifyed(C_UndoDataBasePtr ptr, int index);
										//信号 - 某数据 被删除
		void containerDataDeleted(C_UndoDataBasePtr ptr, int index);
	protected:
										//继承 - 某数据 被添加
		void undoDataAdded(C_UndoDataBasePtr data, int index) override;
										//继承 - 某数据 被修改
		void undoDataModifyed(C_UndoDataBasePtr data, int index) override;
										//继承 - 某数据 被删除
		void undoDataDeleted(C_UndoDataBasePtr data, int index) override;

	//-----------------------------------
	//----数据变化（全局数据）
	public:
	signals:
										//信号 - 存储数据 被读取
		void containerDataAllReloaded();
										//信号 - 某全局数据 被修改
		void containerGlobalDataModifyed(C_UndoDataGlobalBasePtr ptr);
	protected:
										//继承 - 某全局数据 被修改
		void undoDataGlobalModifyed(C_UndoDataGlobalBasePtr data) override;

	//-----------------------------------
	//----消息
	protected:
										//消息 - 输出消息
		void u_showNewMessage(QString message) override;



	//-----------------------------------
	//----添加（数组数据）
	public:
										//添加 - 材料
		void add(C_StandardMaterial data);

	//-----------------------------------
	//----修改（数组数据）
	public:
										//修改 - 材料
		void modify(C_StandardMaterial data);

	//-----------------------------------
	//----删除
	public:
										//删除 - 删除数据（根据ID）
		void deleteById(QString id);
										//删除 - 删除数据（根据action）
		void deleteByAction();
										//删除 - 批量删除数据（根据action）
		void deleteBatchByAction();

	signals:
										//删除 - 通知1D结果清理对应的色散曲线
		void signal_deleteMaterialLine(QString);

	//-----------------------------------
	//----复制
	public:
										//复制 - 复制数据（根据ID）
		void copyById(QString id);
										//复制 - 复制数据（根据action）
		void copyByAction();
										//复制 - 批量复制数据（根据action）
		void copyBatchByAction();

	//-----------------------------------
	//----获取（数组数据）
	public:
										//获取 - 材料
		S_UndoDataContainer_COMMON_DATA_GET_Header(S_MaterialDataContainer, C_StandardMaterial, StandardMaterial);
		
										//获取 - 材料
		C_StandardMaterial getDataStandardMaterialByMinName(QString full_name);
		QStringList getDataStandardMaterialAllMinName();

		
	//-----------------------------------
	//----材料
	public:
										//材料 - 刷新材料映射
										//		【说明】：需要导出材料映射 或者 进行材料映射计算时 调用此函数。
		void refreshStandardMaterialMappingId();

	//-----------------------------------
	//----默认材料
	protected:
		C_StandardMaterial m_materialPEC;
		C_StandardMaterial m_materialVacuum;
	public:
										//默认材料 - 初始化
		void initDefaultMaterial();
										//默认材料 - 修改默认材料
		void modifyDefaultMaterial(C_StandardMaterial default_material);
		
										//默认材料 - 获取专有材料名列表
		QStringList getDefaultMaterialName();
										//默认材料 - 是否为默认材料ID
		bool isDefaultMaterialId(QString id);
										//默认材料 - 是否为PEC
		bool isDefaultMaterialId_PEC(QString id);
										//默认材料 - 是否为真空
		bool isDefaultMaterialId_Vacuum(QString id);
										//默认材料 - 是否为默认材料名
		bool isDefaultMaterialName(QString name);
										//默认材料 - 获取材料 - PEC
		C_StandardMaterial getDataDefaultMaterial_PEC();
										//默认材料 - 获取材料 - 真空
		C_StandardMaterial getDataDefaultMaterial_Vacuum();
										//默认材料 - 获取材料
		C_StandardMaterial getDataDefaultMaterialById(QString id);
		
	//-----------------------------------
	//----全材料
	public:
										//全材料 - 获取材料ID
										//		【说明】：包括标准材料和默认材料。
		QStringList get_AllMaterial_IdList();
										//全材料 - 获取材料名称
										//		【说明】：包括标准材料和默认材料。
		QStringList get_AllMaterial_NameList();
		QStringList get_AllMaterial_MinNameList();
										//全材料 - 获取材料颜色
										//		【说明】：包括标准材料和默认材料。
		QList<QColor> get_AllMaterial_ColorList();
		QColor get_AllMaterial_ColorById(QString id);
		QColor get_AllMaterial_ColorByName(QString name);
		C_StandardMaterial getMaterialByName(QString name);
	public:
	signals:
										//全材料 - 材料颜色变化（信号）
										//		【说明】：包括标准材料和默认材料。
		void materialColorChanged(C_StandardMaterial material);
	public:
	signals:
										//全材料 - 材料被删除时颜色（信号）
										//		【说明】：包括标准材料和默认材料。
		void materialColorDeleted(C_StandardMaterial material);

		
	//-----------------------------------
	//----存储数据
	protected:
										//数据 - 清除当前管理器数据
		void clearAllData_noSignal();
	public:
										//数据 - 清除当前管理器数据
		void clearAllData() override;
										//数据 - 获取存储的名称
		QString getSaveName() override;
										//数据 - 全部管理器数据 -> QJsonObject
		QJsonObject getAllDataOfJsonObject() override;
										//数据 - QJsonObject -> 全部管理器数据
		void setAllDataFromJsonObject(QJsonObject obj) override;
		

};

