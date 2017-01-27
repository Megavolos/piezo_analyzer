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
    uchar filesCount=fileNames.count();

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
            file.seek(headers::ch1DataPresentOffset);
            ch1DataPresent.setBit(i,file.read(1).at(0));
            file.seek(headers::ch2DataPresentOffset);
            ch2DataPresent.setBit(i,file.read(1).at(0));
            file.seek(headers::numberOfPointsOffset);
    //чтение и перебор в обратном порядке.
            numberOfPoints[i]=getSignedData(&file,headers::numberOfPointsOffset,32/8);
            timeMult[i]=getUnsignedData(&file,headers::timeMultOffset,64/8);
            delay[i]=getSignedData(&file,headers::delayOffset,64/8);
            sampleRate[i]=getFloatData(&file,headers::sampleRateOffset,32/8);
            triggerMode[i]=getUnsignedData(&file,headers::triggerModeOffset,16/8);
            triggerSource[i]=getUnsignedData(&file,headers::triggerSourceOffset,8/8);
            ch1ProbeDiv[i]=getUnsignedData(&file,headers::ch1ProbeDivOffset,16/8);
            ch1VerticalScale[i]=getUnsignedData(&file,headers::ch1VerticalScaleOffset,32/8);
            ch1VerticalPosition[i]=getSignedData(&file,headers::ch1VerticalPositionOffset,16/8);
            ch2ProbeDiv[i]=getUnsignedData(&file,headers::ch2ProbeDivOffset,16/8);
            ch2VerticalScale[i]=getUnsignedData(&file,headers::ch2VerticalScaleOffset,32/8);
            ch2VerticalPosition[i]=getSignedData(&file,headers::ch2VerticalPositionOffset,16/8);
            file.close();
        }
    }
}
