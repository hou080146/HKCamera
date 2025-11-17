#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMutex>
class GLVideoWidget :
    public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLVideoWidget(QWidget *parent = nullptr);
    ~GLVideoWidget();

public slots:
    void updateFrame(const QImage &img);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    QImage m_frame;       // ×îÐÂÖ¡£¨RGB£©
    GLuint m_textureId;   // OpenGL ÎÆÀí
    QMutex m_mutex;
};

