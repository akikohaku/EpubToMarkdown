#pragma execution_character_set("utf-8")
#ifndef OSS_H
#define OSS_H
#include "QString"
#include"QFile"
#include <fstream>
#include "QCryptographicHash"
#include <alibabacloud/oss/OssClient.h>
using namespace AlibabaCloud::OSS;

class oss{
public:
    QString upload(QString filename,bool isOn,QString keyid,QString secret,QString endpoint,QString bucket,QString path){
        std::string AccessKeyId = keyid.toStdString();
            std::string AccessKeySecret = secret.toStdString();
            std::string Endpoint = endpoint.toStdString();
            /* 填写Bucket名称，例如examplebucket */
            std::string BucketName = bucket.toStdString();
            /* 填写文件完整路径，例如exampledir/exampleobject.txt。文件完整路径中不能包含Bucket名称 */
            std::string ObjectName = path.toStdString()+getFileMd5(filename).toStdString()+"."+filename.split(".").last().toStdString();

            if(isOn){
            /* 初始化网络等资源 */
            InitializeSdk();

            ClientConfiguration conf;
            OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
            std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(filename.toStdString(), std::ios::in | std::ios::binary);
            PutObjectRequest request(BucketName, ObjectName, content);
            auto outcome = client.PutObject(request);

                if (!outcome.isSuccess()) {
                    /* 异常处理 */
                    std::cout << "PutObject fail" <<
                    ",code:" << outcome.error().Code() <<
                    ",message:" << outcome.error().Message() <<
                    ",requestId:" << outcome.error().RequestId() << std::endl;
                    ShutdownSdk();
                    return NULL;
                }

                /* 释放网络等资源 */
                ShutdownSdk();
            }
            QString webpath="https://"+bucket+"."+endpoint+"/"+path+getFileMd5(filename)+"."+filename.split(".").last();
            return webpath;
    }
private:
    QString getFileMd5(QString filePath)
    {
        QFile localFile(filePath);

        if (!localFile.open(QFile::ReadOnly))
        {
            return 0;
        }

        QCryptographicHash ch(QCryptographicHash::Md5);

        quint64 totalBytes = 0;
        quint64 bytesWritten = 0;
        quint64 bytesToWrite = 0;
        quint64 loadSize = 1024 * 4;
        QByteArray buf;

        totalBytes = localFile.size();
        bytesToWrite = totalBytes;

        while (1)
        {
            if(bytesToWrite > 0)
            {
                buf = localFile.read(qMin(bytesToWrite, loadSize));
                ch.addData(buf);
                bytesWritten += buf.length();
                bytesToWrite -= buf.length();
                buf.resize(0);
            }
            else
            {
                break;
            }

            if(bytesWritten == totalBytes)
            {
                break;
            }
        }

        localFile.close();
        QByteArray md5 = ch.result();
        //std::cout<< QString::fromLocal8Bit(md5.toHex()).toStdString();
        return QString::fromLocal8Bit(md5.toHex());
    }
};

#endif // OSS_H
