// Изначально хотел создать отдельный класс для заголовков файлов. В идеале - переработать данный класс (scope), разбив
// на разные классы, или переименовав все как-то более логичней
#ifndef HEADERS_H
#define HEADERS_H

#include <QStringList>      //класс для списков имен файлов
#include <QBitArray>        //класс для списка присутствующих каналов (1 - канал включен, 0 - выключен)
#include <QVector>          //класс для создания векторов (аля массивы)
#include <QFile>            //класс для работы с файлами
#include <QDataStream>      //класс для облегчения чтения данных из файлов

//ниже смещения для заголовка wfm файла
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
    QStringList         fileNames;              //содержит имена файлов, данные которых нужно отобразить на графике
    QBitArray           ch1DataPresent;         //канал 1 включен?
    QBitArray           ch2DataPresent;         //канал 2 включен?
    QVector<quint32>    numberOfPoints;         //кол-во семплов в файле
    QVector<qreal>      timeMult;               //всю дальнеюшую инфу можно найти на https://goo.gl/tTfIZH
    QVector<qint64>     delay;
    QVector<qreal>      sampleRate;
    QVector<quint16>    triggerMode;
    QVector<quint8>     triggerSource;

    QVector<quint16>    ch1ProbeDiv;
    QVector<qreal>      ch1VerticalScale;
    QVector<qreal>      ch1VerticalPosition;
    QVector<quint16>    ch2ProbeDiv;
    QVector<qreal>      ch2VerticalScale;
    QVector<qreal>      ch2VerticalPosition;
    QVector<qreal>      data;                   //содержит в себе прочитанные из файла данные для одного канала
    void readHeaders();                         //функция, читающая заголовки файлов
    QVector<qreal> recalcSamples(int fileIndex, int channel);       //пересчитывает семплы в вольты
    QVector<qreal> filter(QVector<qreal> *in, qreal coeff );        //ФНЧ на базе экспоненциально скользящего среднего
    QVector<qreal> integrate(QVector<qreal> *in, qreal zeroLevel); //Интегрирование с отсечкой по уровню.
    ~Scope();
private:

};

#endif // HEADERS_H
