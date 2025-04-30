#include "stdafx.h"
#include "S_StandardMaterial_MTDReader.h"

#include "Source/Utils/Manager/TxtRead/P_TxtFastReader.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		材料库文件 读取器.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		读取指定文件夹下的 材料文件，并返回材料数据类对象列表。
-----==========================================================-----
*/

S_StandardMaterial_MTDReader::S_StandardMaterial_MTDReader(){


	//// > 单一材料参数解析
	//QFile test_file("D:/Program Files (x86)/CST STUDIO SUITE 2019/Library/Materials/Air.mtd");
	//if (!test_file.open(QFile::ReadOnly)){
	//	QMessageBox::warning(nullptr, "错误", "无法打开材料文件。", QMessageBox::Yes);
	//}
	//QString test_text = test_file.readAll();
	//test_file.close();
	//C_StandardMaterial material = this->readFromText(test_text, "Air");


	//// > 从材料库中获取
	//QList<C_StandardMaterial> material_list = this->readFromDir(QDir("D:/Program Files (x86)/CST STUDIO SUITE 2019/Library/Materials"));


}
S_StandardMaterial_MTDReader::~S_StandardMaterial_MTDReader() {
}

/* --------------------------------------------------------------
----------单例
*/
S_StandardMaterial_MTDReader* S_StandardMaterial_MTDReader::cur_manager = NULL;
S_StandardMaterial_MTDReader* S_StandardMaterial_MTDReader::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_StandardMaterial_MTDReader();
	}
	return cur_manager;
}



/* --------------------------------------------------------------
		读取器 - 读取材料（单个）
*/
C_StandardMaterial S_StandardMaterial_MTDReader::readFromFile_Single(QFileInfo mtd_file){
	if (mtd_file.suffix().toLower() != "mtd"){ return C_StandardMaterial(); }

	// > 读取文件
	QFile file(mtd_file.absoluteFilePath());
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, tr("错误"), tr("未找到文件：")+mtd_file.absoluteFilePath(), QMessageBox::Yes);
		return C_StandardMaterial();
	}
	QString context = file.readAll();
	file.close();

	// > 解析文本（私有）
	C_StandardMaterial temp_class = this->readFromText(context, mtd_file.completeBaseName(), mtd_file.absolutePath());
	temp_class.init();		//（材料对象初始化）
	return temp_class;
}

/* --------------------------------------------------------------
		读取器 - 读取材料（多个）
*/
QList<C_StandardMaterial> S_StandardMaterial_MTDReader::readFromFile_Batch(QList<QFileInfo> mtd_file_list){

	QList<C_StandardMaterial> result_list;
	for (int i = 0; i < mtd_file_list.count(); i++){
		QFileInfo fileinfo = mtd_file_list.at(i);
		if (fileinfo.suffix().toLower() != "mtd"){ continue; }

		// > 读取文件
		QFile mtd_file(fileinfo.absoluteFilePath());
		if (!mtd_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			continue;
		}
		QString context = mtd_file.readAll();
		mtd_file.close();

		// > 解析文本（私有）
		C_StandardMaterial temp_class = this->readFromText(context, fileinfo.completeBaseName(), fileinfo.absolutePath());
		temp_class.init();		//（材料对象初始化）
		if (temp_class.isNull()){ continue; }
		result_list.append(temp_class);
	}
	return result_list;
}
/* --------------------------------------------------------------
		读取器 - 读取材料（全部）
*/
QList<C_StandardMaterial> S_StandardMaterial_MTDReader::readFromDir(QDir mtd_dir){
	mtd_dir.setFilter(QDir::Files | QDir::NoSymLinks);
	QList<QFileInfo> file_list = mtd_dir.entryInfoList();
	if (file_list.count() == 0){ return QList<C_StandardMaterial>(); }
	return this->readFromFile_Batch(file_list);
}


/* --------------------------------------------------------------
		读取器 - 解析文本
*/
C_StandardMaterial S_StandardMaterial_MTDReader::readFromText(QString context, QString material_name, QString material_location){
	C_StandardMaterial result;
	result.setLocation(material_location);
	P_TxtFastReader p_TxtFastReader(context);

	// > 找到 [Definition]
	int i_definition = p_TxtFastReader.d_indexOf(QRegExp("^\\[Definition\\]"));
	if (i_definition != -1){
		int i_definition_end = p_TxtFastReader.d_indexOf(QRegExp("^[^\\.].*"), i_definition + 1);
		if (i_definition_end != -1){
			i_definition_end -= 1;

			// > 行解析
			QStringList row_list = p_TxtFastReader.d_getRows(i_definition + 1, i_definition_end - i_definition);
			for (int i = 0; i < row_list.count(); i++){
				QString row = row_list.at(i);

				QStringList data_list = row.trimmed().split(QRegExp("[, ]?\"[, ]?"));	//根据双引号来分割
				if (data_list.count() >= 2){
					QString command = data_list.first();
					data_list.removeFirst();
					QStringList args = data_list;

					// > 去掉空行
					TTool::_QStringList_clearEmptyRows_(&args);

					// > 参数解析 - 频率类型
					if (command == ".FrqType"){ result.setFrqType( args.first() ); }
					if (command == ".Type"){ result.setType( args.first() ); }

					// > 参数解析 - 长度单位
					if (command == ".MaterialUnit"){  }
					if (command == ".SetMaterialUnit"){  }

					// > 参数解析 - 参数
					if (command == ".Epsilon"){ result.Epsilon = args.first().toDouble(); }
					if (command == ".Mu"){ result.Mu = args.first().toDouble(); }
					if (command == ".Rho"){ result.Rho = args.first().toDouble(); }
					if (command == ".ThermalType"){ result.ThermalType = args.first(); }
					if (command == ".ThermalConductivity"){ result.ThermalConductivity = args.first().toDouble(); }
					if (command == ".DynamicViscosity"){ result.DynamicViscosity = args.first().toDouble(); }
					if (command == ".HeatCapacity") { result.HeatCapacity = args.first().toDouble(); }

					// > 参数解析 - 颜色/线/透明度
					if (command == ".Colour") {
						if (args.count() >= 3) {
							result.setColor(QColor(args.at(0).toDouble() * 255, args.at(1).toDouble() * 255, args.at(2).toDouble() * 255));
						}
					}
					if (command == ".Transparency") { result.setTransparency(args.first().toInt()*0.01); }
				}
			}

		}
	}

	// > 找到 [Type]（不赋值）
	int i_type = p_TxtFastReader.d_lastIndexOf(QRegExp("^\\[Type\\]"));
	if (i_type != -1){
		int i_type_end = p_TxtFastReader.d_indexOf(QRegExp("(^[^a-zA-Z].*)|(^\\s*$)"), i_type + 1);
		if (i_type_end != -1){
			i_type_end -= 1;

			QStringList data_list = p_TxtFastReader.d_getRows(i_type + 1, i_type_end - i_type);
			if (data_list.count() > 0){
				//（材料参数 不赋值）
			}
		}
	}

	// > 找到 [Attributes]（不赋值）
	int i_attributes = p_TxtFastReader.d_lastIndexOf(QRegExp("^\\[Attributes\\]"));
	if (i_attributes != -1){
		int i_attributes_end = p_TxtFastReader.d_indexOf(QRegExp("(^[^a-zA-Z].*)|(^\\s*$)"), i_attributes + 1);
		if (i_attributes_end != -1){
			i_attributes_end -= 1;

			QStringList data_list = p_TxtFastReader.d_getRows(i_attributes + 1, i_attributes_end - i_attributes);
			if (data_list.count() > 0){
				data_list.removeFirst();	//去掉第一行
				//（材料参数 不赋值）
			}
		}
	}

	// > 找到 [Description]
	int i_description = p_TxtFastReader.d_indexOf(QRegExp("^\\[Description\\]"));
	if (i_description != -1){
		int i_description_end = p_TxtFastReader.d_indexOf(QRegExp("(^[^a-zA-Z].*)|(^\\s*$)"), i_description + 1);
		if (i_description_end != -1){
			i_description_end -= 1;

			QStringList data_list = p_TxtFastReader.d_getRows(i_description + 1, i_description_end - i_description);
			if (data_list.count() > 0){
				result.setDescription(data_list.join("\n"));
			}
		}
	}

	// > 找到 [Folder]
	int i_folder = p_TxtFastReader.d_indexOf(QRegExp("^\\[Folder\\]"));
	if (i_folder != -1){

		QStringList data_list = p_TxtFastReader.d_getRows(i_folder + 1);
		if (data_list.count() > 0){
			result.setName( data_list.at(0).trimmed() + "/" + material_name);
		}
	} else {
		result.setName(material_name);
	}
	
	return result;
}