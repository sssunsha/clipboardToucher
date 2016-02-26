#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

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

