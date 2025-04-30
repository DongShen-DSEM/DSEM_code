#include "stdafx.h"
#include "W_DispersionUser.h"
#include "Source/Utils/Common/TTool.h"
#include "Source/Utils/Common/Tool4Sean.h"
#include "Source/ResultModule/S_ResultTreeManager.h"

W_DispersionUser::W_DispersionUser(C_DispersionUser dispersionUser,QWidget *parent)
	: m_C_DispersionUser(dispersionUser),
	W_DialogBase(parent),
	ui(new Ui::W_DispersionUser)
{
	ui->setupUi(this);

	// > ui初始化
	ui->comboBox_Fitting->addItems(C_DispersionUser::getFittingSchemeList());
	ui->comboBox_DataFormat->addItems(C_DispersionUser::getDataFormatList());
	ui->comboBox_FreqUnit->addItems(C_DispersionUser::getFreqUnitList());

	// > ui初始化 - 表格初始化
	// 创建数据模型
	m_TableModel = new QStandardItemModel(0, 4);
	m_TableModel->setHorizontalHeaderLabels(QStringList() << tr("Freq. [Hz]") << tr("Eps'") << tr("Eps''") << tr("Weight"));

	// 初始化model
	initModel();

	// 设置qss
	ui->tableView_UserInput->setStyleSheet(R"(
		QTableView {
			border:1px solid #d8d8d8;
		}
	)");
	
	// 隐藏列头
	ui->tableView_UserInput->verticalHeader()->setVisible(false);
	// 靠左对齐
	ui->tableView_UserInput->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

	// 设置数据
	ui->tableView_UserInput->setModel(m_TableModel);

	// > 窗口初始化
	this->setDataInAddMode();
	this->initDialogButtonBox(ui->buttonBox);

	//-----------------------------------
	//----事件绑定
	connect(ui->pushButton_Load, &QPushButton::clicked, this, &W_DispersionUser::onLoadModel);
	connect(ui->pushButton_Add, &QPushButton::clicked, this, &W_DispersionUser::onAddModelRow);
	connect(ui->pushButton_Del, &QPushButton::clicked, this, &W_DispersionUser::onDelModelRow);
	connect(ui->pushButton_Clear, &QPushButton::clicked, this, &W_DispersionUser::onClearModel);
	connect(ui->comboBox_FreqUnit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &W_DispersionUser::onSelectFreqUnit);
	connect(ui->pushButton_Details, &QPushButton::clicked, this, &W_DispersionUser::onClickDetails);
	
	QPushButton *applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    if (applyButton) {
        connect(applyButton, &QPushButton::clicked, this, &W_DispersionUser::onClickApply);
    }
}

/*-------------------------------------------------
		窗口 - 设置数据（添加）
*/
void W_DispersionUser::setDataInAddMode() {
	this->isAddMode = true;
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_DispersionUser::setDataInModifyMode(C_DispersionUser p) {
	this->m_C_DispersionUser = p;
	this->isAddMode = false;
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_DispersionUser W_DispersionUser::getData() {
	return this->m_C_DispersionUser;
};

/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_DispersionUser::putDataToUi() {
	ui->comboBox_FreqUnit->setCurrentIndex(m_C_DispersionUser.m_FreqUnitIndex);
	ui->comboBox_DataFormat->setCurrentIndex(m_C_DispersionUser.m_DataFormatIndex);
	ui->comboBox_Fitting->setCurrentIndex(m_C_DispersionUser.m_FittingIndex);

	ui->lineEdit_MaxOrder->setText(QString::number(m_C_DispersionUser.getMaxOrder()));
	ui->lineEdit_ErrorLimit->setText(QString::number(m_C_DispersionUser.getErrorLimit()));
	ui->lineEdit_UsedOrder->setText(QString::number(m_C_DispersionUser.getUsedOrder()));
	ui->lineEdit_Error->setText(QString::number(m_C_DispersionUser.getError()));
};

/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_DispersionUser::putUiToData() {
	m_C_DispersionUser.m_FreqUnitIndex = ui->comboBox_FreqUnit->currentIndex();
	m_C_DispersionUser.m_DataFormatIndex = ui->comboBox_DataFormat->currentIndex();
	m_C_DispersionUser.m_FittingIndex = ui->comboBox_Fitting->currentIndex();

	m_C_DispersionUser.setMaxOrder(TTool::_to_double_(ui->lineEdit_MaxOrder->text()));
	m_C_DispersionUser.setErrorLimit(TTool::_to_double_(ui->lineEdit_ErrorLimit->text()));
};

/*-------------------------------------------------
		控件 - 表格 - 点击应用按钮
*/
void W_DispersionUser::onClickApply()
{
	putUiToData();

	// 获取数据
	std::vector<std::vector<double>> results;

    // 遍历模型的行
    for (int row = 0; row < m_TableModel->rowCount(); ++row) {
        std::vector<double> rowData;

		bool inValidRow = false;
        // 遍历模型的列
        for (int col = 0; col < m_TableModel->columnCount(); ++col) {
            // 获取单元格的数据
            QModelIndex index = m_TableModel->index(row, col);
            QVariant value = m_TableModel->data(index);

			// 确保数据有效
			if (value.isNull() || !value.isValid()) {
				// 标记无效行
				inValidRow = true;
				break;
			}

            // 转换数据为 double 并添加到当前行的 vector 中
            rowData.push_back(value.toDouble());
        }

		if (!inValidRow)
		{
			results.push_back(rowData);
		}
	}

	// 去重
	results = removeDuplicates(results);
	// Note: 生成曲线txt文件
	m_C_DispersionUser.setModelData(results);
	m_C_DispersionUser.genUserDispersionTxt(true);

	ui->lineEdit_UsedOrder->setText(QString::number(m_C_DispersionUser.getUsedOrder()));
	ui->lineEdit_Error->setText(QString::number(m_C_DispersionUser.getError()));

	if (m_C_DispersionUser.getError() > m_C_DispersionUser.getErrorLimit())
	{
		QMessageBox::warning(nullptr, "DSEMUI", QString(tr(
			"Nth order dispersion fit for electric material [%1]."
			"The reached model error [%2%] is greater than the desired limit [%3%]."
			"Please check your input data or try to use different fitting parameters."))
			.arg(m_C_DispersionUser.getName())
			.arg(m_C_DispersionUser.getError())
			.arg(m_C_DispersionUser.getErrorLimit()));
	}

	emit signal_clickApply(m_C_DispersionUser);
}

/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_DispersionUser::base_acceptData() {
	onClickApply();
	W_DialogBase::base_acceptData();
};

/*-------------------------------------------------
		窗口 - 取消数据（校验）
*/
void W_DispersionUser::base_rejectData() {
	W_DialogBase::base_rejectData();
}

/*-------------------------------------------------
		窗口 - 关闭事件
*/
void W_DispersionUser::closeEvent(QCloseEvent *e) {
	W_DialogBase::closeEvent(e);
}

/*-------------------------------------------------
		控件 - 表格 - 用已有数据初始化model
*/
void W_DispersionUser::initModel()
{
	auto &data = m_C_DispersionUser.getModelData();
	if (data.empty())
	{
		// 添加空行
		onAddModelRow();
	}
	else
	{
		for (const auto &innerVector : data)
		{
			QList<QStandardItem *> rowItems;
			for (const auto &value : innerVector)
			{
				QStandardItem *item = new QStandardItem(QString::number(value));
				rowItems.append(item);
			}
			m_TableModel->appendRow(rowItems);
		}
	}
}

/*-------------------------------------------------
		控件 - 表格 - 从文件添加数据
*/
void W_DispersionUser::onLoadModel()
{
	// > 用户选择项目文件
	QString filePath;
	QFileDialog *fd = new QFileDialog(this);
	fd->setWindowTitle(tr("Open User Defined Dispersion File"));
	fd->setAcceptMode(QFileDialog::AcceptOpen);
	// fd.setDirectory(this->data_ProjectData.getParentPath());
	fd->setNameFilters(QStringList() << QString(tr("Text Files(*.txt)")));
	fd->setViewMode(QFileDialog::Detail);

	if (fd->exec() == QDialog::Accepted)
	{
		filePath = fd->selectedFiles().at(0);

		// 获取数据并写入
		auto userData = Tool4Sean::getUserDataFromFilePath(filePath);

		for (auto row : userData)
		{
			QList<QStandardItem *> items;
			for (double value : row)
			{
				// 转换单位
				QString strValue = QString::number(value * getConversionValue());
				QStandardItem *item = new QStandardItem(strValue);
				items.append(item);
			}
			m_TableModel->appendRow(items);
		}
	}
}

/*-------------------------------------------------
		控件 - 表格 - 添加一行数据 
*/
void W_DispersionUser::onAddModelRow()
{
	QList<QStandardItem *> emptyRow;
	emptyRow << new QStandardItem()
			 << new QStandardItem()
			 << new QStandardItem()
			 << new QStandardItem(QString("1.0"));

	m_TableModel->appendRow(emptyRow);
}

/*-------------------------------------------------
		控件 - 表格 - 删除一行数据 
*/
void W_DispersionUser::onDelModelRow()
{
	// 获取选中行的索引
	QModelIndex index = ui->tableView_UserInput->currentIndex();

	// 如果没有选择行,直接返回
	if (!index.isValid())
		return;

	// 获取行号
	int row = index.row();

	// 删除选中的行
	m_TableModel->removeRow(row);
}

/*-------------------------------------------------
		控件 - 表格 - 清空数据
*/
void W_DispersionUser::onClearModel()
{
	m_TableModel->removeRows(0, m_TableModel->rowCount());
	onAddModelRow();
}

/*-------------------------------------------------
		控件 - 表格 - 从下标差值计算出单位转换需要的倍数值
*/
double W_DispersionUser::getConversionValue()
{
	int powerDiff = m_C_DispersionUser.m_FreqUnitIndex * 3; // 下标差值乘以 3，因为是每个单位相差 1000 的倍数
    return pow(10, powerDiff);
}

/*-------------------------------------------------
		控件 - 表格 - 选择频率单位
*/
void W_DispersionUser::onSelectFreqUnit(int index)
{
	m_C_DispersionUser.m_FreqUnitIndex = index;
}

/*-------------------------------------------------
		控件 - 表格 - 剔除重复行
*/
std::vector<std::vector<double>> W_DispersionUser::removeDuplicates(std::vector<std::vector<double>> results)
{
	std::sort(results.begin(), results.end());

    auto last = std::unique(results.begin(), results.end(), [](const std::vector<double>& a, const std::vector<double>& b) {
        return a == b;
    });

	results.erase(last, results.end());
	return results;
}

/*-------------------------------------------------
		控件 - 表格 - 点击详情
*/
void W_DispersionUser::onClickDetails()
{
	QDialog dialog;
	dialog.setWindowTitle(tr("Pole and zeros information"));

	QTextEdit *textEdit = new QTextEdit();
	textEdit->setReadOnly(true);

	textEdit->setPlainText(m_C_DispersionUser.getDetailsStr());

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(textEdit);
	dialog.setLayout(layout);

	// 尺寸微调
	dialog.resize(this->width(), this->height());
	dialog.exec();
}

/*-------------------------------------------------
		参数 - 设置材料名称
*/
void W_DispersionUser::setName(QString name)
{
	m_C_DispersionUser.setName(name);
}

W_DispersionUser::~W_DispersionUser()
{
	delete ui;
}
