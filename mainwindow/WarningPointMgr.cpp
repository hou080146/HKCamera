#include "WarningPointMgr.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <cmath>

WarningPointMgr::WarningPointMgr(QObject* parent)
    : QObject(parent)
{
}

WarningPointMgr::~WarningPointMgr()
{
}

bool WarningPointMgr::loadWarningPoints(const QString& csvFilePath)
{
    QFile file(csvFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开预警点CSV文件:" << csvFilePath;
        return false;
    }

    m_warningPoints.clear();
    QTextStream in(&file);
    in.setCodec("UTF-8");  // 设置编码为UTF-8

    // 跳过表头
    QString header = in.readLine();

    int lineNum = 1;
    while (!in.atEnd()) {
        QString line = in.readLine();
        lineNum++;

        if (line.trimmed().isEmpty()) {
            continue;  // 跳过空行
        }

        QStringList fields = line.split(',');
        if (fields.size() < 5) {
            qDebug() << "CSV文件第" << lineNum << "行格式错误，字段数不足";
            continue;
        }

        WarningPoint point;
        point.line = fields[0].trimmed();
        point.section = fields[1].trimmed();
        point.upOrDown = fields[2].trimmed();

        // 解析预警点百米标（去除k3+194中的"k"和"+"）
        QString hectometerStr = fields[3].trimmed();
        hectometerStr = hectometerStr.replace("k", "").replace("K", "").replace("+", "");
        point.hectometer = hectometerStr.toDouble();

        // 解析里程
        point.mileage = fields[4].trimmed().toDouble();

        m_warningPoints.append(point);
        qDebug() << "加载预警点:" << point.line << point.section
            << point.upOrDown << point.hectometer << point.mileage;
    }

    file.close();

    // 初始化触发标志
    m_triggeredFlags.resize(m_warningPoints.size());
    resetTriggerFlags();

    qDebug() << "成功加载" << m_warningPoints.size() << "个预警点";
    return true;
}

void WarningPointMgr::checkWarningPoint(double currentMileage, const QString& currentUpOrDown)
{
    // 将里程从公里转换为米
    double currentMileageInMeters = currentMileage * 1000.0;

    for (int i = 0; i < m_warningPoints.size(); ++i) {
        const WarningPoint& point = m_warningPoints[i];

        // 检查行别是否匹配
        if (!currentUpOrDown.isEmpty() && point.upOrDown != currentUpOrDown) {
            continue;
        }

        // 计算与预警点的距离（米）
        double pointMileageInMeters = point.mileage * 1000.0;
        double distance = pointMileageInMeters - currentMileageInMeters;

        // 检查是否在预警范围内（前方0-30米）
        if (distance > 0 && distance <= WARNING_DISTANCE) {
            // 检查该预警点是否已触发过
            if (!m_triggeredFlags[i]) {
                m_triggeredFlags[i] = true;
                emit warningTriggered(point, distance);
                qDebug() << "触发预警！距离预警点" << distance << "米";
            }
        }
        // 如果已经过了预警点（距离为负），重置触发标志
        else if (distance < -10.0) {  // 过了预警点10米后重置
            m_triggeredFlags[i] = false;
        }
    }
}

void WarningPointMgr::resetTriggerFlags()
{
    for (int i = 0; i < m_triggeredFlags.size(); ++i) {
        m_triggeredFlags[i] = false;
    }
}