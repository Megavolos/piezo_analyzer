#include "headers.h"

Scope::Scope()
{

}
Scope::~Scope()
{

}
void Scope::readHeaders(QStringList filelist)
{
    qint64 data;
    uchar filesCount=filelist.count();
    QByteArray temp;
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
        QFile file(filelist.at(i));
        if(file.open(QIODevice::ReadOnly))
        {
        file.seek(headers::ch1DataPresentOffset);
        ch1DataPresent.setBit(i,file.read(1).at(0));
        file.seek(headers::ch2DataPresentOffset);
        ch2DataPresent.setBit(i,file.read(1).at(0));
        file.seek(headers::numberOfPointsOffset);
        temp = file.read(4);
        for (unsigned int j=3; j>0; j--)
        {
            data|=temp.at(j);
            data<<=8;
        }
        numberOfPoints[i]=data;
        file.seek(headers::timeMultOffset);

        //ui->textBrowser->setText();
        file.close();
        }
    }
}
