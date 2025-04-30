#ifndef W_DispersionUser_H
#define W_DispersionUser_H

#include "stdafx1.h"
#include "ui_W_DispersionUser.h"
#include "C_DispersionUser.h"
#include "Source/Utils/WidgetForm/DialogBase/W_DialogBase.h"

class W_DispersionUser : public W_DialogBase
{
	Q_OBJECT

	public:
		W_DispersionUser(C_DispersionUser, QWidget *parent = nullptr);
		~W_DispersionUser();
	
	//-----------------------------------
	//----参数
	public:
							//参数 - 传递材料名称给数据类
		void setName(QString name);

	// signals:
	// 						//参数 - 同步数据到父窗口
	// 	void signal_acceptDispersionUserData(C_DispersionUser);

	//-----------------------------------
	//----窗口
	public:
		bool isAddMode;
		C_DispersionUser m_C_DispersionUser;

							//窗口 - 设置数据（添加）
		void setDataInAddMode();
							//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_DispersionUser p);
							//窗口 - 取出数据
		C_DispersionUser getData();
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

	//-----------------------------------
	//----控件
	public:
							//控件 - 表格 - 用已有数据初始化model
		void initModel();
							//控件 - 表格 - 从文件添加数据
		void onLoadModel();
							//控件 - 表格 - 添加一行数据
		void onAddModelRow();
							//控件 - 表格 - 删除一行数据
		void onDelModelRow();
							//控件 - 表格 - 清空数据
		void onClearModel();
							//控件 - 表格 - 从下标差值计算出单位转换需要的倍数值
		double getConversionValue();
							//控件 - 表格 - 选择频率单位
		void onSelectFreqUnit(int);
							//控件 - 表格 - 剔除重复行
		std::vector<std::vector<double>> removeDuplicates(std::vector<std::vector<double>>);
							//控件 - 点击应用按钮
		void onClickApply();
							//控件 - 点击详情
		void onClickDetails();

	signals:
							//控件 - 点击应用按钮，同步给上层材料弹窗
		void signal_clickApply(C_DispersionUser);

	private:
							//控件 - 表格数据
		QStandardItemModel *m_TableModel = nullptr;
		Ui::W_DispersionUser *ui;
};

#endif // W_DispersionUser_H
