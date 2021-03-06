#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTcpSocket>
#include <QDir>
#include "httpservicehandler.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QPixmap>
#include <QBuffer>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

int TCPPORT = 9527;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_setting;
    delete m_httpListener;
}


void MainWindow::init()
{
    this->setWindowTitle("Clipboard toucher");    

    // inint cmp
    this->m_helpMessageLabel = ui->label_helpMessage;
    this->m_okButton = ui->pushButton_ok;
    this->m_okButton->setText("确定(ok)");
    this->m_closeButton = ui->pushButton_close;
    this->m_closeButton->setText("退出(quit)");
    this->m_contentTextEdit = ui->textEdit_content;
    ui->label_content->setText("输入点什么吧(keywords)：");
    this->m_statusBar = ui->statusBar;
    this->m_screenshotButton = ui->pushButton_screenshot;

    setHelpMessage();

    // inin the signal and slots
    connect(this->m_okButton, SIGNAL(clicked()), this, SLOT(handle_okButtonClicked()));
    connect(this->m_closeButton, SIGNAL(clicked()), this, SLOT(handle_closeButtonClicked()));
    connect(this->m_screenshotButton, SIGNAL(clicked()), this, SLOT(handle_screenshotButtonClicked()));
    connect(this, SIGNAL(messageChanged()), this, SLOT(handle_messageChanged()));

    // setup clipboard
    this->m_clipboard = QApplication::clipboard();

    QString iniFileName = searchConfigFile();

    // load the ini file for QtWebApp
    m_setting = new QSettings(iniFileName,QSettings::IniFormat,this);
    m_setting->beginGroup("listener");
    qDebug() << "settings keys : " << m_setting->childKeys().length();
    if(m_setting->childKeys().length() > 0)
    {
        qDebug() << "config file loaded";

        // setup the server at port 8080
        m_httpListener = new HttpListener(m_setting, new httpServiceHandler(this), this);

    }
    else
    {
        close();
    }
}


void MainWindow::setHelpMessage()
{
        // get the locate
    QString welcome1Str = "欢迎使用“电脑小帮手.\n 本机IP地址是：";
    QString portStr = " : 9527\n";
    QString welcome2Str = "请打开手机浏览器，并输入上述IP地址使用。谢谢。";
    QString ipAddress = "";

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            ipAddress = address.toString();
    }

    m_helpMessageLabel->setText(welcome1Str + ipAddress +  portStr + welcome2Str);
}

void MainWindow::setMessage(QString str)
{
    if(str.length() > 0)
    {    
        // do the URL decode for Chinese
        QByteArray text = str.toUtf8();        
        m_message = QString(QByteArray::fromPercentEncoding(text));
        emit messageChanged();
    }

}

QString MainWindow::searchConfigFile()
{
//    QString appName = "cbt";
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QString fileName(appName+".ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../../etc"); // for development without shadow build
    searchList.append(binDir+"/../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            // found
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return 0;
}

// private slots
void MainWindow::handle_okButtonClicked()
{
    qDebug()<< "ok button is clicked, content = " << this->m_contentTextEdit->toPlainText();

    QString str = this->m_contentTextEdit->toPlainText();
    if(str.length() > 0 && m_clipboard)
    {
        m_clipboard->setText(str);

        // update the status bar
        m_statusBar->showMessage(str + " saved", 1000);
    }
}

void MainWindow::handle_closeButtonClicked()
{
    this->close();
}

void MainWindow::handle_messageChanged()
{
    this->m_clipboard->setText(m_message);
}

void MainWindow::handle_screenshotButtonClicked()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        QPixmap pixmap = screen->grabWindow(QApplication::desktop()->winId());
        pixmap.save("clipboardToucher_screenshot.png", "png");
    }
}
