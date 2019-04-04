#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QList>

class Widget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void timerEvent(QTimerEvent *event);

    QOpenGLTexture* genTexture(const QString &text, int textSize, const QColor &textColor);
    QString readText(QString url);

private:
    QOpenGLBuffer                   buffer;
    QOpenGLTexture          *       textureObj;
    QOpenGLShaderProgram    *       shaderProgram;
    QList<QImage>                   listImage;

    GLuint                          vertexCount;
    GLuint                          posAttr;
    GLuint                          texAttr;
};

#endif // WIDGET_H
