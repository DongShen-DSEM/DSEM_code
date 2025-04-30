#ifndef W_StandardMaterial_H
#define W_StandardMaterial_H
#include "stdafx1.h"

#include "ui_W_StandardMaterial.h"
#include "C_StandardMaterial.h"

#include "Source/Utils/WidgetForm/DialogBase/W_DialogBase.h"
#include "Source/Utils/WidgetForm/ValueSynchronizer/P_QLineEditValueSynchronizer.h"

#include "DispersionUser/W_DispersionUser.h"

/*
-----==========================================================-----
		类：		材料 编辑窗口.h
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料数据类的编辑窗口。
-----==========================================================-----
*/

class W_StandardMaterial : public W_DialogBase
{
	Q_OBJECT

	public:
		W_StandardMaterial(QWidget *parent);
		~W_StandardMaterial();
		
	//-----------------------------------
	//----控件
	public:
		bool m_typeBlock;						//阻塞
		bool isEditingDefaultMaterial;			//是否在编辑默认材料
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_1;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_2;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_3;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_4;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_5;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_6;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_7;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_8;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_9;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_10;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_11;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_El_12;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_Ma_1;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_Ma_2;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_Ma_3;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_Ma_6;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_Ma_7;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_Ma_8;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_Ma_9;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_Ma_10;	//同步值
		P_QLineEditValueSynchronizer* m_P_QLineEditValueSynchronizer_Ma_11;	//同步值
	public:
									//控件 - 获取自动命名
		QString getAutoName();
									//控件 - 类型变化 材料
		void typeChanged_Material();	
									//控件 - 类型变化 电导率
		void typeChanged_ElectricConductivity();
		void typeChanged_DielectricDispersion();
									//控件 - 类型变化 磁导率
		void typeChanged_MagneticConductivity();
		void typeChanged_MagneticDispersion();
									//控件 - 刷新频率值
		void refreshFrequencyValue();	
									//控件 - 编辑颜色
		void editColor();	
									//控件 - 离散拟合 - 电导率
		void openWindowDispersionFit_Dielectric();
									//控件 - 离散拟合 - 磁导率
		void openWindowDispersionFit_Magnetic();
									//控件 - 电导率 - 用户选项 2023-12-27 杨骁
		void onDielectricUserOption(int);
									//控件 - 磁导率 - 用户选项 2023-12-27 杨骁
		void onMagneticUserOption(int);
	private:
		QButtonGroup * m_DielectricUserGroup = nullptr; // 0:Disp.model 1:User
		QButtonGroup * m_MagneticUserGroup = nullptr; // 0:Disp.model 1:User

	//-----------------------------------
	//----用户输入子窗口
	public:
										//用户输入子窗口 - 电导率 - 打开子窗口
		void onOpenDispersionUserWindow();
	private:
										//用户输入子窗口 - 电导率 - 子窗口
		W_DispersionUser* m_W_DispersionUser=nullptr;
		
	//-----------------------------------
	//----窗口
	public:
		bool isAddMode;
		C_StandardMaterial local_StandardMaterial;
	public:
										//窗口 - 设置数据（添加）
		void setDataInAddMode();
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_StandardMaterial p);
										//窗口 - 设置默认材料数据（修改）
		void setDefaultDataInModifyMode(C_StandardMaterial p);
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
										//窗口 - 提交数据（校验）- 但是不关闭窗口
		void onApplyData(C_DispersionUser);
										//窗口 - 子窗口提交数据同步到当前窗口数据类对应的变量中 2024-1-4 杨骁
		void onAcceptDispersionUserData(C_DispersionUser);
	private:
										//窗口 - 标记子窗口是否点击了应用按钮
		bool m_DielectricUserApply = false;
	signals:
		void acceptDataSignal(bool isAddMode, C_StandardMaterial data);

	private:
		Ui::W_StandardMaterial ui;
	//-----------------------------------

};

#endif // W_StandardMaterial_H
