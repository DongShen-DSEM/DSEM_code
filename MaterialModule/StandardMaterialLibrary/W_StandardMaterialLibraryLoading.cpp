#include "stdafx.h"
#include "W_StandardMaterialLibraryLoading.h"
#include "Source/MainModule/Global/S_TabOrderManager.h"

/*
-----==========================================================-----
		类：		材料库 导入窗口.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料库数据的导入窗口。
-----==========================================================-----
*/

W_StandardMaterialLibraryLoading::W_StandardMaterialLibraryLoading(QWidget *parent)
	: W_DialogBase(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----ui初始化
	this->initDialogButtonBox(ui.buttonBox);
	m_Table = ui.tableWidget_materialLibrary;
	m_TypeFilter = ui.comboBox_searchType;
	m_AttrFilter = ui.comboBox_searchAttribute;
	m_NameSearch = ui.lineEdit_searchMaterial;
	m_CheckUseFilter = ui.checkBox_searchFilter;

	ui.comboBox_searchType->addItems(C_StandardMaterial::getLibParam_TypeOptions());
	ui.comboBox_searchAttribute->addItems(C_StandardMaterial::getLibParam_AttrOptions());

	// > 列设置
	ui.tableWidget_materialLibrary->horizontalHeader()->resizeSection(0, 40);
	// 设置表头中序号为0的列的宽度为40个像素

	ui.tableWidget_materialLibrary->horizontalHeader()->resizeSection(2, 90);
	// 设置表头中序号为2的列的宽度为90个像素

	ui.tableWidget_materialLibrary->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	// 设置表头中序号为1的列的宽度为自动拉伸的模式，宽度会动态调整

	ui.tableWidget_materialLibrary->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
	// 设置表头中序号为3的列的宽度为自动拉伸的模式，宽度会动态调整

	// qss微调
	// ui.tableWidget_materialLibrary->horizontalHeader()->setStyleSheet(R"(
	// 	border-top:1px solid #EEF0F2;
	// 	border-right:1px solid #EEF0F2;
	// 	border-top-right-radius: 4px;
	// )");

	// ui.tableWidget_materialLibrary->verticalHeader()->setStyleSheet(R"(
	// 	border-left:1px solid #EEF0F2;
	// 	border-bottom:1px solid #EEF0F2;
	// 	border-bottom-left-radius:4px;
	// )");

	ui.tableWidget_materialLibrary->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableWidget_materialLibrary->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.splitter->setStretchFactor(0,9);
	ui.splitter->setStretchFactor(1,1);

	//-----------------------------------
	//----事件绑定
	connect(ui.tableWidget_materialLibrary, &QTableWidget::itemSelectionChanged, this, &W_StandardMaterialLibraryLoading::selectedOneRow);
	connect(ui.comboBox_searchType, QOverload<const QString&>::of(&QComboBox::activated), 
		this, &W_StandardMaterialLibraryLoading::filterMaterialForm);
	connect(ui.comboBox_searchAttribute, QOverload<int>::of(&QComboBox::activated),
		this, &W_StandardMaterialLibraryLoading::filterMaterialForm);
	connect(ui.lineEdit_searchMaterial, &QLineEdit::textChanged, this, &W_StandardMaterialLibraryLoading::filterMaterialForm);
	connect(ui.checkBox_searchFilter, &QCheckBox::stateChanged, this, &W_StandardMaterialLibraryLoading::updateUseFilter);

	this->refreshTable();

	//-----------------------------------
	//----配置tab顺序
	S_TabOrderManager::getInstance()->configureTabOrder(this);
}

W_StandardMaterialLibraryLoading::~W_StandardMaterialLibraryLoading(){
}

Ui::W_StandardMaterialLibraryLoading *W_StandardMaterialLibraryLoading::getUI()
{
	return &ui;
}

/*-------------------------------------------------
		控件 - 选中一行
*/
void W_StandardMaterialLibraryLoading::selectedOneRow(){
	QList<QTableWidgetSelectionRange> range = ui.tableWidget_materialLibrary->selectedRanges();
	if (range.size() == 0) { return; }
	int pos = range.at(0).topRow();//（可选多行）

	QTableWidgetItem* item = ui.tableWidget_materialLibrary->item( pos, 1 );
	QString name = item->text();
	C_StandardMaterial material = S_StandardMaterialLibrary::getInstance()->getMaterial_ByName(name);

	// > 绑定选中的材料
	this->local_selectedMaterial = material;

	// > 显示材料信息
	ui.lineEdit_name->setText(material.getName());
	ui.lineEdit_type->setText(material.getType());
	ui.lineEdit_materialSets->setText(material.getLibParam_MaterialSets());
	ui.plainTextEdit_attributes->setPlainText(material.getAttributesDesc());
	ui.plainTextEdit_description->setPlainText(material.getDescription());
}
/*-------------------------------------------------
		控件 - 刷新表格
*/
void W_StandardMaterialLibraryLoading::refreshTable(){
	QList<C_StandardMaterial> material_list = S_StandardMaterialLibrary::getInstance()->getMaterial_All();
	
	ui.tableWidget_materialLibrary->clearContents();
	ui.tableWidget_materialLibrary->setRowCount(material_list.count());
	for (int i = 0; i < material_list.count(); i++){
		C_StandardMaterial material = material_list.at(i);

		QTableWidgetItem* item1 = new QTableWidgetItem();
		item1->setIcon(material.getLibParam_TypeIcon());
		ui.tableWidget_materialLibrary->setItem(i, 0, item1);
		ui.tableWidget_materialLibrary->setItem(i, 1, new QTableWidgetItem(material.getName()));
		ui.tableWidget_materialLibrary->setItem(i, 2, new QTableWidgetItem(material.getType()));
		ui.tableWidget_materialLibrary->setItem(i, 3, new QTableWidgetItem(material.getLocation()));
	}
}

/*-------------------------------------------------
		控件 - 筛选表格
*/
void W_StandardMaterialLibraryLoading::filterMaterialForm() {
	QString typeFilter = m_TypeFilter->currentText();
	QString attrFilter = m_AttrFilter->currentText();

	// 创建一个正则表达式来匹配搜索字符串
	QRegExp nameSearch(m_NameSearch->text(), Qt::CaseInsensitive, QRegExp::FixedString);

	for (int row = 0; row < m_Table->rowCount(); ++row) {
		// 类型筛选
		bool visible = (typeFilter == "<All>" || typeFilter == m_Table->item(row, 2)->text());

		// 描述筛选
		C_StandardMaterial material = S_StandardMaterialLibrary::getInstance()->getMaterial_ByName(m_Table->item(row, 1)->text());
		QString attr = material.getAttributesDesc();
		bool visible2 = (attrFilter == "<All>"|| attr.contains(attrFilter));

		// 字符串搜索
		bool visible3 = m_Table->item(row, 1) && m_Table->item(row, 1)->text().contains(nameSearch);

		if (visible && visible2 && visible3) {
			m_Table->showRow(row);
		}
		else {
			m_Table->hideRow(row);
		}
	}
}

/*-------------------------------------------------
		控件 - 是否使用筛选
*/
void W_StandardMaterialLibraryLoading::updateUseFilter(int state) {
	if (state == Qt::Checked) {
		// 复选框被选中
		filterMaterialForm();
	}
	else if (state == Qt::Unchecked) {
		// 复选框被取消选中
		for (int row = 0; row < m_Table->rowCount(); ++row) {
			m_Table->showRow(row);
		}
	}
	else if (state == Qt::PartiallyChecked) {
		// 复选框处于部分选中状态
	}
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_StandardMaterial W_StandardMaterialLibraryLoading::getData() {
	return this->local_selectedMaterial;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_StandardMaterialLibraryLoading::putDataToUi() {
	//（无）
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_StandardMaterialLibraryLoading::putUiToData() {
	//（无）
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_StandardMaterialLibraryLoading::base_acceptData(){
	this->putUiToData();

	// > 校验
	//	（无）

	// > 发送选中的信号
	emit acceptDataSignal(this->local_selectedMaterial);

	W_DialogBase::base_acceptData();
};
/*-------------------------------------------------
		窗口 - 取消数据
*/
void W_StandardMaterialLibraryLoading::base_rejectData() {
	
	// > 清空选择
	this->local_selectedMaterial = C_StandardMaterial();
	
	W_DialogBase::base_rejectData();
};
/*-------------------------------------------------
		窗口 - 关闭事件
*/
void W_StandardMaterialLibraryLoading::closeEvent(QCloseEvent *e) {
	W_DialogBase::closeEvent(e);

};