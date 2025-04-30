#include "stdafx.h"
#include "W_StandardMaterial.h"

#include "DispersionFit/W_MaterialDispersionFit_Dielectric.h"
#include "DispersionFit/W_MaterialDispersionFit_Magnetic.h"
#include "Source/SettingModule/S_SettingDataContainer.h"
#include "Source/MaterialModule/S_MaterialDataContainer.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		材料 编辑窗口.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		材料数据类的编辑窗口。
-----==========================================================-----
*/

W_StandardMaterial::W_StandardMaterial(QWidget *parent)
	: W_DialogBase(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----ui初始化
	this->isEditingDefaultMaterial = false;
	this->isAddMode = true;
	this->m_typeBlock = false;
	ui.tabWidget->setCurrentIndex(0);

	// > 类型 - 材料
	ui.comboBox_type->clear();
	ui.comboBox_type->addItems(C_StandardMaterial::getAllType_Material());

	// > 类型 - 材料（选项Anisotropic，第3项不可用）
	#ifdef DSEMUI_Publish_Setting_20240327
		ui.comboBox_type->setItemData(2, QVariant(0), Qt::UserRole - 1);
	#endif

	// > 类型 - 电导率
	ui.comboBox_ElectricConductivity->clear();
	ui.comboBox_ElectricConductivity->addItems(C_StandardMaterial::getAllType_ElectricConductivity());
	ui.comboBox_DielectricDispersion->clear();
	ui.comboBox_DielectricDispersion->addItems(C_StandardMaterial::getAllType_DielectricDispersion());

	// > 类型 - 电导率 - 用户选项 2023-12-27 杨骁
	m_DielectricUserGroup = new QButtonGroup(this);
	m_DielectricUserGroup->addButton(ui.radioButton_DielectricDisp, 0);
	m_DielectricUserGroup->addButton(ui.radioButton_DielectricUser, 1);

	// > 类型 - 磁导率
	ui.comboBox_MagneticConductivity->clear();
	ui.comboBox_MagneticConductivity->addItems(C_StandardMaterial::getAllType_MagneticConductivity());
    ui.comboBox_MagneticDispersion->clear();
    ui.comboBox_MagneticDispersion->addItems(C_StandardMaterial::getAllType_MagneticDispersion());

    // > 类型 - 磁导率 - 用户选项 2023-12-27 杨骁
    m_MagneticUserGroup = new QButtonGroup(this);
    m_MagneticUserGroup->addButton(ui.radioButton_MagneticDisp, 0);
    m_MagneticUserGroup->addButton(ui.radioButton_MagneticUser, 1);
    m_MagneticUserGroup->button(local_StandardMaterial.m_MagneticUserOption)->setChecked(true);

	// > 类型 - 磁导率 隐藏 2024-8-29 杨骁
    ui.widget_mag->hide();

	// > 值同步
	this->m_P_QLineEditValueSynchronizer_El_1 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_1->setMain_Widget(ui.lineEdit_ElEpsilonInfinity);
	this->m_P_QLineEditValueSynchronizer_El_1->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElEpsilonInfinity_2 << ui.lineEdit_ElEpsilonInfinity_3 << ui.lineEdit_ElEpsilonInfinity_4 << ui.lineEdit_ElEpsilonInfinity_5 << ui.lineEdit_ElEpsilonInfinity_6 << ui.lineEdit_ElEpsilonInfinity_7 );
	this->m_P_QLineEditValueSynchronizer_El_2 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_2->setMain_Widget(ui.lineEdit_ElEpsilonStatic1);
	this->m_P_QLineEditValueSynchronizer_El_2->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElEpsilonStatic1_2 << ui.lineEdit_ElEpsilonStatic1_3);
	this->m_P_QLineEditValueSynchronizer_El_3 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_3->setMain_Widget(ui.lineEdit_ElRelaxationTime1);
	this->m_P_QLineEditValueSynchronizer_El_3->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElRelaxationTime1_2 << ui.lineEdit_ElRelaxationTime1_3);
	this->m_P_QLineEditValueSynchronizer_El_4 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_4->setMain_Widget(ui.lineEdit_ElPlasmaFrequency);
	this->m_P_QLineEditValueSynchronizer_El_4->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElPlasmaFrequency_2);
	this->m_P_QLineEditValueSynchronizer_El_5 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_5->setMain_Widget(ui.lineEdit_ElCollisionFrequency);
	this->m_P_QLineEditValueSynchronizer_El_5->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElCollisionFrequency_2);
	this->m_P_QLineEditValueSynchronizer_El_6 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_6->setMain_Widget(ui.lineEdit_ElAlpha0);
	this->m_P_QLineEditValueSynchronizer_El_6->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElAlpha0_2);
	this->m_P_QLineEditValueSynchronizer_El_7 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_7->setMain_Widget(ui.lineEdit_ElBeta0);
	this->m_P_QLineEditValueSynchronizer_El_7->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElBeta0_2);
	this->m_P_QLineEditValueSynchronizer_El_8 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_8->setMain_Widget(ui.lineEdit_ElResonanceFrequency);
	this->m_P_QLineEditValueSynchronizer_El_8->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElResonanceFrequency_2);
	this->m_P_QLineEditValueSynchronizer_El_9 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_9->setMain_Widget(ui.lineEdit_ElDampingFrequency);
	this->m_P_QLineEditValueSynchronizer_El_9->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElDampingFrequency_2);
	this->m_P_QLineEditValueSynchronizer_El_10 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_10->setMain_Widget(ui.lineEdit_ElChi3Infinity);
	this->m_P_QLineEditValueSynchronizer_El_10->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElChi3Infinity_2);
	this->m_P_QLineEditValueSynchronizer_El_11 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_11->setMain_Widget(ui.lineEdit_ElChi3Static);
	this->m_P_QLineEditValueSynchronizer_El_11->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElChi3Static_2);
	this->m_P_QLineEditValueSynchronizer_El_12 = new P_QLineEditValueSynchronizer();
	this->m_P_QLineEditValueSynchronizer_El_12->setMain_Widget(ui.lineEdit_EleConductivity);	//（EleConductivity和ElConductivity 是同一个参数，但存储了两个变量值而已）
	this->m_P_QLineEditValueSynchronizer_El_12->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_ElConductivity);

	this->m_P_QLineEditValueSynchronizer_Ma_1 = new P_QLineEditValueSynchronizer();
    this->m_P_QLineEditValueSynchronizer_Ma_1->setMain_Widget(ui.lineEdit_MaMuInfinity);
    this->m_P_QLineEditValueSynchronizer_Ma_1->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_MaMuInfinity_2 << ui.lineEdit_MaMuInfinity_3 << ui.lineEdit_MaMuInfinity_4 << ui.lineEdit_MaMuInfinity_5 << ui.lineEdit_MaMuInfinity_6 << ui.lineEdit_MaMuInfinity_7);
	this->m_P_QLineEditValueSynchronizer_Ma_2 = new P_QLineEditValueSynchronizer();
    this->m_P_QLineEditValueSynchronizer_Ma_2->setMain_Widget(ui.lineEdit_MaMuStatic1);
    this->m_P_QLineEditValueSynchronizer_Ma_2->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_MaMuStatic1_2 << ui.lineEdit_MaMuStatic1_3);
	this->m_P_QLineEditValueSynchronizer_Ma_3 = new P_QLineEditValueSynchronizer();
    this->m_P_QLineEditValueSynchronizer_Ma_3->setMain_Widget(ui.lineEdit_MaRelaxationTime1);
    this->m_P_QLineEditValueSynchronizer_Ma_3->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_MaRelaxationTime1_2 << ui.lineEdit_MaRelaxationTime1_3);
	this->m_P_QLineEditValueSynchronizer_Ma_6 = new P_QLineEditValueSynchronizer();
    this->m_P_QLineEditValueSynchronizer_Ma_6->setMain_Widget(ui.lineEdit_MaAlpha0);
    this->m_P_QLineEditValueSynchronizer_Ma_6->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_MaAlpha0_2);
	this->m_P_QLineEditValueSynchronizer_Ma_7 = new P_QLineEditValueSynchronizer();
    this->m_P_QLineEditValueSynchronizer_Ma_7->setMain_Widget(ui.lineEdit_MaBeta0);
    this->m_P_QLineEditValueSynchronizer_Ma_7->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_MaBeta0_2);
	this->m_P_QLineEditValueSynchronizer_Ma_8 = new P_QLineEditValueSynchronizer();
    this->m_P_QLineEditValueSynchronizer_Ma_8->setMain_Widget(ui.lineEdit_MaResonanceFrequency);
    this->m_P_QLineEditValueSynchronizer_Ma_8->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_MaResonanceFrequency_2);
	this->m_P_QLineEditValueSynchronizer_Ma_9 = new P_QLineEditValueSynchronizer();
    this->m_P_QLineEditValueSynchronizer_Ma_9->setMain_Widget(ui.lineEdit_MaDampingFrequency);
    this->m_P_QLineEditValueSynchronizer_Ma_9->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_MaDampingFrequency_2);
	this->m_P_QLineEditValueSynchronizer_Ma_10 = new P_QLineEditValueSynchronizer();
    this->m_P_QLineEditValueSynchronizer_Ma_10->setMain_Widget(ui.lineEdit_MaChi3Infinity);
    this->m_P_QLineEditValueSynchronizer_Ma_10->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_MaChi3Infinity_2);
	this->m_P_QLineEditValueSynchronizer_Ma_11 = new P_QLineEditValueSynchronizer();
    this->m_P_QLineEditValueSynchronizer_Ma_11->setMain_Widget(ui.lineEdit_MaChi3Static);
    this->m_P_QLineEditValueSynchronizer_Ma_11->setChild_Widget(QList<QLineEdit*>() << ui.lineEdit_MaChi3Static_2);

	
	//-----------------------------------
	//----校验绑定
	this->setDataInAddMode();
	this->initDialogButtonBox(ui.buttonBox);

	//-----------------------------------
	//----事件绑定
	connect(ui.comboBox_type, &QComboBox::currentTextChanged, this, &W_StandardMaterial::typeChanged_Material);
	connect(ui.comboBox_ElectricConductivity, &QComboBox::currentTextChanged, this, &W_StandardMaterial::typeChanged_ElectricConductivity);
	connect(ui.comboBox_DielectricDispersion, &QComboBox::currentTextChanged, this, &W_StandardMaterial::typeChanged_DielectricDispersion);
	connect(ui.comboBox_MagneticConductivity, &QComboBox::currentTextChanged, this, &W_StandardMaterial::typeChanged_MagneticConductivity);
    connect(ui.comboBox_MagneticDispersion, &QComboBox::currentTextChanged, this, &W_StandardMaterial::typeChanged_MagneticDispersion);
	connect(ui.pushButton_ElDispersionList, &QToolButton::clicked, this, &W_StandardMaterial::openWindowDispersionFit_Dielectric);
    connect(ui.pushButton_MaDispersionList, &QToolButton::clicked, this, &W_StandardMaterial::openWindowDispersionFit_Magnetic);
	connect(ui.toolButton_color, &QToolButton::clicked, this, &W_StandardMaterial::editColor);

	connect(m_DielectricUserGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &W_StandardMaterial::onDielectricUserOption);
	connect(m_MagneticUserGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &W_StandardMaterial::onMagneticUserOption);

	connect(ui.pushButton_DispersionUser, &QPushButton::clicked, this, &W_StandardMaterial::onOpenDispersionUserWindow);

	// > 刷新控件
	this->typeChanged_Material();
	this->typeChanged_DielectricDispersion();
	this->typeChanged_ElectricConductivity();
	this->typeChanged_MagneticDispersion();
	this->typeChanged_MagneticConductivity();
	this->refreshFrequencyValue();
}
W_StandardMaterial::~W_StandardMaterial(){
}



/*-------------------------------------------------
		控件 - 获取自动命名
*/
QString W_StandardMaterial::getAutoName() {
	QString result_name;
	int temp_id = 1;
	while (true) {
		result_name = tr("material") + QString::number(temp_id);
		if (S_MaterialDataContainer::getInstance()->hasName(result_name) == false) {
			break;
		}else {
			temp_id++; continue;
		}
	}
	return result_name;
}
/*-------------------------------------------------
		控件 - 类型变化 材料
*/
void W_StandardMaterial::typeChanged_Material() {
	QString cur_type = ui.comboBox_type->currentText();
	if (cur_type == "PEC") {
		ui.stackedWidget->setCurrentIndex(0);
	}else if (cur_type == "Normal") {
		ui.stackedWidget->setCurrentIndex(1);
	}else if (cur_type == "Anisotropic") {
		ui.stackedWidget->setCurrentIndex(2);
	}else if (cur_type == "Lossy metal") {
		ui.stackedWidget->setCurrentIndex(3);
	}
}
/*-------------------------------------------------
		控件 - 类型变化 电导率
*/
void W_StandardMaterial::typeChanged_ElectricConductivity() {
	if (this->m_typeBlock == true) { return; }
	this->m_typeBlock = true;
	QString cur_type = ui.comboBox_ElectricConductivity->currentText();
	if (cur_type == "None" ) {
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "El.conductivity") {
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(1);
		ui.comboBox_DielectricDispersion->setCurrentText("None");
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(0);

	}else if (cur_type == "Tangent delta el") {
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(2);
		ui.comboBox_DielectricDispersion->setCurrentText("None");
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(0);

	}
	this->m_typeBlock = false;
}
void W_StandardMaterial::typeChanged_DielectricDispersion() {
	if (this->m_typeBlock == true) { return; }
	this->m_typeBlock = true;
	QString cur_type = ui.comboBox_DielectricDispersion->currentText();
	if (cur_type == "None" ) {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(0);

	}else if (cur_type == "Debye 1st order") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(1);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "Debye 2nd order") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(2);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "Drude") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(3);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "Lorentz") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(4);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "Gyrotropic") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(5);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "General 1st order") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(6);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "General 2nd order") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(7);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "Nonlinear 2nd order") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(8);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "Nonlinear 3rd order") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(9);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "Nonlinear Kerr") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(10);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "Nonlinear Raman") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(11);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}else if (cur_type == "User") {
		ui.stackedWidget_DielectricDispersion->setCurrentIndex(12);
		ui.comboBox_ElectricConductivity->setCurrentText("None");
		ui.stackedWidget_ElectricConductivity->setCurrentIndex(0);

	}
	this->m_typeBlock = false;
}
/*-------------------------------------------------
		控件 - 类型变化 磁导率
*/
void W_StandardMaterial::typeChanged_MagneticConductivity() {
	if (this->m_typeBlock == true) { return; }
	this->m_typeBlock = true;
	QString cur_type = ui.comboBox_MagneticConductivity->currentText();
	if (cur_type == "None" ) {
		ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

	}else if (cur_type == "Mag.conductivity") {
		ui.stackedWidget_MagneticConductivity->setCurrentIndex(1);
        ui.comboBox_MagneticDispersion->setCurrentText("None");
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(0);

	}else if (cur_type == "Tangent delta mag") {
		ui.stackedWidget_MagneticConductivity->setCurrentIndex(2);
        ui.comboBox_MagneticDispersion->setCurrentText("None");
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(0);

	}
	this->m_typeBlock = false;
}
void W_StandardMaterial::typeChanged_MagneticDispersion() {
	if (this->m_typeBlock == true) { return; }
	this->m_typeBlock = true;
    QString cur_type = ui.comboBox_MagneticDispersion->currentText();
    if (cur_type == "None" ) {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(0);

    }else if (cur_type == "Debye 1st order") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(1);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "Debye 2nd order") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(2);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "Drude") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(3);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "Lorentz") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(4);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "Gyrotropic") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(5);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "General 1st order") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(6);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "General 2nd order") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(7);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "Nonlinear 2nd order") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(8);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "Nonlinear 3rd order") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(9);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "Nonlinear Kerr") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(10);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "Nonlinear Raman") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(11);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }else if (cur_type == "User") {
        ui.stackedWidget_MagneticDispersion->setCurrentIndex(12);
        ui.comboBox_MagneticConductivity->setCurrentText("None");
        ui.stackedWidget_MagneticConductivity->setCurrentIndex(0);

    }
	this->m_typeBlock = false;
}
/*-------------------------------------------------
		控件 - 刷新频率值
*/
void W_StandardMaterial::refreshFrequencyValue() {

	// > 【频率值】-临时只读（不转单位）
	//		（UI显示频率值）
	C_SettingFrequency freq = S_SettingDataContainer::getInstance()->getDataFrequency();
	ui.lineEdit_Fmin->setText(QString::number(freq.getFreqMin()));
	ui.lineEdit_Fmax->setText(QString::number(freq.getFreqMax()));

	// > 【频率单位】-临时只读
	//		（UI显示频率单位）
	C_SettingUnits unit = S_SettingDataContainer::getInstance()->getDataUnits();
	ui.label_FminUnit->setText(unit.getCurrentString_frequency());
	ui.label_FmaxUnit->setText(unit.getCurrentString_frequency());
}
/*-------------------------------------------------
		控件 - 编辑颜色
*/
void W_StandardMaterial::editColor() {
	QColorDialog d;
	d.setCurrentColor(local_StandardMaterial.getColor());
	if (d.exec() == QDialog::Accepted) {
		local_StandardMaterial.setColor( d.currentColor() );
		ui.toolButton_color->setIcon(local_StandardMaterial.getIcon_Color());
	}
}

/*-------------------------------------------------
		控件 - 离散拟合 - 电导率
*/
void W_StandardMaterial::openWindowDispersionFit_Dielectric() {

	W_MaterialDispersionFit_Dielectric d(this);
	C_MaterialDispersionFit_Dielectric data = this->local_StandardMaterial.dielectricFit;
	if (data.isNull()) {
		d.setDataInAddMode();
	}else {
		d.setDataInModifyMode(data);
	}
	if (d.exec() == QDialog::Accepted) {
		this->local_StandardMaterial.dielectricFit = d.getData();
	}
}
/*-------------------------------------------------
		控件 - 离散拟合 - 磁导率
*/
void W_StandardMaterial::openWindowDispersionFit_Magnetic() {

	W_MaterialDispersionFit_Magnetic d(this);
	C_MaterialDispersionFit_Magnetic data = this->local_StandardMaterial.magneticFit;
	if (data.isNull()) {
		d.setDataInAddMode();
	}
	else {
		d.setDataInModifyMode(data);
	}
	if (d.exec() == QDialog::Accepted) {
		this->local_StandardMaterial.magneticFit = d.getData();
	}
}

/*-------------------------------------------------
		控件 - 电导率 - 用户选项 2023-12-27 杨骁
*/
void W_StandardMaterial::onDielectricUserOption(int index)
{
	ui.stackedWidget_DielectricUser->setCurrentIndex(index);
}

/*-------------------------------------------------
		控件 - 磁导率 - 用户选项 2023-12-27 杨骁
*/
void W_StandardMaterial::onMagneticUserOption(int index)
{
    ui.stackedWidget_MagneticUser->setCurrentIndex(index);
}

/*-------------------------------------------------
		用户输入子窗口 - 电导率 - 打开子窗口 2023-12-27 杨骁
*/
void W_StandardMaterial::onOpenDispersionUserWindow()
{
	m_W_DispersionUser = new W_DispersionUser(local_StandardMaterial.m_C_DispersionUser);
	
	// 用户自定义色散列表点击apply同步给当前窗口
	connect(m_W_DispersionUser, &W_DispersionUser::signal_clickApply, this, &W_StandardMaterial::onApplyData);

	m_W_DispersionUser->setName(ui.lineEdit_name->text());
	m_W_DispersionUser->exec();
	// this->hide();
}

/*-------------------------------------------------
		窗口 - 设置数据（添加）
*/
void W_StandardMaterial::setDataInAddMode() {
	this->local_StandardMaterial = C_StandardMaterial();
	this->local_StandardMaterial.init();
	this->isAddMode = true;
	this->isEditingDefaultMaterial = false;
	this->putDataToUi();

	// > 自动命名
	ui.lineEdit_name->setText(this->getAutoName());
}

/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_StandardMaterial::setDataInModifyMode(C_StandardMaterial p) {
	this->local_StandardMaterial = p;
	this->isAddMode = false;
	this->isEditingDefaultMaterial = false;

	// 编辑的时候不可以修改名称 2024-1-8 杨骁
	ui.lineEdit_name->setDisabled(true);
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 设置默认材料数据（修改）
*/
void W_StandardMaterial::setDefaultDataInModifyMode(C_StandardMaterial p) {
	this->local_StandardMaterial = p;
	this->isAddMode = false;
	this->isEditingDefaultMaterial = true;
	this->putDataToUi();

	ui.widget_prop->setEnabled(false);
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_StandardMaterial W_StandardMaterial::getData(){
	return this->local_StandardMaterial;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_StandardMaterial::putDataToUi() {
	
	// > 属性
	ui.lineEdit_name->setText(this->local_StandardMaterial.getMinName());
	ui.comboBox_type->setCurrentText(this->local_StandardMaterial.getType());


	// > 参数（基本参数）
	TTool::_double_(ui.lineEdit_Epsilon, &this->local_StandardMaterial.Epsilon );
	TTool::_double_(ui.lineEdit_Mu, &this->local_StandardMaterial.Mu );

	// > 参数（有耗金属基本参数）
	TTool::_double_(ui.lineEdit_EleConductivity, &this->local_StandardMaterial.ElConductivity);
	TTool::_double_(ui.lineEdit_Mu_2, &this->local_StandardMaterial.MuLossyMetal);
	TTool::_double_(ui.lineEdit_SurfaceRoughness, &this->local_StandardMaterial.SurfaceRoughness);

	// > 参数（各项异性 Anisotropic）
	TTool::_double_(ui.lineEdit_EpsilonX, &this->local_StandardMaterial.EpsilonX);
	TTool::_double_(ui.lineEdit_EpsilonY, &this->local_StandardMaterial.EpsilonY);
	TTool::_double_(ui.lineEdit_EpsilonZ, &this->local_StandardMaterial.EpsilonZ);
	TTool::_double_(ui.lineEdit_MuX, &this->local_StandardMaterial.MuX);
	TTool::_double_(ui.lineEdit_MuY, &this->local_StandardMaterial.MuY);
	TTool::_double_(ui.lineEdit_MuZ, &this->local_StandardMaterial.MuZ);

	// > 参数（电导率/磁导率）
	m_DielectricUserGroup->button(local_StandardMaterial.m_DielectricUserOption)->setChecked(true);
	ui.stackedWidget_DielectricUser->setCurrentIndex(local_StandardMaterial.m_DielectricUserOption);

	m_MagneticUserGroup->button(local_StandardMaterial.m_MagneticUserOption)->setChecked(true);
    ui.stackedWidget_MagneticUser->setCurrentIndex(local_StandardMaterial.m_MagneticUserOption);

	ui.comboBox_ElectricConductivity->setCurrentText(this->local_StandardMaterial.ElType);	//第二页的电导率
	ui.comboBox_DielectricDispersion->setCurrentText(this->local_StandardMaterial.ElType);	//第三页的电导率
	this->typeChanged_DielectricDispersion();
	this->typeChanged_ElectricConductivity();
	ui.comboBox_MagneticConductivity->setCurrentText(this->local_StandardMaterial.MaType);	//第二页的磁导率
    ui.comboBox_MagneticDispersion->setCurrentText(this->local_StandardMaterial.MaType);	//第三页的磁导率
	this->typeChanged_MagneticDispersion();
	this->typeChanged_MagneticConductivity();

	TTool::_double_(ui.lineEdit_ElConductivity, &this->local_StandardMaterial.ElConductivity);			//（Electric conductivity）
	TTool::_double_(ui.lineEdit_ElDeltaEl, &this->local_StandardMaterial.ElDeltaEl);
	TTool::_double_(ui.lineEdit_ElDeltaElAtFrequency, &this->local_StandardMaterial.ElDeltaElAtFrequency);
	TTool::_double_(ui.lineEdit_MaConductivity, &this->local_StandardMaterial.MaConductivity);
	TTool::_double_(ui.lineEdit_MaDeltaMag, &this->local_StandardMaterial.MaDeltaMag);
	TTool::_double_(ui.lineEdit_MaDeltaMagAtFrequency, &this->local_StandardMaterial.MaDeltaMagAtFrequency);

	TTool::_double_(ui.lineEdit_ElEpsilonInfinity, &this->local_StandardMaterial.ElEpsilonInfinity);		//（Debye 1st/2st order）
	TTool::_double_(ui.lineEdit_ElEpsilonStatic1, &this->local_StandardMaterial.ElEpsilonStatic1);
	TTool::_double_(ui.lineEdit_ElEpsilonStatic2_2, &this->local_StandardMaterial.ElEpsilonStatic2);
	TTool::_double_(ui.lineEdit_ElRelaxationTime1, &this->local_StandardMaterial.ElRelaxationTime1);
	TTool::_double_(ui.lineEdit_ElRelaxationTime2_2, &this->local_StandardMaterial.ElRelaxationTime2);
	TTool::_double_(ui.lineEdit_MaMuInfinity, &this->local_StandardMaterial.MaMuInfinity);
	TTool::_double_(ui.lineEdit_MaMuStatic1, &this->local_StandardMaterial.MaMuStatic1);
	TTool::_double_(ui.lineEdit_MaMuStatic2, &this->local_StandardMaterial.MaMuStatic2);
	TTool::_double_(ui.lineEdit_MaRelaxationTime1, &this->local_StandardMaterial.MaRelaxationTime1);
	TTool::_double_(ui.lineEdit_MaRelaxationTime2, &this->local_StandardMaterial.MaRelaxationTime2);

	TTool::_double_(ui.lineEdit_ElPlasmaFrequency, &this->local_StandardMaterial.ElPlasmaFrequency);		//（Drude）
	TTool::_double_(ui.lineEdit_ElCollisionFrequency, &this->local_StandardMaterial.ElCollisionFrequency);
	TTool::_double_(ui.lineEdit_ElFieldBreakdown, &this->local_StandardMaterial.ElFieldBreakdown);
	TTool::_double_(ui.lineEdit_ElPlasmaMaintainFrequency, &this->local_StandardMaterial.ElPlasmaMaintainFrequency);
	TTool::_double_(ui.lineEdit_MaPlasmaFrequency, &this->local_StandardMaterial.MaPlasmaFrequency);
	TTool::_double_(ui.lineEdit_MaCollisionFrequency, &this->local_StandardMaterial.MaCollisionFrequency);

	TTool::_double_(ui.lineEdit_ElResonanceFrequency, &this->local_StandardMaterial.ElResonanceFrequency);	//（Lorentz）
	TTool::_double_(ui.lineEdit_ElDampingFrequency, &this->local_StandardMaterial.ElDampingFrequency);
	TTool::_double_(ui.lineEdit_MaResonanceFrequency, &this->local_StandardMaterial.MaResonanceFrequency);
	TTool::_double_(ui.lineEdit_MaDampingFrequency, &this->local_StandardMaterial.MaDampingFrequency);

	TTool::_double_(ui.lineEdit_ElCyclotronFrequency, &this->local_StandardMaterial.ElCyclotronFrequency);	//（Gyrotropic）
	TTool::_double_(ui.lineEdit_ElBiasingDirectionX, &this->local_StandardMaterial.ElBiasingDirectionX);
	TTool::_double_(ui.lineEdit_ElBiasingDirectionY, &this->local_StandardMaterial.ElBiasingDirectionY);
	TTool::_double_(ui.lineEdit_ElBiasingDirectionZ, &this->local_StandardMaterial.ElBiasingDirectionZ);
	TTool::_double_(ui.lineEdit_MaLandFactor, &this->local_StandardMaterial.MaLandFactor);
	TTool::_double_(ui.lineEdit_MaSatMagnetization, &this->local_StandardMaterial.MaSatMagnetization);
	TTool::_double_(ui.lineEdit_MaResonanceLineWidth, &this->local_StandardMaterial.MaResonanceLineWidth);
	TTool::_double_(ui.lineEdit_MaFieldVectorX, &this->local_StandardMaterial.MaFieldVectorX);
	TTool::_double_(ui.lineEdit_MaFieldVectorY, &this->local_StandardMaterial.MaFieldVectorY);
	TTool::_double_(ui.lineEdit_MaFieldVectorZ, &this->local_StandardMaterial.MaFieldVectorZ);

	TTool::_double_(ui.lineEdit_ElAlpha0, &this->local_StandardMaterial.ElAlpha0);							//（General 1st/2st order）
	TTool::_double_(ui.lineEdit_ElBeta0, &this->local_StandardMaterial.ElBeta0);
	TTool::_double_(ui.lineEdit_ElAlpha1, &this->local_StandardMaterial.ElAlpha1);
	TTool::_double_(ui.lineEdit_ElBeta1, &this->local_StandardMaterial.ElBeta1);
	TTool::_double_(ui.lineEdit_MaAlpha0, &this->local_StandardMaterial.MaAlpha0);
	TTool::_double_(ui.lineEdit_MaBeta0, &this->local_StandardMaterial.MaBeta0);
	TTool::_double_(ui.lineEdit_MaAlpha1, &this->local_StandardMaterial.MaAlpha1);
	TTool::_double_(ui.lineEdit_MaBeta1, &this->local_StandardMaterial.MaBeta1);

	TTool::_double_(ui.lineEdit_ElChi2, &this->local_StandardMaterial.ElChi2);								//（Nonlinear 2nd/3rd order）
	TTool::_double_(ui.lineEdit_ElChi3, &this->local_StandardMaterial.ElChi3);
	TTool::_double_(ui.lineEdit_MaChi2, &this->local_StandardMaterial.MaChi2);
	TTool::_double_(ui.lineEdit_MaChi3, &this->local_StandardMaterial.MaChi3);

	TTool::_double_(ui.lineEdit_ElChi3Infinity, &this->local_StandardMaterial.ElChi3Infinity);				//（Nonlinear Kerr）
	TTool::_double_(ui.lineEdit_ElChi3Static, &this->local_StandardMaterial.ElChi3Static);
	TTool::_double_(ui.lineEdit_MaChi3Infinity, &this->local_StandardMaterial.MaChi3Infinity);
	TTool::_double_(ui.lineEdit_MaChi3Static, &this->local_StandardMaterial.MaChi3Static);

	//（不需要传给UI值） //（User）


	// > 参数（热传导）
	//...

	// > 参数（密度）
	TTool::_double_(ui.lineEdit_Rho, &this->local_StandardMaterial.Rho);


	// > 颜色
	ui.toolButton_color->setIcon(local_StandardMaterial.getIcon_Color());
	ui.horizontalSlider_transparency->setValue(this->local_StandardMaterial.getTransparency_Per());

	// > 分类
	ui.comboBox_folder->setCurrentText(this->local_StandardMaterial.getFolder());

	// > 刷新控件
	this->typeChanged_Material();
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_StandardMaterial::putUiToData() {

	// > 属性
	QString cur_type = ui.comboBox_type->currentText();
	this->local_StandardMaterial.setType(cur_type);

	// > 名称
	QString folder = ui.comboBox_folder->currentText();
	if (folder == "") {
		this->local_StandardMaterial.setName(ui.lineEdit_name->text());
	}else {
		this->local_StandardMaterial.setName(folder + "/" + ui.lineEdit_name->text());
	}

	// > 参数（基本参数）
	TTool::_double_(&this->local_StandardMaterial.Epsilon, ui.lineEdit_Epsilon);
	TTool::_double_(&this->local_StandardMaterial.Mu, ui.lineEdit_Mu);

	// > 参数（有耗金属基本参数）
	TTool::_double_(&this->local_StandardMaterial.ElConductivity, ui.lineEdit_ElConductivity);
	TTool::_double_(&this->local_StandardMaterial.MuLossyMetal, ui.lineEdit_Mu_2);
	TTool::_double_(&this->local_StandardMaterial.SurfaceRoughness, ui.lineEdit_SurfaceRoughness);

	// > 参数（各项异性 Anisotropic）
	if (cur_type == "Anisotropic") {
		TTool::_double_(&this->local_StandardMaterial.EpsilonX, ui.lineEdit_EpsilonX);
		TTool::_double_(&this->local_StandardMaterial.EpsilonY, ui.lineEdit_EpsilonY);
		TTool::_double_(&this->local_StandardMaterial.EpsilonZ, ui.lineEdit_EpsilonZ);
		TTool::_double_(&this->local_StandardMaterial.MuX, ui.lineEdit_MuX);
		TTool::_double_(&this->local_StandardMaterial.MuY, ui.lineEdit_MuY);
		TTool::_double_(&this->local_StandardMaterial.MuZ, ui.lineEdit_MuZ);

	// > 参数（非各项异性 Anisotropic）
	}else {
		this->local_StandardMaterial.EpsilonX = this->local_StandardMaterial.Epsilon;
		this->local_StandardMaterial.EpsilonY = this->local_StandardMaterial.Epsilon;
		this->local_StandardMaterial.EpsilonZ = this->local_StandardMaterial.Epsilon;
		this->local_StandardMaterial.MuX = this->local_StandardMaterial.Mu;
		this->local_StandardMaterial.MuY = this->local_StandardMaterial.Mu;
		this->local_StandardMaterial.MuZ = this->local_StandardMaterial.Mu;
	}


	// > 参数（电导率/磁导率）
	local_StandardMaterial.m_DielectricUserOption = m_DielectricUserGroup->checkedId();
	local_StandardMaterial.m_MagneticUserOption = m_MagneticUserGroup->checkedId();

	QString cur_type1 = ui.comboBox_ElectricConductivity->currentText();	//第二页的电导率
	QString cur_type2 = ui.comboBox_DielectricDispersion->currentText();	//第三页的电导率
	if (cur_type1 != "" && cur_type1 != "None") {
		this->local_StandardMaterial.ElType = cur_type1;
	}else {
		this->local_StandardMaterial.ElType = cur_type2;
	}
	QString cur_type3 = ui.comboBox_MagneticConductivity->currentText();	//第二页的磁导率
	QString cur_type4 = ui.comboBox_MagneticDispersion->currentText();		//第三页的磁导率
	if (cur_type3 != "" && cur_type3 != "None") {
		this->local_StandardMaterial.MaType = cur_type3;
	}else {
		this->local_StandardMaterial.MaType = cur_type4;
	}

	TTool::_double_(&this->local_StandardMaterial.ElConductivity, ui.lineEdit_ElConductivity);			//（Electric conductivity）
	TTool::_double_(&this->local_StandardMaterial.ElDeltaEl, ui.lineEdit_ElDeltaEl);
	TTool::_double_(&this->local_StandardMaterial.ElDeltaElAtFrequency, ui.lineEdit_ElDeltaElAtFrequency);
	TTool::_double_(&this->local_StandardMaterial.MaConductivity, ui.lineEdit_MaConductivity);
	TTool::_double_(&this->local_StandardMaterial.MaDeltaMag, ui.lineEdit_MaDeltaMag);
	TTool::_double_(&this->local_StandardMaterial.MaDeltaMagAtFrequency, ui.lineEdit_MaDeltaMagAtFrequency);

	if (cur_type2 == "Debye 1st order")
	{
		TTool::_double_(&this->local_StandardMaterial.ElRelaxationTime1, ui.lineEdit_ElRelaxationTime1);
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonInfinity, ui.lineEdit_ElEpsilonInfinity);
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonStatic1, ui.lineEdit_ElEpsilonStatic1);
	}

	if (cur_type2 == "Debye 2nd order")
	{
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonStatic2, ui.lineEdit_ElEpsilonStatic2_2);
		TTool::_double_(&this->local_StandardMaterial.ElRelaxationTime1, ui.lineEdit_ElRelaxationTime1_2);
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonInfinity, ui.lineEdit_ElEpsilonInfinity_2);
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonStatic1, ui.lineEdit_ElEpsilonStatic1_2);
		TTool::_double_(&this->local_StandardMaterial.ElRelaxationTime2, ui.lineEdit_ElRelaxationTime2_2);
	}

	if (cur_type2 == "Drude")
	{
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonInfinity, ui.lineEdit_ElEpsilonInfinity_3);
		TTool::_double_(&this->local_StandardMaterial.ElFieldBreakdown, ui.lineEdit_ElFieldBreakdown);
		TTool::_double_(&this->local_StandardMaterial.ElPlasmaFrequency, ui.lineEdit_ElPlasmaFrequency);
		TTool::_double_(&this->local_StandardMaterial.ElCollisionFrequency, ui.lineEdit_ElCollisionFrequency);
		TTool::_double_(&this->local_StandardMaterial.ElPlasmaMaintainFrequency, ui.lineEdit_ElPlasmaMaintainFrequency);
	}

	if (cur_type2 == "Lorentz")
	{
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonStatic1, ui.lineEdit_ElEpsilonStatic1_3);
		TTool::_double_(&this->local_StandardMaterial.ElResonanceFrequency, ui.lineEdit_ElResonanceFrequency);
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonInfinity, ui.lineEdit_ElEpsilonInfinity_5);
		TTool::_double_(&this->local_StandardMaterial.ElDampingFrequency, ui.lineEdit_ElDampingFrequency);
	}

	if (cur_type2 == "Gyrotropic")
	{
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonInfinity, ui.lineEdit_ElEpsilonInfinity_4);
		TTool::_double_(&this->local_StandardMaterial.ElBiasingDirectionX, ui.lineEdit_ElBiasingDirectionX);
		TTool::_double_(&this->local_StandardMaterial.ElBiasingDirectionY, ui.lineEdit_ElBiasingDirectionY);
		TTool::_double_(&this->local_StandardMaterial.ElBiasingDirectionZ, ui.lineEdit_ElBiasingDirectionZ);
		TTool::_double_(&this->local_StandardMaterial.ElCyclotronFrequency, ui.lineEdit_ElCyclotronFrequency);
		TTool::_double_(&this->local_StandardMaterial.ElPlasmaFrequency, ui.lineEdit_ElPlasmaFrequency_2);
		TTool::_double_(&this->local_StandardMaterial.ElCollisionFrequency, ui.lineEdit_ElCollisionFrequency_2);
	}

	if (cur_type2 == "General 1st order")
	{
		TTool::_double_(&this->local_StandardMaterial.ElAlpha0, ui.lineEdit_ElAlpha0_2);
		TTool::_double_(&this->local_StandardMaterial.ElBeta0, ui.lineEdit_ElBeta0_2);
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonInfinity, ui.lineEdit_ElEpsilonInfinity_7);
	}

	if (cur_type2 == "General 2nd order")
	{
		TTool::_double_(&this->local_StandardMaterial.ElBeta1, ui.lineEdit_ElBeta1);
		TTool::_double_(&this->local_StandardMaterial.ElEpsilonInfinity, ui.lineEdit_ElEpsilonInfinity_6);
		TTool::_double_(&this->local_StandardMaterial.ElAlpha0, ui.lineEdit_ElAlpha0);
		TTool::_double_(&this->local_StandardMaterial.ElBeta0, ui.lineEdit_ElBeta0);
		TTool::_double_(&this->local_StandardMaterial.ElAlpha1, ui.lineEdit_ElAlpha1);
	}

	if (cur_type2 == "Nonlinear 2nd order")
	{
		TTool::_double_(&this->local_StandardMaterial.ElChi2, ui.lineEdit_ElChi2); // （Nonlinear 2nd/3rd order）
	}

	if (cur_type2 == "Nonlinear 3rd order")
	{
		TTool::_double_(&this->local_StandardMaterial.ElChi3, ui.lineEdit_ElChi3);
	}

	if (cur_type2 == "Nonlinear Kerr")
	{
		TTool::_double_(&this->local_StandardMaterial.ElChi3Infinity, ui.lineEdit_ElChi3Infinity); // （Nonlinear Kerr）
		TTool::_double_(&this->local_StandardMaterial.ElChi3Static, ui.lineEdit_ElChi3Static);
		TTool::_double_(&this->local_StandardMaterial.ElRelaxationTime1, ui.lineEdit_ElRelaxationTime1_3);
	}

	if (cur_type2 == "Nonlinear Raman")
	{
		TTool::_double_(&this->local_StandardMaterial.ElChi3Infinity, ui.lineEdit_ElChi3Infinity_2);
		TTool::_double_(&this->local_StandardMaterial.ElChi3Static, ui.lineEdit_ElChi3Static_2);
		TTool::_double_(&this->local_StandardMaterial.ElResonanceFrequency, ui.lineEdit_ElResonanceFrequency_2);
		TTool::_double_(&this->local_StandardMaterial.ElDampingFrequency, ui.lineEdit_ElDampingFrequency_2);
	}

	if (cur_type4 == "Debye 1st order")
	{
		TTool::_double_(&this->local_StandardMaterial.MaRelaxationTime1, ui.lineEdit_MaRelaxationTime1);
		TTool::_double_(&this->local_StandardMaterial.MaMuInfinity, ui.lineEdit_MaMuInfinity);
		TTool::_double_(&this->local_StandardMaterial.MaMuStatic1, ui.lineEdit_MaMuStatic1);
	}

	if (cur_type4 == "Debye 2nd order")
	{
		TTool::_double_(&this->local_StandardMaterial.MaMuStatic2, ui.lineEdit_MaMuStatic2);
		TTool::_double_(&this->local_StandardMaterial.MaRelaxationTime1, ui.lineEdit_MaRelaxationTime1_2);
		TTool::_double_(&this->local_StandardMaterial.MaMuInfinity, ui.lineEdit_MaMuInfinity_2);
		TTool::_double_(&this->local_StandardMaterial.MaMuStatic1, ui.lineEdit_MaMuStatic1_2);
		TTool::_double_(&this->local_StandardMaterial.MaRelaxationTime2, ui.lineEdit_MaRelaxationTime2);
	}

	if (cur_type4 == "Drude")
	{
		TTool::_double_(&this->local_StandardMaterial.MaMuInfinity, ui.lineEdit_MaMuInfinity_3);
		TTool::_double_(&this->local_StandardMaterial.MaPlasmaFrequency, ui.lineEdit_MaPlasmaFrequency);
		TTool::_double_(&this->local_StandardMaterial.MaCollisionFrequency, ui.lineEdit_MaCollisionFrequency);
	}

	if (cur_type4 == "Lorentz")
	{
		TTool::_double_(&this->local_StandardMaterial.MaMuStatic1, ui.lineEdit_MaMuStatic1_3);
		TTool::_double_(&this->local_StandardMaterial.MaResonanceFrequency, ui.lineEdit_MaResonanceFrequency);
		TTool::_double_(&this->local_StandardMaterial.MaMuInfinity, ui.lineEdit_MaMuInfinity_4);
		TTool::_double_(&this->local_StandardMaterial.MaDampingFrequency, ui.lineEdit_MaDampingFrequency);
	}

	if (cur_type4 == "Gyrotropic")
	{
		TTool::_double_(&this->local_StandardMaterial.MaMuInfinity, ui.lineEdit_MaMuInfinity_5);
		TTool::_double_(&this->local_StandardMaterial.MaFieldVectorX, ui.lineEdit_MaFieldVectorX);
		TTool::_double_(&this->local_StandardMaterial.MaFieldVectorY, ui.lineEdit_MaFieldVectorY);
		TTool::_double_(&this->local_StandardMaterial.MaFieldVectorZ, ui.lineEdit_MaFieldVectorZ);
		TTool::_double_(&this->local_StandardMaterial.MaResonanceLineWidth, ui.lineEdit_MaResonanceLineWidth);
		TTool::_double_(&this->local_StandardMaterial.MaLandFactor, ui.lineEdit_MaLandFactor);
		TTool::_double_(&this->local_StandardMaterial.MaSatMagnetization, ui.lineEdit_MaSatMagnetization);
	}

	if (cur_type4 == "General 1st order")
	{
		TTool::_double_(&this->local_StandardMaterial.MaAlpha0, ui.lineEdit_MaAlpha0);
		TTool::_double_(&this->local_StandardMaterial.MaBeta0, ui.lineEdit_MaBeta0);
		TTool::_double_(&this->local_StandardMaterial.MaMuInfinity, ui.lineEdit_MaMuInfinity_6);
	}

	if (cur_type4 == "General 2nd order")
	{
		TTool::_double_(&this->local_StandardMaterial.MaBeta1, ui.lineEdit_MaBeta1);
		TTool::_double_(&this->local_StandardMaterial.MaMuInfinity, ui.lineEdit_MaMuInfinity_7);
		TTool::_double_(&this->local_StandardMaterial.MaAlpha0, ui.lineEdit_MaAlpha0_2);
		TTool::_double_(&this->local_StandardMaterial.MaBeta0, ui.lineEdit_MaBeta0_2);
		TTool::_double_(&this->local_StandardMaterial.MaAlpha1, ui.lineEdit_MaAlpha1);
	}

	if (cur_type4 == "Nonlinear 2nd order")
	{
		TTool::_double_(&this->local_StandardMaterial.MaChi2, ui.lineEdit_MaChi2);
	}

	if (cur_type4 == "Nonlinear 3rd order")
	{
		TTool::_double_(&this->local_StandardMaterial.MaChi3, ui.lineEdit_MaChi3);
	}

	if (cur_type4 == "Nonlinear Kerr")
	{
		TTool::_double_(&this->local_StandardMaterial.MaChi3Infinity, ui.lineEdit_MaChi3Infinity);
		TTool::_double_(&this->local_StandardMaterial.MaChi3Static, ui.lineEdit_MaChi3Static);
		TTool::_double_(&this->local_StandardMaterial.MaRelaxationTime1, ui.lineEdit_MaRelaxationTime1_3);
	}

	if (cur_type4 == "Nonlinear Raman")
	{
		TTool::_double_(&this->local_StandardMaterial.MaChi3Infinity, ui.lineEdit_MaChi3Infinity_2);
		TTool::_double_(&this->local_StandardMaterial.MaChi3Static, ui.lineEdit_MaChi3Static_2);
		TTool::_double_(&this->local_StandardMaterial.MaResonanceFrequency, ui.lineEdit_MaResonanceFrequency_2);
		TTool::_double_(&this->local_StandardMaterial.MaDampingFrequency, ui.lineEdit_MaDampingFrequency_2);
	}

	//（通过新窗口直接编辑数据对象） //（User）


	// > 参数（热传导）
	//...

	// > 参数（密度）
	TTool::_double_(&this->local_StandardMaterial.Rho, ui.lineEdit_Rho);


	// > 颜色
	//（颜色直接通过对话框编辑）
	this->local_StandardMaterial.setTransparency_Per(ui.horizontalSlider_transparency->value());

	// > 分类
	//	（见名称）

};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_StandardMaterial::base_acceptData() {
	if (!m_DielectricUserApply){
		this->putUiToData();

		// > 新建校验
		QString name = this->local_StandardMaterial.getName();
		if (name == ""){
			QMessageBox::warning(this, tr("提示"), tr("名称不能为空。"));
			return;
		}
		if (this->isEditingDefaultMaterial == false && S_MaterialDataContainer::getInstance()->isDefaultMaterialName(name)){
			QMessageBox::warning(this, tr("提示"), tr("不能以默认材料的专有名称进行命名。"));
			return;
		}
		if (this->isAddMode == true && S_MaterialDataContainer::getInstance()->hasName(name)){
			QMessageBox::warning(this, tr("提示"), tr("已有此名称，不可重复。"));
			return;
		}
		
		// > 值校验
		if (this->local_StandardMaterial.getType() == "Normal") {
			if (this->local_StandardMaterial.Epsilon == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Epsilon") + tr("的值不能为零。"));
				return;
			}
			if (this->local_StandardMaterial.Mu == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Mu") + tr("的值不能为零。"));
				return;
			}
		}
		if (this->local_StandardMaterial.getType() == "Anisotropic") {
			if (this->local_StandardMaterial.EpsilonX == 0 ||
				this->local_StandardMaterial.EpsilonY == 0 ||
				this->local_StandardMaterial.EpsilonZ == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Epsilon") + tr("的值不能为零。"));
				return;
			}
			if (this->local_StandardMaterial.MuX == 0 ||
				this->local_StandardMaterial.MuY == 0 ||
				this->local_StandardMaterial.MuZ == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Mu") + tr("的值不能为零。"));
				return;
			}
		}
		if (this->local_StandardMaterial.getType() == "Lossy metal") {
			if (this->local_StandardMaterial.ElConductivity <= 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("电导率") + tr("的值必须大于零。"));
				return;
			}
			if (this->local_StandardMaterial.Mu == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Mu") + tr("的值不能为零。"));
				return;
			}
		}

		if (ui.comboBox_DielectricDispersion->currentIndex() > 0 && local_StandardMaterial.ElType != "None"){
			// 如果存在色散配置，并且已经配置了频率，要生成色散曲线 2024-8-28 杨骁
			local_StandardMaterial.genDispersionTxt(true);
		}

		emit acceptDataSignal(this->isAddMode, this->local_StandardMaterial);
	}

	W_DialogBase::base_acceptData();
};

/*-------------------------------------------------
		窗口 - 提交数据（校验）- 但是不关闭窗口
*/
void W_StandardMaterial::onApplyData(C_DispersionUser data)
{
	// 子窗口数据同步
	local_StandardMaterial.m_C_DispersionUser = data;

	// 当前窗口数据同步
	this->putUiToData();

	if (m_DielectricUserApply){
		// 修改
		this->isAddMode = false;
	}else{

		// > 新建校验
		QString name = this->local_StandardMaterial.getName();
		if (name == ""){
			QMessageBox::warning(this, tr("提示"), tr("名称不能为空。"));
			return;
		}
		if (this->isEditingDefaultMaterial == false && S_MaterialDataContainer::getInstance()->isDefaultMaterialName(name)){
			QMessageBox::warning(this, tr("提示"), tr("不能以默认材料的专有名称进行命名。"));
			return;
		}
		if (this->isAddMode == true && S_MaterialDataContainer::getInstance()->hasName(name)){
			QMessageBox::warning(this, tr("提示"), tr("已有此名称，不可重复。"));
			return;
		}

		// > 值校验
		if (this->local_StandardMaterial.getType() == "Normal") {
			if (this->local_StandardMaterial.Epsilon == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Epsilon") + tr("的值不能为零。"));
				return;
			}
			if (this->local_StandardMaterial.Mu == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Mu") + tr("的值不能为零。"));
				return;
			}
		}
		if (this->local_StandardMaterial.getType() == "Anisotropic") {
			if (this->local_StandardMaterial.EpsilonX == 0 ||
				this->local_StandardMaterial.EpsilonY == 0 ||
				this->local_StandardMaterial.EpsilonZ == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Epsilon") + tr("的值不能为零。"));
				return;
			}
			if (this->local_StandardMaterial.MuX == 0 ||
				this->local_StandardMaterial.MuY == 0 ||
				this->local_StandardMaterial.MuZ == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Mu") + tr("的值不能为零。"));
				return;
			}
		}
		if (this->local_StandardMaterial.getType() == "Lossy metal") {
			if (this->local_StandardMaterial.ElConductivity <= 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("电导率") + tr("的值必须大于零。"));
				return;
			}
			if (this->local_StandardMaterial.Mu == 0) {
				QMessageBox::warning(this, tr("提示"), tr("参数") + tr("Mu") + tr("的值不能为零。"));
				return;
			}
		}

		m_DielectricUserApply = true;
	}
	
	emit acceptDataSignal(this->isAddMode, this->local_StandardMaterial);
}

/*-------------------------------------------------
		窗口 - 子窗口提交数据同步到当前窗口数据类对应的变量中 2024-1-4 杨骁
*/
void W_StandardMaterial::onAcceptDispersionUserData(C_DispersionUser data)
{
	local_StandardMaterial.m_C_DispersionUser = data;
}

/*-------------------------------------------------
		窗口 - 取消数据
*/
void W_StandardMaterial::base_rejectData() {
	W_DialogBase::base_rejectData();
};

/*-------------------------------------------------
		窗口 - 关闭事件
*/
void W_StandardMaterial::closeEvent(QCloseEvent *e) {
	W_DialogBase::closeEvent(e);

};
