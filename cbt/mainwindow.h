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
#include <QLabel>

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
private:    
    void init();
    void setHelpMessage();
    QString searchConfigFile();

    Ui::MainWindow *ui;

    QPushButton* m_closeButton;
    QPushButton* m_okButton;
    QTextEdit* m_contentTextEdit;
    QStatusBar* m_statusBar;
    QLabel* m_helpMessageLabel;

    QClipboard *m_clipboard;

    QSettings *m_setting;

    HttpListener* m_httpListener;

    QString m_message;

};

#endif // MAINWINDOW_H
