#include "headers.h"

Scope::Scope()
{
    filter_store=0;
}
Scope::~Scope()
{
ssssss
}



QVector<qreal> Scope::filter(QVector<qreal> *in, qreal coeff )              //in - вход фильтра, coeff - коэф.фильтра от 0 до 1
{
    QVector<qreal> out(in->size());                                         //Выход фильтра
    for (int i=1; i<in->size(); i++)
    {
        out[i]=coeff*in->at(i) + (1.0-coeff)*out.at(i-1);                   //сам фильтр
    }
    return out;

}

QVector<qreal> Scope::filter_rs232(QVector<qreal> *in, qreal coeff )              //in - вход фильтра, coeff - коэф.фильтра от 0 до 1
{
    QVector<qreal> out(in->size());                                         //Выход фильтра
    out[0]=filter_store;
    for (int i=1; i<in->size(); i++)
    {
        out[i]=coeff*in->at(i) + (1.0-coeff)*out.at(i-1);                   //сам фильтр
    }
    filter_store=out.at(out.size()-1);
    return out;

}

void Scope::readHeaders()
{
    QDataStream stream;
    stream.setByteOrder(QDataStream::LittleEndian);
    uchar filesCount=fileNames.count();
    quint32 scale=0;
    qint16 pos=0;
    quint64 divx=0;
    ch1DataPresent.resize(filesCount);
    ch2DataPresent.resize(filesCount);
    numberOfPoints.resize(filesCount);
    timeMult.resize(filesCount);
    delay.resize(filesCount);
    sampleRate.resize(filesCount);
    triggerMode.resize(filesCount);
    triggerSource.resize(filesCount);

    ch1ProbeDiv.resize(filesCount);
    ch1VerticalScale.resize(filesCount);
    ch1VerticalPosition.resize(filesCount);
    ch2ProbeDiv.resize(filesCount);
    ch2VerticalScale.resize(filesCount);
    ch2VerticalPosition.resize(filesCount);
    for (int i=0;i<filesCount;i++)
    {
        QFile file(fileNames.at(i));
        if(file.open(QIODevice::ReadOnly))
        {
            stream.setDevice(&file);

            file.seek(ch1DataPresentOffset);
            ch1DataPresent.setBit(i,file.read(1).at(0));
            file.seek(ch2DataPresentOffset);
            ch2DataPresent.setBit(i,file.read(1).at(0));

            file.seek(numberOfPointsOffset);
            stream>>numberOfPoints[i];
            file.seek(timeMultOffset);
            stream>>divx;
            timeMult[i]=divx*0.000000000001;
            file.seek(delayOffset);
            stream>>delay[i];
            stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
            file.seek(sampleRateOffset);
            stream>>sampleRate[i];
            file.seek(triggerModeOffset);
            stream>>triggerMode[i];
            file.seek(triggerSourceOffset);
            stream>>triggerSource[i];
            file.seek(ch1ProbeDivOffset);
            stream>>ch1ProbeDiv[i];
            switch (ch1ProbeDiv[i]) {
            case 0x3F80:
                ch1ProbeDiv[i]=1;
                break;
            case 0x4120:
                ch1ProbeDiv[i]=10;
                break;
            case 0x42C8:
                ch1ProbeDiv[i]=100;
                break;
            case 0x447A:
                ch1ProbeDiv[i]=1000;
                break;
            }
            file.seek(ch1VerticalScaleOffset);
            stream>>scale;
            ch1VerticalScale[i]=scale*ch1ProbeDiv.at(i)*0.000001;
            file.seek(ch1VerticalPositionOffset);
            stream>>pos;
            ch1VerticalPosition[i]=(pos/(25.0*ch1ProbeDiv.at(i)))*ch1VerticalScale.at(i);
            file.seek(ch2ProbeDivOffset);
            stream>>ch2ProbeDiv[i];
            switch (ch2ProbeDiv[i]) {
            case 0x3F80:
                ch2ProbeDiv[i]=1;
                break;
            case 0x4120:
                ch2ProbeDiv[i]=10;
                break;
            case 0x42C8:
                ch2ProbeDiv[i]=100;
                break;
            case 0x447A:
                ch2ProbeDiv[i]=1000;
                break;
            }
            file.seek(ch2VerticalScaleOffset);
            stream>>scale;
            ch2VerticalScale[i]=scale*ch2ProbeDiv.at(i)*0.000001;
            file.seek(ch2VerticalPositionOffset);
            stream>>pos;
            ch2VerticalPosition[i]=(pos/(25.0*ch2ProbeDiv.at(i)))*ch2VerticalScale.at(i);
            file.close();
        }
    }
}

QVector<qreal> Scope::recalcSamples(int fileIndex, int channel)
{
    QDataStream stream;
    int size=numberOfPoints.at(fileIndex);

    QVector<quint8> yData(size);
    data.clear();
    data.resize(size);
    QFile file(fileNames.at(fileIndex));
    if(file.open(QIODevice::ReadOnly))
    {
        stream.setDevice(&file);
        if (channel==1)
        {
            file.seek(272);     //отсюда начинаются данные 1 канала
            for (unsigned int i=0; i<numberOfPoints.at(fileIndex);i++)
            {
                stream>>yData[i];

                data[i]= ch1VerticalScale.at(fileIndex)*((((float)(128-(yData[i])))/256*10)) - ch1VerticalPosition.at(fileIndex); //хитрая , но правильная формула расчета значений в Вольтах.

            }
            return data;
        }
        if (channel==2)
        {
            file.seek(272+numberOfPoints.at(fileIndex));    //отсюда начинаются данные 2 канала
            for (unsigned int i=0; i<numberOfPoints.at(fileIndex);i++)
            {
                stream>>yData[i];

                data[i]= ch2VerticalScale.at(fileIndex)*((((float)(128-(yData[i])))/256*10)) - ch2VerticalPosition.at(fileIndex);

            }
            return data;
        }

    }
    data.clear();
    return data;
}
QVector<qreal> Scope::integrate(QVector<qreal> *in, qreal zeroLevel)
{
    QVector<qreal> out(in->size());
    for (int i=1; i<in->size(); i++)
    {
        if (in->at(i)>zeroLevel)
        {
            out[i]=in->at(i) + out.at(i-1);
        }
        else
        {
            out[i]=0;
        }
    }
    return out;
}
