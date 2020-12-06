#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cluster.h"
#include "gendialog.h"
#include "plotmodel.h"
#include "plotpresenter.h"
#include "rundialog.h"
#include <QMainWindow>
#include <QVector>
#include <random>

namespace Ui {
    class MainWindow;
}

// Класс "Главное окно"
class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    // Конструктор
    explicit MainWindow(QWidget *parent = 0);
    // Деструктор
    ~MainWindow();

  protected:
    // Диапазоны цветов HSV для кластеров
    // Hue, Saturation, Value - Тон, Насыщенность, Значение (Яркость)
    const int Hmin = 0, Hmax = 359, Smin = 230, Smax = 255, Vmin = 230,
              Vmax = 255;

    // Событие изменения размера MainWindow
    virtual void resizeEvent(QResizeEvent *);

    // Генерация случайного целого числа [a, b]
    int randInt(int, int);

  private slots:

    // Генерация исходных данных
    void on_genData_triggered();

    // Сохранение графика в PNG формате
    void on_saveAsPNG_triggered();

    // Сохранение графика в JPG формате
    void on_saveAsJPG_triggered();

    // Сохранение графика в PDF формате
    void on_saveAsPDF_triggered();

    // Выход
    void on_quit_triggered();

    // Кластеризация данных K-Means
    void on_clusterDataRnd_triggered(bool isPlusPlus);

    // Кластеризация данных K-Means++
    void on_clusterDataPlusPlus_triggered();

    // О программе
    void on_about_triggered();

    // Краткое описание программы
    void on_description_triggered();

    // Открытие файла с данными
    void on_loadData_triggered();

    // Сохранение внутренних данных в файл
    void on_saveData_triggered();

    // Отображение внутренних данных
    bool on_showData_triggered(bool bNoWarning);

    // Стандартизация по Z-оценке
    void on_stdZ_triggered();

    // Стандартизация по среднеквадратическому отклонению
    void on_stdSigma_triggered();

    // Стандартизация по минимальному и максимальному значениям
    void on_stdMinMax_triggered();

    // Объединение всех кластеров
    void on_combineData_triggered();

    // Очистка всех данных
    void on_clearData_triggered();

  private:
    RunDialog runDialog; // Диалоговое окно просмотра данных
    GenDialog genDialog; // Диалоговое окно генерации данных
    Ui::MainWindow *ui;           // Интерфейс окна
    PlotModel plotModel;          // Модель
    PlotPresenter *plotPresenter; // Представитель
    // Число кластеров, число итераций, номер признака для X координаты, номер
    // признака для Y координаты
    int nClusters_ = 0, nIterations_ = 1000, nX_ = 0, nY_ = 1;
    // Недетерминированный генератор случайных чисел с использованием
    // аппаратного источника энтропии
    mutable std::random_device rd;
};

#endif // MAINWINDOW_H
