#ifndef HEADERS_H
#define HEADERS_H

#include <QStringList>
#include <QBitArray>
#include <QVector>
#include <QFile>
#include <QDataStream>
enum offsets{
ch1DataPresentOffset=49,
ch2DataPresentOffset=73,
numberOfPointsOffset=28,
timeMultOffset=84,
delayOffset=92,
sampleRateOffset=100,
triggerModeOffset=142,
triggerSourceOffset=142,

ch1ProbeDivOffset=46,
ch1VerticalScaleOffset=36,
ch1VerticalPositionOffset=40,
ch2ProbeDivOffset=70,
ch2VerticalScaleOffset=60,
ch2VerticalPositionOffset=64,

dataBeginOffset=272

} ;

class Scope
{
public:
    Scope();
    QStringList         fileNames;
    QBitArray           ch1DataPresent;
    QBitArray           ch2DataPresent;
    QVector<quint32>    numberOfPoints;
    QVector<quint64>    timeMult;
    QVector<qint64>     delay;
    QVector<qreal>      sampleRate;
    QVector<quint16>    triggerMode;
    QVector<quint8>     triggerSource;

    QVector<quint16>    ch1ProbeDiv;
    QVector<quint32>    ch1VerticalScale;
    QVector<qint16>     ch1VerticalPosition;
    QVector<quint16>    ch2ProbeDiv;
    QVector<quint32>    ch2VerticalScale;
    QVector<qint16>     ch2VerticalPosition;
    void readHeaders();

    ~Scope();
private:
    qint64 getSignedData(QFile*,int offset, int length);
    quint64 getUnsignedData(QFile*, int offset, int length);
    float getFloatData(QFile*, int offset, int length);
};

#endif // HEADERS_H
