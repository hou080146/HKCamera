#include "GLVideoWidget.h"
#include <QOpenGLTexture>

GLVideoWidget::GLVideoWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_textureId(0)
{
    //setFixedSize(1280, 720);   // 固定为 720p 显示窗口（适合 1080p 视频输入）

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

void GLVideoWidget::setDrawingMode(bool enable)
{
    m_canDraw = enable;

    // 设置鼠标样式：允许画的时候是十字，平时是箭头
    if (enable) {
        setCursor(Qt::CrossCursor);
    }
    else {
        setCursor(Qt::ArrowCursor);
    }
}


void GLVideoWidget::clearROI()
{
    m_selectionRect = QRect();
    m_isDrawing = false;
    m_canDraw = false; // 清除后默认不能马上画，必须再次点击按钮
    setCursor(Qt::ArrowCursor);
    update();
}

void GLVideoWidget::mousePressEvent(QMouseEvent* event)
{
    if (!m_canDraw) return;

    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_startPoint = event->pos();
        m_selectionRect = QRect(); // 清空旧的
        update();
    }
}

void GLVideoWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDrawing && m_canDraw) {
        m_selectionRect = QRect(m_startPoint, event->pos()).normalized();
        update();
    }
}

void GLVideoWidget::mouseReleaseEvent(QMouseEvent* event)
{
    //只有在拖拽状态下才处理
    if (event->button() == Qt::LeftButton && m_isDrawing && m_canDraw) {
        m_isDrawing = false;
        m_selectionRect = QRect(m_startPoint, event->pos()).normalized();

        if (m_selectionRect.width() > 10 && m_selectionRect.height() > 10) {
            // 1. 发送信号给主界面
            emit roiSelected(m_selectionRect, this->size());

            // 2. 【关键】画完这一次，立即锁定！防止用户手抖又画了一个
            // 必须等待用户点“清除”或者再次点“选择ROI”才能重画
            setDrawingMode(false);
        }
        else {
            // 框太小视为无效，重置
            m_selectionRect = QRect();
        }
        update();
    }
}

void GLVideoWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    // 1. 画视频
    if (!m_frame.isNull()) {
        painter.drawImage(rect(), m_frame);
    }
    else {
        painter.fillRect(rect(), Qt::black);
    }

    // 2. 画框
    if (!m_selectionRect.isNull()) {
        QPen pen(Qt::red, 3, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawRect(m_selectionRect);
    }
}