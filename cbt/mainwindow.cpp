#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTcpSocket>
#include <QDir>
#include "httpservicehandler.h"

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
    delete m_server;
    m_server = NULL;

    delete m_setting;
    delete m_httpListener;
}


void MainWindow::init()
{
    this->setWindowTitle("Clipboard toucher");
    // inint cmp
    this->m_okButton = ui->pushButton_ok;
    this->m_closeButton = ui->pushButton_close;
    this->m_contentTextEdit = ui->textEdit_content;
    this->m_statusBar = ui->statusBar;

    // inin the signal and slots
    connect(this->m_okButton, SIGNAL(clicked()), this, SLOT(handle_okButtonClicked()));
    connect(this->m_closeButton, SIGNAL(clicked()), this, SLOT(handle_closeButtonClicked()));

    // setup clipboard
    this->m_clipboard = QApplication::clipboard();

    // setup the tcp server
    m_server = new QTcpServer();

    // setup the tcp server connect and wait for the browser to connect with request
    connect(m_server, SIGNAL(newConnection()), this, SLOT(handle_newTcpConnect()));

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

    // set up the port to listen
    if(m_server->listen(QHostAddress::Any, TCPPORT))
    {
        qDebug() << "set up the server to listen at port: " << TCPPORT;
    }
    else
    {
        qDebug() << "can not listen at port:" << TCPPORT;
         this->close();
    }
}

void MainWindow::setMessage(QString str)
{
    if(str.length() > 0)
    {

        // do the URL decode for Chinese
        QByteArray text = str.toUtf8();
        qDebug() <<"str.toUtf8() --->" << str.toUtf8();
        m_message = QString(text.toPercentEncoding());

        // set the clipboard
        this->m_clipboard->setText(m_message);
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



void MainWindow::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << "----------------------------";
 response.write("Hello World",true);
}


void MainWindow::handle_newTcpConnect()
{

    qDebug() << "get the new tcp connect";

//    static qint16 count;  //count number to be displayed on web browser
    QTcpSocket* socket = m_server->nextPendingConnection();
    while(!(socket->waitForReadyRead()));  //waiting for data to be read from web browser

    char webBrowerRXData[1000];
    int sv=socket->read(webBrowerRXData,1000);

    // filter the http request for content
    QString contentStr = filterContent(QString(webBrowerRXData));
    if(contentStr.length() > 0)
    {
        // send the content to system clipboard
        this->m_clipboard->setText(contentStr);
    }


//    qDebug() << "webBrowerRXData = " << webBrowerRXData;
    socket->write("HTTP/1.1 200 OK\r\n");       // \r needs to be before \n
    socket->write("Content-Type: text/html\r\n");
    socket->write("Connection: close\r\n");
    socket->write("Refresh: 100\r\n\r\n");     //refreshes web browser     every second. Require two \r\n.
     socket->write("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>");
    socket->write("<!DOCTYPE html>\r\n");
    socket->write("<head><meta charset=\"UTF-8\"> <title>Clipboard Toucher</title></head><body>");
//    socket->write("<script type=\"text/javascript\">");
//    socket->write("function sendMessage(){\n");
//    socket->write("\n}");
//    socket->write("</head><body>");
//    socket->write("<button type=\"button\" onclick=\"sendMessage()\">Change Content</button>");
    socket->write("<form method=\"post\">");
    socket->write("  <input type=\"hidden\" name=\"action\" value=\"show\">");
    socket->write("  输入点啥吧： <input type=\"text\" name=\"name\"><br>");
    socket->write("  <input type=\"submit\">");
    socket->write("</form>");
    socket->write(" </body>\n</html>\n");

    socket->flush();
    connect(socket, SIGNAL(disconnected()),socket, SLOT(deleteLater()));
    socket->disconnectFromHost();
    qDebug() << "close the tcp connect";
}

QString MainWindow::filterContent(QString requestStr)
{
    qDebug() << "------------------------------------------";
    qDebug() << requestStr;
    qDebug() << "------------------------------------------";
    int name_index = -1;
    int end_index = -1;
    name_index = requestStr.indexOf("name=", 0);
    qDebug() << "name_index = " << name_index;
    if(name_index > 0)
    {
//        qDebug() << "end_index = " << end_index;
//        end_index = requestStr.indexOf("\u007F", name_index);
        end_index = requestStr.length();
        if(end_index > 0)
        {
            QString content = requestStr.mid(name_index + 5, end_index-name_index-5);
            qDebug() << "content = " << content;
            if(content.length() > 0)
            {
                return content;
            }
            else
            {
                return "";
            }

        }
//        qDebug() << "end_index = " << end_index;
    }
    else
    {
        return "";
    }
}
