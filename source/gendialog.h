#ifndef GENDIALOG_H
#define GENDIALOG_H

#include <QDialog>

namespace Ui {
    class GenDialog;
}

class GenDialog : public QDialog {
    Q_OBJECT

  public:
    // Конструктор
    explicit GenDialog(QWidget *parent = 0);
    // Деструктор
    ~GenDialog();

    // Обновление информации на форме
    void updateInfo(int, int, qreal, qreal, qreal, qreal);

    // Получение числа точек
    int getNumberOfPoints();
    // Получение вида фигуры
    int getKind();
    // Получение диапазона по X
    QPair<qreal, qreal> getRangeX();
    // Получение диапазона по Y
    QPair<qreal, qreal> getRangeY();

  private:
    Ui::GenDialog *ui;
};

#endif // GENDIALOG_H
