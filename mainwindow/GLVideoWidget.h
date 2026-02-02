#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QMutex>

class GLVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLVideoWidget(QWidget* parent = nullptr);
    ~GLVideoWidget();

    void updateFrame(const QImage& img);
    void clearROI();
    void setDrawingMode(bool enable);

    // 【修改】直接传入视频真实分辨率下的坐标 (如 1920x1088 下的 rect)
    void setStaticROI(QRect sourceRect);

signals:
    void roiSelected(QRect rect, QSize widgetSize);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;     // 专门负责画视频
    void paintEvent(QPaintEvent* event) override; // 负责调度和画框

    // 鼠标事件
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    // OpenGL 资源
    GLuint m_textureId = 0;

    // 数据
    QImage m_frame;
    QMutex m_mutex;

    // ROI 逻辑
    QPoint m_startPoint;      // 鼠标起点 (UI坐标)
    QRect m_currentUiRect;    // 当前正在拖拽的红框 (UI坐标)

    // 【核心】存储真实的 ROI (相对于视频源 1920x1088)
    QRect m_sourceRoiRect;

    bool m_isDrawing = false;
    bool m_canDraw = false;
};
