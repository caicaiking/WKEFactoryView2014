#ifndef CLSDOWNLOAD_H
#define CLSDOWNLOAD_H

#include "ui_clsDownLoad.h"
#include <QHash>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QProgressDialog>
#include <QtFtp/QFtp>
#include <QFile>

class clsDownLoad : public QDialog, private Ui::clsDownLoad
{
    Q_OBJECT

public:
    explicit clsDownLoad(QWidget *parent = 0);

signals:
    void hasDownloadFile(QString value);
private slots:
    void connectOrDisconnect();
    void downloadFile();
    void cancelDownload();
    void connectToFtp();

    void ftpCommandFinished(int commandId, bool error);
    void addToList(const QUrlInfo &urlInfo);
    void processItem(QTreeWidgetItem *item, int column);
    void cdToParent();
    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);
    void enableDownloadButton();
    void enableConnectButton();

    void on_btnConnect_clicked();

    void on_btnUp_clicked();

    void on_btnDownLoad_clicked();

    void on_btnQuit_clicked();

    void on_btnUpload_clicked();

    void uploadFile();
    void canUpload();

private:
    QHash<QString, bool> isDirectory;
    QString currentPath;
    QFtp *ftp;
    QFile *file;
    QProgressDialog *progressDialog, *uplaodDialog;
    QNetworkSession *networkSession;
    QNetworkConfigurationManager manager;

    QString _ToSpecialEncoding(const QString &InputStr);
    QString _FromSpecialEncoding(const QString &InputStr);
protected:
    QSize sizeHint();
};

#endif // CLSDOWNLOAD_H
