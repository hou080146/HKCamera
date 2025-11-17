#include "videowidget.h"

#include <QPainter>
#include <qDebug>

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(640, 480);    // 设置固定大小
    setAttribute(Qt::WA_OpaquePaintEvent);
    m_frameAtomic.store(nullptr);

}

//15（2）.信号传递到VideoWidget::updateFrame 缩放图像到控件大小
//17（1）.发送信号到mainwindow，video触发重绘
void VideoWidget::updateFrame(const QImage& frame)
{
#if 0:
    if (!frame.isNull()) {
        qDebug() << "接收到的QImage图像 不为空";
    }
    else {
        qDebug() << "接收到的QImage图像 为空";
    }
    QMutexLocker locker(&m_mutex);
    m_frameSize = frame.size();
    m_currentFrame = QPixmap::fromImage(frame.scaled(size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
    //16（1）.发送信号到mainwindow，update()触发paintEvent
    update();
#endif
    if (frame.isNull()) return;

    // 1. 构造 QPixmap（耗时操作锁外）
    QPixmap* newPixmap = new QPixmap(QPixmap::fromImage(frame.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    //QImage scaledImg = frame.scaled(size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    // 2. 原子交换，保证线程安全
    QPixmap* oldPixmap = m_frameAtomic.fetchAndStoreRelaxed(newPixmap);
    //QPixmap* oldPixmap = m_frameAtomic.load();
    if (oldPixmap) delete oldPixmap;

    // 3. 触发 repaint
    update();
}

void VideoWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
#if 0:
    QMutexLocker locker(&m_mutex);
    if (!m_currentFrame.isNull()) {
        painter.drawPixmap((width() - m_currentFrame.width()) / 2,
            (height() - m_currentFrame.height()) / 2,
            m_currentFrame);
    }
#endif
    QPixmap* pix = m_frameAtomic.load(); // 原子读取
    if (pix && !pix->isNull()) {
        painter.drawPixmap((width() - pix->width()) / 2,
            (height() - pix->height()) / 2,
            *pix);
    }
}

void VideoWidget::resizeEvent(QResizeEvent*)
{
    update();
}
