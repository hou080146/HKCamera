#pragma once
#include <QObject>
#include <QVector>
#include <QString>

// 预警点数据结构
struct WarningPoint {
    QString line;           // 线路
    QString section;        // 区间
    QString upOrDown;      // 行别（上/下）
    double hectometer;      // 预警点百米标值
    double mileage;         // 里程
};

class WarningPointMgr : public QObject
{
    Q_OBJECT

public:
    explicit WarningPointMgr(QObject* parent = nullptr);
    ~WarningPointMgr();

    // 加载CSV文件
    bool loadWarningPoints(const QString& csvFilePath);

    // 检查当前里程是否接近预警点（30米内）
    void checkWarningPoint(double currentMileage, const QString& currentUpOrDown);

    // 获取所有预警点
    QVector<WarningPoint> getAllWarningPoints() const { return m_warningPoints; }

signals:
    // 触发预警信号
    void warningTriggered(const WarningPoint& point, double distance);

private:
    QVector<WarningPoint> m_warningPoints;
    QVector<bool> m_triggeredFlags;  // 记录哪些预警点已触发，防止重复提示
    const double WARNING_DISTANCE = 30.0;  // 预警距离（米）

    void resetTriggerFlags();
};
