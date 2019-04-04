#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat format;
    //format.setDepthBufferSize(24);
    //format.setStencilBufferSize(8);
    //format.setVersion(3, 2);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);

    Widget w;
    w.setFormat(format);
    w.show();

    return a.exec();
}
