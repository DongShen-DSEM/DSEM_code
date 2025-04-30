#include "stdafx.h"
#include "W_MaterialDispersionFit_DielectricCell.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		材料属性-离散拟合-电导率 单条数据窗口.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料数据类的编辑窗口。
-----==========================================================-----
*/

W_MaterialDispersionFit_DielectricCell::W_MaterialDispersionFit_DielectricCell(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----ui初始化
	this->isAddMode = true;

	//-----------------------------------
	//----校验绑定
	this->setDataInAddMode();

	//-----------------------------------
	//----事件绑定
	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &W_MaterialDispersionFit_DielectricCell::dialogButtonClicked);

	// > 特殊按键
	ui.buttonBox->button(QDialogButtonBox::Save)->setText(tr("确定并继续"));

}
W_MaterialDispersionFit_DielectricCell::~W_MaterialDispersionFit_DielectricCell(){
}


/*-------------------------------------------------
		控件 - 设置模式
*/
void W_MaterialDispersionFit_DielectricCell::setMode_Imag() {
	ui.stackedWidget->setCurrentIndex(0);
}
void W_MaterialDispersionFit_DielectricCell::setMode_Tand() {
	ui.stackedWidget->setCurrentIndex(1);
}

/*-------------------------------------------------
		窗口 - 设置数据（添加）
*/
void W_MaterialDispersionFit_DielectricCell::setDataInAddMode() {
	this->local_data = C_MaterialDispersionFit_DielectricCell();
	this->isAddMode = true;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_MaterialDispersionFit_DielectricCell::setDataInModifyMode(C_MaterialDispersionFit_DielectricCell data) {
	this->local_data = data;
	this->isAddMode = false;
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_MaterialDispersionFit_DielectricCell W_MaterialDispersionFit_DielectricCell::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_MaterialDispersionFit_DielectricCell::putDataToUi() {

	// > 参数（基本参数）
	TTool::_double_(ui.lineEdit_Freq, &this->local_data.freq );
	TTool::_double_(ui.lineEdit_Real, &this->local_data.value_real);
	TTool::_double_(ui.lineEdit_Imag, &this->local_data.value_imag);
	TTool::_double_(ui.lineEdit_Tand, &this->local_data.value_tand);
	TTool::_double_(ui.lineEdit_Weight, &this->local_data.weight);

};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_MaterialDispersionFit_DielectricCell::putUiToData() {

	// > 参数（基本参数）
	TTool::_double_(&this->local_data.freq, ui.lineEdit_Freq );
	TTool::_double_(&this->local_data.value_real, ui.lineEdit_Real );
	TTool::_double_(&this->local_data.value_imag, ui.lineEdit_Imag );
	TTool::_double_(&this->local_data.value_tand, ui.lineEdit_Tand );
	TTool::_double_(&this->local_data.weight, ui.lineEdit_Weight );

	// > 取值转换
	if (ui.stackedWidget->currentIndex() == 0) {
		this->local_data.calculateTand();
	}
	if (ui.stackedWidget->currentIndex() == 1) {
		this->local_data.calculateImag();
	}

};

/*-------------------------------------------------
		窗口 - 校验数据
*/
bool W_MaterialDispersionFit_DielectricCell::isValid(){
	
	// > 校验（暂无）
	//...

	return true;
}
/*-------------------------------------------------
		窗口 - 提交/取消数据（校验）
*/
void W_MaterialDispersionFit_DielectricCell::dialogButtonClicked(QAbstractButton * btn) {

	// > 按下 - 确定并继续键
	if (btn == ui.buttonBox->button(QDialogButtonBox::Save)) {
		this->putUiToData();

		// > 校验
		if (this->isValid() == false) { return; }

		emit signal_acceptOne(this->local_data);
		return;
	}
	// > 按下 - 确定键
	if (btn == ui.buttonBox->button(QDialogButtonBox::Ok)) {
		this->putUiToData();

		// > 校验
		if (this->isValid() == false) { return; }

		this->accept();
	}
	// > 按下 - 取消键
	if (btn == ui.buttonBox->button(QDialogButtonBox::Cancel) ||
		btn == ui.buttonBox->button(QDialogButtonBox::Close)) {
		this->reject();
	}

};
/*-------------------------------------------------
		窗口 - 关闭事件
*/
void W_MaterialDispersionFit_DielectricCell::closeEvent(QCloseEvent *e) {
	this->reject();
};