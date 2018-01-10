#ifndef CLCOMBOBOX_H
#define CLCOMBOBOX_H

#include <QCheckBox>
#include <QColorDialog>
#include <QPushButton>
#include <QHBoxLayout>

class CLCheckBox : public QWidget
{
public:
    explicit CLCheckBox(QString _name, QWidget *parent = 0);
    void setCheckBoxStyleSheet(QString _style);
    void setChecked(bool _b);
    QCheckBox *getCheckBox() const;
protected:
    QCheckBox *m_checkBox;
    QPushButton *m_color;
    QHBoxLayout *m_hbox;
};

#endif // CLCOMBOBOX_H
