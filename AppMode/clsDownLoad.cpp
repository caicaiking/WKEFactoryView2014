#include "clsDownLoad.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

clsDownLoad::clsDownLoad(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    ftp =0;
    networkSession=0;
    this->btnDownLoad->setEnabled(false);
    this->btnUpload->setEnabled(false);

    btnUp->setVisible(false);

    fileList->setHeaderLabels(QStringList() << tr("Name") << tr("Size") << tr("Owner") << tr("Group") << tr("Time"));
    fileList->header()->setStretchLastSection(false);

    progressDialog = new QProgressDialog(this);
    this->uplaodDialog = new QProgressDialog(this);

    connect(fileList,SIGNAL(itemActivated(QTreeWidgetItem*,int)),
            this,SLOT(processItem(QTreeWidgetItem*,int)));

    connect(fileList,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this,SLOT(enableDownloadButton()));

    connect(progressDialog,SIGNAL(canceled()),this,SLOT(cancelDownload()));

    connect(this->uplaodDialog,SIGNAL(canceled()),this,SLOT(canUpload()));
}

void clsDownLoad::connectOrDisconnect()
{
    if(ftp)
    {
        ftp->abort();
        ftp->deleteLater();
        ftp=0;

        fileList->setEnabled(false);
        btnUp->setEnabled(false);
        btnDownLoad->setEnabled(false);
        btnConnect->setEnabled(true);
        btnConnect->setText(tr("connect"));

        this->setCursor(Qt::ArrowCursor);

        lblInformation->setText(tr("please input ftp server name."));
        return;
    }

    this->setCursor(Qt::WaitCursor);

    if(!networkSession || !networkSession->isOpen())
    {
        if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
        {
            if (!networkSession)
            {
                // Get saved network configuration
                QSettings settings(QSettings::UserScope, QLatin1String("WayneKerr"));
                settings.beginGroup(QLatin1String("QtNetwork"));
                const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
                settings.endGroup();

                // If the saved network configuration is not currently discovered use the system default
                QNetworkConfiguration config = manager.configurationFromIdentifier(id);
                if ((config.state() & QNetworkConfiguration::Discovered) !=
                        QNetworkConfiguration::Discovered) {
                    config = manager.defaultConfiguration();
                }

                networkSession = new QNetworkSession(config, this);
                connect(networkSession, SIGNAL(opened()), this, SLOT(connectToFtp()));
                connect(networkSession, SIGNAL(error(QNetworkSession::SessionError)), this, SLOT(enableConnectButton()));
            }
            btnConnect->setEnabled(false);
            lblInformation->setText(tr("Opening network session."));
            networkSession->open();
            return;
        }
    }
    connectToFtp();
}

void clsDownLoad::downloadFile()
{
    QString fileName = fileList->currentItem()->text(0);


    if(QFile::exists(fileName))
    {
        QMessageBox::information(this, tr("FTP"),
                                 tr("There already exists a file called %1 in "
                                    "the current directory.")
                                 .arg(fileName));
        return;
    }


    file = new QFile(fileName);

    if(!file->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("FTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg(fileName).arg(file->errorString()));
        delete file;
        return;
    }

    ftp->get(_ToSpecialEncoding( fileList->currentItem()->text(0)),file);



    progressDialog->setLabelText(tr("%1 file downloading..").arg(fileName));

    btnDownLoad->setEnabled(false);
    progressDialog->exec();

}

void clsDownLoad::canUpload()
{
    ftp->abort();
    delete file;
}

void clsDownLoad::cancelDownload()
{
    ftp->abort();
    if(file->exists())
    {
        file->close();
        file->remove();

    }
    //delete file;
}

void clsDownLoad::connectToFtp()
{
    ftp = new QFtp(this);

    connect(ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(ftpCommandFinished(int,bool)));
    connect(ftp,SIGNAL(listInfo(QUrlInfo)),this,SLOT(addToList(QUrlInfo)));
    connect(ftp,SIGNAL(dataTransferProgress(qint64,qint64)),
            this,SLOT(updateDataTransferProgress(qint64,qint64)));

    fileList->clear();

    currentPath.clear();
    isDirectory.clear();

    ftp->connectToHost(txtServer->text());
    ftp->login("sd0829169.03@gmoserver.jp","#Z90HzWo");

    fileList->setEnabled(true);
    btnConnect->setEnabled(false);
    btnConnect->setText(tr("DisConnect"));
    btnUpload->setEnabled(true);
    lblInformation->setText(tr("Have connect to %1..").arg(txtServer->text()));
}

void clsDownLoad::ftpCommandFinished(int /*commandId*/, bool error)
{
    setCursor(Qt::ArrowCursor);

    if(ftp->currentCommand() == QFtp::ConnectToHost)
    {
        if(error)
        {
            QMessageBox::information(this, tr("FTP"),
                                     tr("Unable to connect to the FTP server "
                                        "at %1. Please check that the host "
                                        "name is correct.")
                                     .arg(txtServer->text()));
            connectOrDisconnect();
            return;
        }
        this->lblInformation->setText(tr("Have logged to server: %1.").arg(txtServer->text()));
        fileList->setFocus();
        btnDownLoad->setEnabled(true);
        btnConnect->setEnabled(true);
        return;
    }

    if(ftp->currentCommand() == QFtp::Login)
    {
        ftp->cd("/KE/App");
        ftp->list();
    }

    if(ftp->currentCommand() == QFtp::Put)
    {
        if(error)
        {
            lblInformation->setText(tr("upload cancel %1.").arg(file->fileName()));
            file->close();
            file->remove();
        }
        else
        {
            lblInformation->setText(tr("upload %1 file to current dir.").arg(file->fileName()));
            file->close();
            fileList->clear();
            isDirectory.clear();
            ftp->list();
        }
        delete file;
        btnUpload->setEnabled(true);
        uplaodDialog->hide();
    }

    if(ftp->currentCommand() == QFtp::Get)
    {
        if(error)
        {
            lblInformation->setText(tr("Download cancel %1.").arg(file->fileName()));
            file->close();
            file->remove();
        }
        else
        {
            lblInformation->setText(tr("download  %1 file to current dir.").arg(file->fileName()));

            emit hasDownloadFile(file->fileName());
            file->close();
        }

        delete file;
        enableDownloadButton();
        progressDialog->hide();
    }
    else if(ftp->currentCommand() == QFtp::List)
    {
        if(isDirectory.isEmpty())
        {
            fileList->addTopLevelItem(new QTreeWidgetItem(QStringList()<<tr("<empty>")));
            fileList->setEnabled(false);
        }
    }

}

void clsDownLoad::addToList(const QUrlInfo &urlInfo)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;

    item->setText(0, _FromSpecialEncoding(urlInfo.name()));
    item->setText(1,QString::number(urlInfo.size()));
    item->setText(2,urlInfo.owner());
    item->setText(3,urlInfo.group());
    item->setText(4,urlInfo.lastModified().toString("MMM dd yyyy"));

    QPixmap pixmap(urlInfo.isDir()? ":/Icons/dir.png":":/Icons/file.png");
    item->setIcon(0,pixmap);

    isDirectory[_FromSpecialEncoding(urlInfo.name())] = urlInfo.isDir();
    fileList->addTopLevelItem(item);

    if(!fileList->currentItem())
    {
        fileList->setCurrentItem(fileList->topLevelItem(0));
        fileList->setEnabled(true);
    }

}

void clsDownLoad::processItem(QTreeWidgetItem *item, int /*column*/)
{
    QString name = item->text(0);
    if(isDirectory.value(name))
    {
        fileList->clear();
        isDirectory.clear();
        currentPath+="/";
        currentPath+=name;
        ftp->cd(name);
        ftp->list();
        btnUp->setEnabled(true);
        setCursor(Qt::WaitCursor);
        return;
    }
}

void clsDownLoad::cdToParent()
{
    this->setCursor(Qt::WaitCursor);

    fileList->clear();
    isDirectory.clear();

    currentPath = currentPath.left(currentPath.lastIndexOf("/"));

    if(currentPath.isEmpty())
    {
        btnUp->setEnabled(false);
        ftp->cd("/");
    }
    else
    {
        ftp->cd(currentPath);
    }

    ftp->list();
}

QString clsDownLoad::_FromSpecialEncoding(const QString &InputStr)
{
#ifdef Q_OS_WIN
    return  QString::fromLocal8Bit(InputStr.toLatin1());
#else
    QTextCodec *codec = QTextCodec::codecForName("gbk");
    if (codec)
    {
        return codec->toUnicode(InputStr.toLatin1());
    }
    else
    {
        return QString("");
    }
#endif
}


QString clsDownLoad::_ToSpecialEncoding(const QString &InputStr)
{
#ifdef Q_OS_WIN
    return QString::fromLatin1(InputStr.toLocal8Bit());
#else
    QTextCodec *codec= QTextCodec::codecForName("gbk");
    if (codec)
    {
        return QString::fromLatin1(codec->fromUnicode(InputStr));
    }
    else
    {
        return QString("");
    }
#endif
}

void clsDownLoad::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    if(progressDialog->isVisible())
    {
        this->progressDialog->setMaximum(totalBytes);
        this->progressDialog->setValue(readBytes);
    }
    else
    {
        this->uplaodDialog->setMaximum(totalBytes);
        this->uplaodDialog->setValue(readBytes);
    }
}

void clsDownLoad::enableDownloadButton()
{
    QTreeWidgetItem *current = fileList->currentItem();
    if(current)
    {
        QString currentFile = current->text(0);
        btnDownLoad->setEnabled(!isDirectory.value(currentFile));
    }
    else
    {
        btnDownLoad->setEnabled(false);
    }
}

void clsDownLoad::enableConnectButton()
{
    QNetworkConfiguration config = networkSession->configuration();
    QString id;

    if(config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("WayneKerr"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    btnConnect->setEnabled(true);
    lblInformation->setText(tr("Please input ftp server name."));
}

QSize clsDownLoad::sizeHint()
{
    return QSize(500, 300);
}

void clsDownLoad::on_btnConnect_clicked()
{
    connectOrDisconnect();
}

void clsDownLoad::on_btnUp_clicked()
{
    cdToParent();
}

void clsDownLoad::on_btnDownLoad_clicked()
{
    downloadFile();
}

void clsDownLoad::on_btnQuit_clicked()
{
    this->close();
}

void clsDownLoad::on_btnUpload_clicked()
{
    uploadFile();
}

void clsDownLoad::uploadFile()
{


    QString  fileName = QFileDialog::getOpenFileName(this,
                                                     tr("open file"), "/home", tr("data file (*.csv)"));

    if(fileName.isEmpty())
        return;

    file = new QFile(fileName);

    if(!file->open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("FTP"),
                                 tr("can not read file %1: %2.")
                                 .arg(fileName).arg(file->errorString()));
        delete file;
        return;
    }

    QFileInfo info(fileName);

    ftp->put(file,_ToSpecialEncoding(info.fileName()));

    uplaodDialog->setLabelText(tr("%1 file uploading..").arg(info.fileName()));

    btnUpload->setEnabled(false);
    uplaodDialog->exec();

}
