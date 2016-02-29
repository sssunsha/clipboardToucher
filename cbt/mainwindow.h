#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QClipboard>
#include <QStatusBar>
#include <QTcpServer>
#include <QString>
#include <QSettings>

#include "httplistener.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setMessage(QString str);
private slots:
    void handle_okButtonClicked();
    void handle_closeButtonClicked();
    void handle_newTcpConnect();
    QString filterContent(QString requestStr);
private:    
    void init();
    QString searchConfigFile();
    void service(HttpRequest &request, HttpResponse &response);

    Ui::MainWindow *ui;

    QPushButton* m_closeButton;
    QPushButton* m_okButton;
    QTextEdit* m_contentTextEdit;
    QStatusBar* m_statusBar;

    QClipboard *m_clipboard;

    QTcpServer *m_server;

    QSettings *m_setting;

    HttpListener* m_httpListener;

    QString m_message;

};

#endif // MAINWINDOW_H
