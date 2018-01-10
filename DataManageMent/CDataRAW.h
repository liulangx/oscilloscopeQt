#ifndef CDATARAW_H
#define CDATARAW_H
#include "MyOpenglWidget/Clines.h"
#include <fstream>
#include <iostream>
#include <QString>
#include <sstream>
#include <vector>
#include <map>
#include <QTime>
#include <QMessageBox>

using namespace std;

class CDataRAW
{
public:
    CDataRAW();
    void setDir();
    void addNewImage(u_short _imgNumber);
    void addNewLineColor(u_short _imgNumber, u_short _lineNumber, vector4f _linecolor);
    void addNewLineRange(u_short _imgNumber, u_char _frameType, vector2f _xrange, vector2f _yrange, vector2f _zrange);
    void writeDataToFile(u_short _imgNumber, u_short _lineNumber, float _frame, float _x, float _y, float _z);
    void readDataFromFile(u_int _imgNumber, list<vector<float> > &_linesVbo, vector<size_t> &_linesAmount, map<u_short, vector4f> &_lineColors);
    void readLineRangeFromFile(u_short _imgNumber, u_char &_frameType, vector2f &_xrange, vector2f &_yrange, vector2f &_zrange);

    //从文件读取
    void setDir(QString _fileName);
    void writeDataToFileNoName(u_short _imgNumber, u_short _lineNumber, float _frame, float _x, float _y, float _z);
    void readDataFromFileNoName(u_int _imgNumber, list<vector<float> > &_linesVbo, vector<size_t> &_linesAmount, map<u_short, vector4f> &_lineColors);
    void readLineRangeFromFileNoName(u_short _imgNumber, u_char &_frameType, vector2f &_xrange, vector2f &_yrange, vector2f &_zrange);
private:
    ifstream    m_fin;
    ofstream    m_fout;
    map<u_short, QString>   m_fPointsNames;
    map<u_short, QString>   m_fColorNames;
    map<u_short, QString>   m_fRangeNames;

    QString                 m_currentTimeS;
    QString                 m_dir;

    //从文件读取
    QString                 m_dirFile;
};

#endif // CDATARAW_H
