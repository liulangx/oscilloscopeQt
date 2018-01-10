#ifndef CCOMBOBOX_H
#define CCOMBOBOX_H

#include "MyOpenglWidget/Clines.h"
#include "MyOpenglWidget/QOpenglMain.h"
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QTableWidget>
#include <QList>
#include <QColorDialog>

class CComboBox : public QComboBox
{
    Q_OBJECT
public:
    //构造函数，传入与其相关的openglwidget的指针
    explicit CComboBox(QOpenglMain* _qOpenglMain, QWidget *parent = 0);
    //析构函数
    ~CComboBox();
    //添加新线条的索引checkbox和调色按钮的函数
    void addNewItemFromOpenglMain(QString _name, QVariant _data, vector3f _color);
private slots:
    //点击checkbox后的槽函数
    void setLineEnabled(int state);
    //点击调色的pushbutton后的槽函数
    void onColorDialog();

private:
    //由外部传来的openglwidget指针，不能在本类中删除
    QOpenglMain *m_qOpenglMain;
    //tableWidget类
    QTableWidget *m_tableWidget;
    //checkbox类的list
    QList<QCheckBox*> m_checkBoxs;
    //QPushButton类的list
    QList<QPushButton*> m_pushButtons;
    //QString类的list，保存的是checkbox类的text内容
    QList<QString>  m_fslist;
    //QVariant类的list，保存的是checkbox类的线段索引号（size_t）
    QList<QVariant> m_data;
    //checkbox的bool状态
    QList<bool>     m_bchecked;
    //QColorDialog，用于更改线条的颜色
    QColorDialog*    m_colorDialog;
};

#endif // CCOMBOBOX_H
