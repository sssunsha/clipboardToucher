#ifndef HTTPSERVICEHANDLER_H
#define HTTPSERVICEHANDLER_H

#include <QObject>
#include <QString>
#include "httprequesthandler.h"
#include "mainwindow.h"

class httpServiceHandler : public HttpRequestHandler
{
    Q_OBJECT
public:
    httpServiceHandler(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
private:
    bool loadHtmlFile();
    MainWindow* m_mainwindow;
    QByteArray m_htmlFile;
    bool m_isHaveHtmlFile;
};

#endif // HTTPSERVICEHANDLER_H
