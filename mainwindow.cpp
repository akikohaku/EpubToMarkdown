#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"
#include "QDesktopServices"
#include "QXmlStreamReader"
#include <fstream>
#include "OSS.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString dir = QCoreApplication::applicationDirPath()+"/out/";
    golbal.SaveFile=dir;
    ui->lineEdit_2->setText(dir);
    QString command="md "+golbal.SaveFile.replace(QRegExp("/"),"\\");
    system(command.toLocal8Bit());
    std::locale::global(std::locale(""));
    QString str = "中文";
    std::cout << str.toLocal8Bit().data();
    QFile file(QCoreApplication::applicationDirPath()+"/OSS.config");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QList<QByteArray> temp=QList<QByteArray>();
        while (!file.atEnd()){
            temp.append(file.readLine());
            qDebug() << str;
        }
        ui->keyID->setText(temp.at(0).split('\n').at(0));
        ui->lineEdit_4->setText(temp.at(1).split('\n').at(0));
        ui->lineEdit_5->setText(temp.at(2).split('\n').at(0));
        ui->lineEdit_6->setText(temp.at(3).split('\n').at(0));
        ui->lineEdit_7->setText(temp.at(4).split('\n').at(0));
        file.close();
    }else{
        ui->textEdit->append("[System]:读取OSS文件失败,请先配置OSS");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_selectFile_clicked()
{
    golbal.SelectFile=QFileDialog::getOpenFileName(this,tr(""),"",tr("EPUB(*.epub)"));
    if(golbal.SelectFile==""){
        return;
    }
    ui->lineEdit->setText(golbal.SelectFile);
    golbal.isFileSelected=true;
    ui->textEdit->append("[System]:文件已选择");
}

void MainWindow::on_pushButton_clicked()
{
    if(!golbal.isFileSelected){
        ui->textEdit->append("[System]:需要先选择文件");
        return;

    }
    QString dir = QCoreApplication::applicationDirPath()+"/cache/";
    //unzip -o test.zip -d tmp/
    const QString command="unzip -o \""+golbal.SelectFile+"\" -d "+dir;
    ui->textEdit->append("[Command]:"+command);
    system(command.toLocal8Bit());
    ui->textEdit->append("[System]:解压完成");
    golbal.isUnzip=true;
}

void MainWindow::on_clearcache_clicked()
{
    QString dir = QCoreApplication::applicationDirPath()+"/cache/";
    QString command="rd /S /Q "+dir.replace(QRegExp("/"),"\\");
    ui->textEdit->append("[Command]:"+command);
    system(command.toLocal8Bit());
    command="md "+QCoreApplication::applicationDirPath().replace(QRegExp("/"),"\\")+"\\cache\\";
    system(command.toLocal8Bit());
    ui->textEdit->append("[System]:缓存已清空");
}

void MainWindow::on_readfile_clicked()
{
    if(!ui->useopf->isChecked()){
    QDir dir(QCoreApplication::applicationDirPath()+"/cache/OEBPS/Text/");

       if (!dir.exists()) {
           return;
       }

     //取到所有的文件和文件名，但是去掉.和..的文件夹（这是QT默认有的）
       dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

       //文件夹优先
       dir.setSorting(QDir::DirsFirst);

       //转化成一个list
       QFileInfoList list = dir.entryInfoList();
       QStringList infolist = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
       if(list.size()< 1 ) {
           return;
       }
       int i=0;

       //递归算法的核心部分
       do{
           QFileInfo fileInfo = list.at(i);
               for(int m = 0; m <infolist.size(); m++) {
                                   //这里是获取当前要处理的文件名
                   //qDebug() << infolist.at(m);
                   ui->textEdit->append("[System]:添加文件"+infolist.at(m));
                   golbal.fileName.append(infolist.at(m));
               }
               break;
           //end else
           i++;
       } while(i < list.size());
    }else if(!ui->connectedopf->isChecked()){
        QFile file(QCoreApplication::applicationDirPath()+"/cache/OEBPS/content.opf");

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QXmlStreamReader reader(&file);
            ui->textEdit->append("[System]:解析opf文件中 ");
            while(!reader.atEnd()){
                reader.readNextStartElement();
                QString strElementName = reader.name().toString();
                if(QString::compare(strElementName,"itemref")==0){
                    QXmlStreamAttributes attributes = reader.attributes();
                    if (attributes.hasAttribute("idref")) {
                       QString strSRC = attributes.value("idref").toString();
                       ui->textEdit->append("[System]:添加文件"+strSRC);
                       golbal.fileName.append(strSRC);
                    }
                }
            }
        }
        file.close();
    }else{
        QFile file(QCoreApplication::applicationDirPath()+"/cache/OEBPS/content.opf");

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QXmlStreamReader reader(&file);
            ui->textEdit->append("[System]:解析opf文件中 ");
            while(!reader.atEnd()){
                reader.readNextStartElement();
                QString strElementName = reader.name().toString();
                if(QString::compare(strElementName,"itemref")==0){
                    QXmlStreamAttributes attributes = reader.attributes();
                    if (attributes.hasAttribute("idref")) {
                       QString strSRC = attributes.value("idref").toString();
                       golbal.fileName.append(strSRC);
                    }
                }
            }

        }
        file.close();
        QFile sfile(QCoreApplication::applicationDirPath()+"/cache/OEBPS/content.opf");
        if (sfile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QXmlStreamReader reader(&sfile);
            ui->textEdit->append("[System]:链接opf文件中 ");
            while(!reader.atEnd()){
                reader.readNextStartElement();
                QString strElementName = reader.name().toString();

                if(QString::compare(strElementName,"item")==0){
                    QXmlStreamAttributes attributes = reader.attributes();
                    if (attributes.hasAttribute("id")) {
                       QString strSRC = attributes.value("id").toString();
                       for(int i=0;i<golbal.fileName.size();i++){
                           if(QString::compare(golbal.fileName.at(i),strSRC)==0){
                               QString temp = attributes.value("href").toString();

                               ui->textEdit->append("[System]:添加文件"+temp);

                               golbal.fileName.replace(i,temp);
                           }
                       }
                    }
                }
            }

        }
        sfile.close();
    }
}

void MainWindow::on_outDir_clicked()
{
    golbal.SaveFile=QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);
    if(golbal.SaveFile==""){
        return;
    }
    ui->lineEdit_2->setText(golbal.SaveFile);
    ui->textEdit->append("[System]:设置输出文件夹");
}

void MainWindow::on_openout_clicked()
{

        QDesktopServices::openUrl(QUrl("file:///"+golbal.SaveFile, QUrl::TolerantMode));

}

void MainWindow::on_analize_clicked()
{
    for(QString filename:golbal.fileName){
        if(QString::compare(filename.split("/").first(),"Image")==0){
            //QFile file(QCoreApplication::applicationDirPath()+"/cache/OEBPS/"+filename);
            std::fstream outfile;
            outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+filename.split("/").last().split(".").first()+".md").toLocal8Bit(),std::ios::out);
            ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+filename.split("/").last().split(".").first()+".md");
            oss uploader;
            QString out = uploader.upload(QCoreApplication::applicationDirPath()+"/cache/OEBPS/"+filename,!ui->disableupload->isChecked(),ui->keyID->text(),ui->lineEdit_4->text(),ui->lineEdit_5->text(),ui->lineEdit_6->text(),ui->lineEdit_7->text());
            outfile<<"![]("<<out.toStdString()<<")"<<std::endl;
            golbal.count++;
        }else{
        QFile file(QCoreApplication::applicationDirPath()+"/cache/OEBPS/Text/"+filename.split("/").last());

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QXmlStreamReader reader(&file);
            if(reader.hasError()){
                ui->textEdit->append("[System]:解析文件失败 "+filename);
            }else{
                ui->textEdit->append("[System]:解析文件中 "+filename);
                bool startBody=false;
                bool isFileCreated=false;

                std::fstream outfile;
                while(!reader.atEnd()){
                     reader.readNextStartElement();
                     QString strElementName = reader.name().toString();
                     if(QString::compare(strElementName,"title")==0&&!isFileCreated){
                         QString name=reader.readElementText();
                         if(name!=""&&ui->unsettitle->isChecked()&&!ui->firstasname->isChecked()){

                            outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+name+".md").toLocal8Bit(),std::ios::out);
                            ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+name+".md");
                            isFileCreated=true;
                            golbal.count++;
                         }
                     }
                     if(QString::compare(strElementName,"body")==0){

                         if(!startBody){
                            startBody=true;
                         }
                         //ui->textEdit->append("body");
                     }
                     if(QString::compare(strElementName,"h1")==0&&startBody){
                         QString content=reader.readElementText(QXmlStreamReader::IncludeChildElements);
                         //ui->textEdit->append(content);
                         if(!isFileCreated&&ui->firstasname->isChecked()){
                             outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md").toLocal8Bit(),std::ios::out);
                             ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md");
                             isFileCreated=true;
                             golbal.count++;
                         }
                         outfile << ui->h1->text().toStdString()<<" "<<content.toStdString()<<std::endl;
                     }
                     if(QString::compare(strElementName,"h2")==0&&startBody){
                         QString content=reader.readElementText(QXmlStreamReader::IncludeChildElements);
                         //ui->textEdit->append(content);
                         if(!isFileCreated&&ui->firstasname->isChecked()){
                             outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md").toLocal8Bit(),std::ios::out);
                             ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md");
                             isFileCreated=true;
                             golbal.count++;
                         }
                         outfile << ui->h2->text().toStdString()<<" "<<content.toStdString()<<std::endl;
                     }
                     if(QString::compare(strElementName,"h3")==0&&startBody){
                         QString content=reader.readElementText(QXmlStreamReader::IncludeChildElements);
                         //ui->textEdit->append(content);
                         if(!isFileCreated&&ui->firstasname->isChecked()){
                             outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md").toLocal8Bit(),std::ios::out);
                             ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md");
                             isFileCreated=true;
                             golbal.count++;
                         }
                         outfile << ui->h3->text().toStdString()<<" "<<content.toStdString()<<std::endl;
                     }
                     if(QString::compare(strElementName,"h4")==0&&startBody){
                         QString content=reader.readElementText(QXmlStreamReader::IncludeChildElements);
                         //ui->textEdit->append(content);
                         if(!isFileCreated&&ui->firstasname->isChecked()){
                             outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md").toLocal8Bit(),std::ios::out);
                             ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md");
                             isFileCreated=true;
                             golbal.count++;
                         }
                         outfile << ui->h4->text().toStdString()<<" "<<content.toStdString()<<std::endl;
                     }
                     if(QString::compare(strElementName,"h5")==0&&startBody){
                         QString content=reader.readElementText(QXmlStreamReader::IncludeChildElements);
                         //ui->textEdit->append(content);
                         if(!isFileCreated&&ui->firstasname->isChecked()){
                             outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md").toLocal8Bit(),std::ios::out);
                             ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+content+".md");
                             isFileCreated=true;
                             golbal.count++;
                         }
                         outfile << ui->h5->text().toStdString()<<" "<<content.toStdString()<<std::endl;
                     }
                     if(QString::compare(strElementName,"p")==0&&startBody){
                         QString content=reader.readElementText(QXmlStreamReader::IncludeChildElements);
                         //ui->textEdit->append(content);
                         if(!isFileCreated&&ui->firstasname->isChecked()){
                             outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+"unknow"+".md").toLocal8Bit(),std::ios::out);
                             ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+"unknow"+".md");
                             isFileCreated=true;
                             golbal.count++;
                         }
                         if(content==""){
                             outfile << "&nbsp;"<<std::endl;
                         }else{
                             outfile << content.trimmed().toStdString()<<std::endl;
                         }
                         outfile << std::endl;
                     }

                     if(QString::compare(strElementName,"img")==0&&startBody){
                         QXmlStreamAttributes attributes = reader.attributes();
                         if(!isFileCreated&&ui->firstasname->isChecked()){
                             outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+"pic"+".md").toLocal8Bit(),std::ios::out);
                             ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+"pic"+".md");
                             isFileCreated=true;
                             golbal.count++;
                         }
                         if (attributes.hasAttribute("src")) {
                            QString strSRC = attributes.value("src").toString();
                            //ui->textEdit->append(strSRC);
                            QString picdir=QCoreApplication::applicationDirPath()+"/cache/OEBPS/Images"+strSRC.replace(QRegExp("../Images"),"");
                            //ui->textEdit->append(picdir);
                            oss uploader;
                            QString webpath=uploader.upload(picdir,!ui->disableupload->isChecked(),ui->keyID->text(),ui->lineEdit_4->text(),ui->lineEdit_5->text(),ui->lineEdit_6->text(),ui->lineEdit_7->text());
                            outfile << "![]("<<webpath.toStdString()<<")" <<std::endl;
                         }
                     }
                     if(QString::compare(strElementName,"image")==0&&startBody){
                         QXmlStreamAttributes attributes = reader.attributes();
                         if(!isFileCreated&&ui->firstasname->isChecked()){
                             outfile.open((golbal.SaveFile+QString::number(golbal.count)+" - "+"pic"+".md").toLocal8Bit(),std::ios::out);
                             ui->textEdit->append("[System]:创建文件 "+golbal.SaveFile+QString::number(golbal.count)+" - "+"pic"+".md");
                             isFileCreated=true;
                             golbal.count++;
                         }
                         if (attributes.hasAttribute("xlink:href")) {
                            QString strSRC = attributes.value("xlink:href").toString();
                            //ui->textEdit->append(strSRC);
                            QString picdir=QCoreApplication::applicationDirPath()+"/cache/OEBPS/Images"+strSRC.replace(QRegExp("../Images"),"");
                            //ui->textEdit->append(picdir);
                            oss uploader;
                            QString webpath=uploader.upload(picdir,!ui->disableupload->isChecked(),ui->keyID->text(),ui->lineEdit_4->text(),ui->lineEdit_5->text(),ui->lineEdit_6->text(),ui->lineEdit_7->text());
                            outfile << "![]("<<webpath.toStdString()<<")" <<std::endl;
                         }
                     }




                }
                outfile.close();
            }
        }else{
            ui->textEdit->append("[System]:打开文件失败 "+filename);
        }

        file.close();
        }
    }
}

void MainWindow::on_clearout_clicked()
{
    QString command="rd /S /Q "+golbal.SaveFile.replace(QRegExp("/"),"\\");
    ui->textEdit->append("[Command]:"+command);
    system(command.toLocal8Bit());
    command="md "+golbal.SaveFile.replace(QRegExp("/"),"\\");
    system(command.toLocal8Bit());
    ui->textEdit->append("[System]:输出已清空");
}

void MainWindow::on_predo_clicked()
{
    for(QString filename:golbal.fileName){
        QString command="sed -i s#<br/>#&nbsp;#g "+QCoreApplication::applicationDirPath().replace(QRegExp("/"),"\\")+"\\cache\\OEBPS\\Text\\"+filename;
        ui->textEdit->append("[Command]:"+command);
        system(command.toLocal8Bit());
    }
}

void MainWindow::on_reset_clicked()
{
    golbal.isUnzip=false;
    golbal.isOutdirSelected=false;
    golbal.fileName=QList<QString>();
    golbal.count=1;
    ui->textEdit->append("[System]:计数器已重置");
}



void MainWindow::on_H1_clicked()
{
    ui->h1->setText("#");
    ui->h2->setText("##");
    ui->h3->setText("###");
    ui->h4->setText("####");
    ui->h5->setText("#####");
}

void MainWindow::on_H2_clicked()
{
    ui->h1->setText("#");
    ui->h2->setText("#");
    ui->h3->setText("##");
    ui->h4->setText("###");
    ui->h5->setText("####");
}

void MainWindow::on_H3_clicked()
{
    ui->h1->setText("#");
    ui->h2->setText("#");
    ui->h3->setText("#");
    ui->h4->setText("##");
    ui->h5->setText("###");
}

void MainWindow::on_H4_clicked()
{
    ui->h1->setText("#");
    ui->h2->setText("#");
    ui->h3->setText("#");
    ui->h4->setText("#");
    ui->h5->setText("##");
}

void MainWindow::on_H5_clicked()
{
    ui->h1->setText("#");
    ui->h2->setText("#");
    ui->h3->setText("#");
    ui->h4->setText("#");
    ui->h5->setText("#");
}

void MainWindow::on_pushButton_2_clicked()
{
    //保存OSS参数
    std::fstream outfile;
    outfile.open((QCoreApplication::applicationDirPath()+"/OSS.config").toLocal8Bit(),std::ios::out);
    outfile <<ui->keyID->text().toStdString()<<std::endl;
    outfile <<ui->lineEdit_4->text().toStdString()<<std::endl;
    outfile <<ui->lineEdit_5->text().toStdString()<<std::endl;
    outfile <<ui->lineEdit_6->text().toStdString()<<std::endl;
    outfile <<ui->lineEdit_7->text().toStdString()<<std::endl;
    ui->textEdit->append("[System]:已保存OSS参数");
}
