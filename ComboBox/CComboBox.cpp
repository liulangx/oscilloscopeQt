#include "CComboBox.h"
#include <QHeaderView>

CComboBox::CComboBox(QOpenglMain* _qOpenglMain, QWidget *parent)
    : QComboBox(parent),
      m_qOpenglMain(_qOpenglMain),
      m_tableWidget(new QTableWidget),
      m_colorDialog(new QColorDialog)
{
    //m_colorDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    setModel(m_tableWidget->model());
    setView(m_tableWidget);
    setEditable(false);
    setMinimumWidth(150);

    m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->horizontalHeader()->setVisible(false);
    m_tableWidget->setShowGrid(false);

    m_tableWidget->setColumnCount(2);
    m_tableWidget->setRowCount(0);
}

CComboBox::~CComboBox()
{

    delete m_colorDialog;
    for(QList<QCheckBox*>::iterator it = m_checkBoxs.begin(); it != m_checkBoxs.end(); ++it)
    {
        delete (*it);
    }
    for(QList<QPushButton*>::iterator it = m_pushButtons.begin(); it != m_pushButtons.end(); ++it)
    {
        delete (*it);
    }
    delete m_tableWidget;
}

void CComboBox::addNewItemFromOpenglMain(QString _name, QVariant _data, vector3f _color)
{
    m_bchecked.append(false);
    m_data.append(_data);

//    qDebug() << m_tableWidget->rowCount()+1;
    m_tableWidget->setRowCount(m_tableWidget->rowCount()+1);
    QCheckBox* checkBox = new QCheckBox(_name);
    checkBox->setStyleSheet("QCheckBox{color:rgb(" + QString::number(static_cast<int>(_color._x * 255.0)) + ", " + QString::number(static_cast<int>(_color._y * 255.0)) + ", " + QString::number(static_cast<int>(_color._z * 255.0)) + ")}");
    checkBox->setChecked(true);
    m_checkBoxs.append(checkBox);
    QPushButton* pushButton = new QPushButton("调色");
    QString tmpIndex = QString::number(_data.value<size_t>());
    pushButton->setObjectName(tmpIndex);
    m_pushButtons.append(pushButton);

//    qDebug() << m_tableWidget->rowCount();
    m_tableWidget->setCellWidget(m_tableWidget->rowCount()-1, 0, checkBox);
    m_tableWidget->setCellWidget(m_tableWidget->rowCount()-1, 1, pushButton);

    QObject::connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(setLineEnabled(int)));
    QObject::connect(pushButton, SIGNAL(clicked(bool)), this, SLOT(onColorDialog()));\

}

void CComboBox::setLineEnabled(int state)
{
    QCheckBox* box = qobject_cast<QCheckBox*>(sender());
    if(box)
    {
        for(int off = 0; off < m_checkBoxs.size(); ++off)
        {

            if(m_checkBoxs.at(off) == box)
            {

                m_bchecked[off] = (box->checkState() == Qt::Checked) ? true : false;

                size_t tmpLineIndex = m_data[off].value<size_t>();
                m_qOpenglMain->setLinesEnabled(tmpLineIndex, m_bchecked[off]);
            }
        }
    }
}

void CComboBox::onColorDialog()
{
//    QColor color = m_colorDialog->getColor(Qt::white, nullptr);
//    m_colorDialog->raise();
    QColor color = QColorDialog::getColor(Qt::white, qobject_cast<QWidget*>(parent()));

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        GLint indexInt = button->objectName().toInt();
        size_t indexUshort = 0;
        if (indexInt >= 0)
        {
            indexUshort = static_cast<size_t>(indexInt);
        }
        vector4f tmpColor = {color.redF(), color.greenF(), color.blueF(), 1.0f};

        m_qOpenglMain->setColor(indexUshort, tmpColor);
        QVariant dataIndex = QVariant::fromValue(indexUshort);
        for(int off(0); off < m_checkBoxs.size(); ++off)
        {
            if(m_data.at(off) == dataIndex)
            {
                m_checkBoxs.at(off)->setStyleSheet("QCheckBox{color:rgb(" + QString::number(static_cast<int>(tmpColor._r * 255.0)) + ", " + QString::number(static_cast<int>(tmpColor._g * 255.0)) + ", " + QString::number(static_cast<int>(tmpColor._b * 255.0)) + ")}");
            }
        }
    }
    else
    {

    }
}
