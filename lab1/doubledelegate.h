#pragma once
#include <qwidget>
#include <qitemdelegate>

class DoubleDelegate : public QItemDelegate {
public:
    DoubleDelegate(QWidget *parent);
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public slots:
    void changed(QString);
};

