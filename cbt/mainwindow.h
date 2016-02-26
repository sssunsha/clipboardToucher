#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QClipboard>
#include <QStatusBar>
#include <QTcpServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void handle_okButtonClicked();
    void handle_closeButtonClicked();

    void handle_newTcpConnect();
private:    
    void init();

    Ui::MainWindow *ui;

    QPushButton* m_closeButton;
    QPushButton* m_okButton;
    QTextEdit* m_contentTextEdit;
    QStatusBar* m_statusBar;

    QClipboard *m_clipboard;

    QTcpServer *m_server;

};

#endif // MAINWINDOW_H
