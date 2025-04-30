#ifndef W_MaterialDispersionFit_Dielectric_H
#define W_MaterialDispersionFit_Dielectric_H
#include "stdafx1.h"

#include "ui_W_MaterialDispersionFit_Dielectric.h"
#include "C_MaterialDispersionFit_Dielectric.h"

#include "Source/Utils/WidgetForm/DialogBase/W_DialogBase.h"

/*
-----==========================================================-----
		类：		材料属性-离散拟合-电导率 编辑窗口.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料数据类的编辑窗口。
-----==========================================================-----
*/

class W_MaterialDispersionFit_Dielectric : public W_DialogBase
{
	Q_OBJECT

	public:
		W_MaterialDispersionFit_Dielectric(QWidget *parent);
		~W_MaterialDispersionFit_Dielectric();
		
	//-----------------------------------
	//----控件（基本数据）
	public slots:
									//控件 - 下拉框变化
		void comboxChanged_FittingScheme();
		
	//-----------------------------------
	//----控件（表格数据）
	public slots:
									//控件 - 下拉框变化
		void comboxChanged_DataFormat();
									//控件 - 添加
		void addOneRow();
									//控件 - 编辑
		void modifyOneRow();
									//控件 - 删除
		void removeOneRow();
									//控件 - 清空全部
		void removeAllRow();
									//控件 - 上移
		void moveUpOneRow();
									//控件 - 下移
		void moveDownOneRow();
									//控件 - 导入
		void importRowList();
									//控件 - 导出
		void exportRowList();
		
									//控件 - 并行添加信号
		void acceptOne(C_MaterialDispersionFit_DielectricCell data);
									//控件 - 刷新表格
		void refreshTable();
		
	//-----------------------------------
	//----窗口
	public:
		bool isAddMode;
		QList<C_MaterialDispersionFit_DielectricCell> local_cellList;
		C_MaterialDispersionFit_Dielectric local_data;
	public:
										//窗口 - 设置数据（添加）
		void setDataInAddMode();
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_MaterialDispersionFit_Dielectric data);
										//窗口 - 取出数据
		C_MaterialDispersionFit_Dielectric getData();
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
		void acceptDataSignal(bool isAddMode, C_MaterialDispersionFit_Dielectric data);

	private:
		Ui::W_MaterialDispersionFit_Dielectric ui;
	//-----------------------------------

};

#endif // W_MaterialDispersionFit_Dielectric_H
