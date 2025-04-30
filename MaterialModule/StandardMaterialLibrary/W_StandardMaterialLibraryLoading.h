#ifndef W_StandardMaterialLibraryLoading_H
#define W_StandardMaterialLibraryLoading_H
#include "stdafx1.h"

#include "Source/Utils/WidgetForm/DialogBase/W_DialogBase.h"
#include "S_StandardMaterialLibrary.h"
#include "ui_W_StandardMaterialLibraryLoading.h"


/*
-----==========================================================-----
		类：		材料库 导入窗口.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料库数据的导入窗口。
-----==========================================================-----
*/

class W_StandardMaterialLibraryLoading : public W_DialogBase
{
	Q_OBJECT

	public:
		W_StandardMaterialLibraryLoading(QWidget *parent);
		~W_StandardMaterialLibraryLoading();
		

	//-----------------------------------
	//----控件
	public slots:
										//控件 - 选中一行
		void selectedOneRow();
										//控件 - 刷新表格
		void refreshTable();
										//控件 - 筛选表格
		void filterMaterialForm();
										//控件 - 是否使用筛选
		void updateUseFilter(int);
	//-----------------------------------
	//----窗口
	protected:
		bool isAddMode;
		C_StandardMaterial local_selectedMaterial;
	public:
										//窗口 - 取出数据
		C_StandardMaterial getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
		
	public slots:
										//窗口 - 提交数据（校验）
		void base_acceptData() override;
										//窗口 - 取消数据
		void base_rejectData() override;
										//窗口 - 关闭事件
		void closeEvent(QCloseEvent *e) override;
	signals:
		void acceptDataSignal(C_StandardMaterial data);

	//-----------------------------------
	//----UI
	public:
		Ui::W_StandardMaterialLibraryLoading *getUI();
	private:
		Ui::W_StandardMaterialLibraryLoading ui;
		QTableWidget * m_Table = nullptr;
		QComboBox * m_TypeFilter = nullptr;
		QComboBox * m_AttrFilter = nullptr;
		QLineEdit * m_NameSearch = nullptr;
		QCheckBox * m_CheckUseFilter = nullptr;
};

#endif // W_StandardMaterialLibraryLoading_H
