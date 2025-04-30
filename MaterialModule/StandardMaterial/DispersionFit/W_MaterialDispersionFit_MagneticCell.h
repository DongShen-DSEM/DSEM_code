#ifndef W_MaterialDispersionFit_MagneticCell_H
#define W_MaterialDispersionFit_MagneticCell_H
#include "stdafx1.h"

#include "ui_W_MaterialDispersionFit_MagneticCell.h"
#include "C_MaterialDispersionFit_Magnetic.h"

#include "Source/Utils/WidgetForm/DialogBase/W_DialogBase.h"

/*
-----==========================================================-----
		类：		材料属性-离散拟合-磁导率 单条数据窗口.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料数据类的编辑窗口。
-----==========================================================-----
*/

class W_MaterialDispersionFit_MagneticCell : public QDialog
{
	Q_OBJECT

	public:
		W_MaterialDispersionFit_MagneticCell(QWidget *parent);
		~W_MaterialDispersionFit_MagneticCell();
		
	//-----------------------------------
	//----控件
	public:
										//控件 - 设置模式
		void setMode_Imag();
		void setMode_Tand();
		
	//-----------------------------------
	//----窗口
	public:
		bool isAddMode;
		C_MaterialDispersionFit_MagneticCell local_data;
	public:
										//窗口 - 设置数据（添加）
		void setDataInAddMode();
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_MaterialDispersionFit_MagneticCell data);
										//窗口 - 取出数据
		C_MaterialDispersionFit_MagneticCell getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 校验数据
		bool isValid();
										//窗口 - 提交/取消数据（校验）
		void dialogButtonClicked(QAbstractButton * btn);
										//窗口 - 关闭事件
		void closeEvent(QCloseEvent *e) override;
	public:
	signals:
										//信号 - 并行添加
		void signal_acceptOne(C_MaterialDispersionFit_MagneticCell data);

	private:
		Ui::W_MaterialDispersionFit_MagneticCell ui;
	//-----------------------------------

};

#endif // W_MaterialDispersionFit_MagneticCell_H
