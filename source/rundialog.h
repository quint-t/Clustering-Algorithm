#ifndef RUNDIALOG_H
#define RUNDIALOG_H

#include "cluster.h"
#include <QDialog>
#include <QTextStream>
namespace Ui {
    class RunDialog;
}

// Класс "Диалоговое окно внутренних данных"
class RunDialog : public QDialog {
    Q_OBJECT

  public:
    // Конструктор
    explicit RunDialog(QWidget *parent = 0);

    // Деструктор
    ~RunDialog();

    // Обновление информации на форме
    void updateInfo(const QVector<Cluster> &, int, int, int, int);

    // Получение числа кластеров из поля ввода формы
    int getClusters() const;
    // Получение числа итераций из поля ввода формы
    int getIterations() const;
    // Получение номера признака для координаты X
    int getX() const;
    // Получение номера признака для координаты Y
    int getY() const;

  private:
    Ui::RunDialog *ui; // Интерфейс окна
};

#endif // RUNDIALOG_H
