#include "DataManageMent/CDataRAW.h"
#include <QDir>

//模板函数：将string类型变量转换为常用的数值类型（此方法具有普遍适用性）
template <class Type>
Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

CDataRAW::CDataRAW()
{
//    system("mkdir data");
    QString tmpDir = QDir::currentPath();
    QDir dir(tmpDir + QString("/data"));
    bool ok = false;
    if(!dir.exists())
    {
        ok = dir.mkdir(tmpDir + QString("/data"));
    }
    m_fPointsNames.clear();
    m_fColorNames.clear();
    m_fRangeNames.clear();
}

void CDataRAW::setDir()
{
    QString tmpDir = QDir::currentPath();

    QDateTime currentTime = QDateTime::currentDateTime();
    m_currentTimeS = currentTime.toString("yyyy_MM_dd_hh_mm_ss_zzz");
    m_dir = tmpDir + QString("/data/") + m_currentTimeS + "/";
    QDir dir(m_dir);
    bool ok = false;
    if(!dir.exists())
    {
        ok = dir.mkdir(m_dir);
    }


//    string mkdir = (QString("mkdir ") + m_dir).toStdString();
//    system(mkdir.c_str());
}

void CDataRAW::addNewImage(u_short _imgNumber)
{
//    QDateTime currentTime = QDateTime::currentDateTime();
//    m_currentTimeS = currentTime.toString("yyyy_MM_dd_hh_mm_ss_zzz_ddd");
//    m_dir = QString("data/") + m_currentTimeS + "/";
//    string mkdir = (QString("mkdir ") + m_dir).toStdString();
//    system(mkdir.c_str());
    QString tmpFileName = m_dir + "img_" + QString::number(_imgNumber) + ".dat";
    m_fPointsNames.insert(std::pair<u_short, QString>(_imgNumber, tmpFileName));
    m_fout.open(m_fPointsNames.at(_imgNumber).toStdString(), ios_base::out /*| ios_base::binary*/);
    m_fout << m_currentTimeS.toStdString() << std::endl;
    m_fout << "数据描述：点数据索引号，帧数，x,y,z" << std::endl;
    m_fout << "points: " << std::endl;
    m_fout.close();



    tmpFileName = m_dir + "img_" + QString::number(_imgNumber) + "_lineColor.datColor";
    m_fColorNames.insert(std::pair<u_short, QString>(_imgNumber, tmpFileName));
    m_fout.open(m_fColorNames.at(_imgNumber).toStdString(), ios_base::out);
    m_fout << "数据描述：线条索引号，颜色rgba值" << std::endl;
    m_fout << "line index and line color:" << std::endl;
    m_fout.close();


    tmpFileName = m_dir + "img_" + QString::number(_imgNumber) + "_lineRange.datRange";
    m_fRangeNames.insert(std::pair<u_short, QString>(_imgNumber, tmpFileName));
//    m_fout.open(m_fRangeNames.at(_imgNumber).toStdString(), ios_base::out);
//    m_fout << "range:" << std::endl;
//    m_fout.close();
}

void CDataRAW::addNewLineColor(u_short _imgNumber, u_short _lineNumber, vector4f _linecolor)
{
    m_fout.open(m_fColorNames.at(_imgNumber).toStdString(), ios_base::out | ios_base::app);
    m_fout << _lineNumber << " " << _linecolor._r << " " << _linecolor._g << " " << _linecolor._b << " " << _linecolor._a << endl;
    m_fout.close();
}

void CDataRAW::addNewLineRange(u_short _imgNumber, u_char _frameType, vector2f _xrange, vector2f _yrange, vector2f _zrange)
{
    m_fout.open(m_fRangeNames.at(_imgNumber).toStdString(), ios_base::out);
    m_fout << "数据描述：图像类型（3D：0/ 2D: 1）, x坐标上限，x坐标下限，y坐标上限，y坐标下限，z坐标上限,z坐标下限" << std::endl;
    m_fout << "range:" << std::endl;
    m_fout << _frameType <<  " " << _xrange._x << " " << _xrange._y << " " << _yrange._x << " " << _yrange._y << " " << _zrange._x << " " << _zrange._y << std::endl;
    m_fout.close();
}

void CDataRAW::writeDataToFile(u_short _imgNumber, u_short _lineNumber, float _frame, float _x, float _y, float _z)
{
    m_fout.open(m_fPointsNames.at(_imgNumber).toStdString(), ios_base::out | ios_base::app/* | ios_base::binary*/);
    if(!m_fout.is_open())
    {
        QMessageBox::information(NULL, m_fPointsNames.at(_imgNumber),
            "文件打开失败",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    else
    {
        //tmpFin
        m_fout << _lineNumber << " " << _frame << " " << _x << " " << _y << " " << _z << std::endl;
    }
    m_fout.close();
}

void CDataRAW::readDataFromFile(u_int _imgNumber, list<vector<float> >& _linesVbo, vector<size_t>& _linesAmount, map<u_short, vector4f>& _lineColors)
{
    map<u_short, vector<float>> tmpLineVbos;
    m_fin.open(m_fPointsNames.at(_imgNumber).toStdString(), ios_base::in);
    if(!m_fin.is_open())
    {
        QMessageBox::information(NULL, m_fPointsNames.at(_imgNumber),
            "文件打开失败",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    string ch;
    string mark1Sta = "points:";

    bool mark1 = 0;

    u_short lineNumber = 0;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float num = 0.0f;

    while(getline(m_fin, ch))
    {
        if((string::npos != ch.find(mark1Sta)) && (!mark1))
        {
            mark1 = true;
            continue;
        }

        if(mark1)
        {
            stringstream ss(ch);
            string subStr;
            getline(ss, subStr, ' ');
            lineNumber = stringToNum<u_short>(subStr);
            for(int i(0); i < 4; ++i)
            {
                getline(ss, subStr, ' ');
                num = stringToNum<float>(subStr);
                switch (i) {
                case 0:
                    break;
                case 1:
                    x = num;
                    break;
                case 2:
                    y = num;
                    break;
                case 3:
                    z = num;
                    break;
                }
            }
            if(tmpLineVbos.find(lineNumber) == tmpLineVbos.end())
            {
                vector<float> tmpLineVbo;
                tmpLineVbos.insert(std::pair<u_short, vector<float>>(lineNumber, tmpLineVbo));
            }
            tmpLineVbos.at(lineNumber).push_back(x);
            tmpLineVbos.at(lineNumber).push_back(y);
            tmpLineVbos.at(lineNumber).push_back(z);
        }
    }

    size_t tmpLineAmount;
    for(map<u_short, vector<float>>::iterator it = tmpLineVbos.begin(); it != tmpLineVbos.end(); ++it)
    {
        _linesVbo.push_back(it->second);
        tmpLineAmount = it->second.size() / 3;
        _linesAmount.push_back(tmpLineAmount);
    }

    m_fin.close();


//    map<u_short, vector4f> tmpLineColors;
    m_fin.open(m_fColorNames.at(_imgNumber).toStdString(), ios_base::in);
    if(!m_fin.is_open())
    {
        QMessageBox::information(NULL, m_fColorNames.at(_imgNumber),
            "文件打开失败",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }

    ch.clear();
    mark1Sta = "line index and line color:";

    mark1 = 0;

    lineNumber = 0;
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
    _lineColors.clear();

    while(getline(m_fin, ch))
    {
        if((string::npos != ch.find(mark1Sta)) && (!mark1))
        {
            mark1 = true;
            continue;
        }

        if(mark1)
        {
            stringstream ss(ch);
            string subStr;
            getline(ss, subStr, ' ');
            lineNumber = stringToNum<u_short>(subStr);
            for(int i(0); i < 4; ++i)
            {
                getline(ss, subStr, ' ');
                num = stringToNum<float>(subStr);
                switch (i) {
                case 0:
                    r = num;
                    break;
                case 1:
                    g = num;
                    break;
                case 2:
                    b = num;
                    break;
                case 3:
                    a = num;
                    break;
                }
            }
            if(_lineColors.find(lineNumber) == _lineColors.end())
            {
                vector4f tmpColor;
                tmpColor._r = r; tmpColor._g = g;
                tmpColor._b = b; tmpColor._a = a;
                _lineColors.insert(std::pair<u_short, vector4f>(lineNumber, tmpColor));
            }
        }
    }

    m_fin.close();

}

void CDataRAW::readLineRangeFromFile(u_short _imgNumber, u_char& _frameType, vector2f& _xrange, vector2f& _yrange, vector2f& _zrange)
{
    m_fin.open(m_fRangeNames.at(_imgNumber).toStdString(), ios_base::in);
    if(!m_fin.is_open())
    {
        QMessageBox::information(NULL, m_fRangeNames.at(_imgNumber),
            "文件打开失败",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    string ch;
    string mark1Sta = "range:";

    bool mark1 = 0;

    float num = 0.0f;

    while(getline(m_fin, ch))
    {
        if((string::npos != ch.find(mark1Sta)) && (!mark1))
        {
            mark1 = true;
            continue;
        }

        if(mark1)
        {
            stringstream ss(ch);
            string subStr;
            getline(ss, subStr, ' ');
            num = stringToNum<u_char>(subStr);
            _frameType = num;
            for(int i(0); i < 6; ++i)
            {
                getline(ss, subStr, ' ');
                num = stringToNum<float>(subStr);
                switch (i) {
                case 0:
                    _xrange._x = num;
                    break;
                case 1:
                    _xrange._y = num;
                    break;
                case 2:
                    _yrange._x = num;
                    break;
                case 3:
                    _yrange._y = num;
                    break;
                case 4:
                    _zrange._x = num;
                    break;
                case 5:
                    _zrange._y = num;
                    break;
                }
            }
        }
    }

    m_fin.close();
}

void CDataRAW::setDir(QString _fileName)
{
    m_dirFile = _fileName.split("img_").at(0);
}

void CDataRAW::writeDataToFileNoName(u_short _imgNumber, u_short _lineNumber, float _frame, float _x, float _y, float _z)
{
    QString fileName = m_dirFile + "img_" + QString::number(_imgNumber) + ".dat";
    m_fout.open(fileName.toStdString(), ios_base::out | ios_base::app/* | ios_base::binary*/);
    if(!m_fout.is_open())
    {
        QMessageBox::information(NULL, fileName,
            "文件打开失败",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    else
    {
        //tmpFin
        m_fout << _lineNumber << " " << _frame << " " << _x << " " << _y << " " << _z << std::endl;
    }
    m_fout.close();
}

void CDataRAW::readDataFromFileNoName(u_int _imgNumber, list<vector<float> > &_linesVbo, vector<size_t> &_linesAmount, map<u_short, vector4f> &_lineColors)
{
    QString fileName = m_dirFile + "img_" + QString::number(_imgNumber) + ".dat";
    map<u_short, vector<float>> tmpLineVbos;
    m_fin.open(fileName.toStdString(), ios_base::in);
    if(!m_fin.is_open())
    {
        QMessageBox::information(NULL, fileName,
            "文件打开失败",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    string ch;
    string mark1Sta = "points:";

    bool mark1 = 0;

    u_short lineNumber = 0;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float num = 0.0f;

    while(getline(m_fin, ch))
    {
        if((string::npos != ch.find(mark1Sta)) && (!mark1))
        {
            mark1 = true;
            continue;
        }

        if(mark1)
        {
            stringstream ss(ch);
            string subStr;
            getline(ss, subStr, ' ');
            lineNumber = stringToNum<u_short>(subStr);
            for(int i(0); i < 4; ++i)
            {
                getline(ss, subStr, ' ');
                num = stringToNum<float>(subStr);
                switch (i) {
                case 0:
                    break;
                case 1:
                    x = num;
                    break;
                case 2:
                    y = num;
                    break;
                case 3:
                    z = num;
                    break;
                }
            }
            if(tmpLineVbos.find(lineNumber) == tmpLineVbos.end())
            {
                vector<float> tmpLineVbo;
                tmpLineVbos.insert(std::pair<u_short, vector<float>>(lineNumber, tmpLineVbo));
            }
            tmpLineVbos.at(lineNumber).push_back(x);
            tmpLineVbos.at(lineNumber).push_back(y);
            tmpLineVbos.at(lineNumber).push_back(z);
        }
    }

    size_t tmpLineAmount;
    for(map<u_short, vector<float>>::iterator it = tmpLineVbos.begin(); it != tmpLineVbos.end(); ++it)
    {
        _linesVbo.push_back(it->second);
        tmpLineAmount = it->second.size() / 3;
        _linesAmount.push_back(tmpLineAmount);
    }

    m_fin.close();


//    map<u_short, vector4f> tmpLineColors;
    fileName = m_dirFile + "img_" + QString::number(_imgNumber) + "_lineColor.datColor";
    m_fin.open(fileName.toStdString(), ios_base::in);
    if(!m_fin.is_open())
    {
        QMessageBox::information(NULL, fileName,
            "文件打开失败",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }

    ch.clear();
    mark1Sta = "line index and line color:";

    mark1 = 0;

    lineNumber = 0;
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
    _lineColors.clear();

    while(getline(m_fin, ch))
    {
        if((string::npos != ch.find(mark1Sta)) && (!mark1))
        {
            mark1 = true;
            continue;
        }

        if(mark1)
        {
            stringstream ss(ch);
            string subStr;
            getline(ss, subStr, ' ');
            lineNumber = stringToNum<u_short>(subStr);
            for(int i(0); i < 4; ++i)
            {
                getline(ss, subStr, ' ');
                num = stringToNum<float>(subStr);
                switch (i) {
                case 0:
                    r = num;
                    break;
                case 1:
                    g = num;
                    break;
                case 2:
                    b = num;
                    break;
                case 3:
                    a = num;
                    break;
                }
            }
            if(_lineColors.find(lineNumber) == _lineColors.end())
            {
                vector4f tmpColor;
                tmpColor._r = r; tmpColor._g = g;
                tmpColor._b = b; tmpColor._a = a;
                _lineColors.insert(std::pair<u_short, vector4f>(lineNumber, tmpColor));
            }
        }
    }

    m_fin.close();
}

void CDataRAW::readLineRangeFromFileNoName(u_short _imgNumber, u_char& _frameType, vector2f &_xrange, vector2f &_yrange, vector2f &_zrange)
{
    QString fileName = m_dirFile + "img_" + QString::number(_imgNumber) + "_lineRange.datRange";
    m_fin.open(fileName.toStdString(), ios_base::in);
    if(!m_fin.is_open())
    {
        QMessageBox::information(NULL, fileName,
            "文件打开失败",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    string ch;
    string mark1Sta = "range:";

    bool mark1 = 0;

    float num = 0.0f;

    while(getline(m_fin, ch))
    {
        if((string::npos != ch.find(mark1Sta)) && (!mark1))
        {
            mark1 = true;
            continue;
        }

        if(mark1)
        {
            stringstream ss(ch);
            string subStr;
            getline(ss, subStr, ' ');
            num = stringToNum<u_char>(subStr);
            _frameType = num;
            for(int i(0); i < 6; ++i)
            {
                getline(ss, subStr, ' ');
                num = stringToNum<float>(subStr);
                switch (i) {
                case 0:
                    _xrange._x = num;
                    break;
                case 1:
                    _xrange._y = num;
                    break;
                case 2:
                    _yrange._x = num;
                    break;
                case 3:
                    _yrange._y = num;
                    break;
                case 4:
                    _zrange._x = num;
                    break;
                case 5:
                    _zrange._y = num;
                    break;
                }
            }
        }
    }

    m_fin.close();
}
