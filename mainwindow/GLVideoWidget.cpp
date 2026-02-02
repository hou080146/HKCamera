#include "GLVideoWidget.h"

GLVideoWidget::GLVideoWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    // setFixedSize 可以保留也可以去掉，现在的逻辑支持任意大小自适应
}

GLVideoWidget::~GLVideoWidget()
{
    makeCurrent();
    if (m_textureId) {
        glDeleteTextures(1, &m_textureId);
    }
    doneCurrent();
}

void GLVideoWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_TEXTURE_2D);
    // 设置背景色为黑色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GLVideoWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

// 纯净的 OpenGL 渲染视频 (GPU加速)
void GLVideoWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    QMutexLocker locker(&m_mutex);
    if (m_frame.isNull()) return;

    if (!m_textureId) {
        glGenTextures(1, &m_textureId);
    }

    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // 只有当图片尺寸变了或者第一帧时才需要重新分配内存，否则可以用 glTexSubImage2D 优化
    // 这里为了简单，直接上传
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frame.width(), m_frame.height(),
        0, GL_RGB, GL_UNSIGNED_BYTE, m_frame.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 绘制纹理四边形 (铺满窗口)
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);  // 左上
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);   // 右上
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);  // 右下
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f); // 左下
    glEnd();
}

// 【核心】混合渲染入口
void GLVideoWidget::paintEvent(QPaintEvent* event)
{
    // 1. 先用 OpenGL 画视频背景
    makeCurrent();
    paintGL();
    // 注意：paintGL 画完后，状态可能需要重置，或者直接交给 QPainter 覆盖

    // 2. 使用 QPainter 画红框 (覆盖在 GL 之上)
    QPainter painter(this);

    // 2.1 画正在拖拽的框 (UI坐标)
    if (m_isDrawing && !m_currentUiRect.isNull()) {
        QPen pen(Qt::red, 2, Qt::DashLine);
        painter.setPen(pen);
        painter.drawRect(m_currentUiRect);
    }
    // 2.2 画已确定的/加载的 ROI (需要从 Source 坐标转为 UI 坐标)
    else if (!m_sourceRoiRect.isNull() && !m_frame.isNull()) {
        // 动态计算比例：当前窗口宽 / 视频原宽
        double scaleX = (double)width() / m_frame.width();
        double scaleY = (double)height() / m_frame.height();

        int x = m_sourceRoiRect.x() * scaleX;
        int y = m_sourceRoiRect.y() * scaleY;
        int w = m_sourceRoiRect.width() * scaleX;
        int h = m_sourceRoiRect.height() * scaleY;

        QPen pen(Qt::red, 3, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawRect(x, y, w, h);
    }

    painter.end();
}

void GLVideoWidget::updateFrame(const QImage& img)
{
    if (img.isNull()) return;
    QMutexLocker locker(&m_mutex);
    // 确保格式正确，OpenGL显示通常需要 RGB888
    if (img.format() != QImage::Format_RGB888) {
        m_frame = img.convertToFormat(QImage::Format_RGB888);
    }
    else {
        m_frame = img.copy();
    }
    update(); // 触发 paintEvent
}

// -----------------------------------------------------------
// 交互逻辑
// -----------------------------------------------------------

void GLVideoWidget::setDrawingMode(bool enable)
{
    m_canDraw = enable;
    setCursor(enable ? Qt::CrossCursor : Qt::ArrowCursor);
}

void GLVideoWidget::clearROI()
{
    m_sourceRoiRect = QRect(); // 清空真实 ROI
    m_currentUiRect = QRect(); // 清空 UI 框
    m_isDrawing = false;
    setDrawingMode(false);
    update();
}

// 【修改】这里直接传入 1920x1088 的坐标
void GLVideoWidget::setStaticROI(QRect sourceRect)
{
    m_sourceRoiRect = sourceRect;
    setDrawingMode(false); // 锁定
    update(); // 重绘，paintEvent 会自动计算出要在哪里画框
}

void GLVideoWidget::mousePressEvent(QMouseEvent* event)
{
    if (!m_canDraw) return;
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_startPoint = event->pos();
        m_currentUiRect = QRect();
        // 按下时清空旧的 ROI
        m_sourceRoiRect = QRect();
        update();
    }
}

void GLVideoWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDrawing && m_canDraw) {
        m_currentUiRect = QRect(m_startPoint, event->pos()).normalized();
        update();
    }
}

void GLVideoWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_isDrawing && m_canDraw) {
        m_isDrawing = false;
        m_currentUiRect = QRect(m_startPoint, event->pos()).normalized();

        if (m_currentUiRect.width() > 10 && m_currentUiRect.height() > 10) {
            // 发送信号 (UI 坐标)
            emit roiSelected(m_currentUiRect, this->size());

            // 【关键】立刻自己算一份 Source ROI 存起来
            // 这样松手后，红框依然会显示，且缩放窗口也不会错位
            // 注意：这里需要知道 m_frame 的尺寸，如果还没视频，可能需要默认值
            int videoW = m_frame.isNull() ? 1920 : m_frame.width();
            int videoH = m_frame.isNull() ? 1088 : m_frame.height();

            double scaleX = (double)videoW / width();
            double scaleY = (double)videoH / height();

            m_sourceRoiRect.setRect(
                m_currentUiRect.x() * scaleX,
                m_currentUiRect.y() * scaleY,
                m_currentUiRect.width() * scaleX,
                m_currentUiRect.height() * scaleY
            );

            setDrawingMode(false);
        }
        else {
            m_currentUiRect = QRect();
        }
        update();
    }
}