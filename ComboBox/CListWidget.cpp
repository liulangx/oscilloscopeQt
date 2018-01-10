#include "ComboBox/CListWidget.h"
CListWidget::CListWidget(QOpenglMain *_qOpenglMain, QComboBox *cmb, QWidget *parent)
    : QListWidget(parent),
      m_cmbox(cmb),
      m_qOpenglMain(_qOpenglMain)
{
    setViewMode(QListWidget::ListMode);
    setSelectionMode(QAbstractItemView::SingleSelection);
    m_cmbox = cmb;
}

void CListWidget::addNewItemFromOpenglMain(QString _name, QVariant _data, vector3f _color)
{
    m_bchecked.append(false);
    m_data.append(_data);
    QTableWidgetItem * tableWidget = new QTableWidget(1, 1);

    QListWidgetItem *item = new QListWidgetItem();
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(Qt::UserRole, _data);
    insertItem(model()->rowCount(), item);

    QCheckBox* box = new QCheckBox(_name);
    box->setStyleSheet("QCheckBox{color:rgb(" + QString::number(static_cast<int>(_color._x * 255.0)) + ", " + QString::number(static_cast<int>(_color._y * 255.0)) + ", " + QString::number(static_cast<int>(_color._z * 255.0)) + ")}");
    box->setChecked(true);
    
    //tableWidget->set
    
    
    setItemWidget(item, box);
    m_listItems.append(item);
    m_checklist.append(box);
    m_fslist.append(_name);
    QObject::connect(box, SIGNAL(stateChanged(int)), this, SLOT(setLinesEnable(int)));

}

void CListWidget::setLinesEnable(int state)
{
    QCheckBox* box = qobject_cast<QCheckBox*>(sender());
    if(box)
    {
        for(int off = 0; off < m_checklist.size(); ++off)
        {

            if(m_checklist.at(off) == box)
            {

                m_bchecked[off] = (box->checkState() == Qt::Checked) ? true : false;
                qDebug() << "1";
                size_t tmpLineIndex = m_listItems[off]->data(Qt::UserRole).value<size_t>();
                qDebug() << tmpLineIndex << " " << m_bchecked[off];
                //qDebug() << m_qOpenglMain;
                m_qOpenglMain->setLinesEnabled(tmpLineIndex, m_bchecked[off]);
                qDebug() << "2";
//                break;
            }
        }
    }
}
