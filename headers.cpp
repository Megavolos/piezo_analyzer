#include "headers.h"

Scope::Scope()
{

}
Scope::~Scope()
{

}
qint64 Scope::getSignedData(QFile* file, int offset, int length)
{
   qint64 data=0;
   unsigned char c=0;
   QByteArray temp;
   file->seek(offset);
   temp = file->read(length);

   for (unsigned int i=length-1; i>0; i--)
   {
       c=temp.at(i);
       data|=c;
       data=data<<8;
   }
   data|=temp.at(0);
   return data;
}
float Scope::getFloatData(QFile* file, int offset, int length)
{
    float data=0;
    QByteArray temp;
    file->seek(offset);
    temp = file->read(length);
    QDataStream ds(&temp, QIODevice::ReadOnly);
    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds>>data;
    return data;
}

quint64 Scope::getUnsignedData(QFile* file, int offset, int length)
{
   quint64 data=0;
   unsigned char c=0;
   QByteArray temp;
   file->seek(offset);
   temp = file->read(length);

   for (unsigned int i=length-1; i>0; i--)
   {
       c=temp.at(i);
       data|=c;
       data=data<<8;
   }
   data|=temp.at(0);
   return data;
}

void Scope::readHeaders()
{
    QDataStream stream;
    stream.setByteOrder(QDataStream::LittleEndian);
    uchar filesCount=fileNames.count();
    quint64 o=0;
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
            stream>>timeMult[i];
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
            stream>>ch1VerticalScale[i];
            file.seek(ch1VerticalScaleOffset);
            stream>>ch1VerticalPosition[i];
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
            stream>>ch2VerticalScale[i];
            file.seek(ch2VerticalPositionOffset);
            stream>>ch2VerticalPosition[i];
            file.close();
        }
    }
}
