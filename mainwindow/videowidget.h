#pragma once

#include <QWidget>
#include <QPixmap>
#include <QMutex>

class VideoWidget :
    public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);

public slots:
    void updateFrame(const QImage& frame);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    //QPixmap m_currentFrame;
    //QMutex m_mutex;
    QAtomicPointer<QPixmap> m_frameAtomic; // 使用原子指针代替 mutex + m_currentFrame
    QSize m_frameSize;
};

