#ifndef HEADERS_H
#define HEADERS_H
#define ch1DataPresentOffset            49
#define ch2DataPresentOffset            73
#define numberOfPointsOffset            28
#define timeMultOffset                  84
#define delayOffset                     92
#define sampleRateOffset                100
#define triggerModeOffset               142
#define triggerSourceOffset             142

#define ch1ProbeDivOffset               46
#define ch1VerticalScaleOffset          36
#define ch1VerticalPositionOffset       40
#define ch2ProbeDivOffset               70
#define ch2VerticalScaleOffset          60
#define ch2VerticalPositionOffset       64

#define dataBeginOffset                 271

#include <QStringList>
#include <QBitArray>
#include <QVector>
class Headers
{
public:
    Headers();
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


    ~Headers();

};

#endif // HEADERS_H
