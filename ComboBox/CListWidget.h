#ifndef CLISTWIDGET_H
#define CLISTWIDGET_H

#include "MyOpenglWidget/Clines.h"
#include "MyOpenglWidget/QOpenglMain.h"
#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>
#include <QTableWidget>
#include <QStringList>
#include <QString>

class CListWidget : public QListWidget
{
    Q_OBJECT    //自定义信号槽时需加上该宏，否则链接信号槽将会失败


public:
    /*
        columnlabels  QCheckBox名称
        data          QListWidgetItem的用户自定义数据
        cmb           添加列表项的QComboBox,此处主要用于及时的将用户选择的结果显示出来
    */
    CListWidget(QOpenglMain* _qOpenglMain, QComboBox* cmb=NULL, QWidget* parent=0);
    void addNewItemFromOpenglMain(QString _name, QVariant _data, vector3f _color);



private slots:
    void setLinesEnable(int state);
//public:
//    //返回用户选择项的用户自定义数据
//    void get_select_data(QList<QVariant> &data);

//private slots:
//    //QCheckBox复选消息处理
//    void set_select_item(int state);



private:
    QComboBox         *m_cmbox;
    QOpenglMain      *m_qOpenglMain;
    QList<bool>       m_bchecked;
    QList<QVariant>   m_data;
    QList<QCheckBox*> m_checklist;
    QList<QListWidgetItem*> m_listItems;
    QList<QString>    m_fslist;
    QList<QPushButton*> m_buttonlist;
    QList<QTableWidgetItem*> m_tableItems;

};

#endif //CLISTWIDGET_H
