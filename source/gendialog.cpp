#include "gendialog.h"
#include "ui_gendialog.h"

GenDialog::GenDialog(QWidget *parent) : QDialog(parent), ui(new Ui::GenDialog) {
    ui->setupUi(this);
    this->setFixedSize(this->size().rwidth(), this->size().rheight());
}

GenDialog::~GenDialog() { delete ui; }

// Обновление информации на форме
void GenDialog::updateInfo(int nPoints, int kind, qreal xFrom, qreal xTo,
                           qreal yFrom, qreal yTo) {
    ui->spinPoints->setValue(nPoints);
    ui->comboKind->setCurrentIndex(kind);
    ui->spinXfrom->setValue(xFrom);
    ui->spinXto->setValue(xTo);
    ui->spinYfrom->setValue(yFrom);
    ui->spinYto->setValue(yTo);
}

// Получение числа точек
int GenDialog::getNumberOfPoints() { return ui->spinPoints->value(); }

// Получение вида фигуры
int GenDialog::getKind() { return ui->comboKind->currentIndex(); }

// Получение диапазона по X
QPair<qreal, qreal> GenDialog::getRangeX() {
    return QPair<qreal, qreal>(ui->spinXfrom->value(), ui->spinXto->value());
}

// Получение диапазона по Y
QPair<qreal, qreal> GenDialog::getRangeY() {
    return QPair<qreal, qreal>(ui->spinYfrom->value(), ui->spinYto->value());
}
