#include "httpservicehandler.h"
#include <QDebug>
#include <QString>

httpServiceHandler::httpServiceHandler(QObject* parent)
    : HttpRequestHandler(parent)
{
    m_mainwindow = ( MainWindow*)parent;
}

void httpServiceHandler::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());
//    response.write("Hello World",true);


    response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");

     if (request.getParameter("action")=="sendMessage")
     {
         QString bodyStr = request.getBody();
         int bodyLen = bodyStr.length();
         int nameIndex = bodyStr.indexOf("name=", 0);
         QString message = "";
         if(bodyLen > 0 && nameIndex > 0)
         {
            message = bodyStr.mid(nameIndex + 5, bodyLen-nameIndex-5);
            qDebug() << "message --> " << message;
            m_mainwindow->setMessage(message);
         }
     }

     response.write("<html><body>");
     response.write("<form method=\"post\">");
     response.write("  <input type=\"hidden\" name=\"action\" value=\"sendMessage\">");
     response.write("  Name: <input type=\"text\" name=\"name\"><br>");
     response.write("  <input type=\"submit\">");
     response.write("</form>");
     response.write("</body></html>",true);
}
