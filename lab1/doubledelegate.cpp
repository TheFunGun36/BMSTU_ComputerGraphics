#include "doubledelegate.h"
#include <qlineedit>

void DoubleDelegate::changed(QString value) {
    QLineEdit *lineEdit = static_cast<QLineEdit *>(sender());
    emit commitData(lineEdit);
    //emit closeEditor(lineEdit);
}

DoubleDelegate::DoubleDelegate(QWidget *parent)
    : QItemDelegate(parent) {
}

void DoubleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
    lineEdit->setText(value);
}

void DoubleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
    QString value;

    value = lineEdit->text();
    if (value.length() == 0)
        value = "0";
    model->setData(index, value, Qt::DisplayRole);
}

QWidget *DoubleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    auto validator = new QDoubleValidator(parent);
    validator->setNotation(QDoubleValidator::ScientificNotation);
    validator->setLocale(QLocale::C);
    editor->setValidator(validator);
    editor->setPlaceholderText("0");
    editor->setText("0");
    connect(editor, &QLineEdit::textChanged, this, &DoubleDelegate::changed);
    return editor;
}

void DoubleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString str = index.data().toString();
    if (str.isEmpty())
        str = "0";
    drawDisplay(painter, option, option.rect, str);
    //drawFocus(painter, option, option.rect);
}
