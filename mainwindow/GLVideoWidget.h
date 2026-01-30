#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMutex>
#include <QMouseEvent>
#include <QPainter>

class GLVideoWidget :
    public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLVideoWidget(QWidget *parent = nullptr);
    ~GLVideoWidget();

    // 清除选区
    void clearROI();
    void setDrawingMode(bool enable);
public slots:
    void updateFrame(const QImage &img);

signals:
    // 当鼠标松开确定选区后，发送此信号
    // rect: UI上的矩形, widgetSize: 控件当时的大小
    void roiSelected(QRect rect, QSize widgetSize);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QImage m_frame;       // 最新帧（RGB）
    GLuint m_textureId;   // OpenGL 纹理
    QMutex m_mutex;
    QPoint m_startPoint;      // 鼠标按下的起点
    QRect m_selectionRect;    // 当前画的框
    bool m_isDrawing = false; // 是否正在拖拽
    bool m_canDraw = false;       //是否允许画框
};

