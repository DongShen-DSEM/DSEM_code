#include "stdafx.h"
#include "W_MaterialDispersionFit_Magnetic.h"
#include "W_MaterialDispersionFit_MagneticCell.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		材料属性-离散拟合-磁导率 编辑窗口.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料数据类的编辑窗口。
-----==========================================================-----
*/

W_MaterialDispersionFit_Magnetic::W_MaterialDispersionFit_Magnetic(QWidget *parent)
	: W_DialogBase(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----ui初始化
	this->isAddMode = true;

	// > 选项
	ui.comboBox_FittingScheme->clear();
	ui.comboBox_FittingScheme->addItems(C_MaterialDispersionFit_Magnetic::getTypeList_FittingScheme());
	ui.comboBox_DataFormat->clear();
	ui.comboBox_DataFormat->addItems(C_MaterialDispersionFit_Magnetic::getTypeList_DataFormat());

	// > 表格
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);		//自适应大小
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);				//单行模式

	//-----------------------------------
	//----事件绑定

	connect(ui.comboBox_FittingScheme, &QComboBox::currentTextChanged, this, &W_MaterialDispersionFit_Magnetic::comboxChanged_FittingScheme);

	// > 列表
	connect(ui.comboBox_DataFormat, &QComboBox::currentTextChanged, this, &W_MaterialDispersionFit_Magnetic::comboxChanged_DataFormat);
	connect(ui.pushButton_add, &QPushButton::clicked, this, &W_MaterialDispersionFit_Magnetic::addOneRow);
	connect(ui.pushButton_modify, &QPushButton::clicked, this, &W_MaterialDispersionFit_Magnetic::modifyOneRow);
	connect(ui.pushButton_remove, &QPushButton::clicked, this, &W_MaterialDispersionFit_Magnetic::removeOneRow);
	connect(ui.pushButton_removeAll, &QPushButton::clicked, this, &W_MaterialDispersionFit_Magnetic::removeAllRow);
	connect(ui.pushButton_moveUp, &QPushButton::clicked, this, &W_MaterialDispersionFit_Magnetic::moveUpOneRow);
	connect(ui.pushButton_moveDown, &QPushButton::clicked, this, &W_MaterialDispersionFit_Magnetic::moveDownOneRow);
	connect(ui.pushButton_import, &QPushButton::clicked, this, &W_MaterialDispersionFit_Magnetic::importRowList);
	connect(ui.pushButton_export, &QPushButton::clicked, this, &W_MaterialDispersionFit_Magnetic::exportRowList);
	connect(ui.tableWidget, &QTableWidget::doubleClicked, this, &W_MaterialDispersionFit_Magnetic::modifyOneRow);	//（双击编辑）

	this->setDataInAddMode();
	this->initDialogButtonBox(ui.buttonBox);
}
W_MaterialDispersionFit_Magnetic::~W_MaterialDispersionFit_Magnetic(){
}


/*-----------------------------------
		控件 - 下拉框变化
*/
void W_MaterialDispersionFit_Magnetic::comboxChanged_FittingScheme() {

	if (ui.comboBox_FittingScheme->currentIndex() == 0) {
		ui.checkBox_useFreqRange->setEnabled(false);
		ui.label_ErrorLimit->setEnabled(false);
		ui.lineEdit_ErrorLimit->setEnabled(false);
		ui.label_MaxOrder->setEnabled(false);
		ui.lineEdit_MaxOrder->setEnabled(false);
	}
	if (ui.comboBox_FittingScheme->currentIndex() == 1) {
		ui.checkBox_useFreqRange->setEnabled(true);
		ui.label_ErrorLimit->setEnabled(true);
		ui.lineEdit_ErrorLimit->setEnabled(true);
		ui.label_MaxOrder->setEnabled(true);
		ui.lineEdit_MaxOrder->setEnabled(true);
	}
}


/*-----------------------------------
		控件 - 下拉框变化
*/
void W_MaterialDispersionFit_Magnetic::comboxChanged_DataFormat() {
	this->refreshTable();
}
/*-----------------------------------
		控件 - 添加
*/
void W_MaterialDispersionFit_Magnetic::addOneRow() {
	
	// > 从末尾添加
	W_MaterialDispersionFit_MagneticCell d(this);
	d.setDataInAddMode();
	if (ui.comboBox_DataFormat->currentIndex() == 0) { d.setMode_Imag(); }
	if (ui.comboBox_DataFormat->currentIndex() == 1) { d.setMode_Tand(); }
	connect(&d, &W_MaterialDispersionFit_MagneticCell::signal_acceptOne, this, &W_MaterialDispersionFit_Magnetic::acceptOne);
	if (d.exec() == QDialog::Accepted ) {
		C_MaterialDispersionFit_MagneticCell data = d.getData();
		this->local_cellList.append(data);
		this->local_data.data_list = this->local_cellList;
		this->refreshTable();
	}
	disconnect(&d);
}
/*-----------------------------------
		控件 - 编辑
*/
void W_MaterialDispersionFit_Magnetic::modifyOneRow() {
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, tr("提示"), tr("请先选择需要编辑的一行。"));
		return;
	}
	int pos = range.at(0).topRow();

	C_MaterialDispersionFit_MagneticCell c_p = this->local_cellList.at(pos);
	W_MaterialDispersionFit_MagneticCell d(this);
	d.setDataInModifyMode(c_p);
	if (ui.comboBox_DataFormat->currentIndex() == 0) { d.setMode_Imag(); }
	if (ui.comboBox_DataFormat->currentIndex() == 1) { d.setMode_Tand(); }
	connect(&d, &W_MaterialDispersionFit_MagneticCell::signal_acceptOne, this, &W_MaterialDispersionFit_Magnetic::acceptOne);
	if (d.exec() == QDialog::Accepted) {
		C_MaterialDispersionFit_MagneticCell data = d.getData();
		this->local_cellList.replace(pos, data);
		this->local_data.data_list = this->local_cellList;
		this->refreshTable();
	}
	disconnect(&d);
}
/*-----------------------------------
		控件 - 删除
*/
void W_MaterialDispersionFit_Magnetic::removeOneRow() {
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, tr("提示"), tr("请先选择需要删除的一行。"));
		return;
	}
	int pos = range.at(0).topRow();

	this->local_cellList.removeAt(pos);
	this->local_data.data_list = this->local_cellList;
	this->refreshTable();
}
/*-----------------------------------
		控件 - 清空全部
*/
void W_MaterialDispersionFit_Magnetic::removeAllRow() {
	if (this->local_cellList.count() == 0) { return; }
	if (QMessageBox::information(this, tr("提示"), tr("确定要清空全部数据？"), tr("是"), tr("否"), 0, 0, 1) == 0) {
		this->local_cellList.clear();
		this->local_data.data_list = this->local_cellList;
		this->refreshTable();
	}
}
/*-----------------------------------
		控件 - 上移
*/
void W_MaterialDispersionFit_Magnetic::moveUpOneRow(){
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, tr("提示"), tr("请先选择一行。"));
		return;
	}
	int pos = range.at(0).topRow();//（只选一行）
	if (pos != range.at(0).bottomRow()) {
		QMessageBox::warning(this, tr("提示"), tr("只能选择一行上移。"));
		return;
	}

	// > 执行上移
	if (pos <= 0){ return; }
	this->local_cellList.swap(pos, pos - 1);
	this->local_data.data_list = this->local_cellList;

	// > 刷新内容
	this->refreshTable();

	// > 刷新选中
	ui.tableWidget->clearSelection();
	ui.tableWidget->selectRow(pos - 1);
}
/*-----------------------------------
		控件 - 下移
*/
void W_MaterialDispersionFit_Magnetic::moveDownOneRow(){
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, tr("提示"), tr("请先选择一行。"));
		return;
	}
	int pos = range.at(0).topRow();//（只选一行）
	if (pos != range.at(0).bottomRow()) {
		QMessageBox::warning(this, tr("提示"), tr("只能选择一行下移。"));
		return;
	}

	// > 执行下移
	if (pos >= this->local_cellList.count() - 1){ return; }
	this->local_cellList.swap(pos, pos + 1);
	this->local_data.data_list = this->local_cellList;

	// > 刷新内容
	this->refreshTable();

	// > 刷新选中
	ui.tableWidget->clearSelection();
	ui.tableWidget->selectRow(pos + 1);
}
/*-----------------------------------
		控件 - 导入
		（注意可能会自定义后缀名、注释格式、以及分隔符）
*/
void W_MaterialDispersionFit_Magnetic::importRowList() {

	// > 选择文件
	QFileDialog fd;
	QString importFile;
	fd.setWindowTitle( tr("导入离散拟合数据") );
	fd.setAcceptMode(QFileDialog::AcceptOpen);
	fd.setDirectory(".");
	fd.setNameFilter("(*.txt)");
	fd.setViewMode(QFileDialog::Detail);
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) { return; }
		importFile = fd.selectedFiles().at(0);
	}else{
		return;
	}

	// > 清空数据
	if (this->local_cellList.count() != 0){
		if (QMessageBox::information(this, tr("提示"), tr("是否清空导入前的数据？"), tr("是"), tr("否"), 0, 0, 1) == 0) {
			this->local_cellList.clear();
			this->local_data.data_list = this->local_cellList;
			this->refreshTable();
		}
	}

	// > 读取文件数据（只读）
	QFile input_file(importFile);
	if (!input_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("错误"), tr("无法打开文件：") + importFile, QMessageBox::Yes);
		return;
	}
	QTextStream in(&input_file);
	QString s = in.readAll();

	// > 文本处理
	QStringList rows = s.split(QRegExp("[\n\r]+"));	//（多空行跳过）
	TTool::_QStringList_clearComment_(&rows, "#");	//（去掉注释行）
	rows.replaceInStrings(QRegExp("^[ \t]+"), "");	//（去掉前面的空格）
	rows.replaceInStrings(QRegExp("[ \t]+$"), "");	//（去掉后面的空格）

	// > 逐行解析
	bool isSuccessed = false;
	for (int i = 0; i < rows.size();i++) {
		QString row = rows.at(i);
		QStringList datas = row.split(QRegExp("[ \t]+"));
		if (datas.size() < 3) {
			continue;
		}
		isSuccessed = true;
		C_MaterialDispersionFit_MagneticCell p;
		if (ui.comboBox_DataFormat->currentIndex() == 0) {
			p.freq = datas.at(0).toDouble();
			p.value_real = datas.at(1).toDouble();
			p.value_imag = datas.at(2).toDouble();
			p.weight = datas.at(3).toDouble();
		}
		if (ui.comboBox_DataFormat->currentIndex() == 1) {
			p.freq = datas.at(0).toDouble();
			p.value_real = datas.at(1).toDouble();
			p.value_tand = datas.at(2).toDouble();
			p.weight = datas.at(3).toDouble();
		}
		this->local_cellList.append(p);
		this->local_data.data_list = this->local_cellList;
	}
	if (isSuccessed == false){
		QMessageBox::warning(this, tr("提示"), tr("文件数据格式不正确，读取失败。"), QMessageBox::Yes);
		return;
	}

	// > 刷新列表
	this->refreshTable();
}
/*-----------------------------------
		控件 - 导出
*/
void W_MaterialDispersionFit_Magnetic::exportRowList() {
	this->putUiToData();

	// > 选择文件
	QFileDialog fd;
	QString exportFile;
	fd.setWindowTitle( tr("导出离散拟合数据") );
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDirectory(".");
	fd.setNameFilter("(*.txt)");
	fd.selectFile( "DispersionFit_Magnetic.txt");
	fd.setViewMode(QFileDialog::Detail);
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) { return; }
		exportFile = fd.selectedFiles().at(0);
	}else{
		return;
	}

	// > 写入文件
	QFile file_to(exportFile);
	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		QMessageBox::warning(this, tr("错误"), tr("无法打开文件。"), QMessageBox::Yes);
		return;
	}
	QString context = "";
	for (int i = 0; i < this->local_cellList.count(); i++){
		C_MaterialDispersionFit_MagneticCell p = this->local_cellList.at(i);
		if (ui.comboBox_DataFormat->currentIndex() == 0) {
			context.append(TTool::_auto_(p.freq));
			context.append("\t");
			context.append(TTool::_auto_(p.value_real));
			context.append("\t");
			context.append(TTool::_auto_(p.value_imag));
			context.append("\t");
			context.append(TTool::_auto_(p.weight));
			context.append("\r\n");
		}
		if (ui.comboBox_DataFormat->currentIndex() == 1) {
			context.append(TTool::_auto_(p.freq));
			context.append("\t");
			context.append(TTool::_auto_(p.value_real));
			context.append("\t");
			context.append(TTool::_auto_(p.value_tand));
			context.append("\t");
			context.append(TTool::_auto_(p.weight));
			context.append("\r\n");
		}
	}
	file_to.write(context.toLocal8Bit());
	file_to.close();
}

/*-----------------------------------
		控件 - 并行添加信号
*/
void W_MaterialDispersionFit_Magnetic::acceptOne(C_MaterialDispersionFit_MagneticCell data){
	this->local_cellList.append(data);
	this->local_data.data_list = this->local_cellList;
	this->refreshTable();
}
/*-----------------------------------
		控件 - 刷新表格
*/
void W_MaterialDispersionFit_Magnetic::refreshTable() {

	// > 列变化
	if (ui.comboBox_DataFormat->currentIndex() == 0) {
		ui.tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Freq") << tr("Mu Real") << tr("Mu Imag") << tr("Weight"));
	}
	if (ui.comboBox_DataFormat->currentIndex() == 1) {
		ui.tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Freq") << tr("Mu Real") << tr("Mu Tand") << tr("Weight"));
	}

	// > 行变化
	ui.tableWidget->setRowCount(this->local_cellList.size());
	for (int i = 0; i< this->local_cellList.size(); i++) {
		C_MaterialDispersionFit_MagneticCell p = this->local_cellList.at(i);
		if (ui.comboBox_DataFormat->currentIndex() == 0) {
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(TTool::_auto_(p.freq)));
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(TTool::_auto_(p.value_real)));
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(TTool::_auto_(p.value_imag)));
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(TTool::_auto_(p.weight)));
		}
		if (ui.comboBox_DataFormat->currentIndex() == 1) {
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(TTool::_auto_(p.freq)));
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(TTool::_auto_(p.value_real)));
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(TTool::_auto_(p.value_tand)));
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(TTool::_auto_(p.weight)));
		}
	}
}


/*-------------------------------------------------
		窗口 - 设置数据（添加）
*/
void W_MaterialDispersionFit_Magnetic::setDataInAddMode() {
	this->local_data = C_MaterialDispersionFit_Magnetic();
	this->local_data.init();
	this->isAddMode = true;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_MaterialDispersionFit_Magnetic::setDataInModifyMode(C_MaterialDispersionFit_Magnetic data) {
	this->local_data = data;
	this->isAddMode = false;
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_MaterialDispersionFit_Magnetic W_MaterialDispersionFit_Magnetic::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_MaterialDispersionFit_Magnetic::putDataToUi() {

	// > 参数（基本参数）
	TTool::_int_(ui.comboBox_FittingScheme, &this->local_data.fittingScheme);
	TTool::_int_(ui.lineEdit_MaxOrder, &this->local_data.maxOrder);
	TTool::_double_(ui.lineEdit_ErrorLimit, &this->local_data.errorLimit);
	ui.checkBox_useFreqRange->setChecked(this->local_data.useFreqRange);
	
	TTool::_int_(ui.lineEdit_UsedOrder, &this->local_data.usedOrder);
	TTool::_double_(ui.lineEdit_Error, &this->local_data.error);

	// > 参数（表格参数）
	TTool::_int_(ui.comboBox_DataFormat, &this->local_data.data_format );
	this->local_cellList = this->local_data.data_list;

	// > 下拉框变化
	this->comboxChanged_FittingScheme();
	this->comboxChanged_DataFormat();
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_MaterialDispersionFit_Magnetic::putUiToData() {

	// > 参数（基本参数）
	TTool::_int_(&this->local_data.fittingScheme,ui.comboBox_FittingScheme );
	TTool::_int_(&this->local_data.maxOrder,ui.lineEdit_MaxOrder );
	TTool::_double_(&this->local_data.errorLimit,ui.lineEdit_ErrorLimit );
	this->local_data.useFreqRange = ui.checkBox_useFreqRange->isChecked();

	TTool::_int_(&this->local_data.usedOrder,ui.lineEdit_UsedOrder );
	TTool::_double_(&this->local_data.error,ui.lineEdit_Error );

	// > 参数（表格参数）
	TTool::_int_(&this->local_data.data_format, ui.comboBox_DataFormat);
	// this->local_cellList有变化就立即赋值了

};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_MaterialDispersionFit_Magnetic::base_acceptData() {
	this->putUiToData();

	// > 校验
	//	（无）

	emit acceptDataSignal(this->isAddMode, this->local_data);
	W_DialogBase::base_acceptData();
};
/*-------------------------------------------------
		窗口 - 取消数据
*/
void W_MaterialDispersionFit_Magnetic::base_rejectData() {
	W_DialogBase::base_rejectData();

};
/*-------------------------------------------------
		窗口 - 关闭事件
*/
void W_MaterialDispersionFit_Magnetic::closeEvent(QCloseEvent *e) {
	W_DialogBase::closeEvent(e);

};