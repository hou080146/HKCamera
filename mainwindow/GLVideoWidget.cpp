#include "GLVideoWidget.h"
#include <QOpenGLTexture>

GLVideoWidget::GLVideoWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_textureId(0)
{
    setFixedSize(640, 480);  // 固定大小
}

GLVideoWidget::~GLVideoWidget()
{
    makeCurrent();
    if (m_textureId)
        glDeleteTextures(1, &m_textureId);
    doneCurrent();
}

void GLVideoWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_TEXTURE_2D);
}

void GLVideoWidget::updateFrame(const QImage &img)
{
    if (img.isNull()) return;

    QMutexLocker locker(&m_mutex);
    m_frame = img.copy();
    update();   // 触发 paintGL()
}

void GLVideoWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    QMutexLocker locker(&m_mutex);
    if (m_frame.isNull()) return;

    // 第一次创建纹理
    if (!m_textureId) {
        glGenTextures(1, &m_textureId);
    }

    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // 上传纹理到 GPU
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB,
        m_frame.width(),
        m_frame.height(),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        m_frame.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 自适应填满窗口
    glBegin(GL_QUADS);

    glTexCoord2f(0, 1); glVertex2f(-1, -1);
    glTexCoord2f(1, 1); glVertex2f(1, -1);
    glTexCoord2f(1, 0); glVertex2f(1, 1);
    glTexCoord2f(0, 0); glVertex2f(-1, 1);

    glEnd();
}

void GLVideoWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}