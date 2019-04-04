#include "widget.h"
#include <QImage>
#include <QPen>
#include <QFont>
#include <QPainter>
#include <QRect>
#include <QDateTime>

#include "vertex.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),textureObj(nullptr)
{
    shaderProgram = new QOpenGLShaderProgram();
    QString imgPath[4];
    imgPath[0] = "D://video//image-001.jpeg";
    imgPath[1] = "D://video//image-002.jpeg";
    imgPath[2] = "D://video//image-003.jpeg";
    imgPath[3] = "D://video//image-004.jpeg";

    for(int i = 0; i < 4; i++) {
        listImage.push_front(QImage(imgPath[i]));
    }

    startTimer(1000 / 30);//fps 30
}

Widget::~Widget()
{

    shaderProgram->release();
    if(textureObj){
        textureObj->destroy();
        delete textureObj;
        textureObj = nullptr;
    }
    delete shaderProgram;
}

QString Widget::readText(QString url) {

    QFile inputFile(url);
    bool result = inputFile.open(QIODevice::ReadOnly);
    qDebug() << result;
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    return line;
}

void Widget::initializeGL()
{
    Vertex vertexs[] = {
        //vertex left top                   //texture left bottom
        {QVector3D(-1.0, 1.0, 0.0),  QVector3D(0.0, 0.0, 0.0)},
        //vertex left bottom                //texture left top
        {QVector3D(-1.0, -1.0, 0.0), QVector3D(0.0, 1.0, 0.0)},
        //vertex right bottom               //texture right top
        {QVector3D(1.0, -1.0, 0.0),  QVector3D(1.0, 1.0, 0.0)},
        //vertex right bottom               //texture right top
        {QVector3D(1.0, -1.0, 0.0),  QVector3D(1.0, 1.0, 0.0)},
        //vertex right top                  //texture right bottom
        {QVector3D(1.0, 1.0, 0.0),   QVector3D(1.0, 0.0, 0.0)},
        //vertex left top                   //texture left bottom
        {QVector3D(-1.0, 1.0, 0.0),  QVector3D(0.0, 0.0, 0.0)}
    };
    vertexCount = sizeof(vertexs) / sizeof(vertexs[0]);

    initializeOpenGLFunctions();

    QString sVert  = readText("D://workspace//project//testglimage//shader.vert");
    QString sFrag = readText("D://workspace//project//testglimage//shader.frag");

    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, sVert);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, sFrag);
    shaderProgram->link();
    shaderProgram->bind();

    buffer.create();
    buffer.bind();
    buffer.allocate(vertexs, sizeof(vertexs));

    posAttr = shaderProgram->attributeLocation("vertPos");
    shaderProgram->enableAttributeArray(posAttr);
    shaderProgram->setAttributeBuffer(posAttr, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());

    texAttr = shaderProgram->attributeLocation("texPos");
    shaderProgram->enableAttributeArray(texAttr);
    shaderProgram->setAttributeBuffer(texAttr, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

    shaderProgram->setUniformValue("img", 0);

    buffer.release();
    shaderProgram->release();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    textureObj = genTexture(date, 20, Qt::red);
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram->bind();
        textureObj->bind(0);
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        textureObj->release();
    shaderProgram->release();
}

void Widget::timerEvent(QTimerEvent *event)
{
    if(textureObj != nullptr) {
        textureObj->destroy();
        delete textureObj;
        textureObj = nullptr;
    }

    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    textureObj = genTexture(date, 20, Qt::red);
    update();
    (void)event;
}

QOpenGLTexture *Widget::genTexture(const QString &text, int textSize, const QColor &textColor)
{
    (void)text;
    (void)textSize;
    (void)textColor;
    auto img = listImage.at(rand() % 4);
#if 0
    //paint text to image
    QPainter painter;
    QFont font;
    QPen redPen;
    //QImage img(this->width(), this->height(), QImage::Format_ARGB32_Premultiplied);
    //img.fill(QColor(210, 100, 50, 0));
    redPen.setColor(textColor);
    font.setPixelSize(textSize);

    painter.begin(&img);
    painter.setFont(font);

    QTextOption option(Qt::AlignLeft | Qt::AlignTop);
    option.setWrapMode(QTextOption::WordWrap);

    QRect rect = QRect(0, 0, width(), height());
    painter.setPen(redPen);
    painter.drawText(rect, text, option);
    painter.end();
#endif

    auto tmp = new QOpenGLTexture(QOpenGLTexture::Target2D);
    tmp->setMinificationFilter(QOpenGLTexture::Nearest);
    tmp->setMagnificationFilter(QOpenGLTexture::Linear);
    tmp->setWrapMode(QOpenGLTexture::Repeat);
    tmp->setData(img);
    return tmp;
}


