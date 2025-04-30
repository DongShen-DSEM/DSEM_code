#pragma once
#include "stdafx1.h"

#include "Source/ProjectModule/Undo/ExtendPart/C_UndoDataBase.h"
#include "Source/ProjectModule/Undo/ExtendPart/C_UndoDataGlobalBase.h"

/*
-----==========================================================-----
		类：		材料模块 - 树节点管理.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		此类定义该模块所使用的树节点。
					（详细见cpp）
-----==========================================================-----
*/

class S_MaterialTreeManager : public QObject
{
	Q_OBJECT

	public:
		S_MaterialTreeManager();
		~S_MaterialTreeManager();
		static S_MaterialTreeManager* cur_manager;			//单例
		static S_MaterialTreeManager* getInstance();		//单例，获取自己



	//-----------------------------------
	//----树节点（通用）
	public:
									//树节点（通用） - 判断树枝
									//		【说明】：参数格式为："aaa/bbb/"。
		bool hasBranch(QString dir_str, QTreeWidgetItem* root);
									//树节点（通用） - 获取树枝
									//		【说明】：参数格式为："aaa/bbb/"。
		QTreeWidgetItem* getBranch(QString dir_str, QTreeWidgetItem* root);
	protected:
									//树节点（通用） - 创建树枝
									//		【说明】：参数格式为："aaa/bbb/"，若已存在则不会创建。
		void createBranchByDir(QString dir_str, QTreeWidgetItem* root);

									//树节点（通用） - 将路径拆解为树枝名称列表
		QStringList getBranchNameByDir(QString dir_str);
									//树节点（通用） - 向下找树节点
		QTreeWidgetItem* findNextBranch(QTreeWidgetItem* cur_item, QString name);
									//树节点（通用） - 找到 树枝下 所有叶子节点
									//		【说明】：叶子不一定是文件节点，也可能存在空树枝的情况。
		QList<QTreeWidgetItem*> findAllLeaf_ByBranch(QTreeWidgetItem* cur_item);
									//树节点（通用） - 找到 树枝下一层的 所有叶子节点
									//		【说明】：叶子不一定是文件节点，也可能存在空树枝的情况。
		QList<QTreeWidgetItem*> findChildLeaf_ByBranch(QTreeWidgetItem* cur_item);


		
	//-----------------------------------
	//----树根
	protected:
		QTreeWidgetItem* m_root = nullptr;
	public:
									//树根 - 树根初始化
									//		【说明】：此函数由树控件调用，调用后能初始化树根。
		void initRoot_Material(QTreeWidget* treeWidget);
									//树根 - 获取树根
		QTreeWidgetItem* getRoot_Material();
									//树根（端口） - 刷新树根
		void refreshRoot_Material();
									//树根（端口） - 清空树根
									//		【说明】：树根下所有树枝/叶子全部删除。
		void clearRoot_Material();
									//树根（端口） - 清空树枝
									//		【说明】：树根下的关键树枝保留，清除所有叶子节点。
		void clearBranch_Material();
									//树根 - 展开树根
		void setRootExpandAll_Material();
									//树根 - 折叠树根
		void setRootCollapseAll_Material();
	
	//-----------------------------------
	//----树节点事件
	public:
									//树节点事件 - 失去焦点时
									//		【说明】：拥有焦点时，失去焦点后才触发；一直没有焦点时不会触发。
		void itemFocusOut_Material(QList<QTreeWidgetItem*> item_list);
									//树节点事件 - 单击操作
									//		【说明】：此函数由树控件调用，该模块的树节点被触发时才会进入函数。
		void itemClicked_Material(QList<QTreeWidgetItem*> item_list);
									//树节点事件 - 改变操作
									//		【说明】：此函数由树控件调用，该模块的树节点被触发时才会进入函数。
		void itemSelectionChanged_Material(QList<QTreeWidgetItem*> item_list);
									//树节点事件 - 双击操作
									//		【说明】：此函数由树控件调用，该模块的树节点被触发时才会进入函数。
		void itemDoubleClicked_Material(QList<QTreeWidgetItem*> item_list);
									//树节点事件 - 右键操作
									//		【说明】：此函数由树控件调用，该模块的树节点被触发时才会进入函数。
		void itemRightClicked_Material(QList<QTreeWidgetItem*> item_list);

	//-----------------------------------
	//----树节点
	protected:
		QTreeWidgetItem* m_item_PEC = nullptr;				//树枝 - 默认材料 - PEC
		QTreeWidgetItem* m_item_Vacuum = nullptr;			//树枝 - 默认材料 - 真空
		QList<QTreeWidgetItem*> m_item_MaterialFolder;		//树枝 - 材料文件夹
		QList<QTreeWidgetItem*> m_item_MaterialList;		//树枝 - 材料集合
		QStringList m_selectedList_MaterialId;
	public slots:
									//树节点 - 是否有选中ID
		bool hasItemSelectedID_Material();
									//树节点 - 获取选中ID
		QStringList getItemSelectedID_Material();
	public:
	signals:
									//信号 - 树节点选中时
		void signal_itemSelected_Material(bool isSelected);


	//-----------------------------------
	//----选中
	protected:
		QList<QTreeWidgetItem*> m_item_selected;			//选中的材料
	public:
										//选中 - 被选中
		bool isMaterialSelected();
										//选中 - 获取被选中的ID
		QStringList getSelectedMaterialId();

	
	//-----------------------------------
	//----数据容器
	public slots:
										//数据容器 - 树变化 - 某数据 被添加
		void containerDataAdded(C_UndoDataBasePtr ptr, int index);
										//数据容器 - 树变化 - 某数据 被修改
		void containerDataModifyed(C_UndoDataBasePtr ptr, int index);
										//数据容器 - 树变化 - 某数据 被删除
		void containerDataDeleted(C_UndoDataBasePtr ptr, int index);
										//数据容器 - 树变化 - 某全局数据 被修改
		void containerGlobalDataModifyed(C_UndoDataGlobalBasePtr ptr);
										//数据容器 - 树变化 - 存储数据 被读取
		void containerDataAllReloaded();
};

