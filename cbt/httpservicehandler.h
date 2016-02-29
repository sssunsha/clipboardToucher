#ifndef HTTPSERVICEHANDLER_H
#define HTTPSERVICEHANDLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "mainwindow.h"

class httpServiceHandler : public HttpRequestHandler
{
    Q_OBJECT
public:
    httpServiceHandler(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
private:
    MainWindow* m_mainwindow;
};

#endif // HTTPSERVICEHANDLER_H
