#include "stdafx.h"
#include "S_StandardMaterial_MTDWriter.h"

/*
-----==========================================================-----
		类：		材料库文件 写入器.cpp
		作者：		drill_up
		所属模块：	材料模块
		功能：		在指定文件夹下的 材料文件 进行覆写。
-----==========================================================-----
*/

S_StandardMaterial_MTDWriter::S_StandardMaterial_MTDWriter(){
	
}
S_StandardMaterial_MTDWriter::~S_StandardMaterial_MTDWriter() {
}

/* --------------------------------------------------------------
----------单例
*/
S_StandardMaterial_MTDWriter* S_StandardMaterial_MTDWriter::cur_manager = NULL;
S_StandardMaterial_MTDWriter* S_StandardMaterial_MTDWriter::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_StandardMaterial_MTDWriter();
	}
	return cur_manager;
}

/* --------------------------------------------------------------
		写入器 - 写入到文件（单个）
*/
void S_StandardMaterial_MTDWriter::writeToFile_Single(C_StandardMaterial material, QFileInfo mtd_file, QString code){

	QString file_data = this->writeMaterial(material);
	QString file_path = mtd_file.absoluteFilePath();
	QString file_dir = mtd_file.absolutePath();

	// > 路径生成
	QDir dir_to(file_dir);
	if (dir_to.exists() == false) {
		dir_to.mkdir(file_dir);
	}

	// > 覆盖文件
	QFile file_to(file_path);
	if (file_to.exists()) { file_to.remove(); }

	// > 写入文件
	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return; }
	QByteArray ba;
	if (code == "toLocal8Bit") {			//windows本地字符
		ba = file_data.toLocal8Bit();
	}else if (code == "toLatin1") {		//ISO-8859-1编码，只有ASCII，不支持汉字
		ba = file_data.toLatin1();
	}else {
		ba = file_data.toUtf8();
	}
	file_to.write(ba);
	file_to.close();
}
/* --------------------------------------------------------------
		写入器 - 写入到文件（多个）
*/
void S_StandardMaterial_MTDWriter::writeToFile_Batch(QList<C_StandardMaterial> material_list, QDir mtd_dir, QString code){
	for (int i = 0; i < material_list.count(); i++) {
		C_StandardMaterial material = material_list.at(i);
		if (material.isNull()) { continue; }
		QString file_path = mtd_dir.absoluteFilePath(material.getName()+".mtd");
		this->writeToFile_Single(material, file_path, code);
	}
}

/* --------------------------------------------------------------
		写入器 - 输出文本
*/
QString S_StandardMaterial_MTDWriter::writeMaterial(C_StandardMaterial material){
	QString context;

	// > 写入 [Definition]
	context.append("[Definition]");
	context.append("\n");

	// > 参数生成 - 频率类型
	context.append(".FrqType ");
	context.append("\"" + material.getFrqType() + "\"");
	context.append("\n");
	context.append(".Type ");
	context.append("\"" + material.getType() + "\"");
	context.append("\n");

	// > 参数生成 - 长度单位
	//context.append(".MaterialUnit ");
	//context.append("\"" + material.getType() + "\"");
	//context.append("\n");

	// > 参数生成 - 参数
	context.append(".Epsilon ");
	context.append("\"" + QString::number(material.Epsilon) + "\"");
	context.append("\n");
	context.append(".Mu ");
	context.append("\"" + QString::number(material.Mu) + "\"");
	context.append("\n");
	context.append(".Rho ");
	context.append("\"" + QString::number(material.Rho) + "\"");
	context.append("\n");
	context.append(".ThermalType ");
	context.append("\"" + material.ThermalType + "\"");
	context.append("\n");
	context.append(".ThermalConductivity ");
	context.append("\"" + QString::number(material.ThermalConductivity) + "\"");
	context.append("\n");
	context.append(".DynamicViscosity ");
	context.append("\"" + QString::number(material.DynamicViscosity) + "\"");
	context.append("\n");
	context.append(".HeatCapacity ");
	context.append("\"" + QString::number(material.HeatCapacity) + "\"");
	context.append("\n");

	// > 参数生成 - 颜色/线/透明度
	context.append(".Colour ");
	context.append(material.getMtdDescription_Color());
	context.append("\n");
	//context.append(".Wireframe ");
	//context.append(".Reflection ");
	//context.append(".Allowoutline ");
	//context.append(".Transparentoutline ");
	//context.append("\"False\"");
	//context.append("\n");
	context.append(".Transparency ");
	context.append(material.getMtdDescription_Transparency());
	context.append("\n");

	context.append(".Create ");
	context.append("\n");
	context.append("\n");


	// > 写入 [Type]
	QString str_Type = material.getType();
	if (str_Type.isEmpty() == false){
		context.append("[Type]");
		context.append("\n");
		context.append("FrqType: ");
		context.append(material.getFrqType());
		context.append("\n");
		context.append(str_Type);
		context.append("\n");
		context.append("\n");
	}

	// > 写入 [Attributes]
	QString str_Attributes = material.getAttributesDesc();
	if (str_Attributes.isEmpty() == false){
		context.append("[Attributes]");
		context.append("\n");
		context.append("FrqType: ");
		context.append(material.getFrqType());
		context.append("\n");
		context.append(str_Attributes);
		context.append("\n");
		context.append("\n");
	}

	// > 写入 [Description]
	QString str_Description = material.getDescription();
	if (str_Description.isEmpty() == false){
		context.append("[Description]");
		context.append("\n");
		context.append(str_Description);
		context.append("\n");
		context.append("\n");
	}

	// > 写入 [Folder]
	QString str_Folder = material.getFolder();
	if (str_Folder.isEmpty() == false){
		context.append("[Folder]");
		context.append("\n");
		context.append(str_Folder);
		context.append("\n");
		context.append("\n");
	}

	qDebug() << context;
	return context;
}