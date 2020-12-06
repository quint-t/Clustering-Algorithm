#ifndef PLOTPRESENTER_H
#define PLOTPRESENTER_H

#include "kmeans.h"
#include "plotmodel.h"
#include "point.h"
#include "qcustomplot.h"

// Класс "Представитель"
class PlotPresenter {
  public:
    // Конструктор
    PlotPresenter(PlotModel &, QCustomPlot &);

    // Добавление кластеров
    // Добавление нового кластера в i-ую позицию
    void addCluster(int, const Cluster &, int, int);
    // Добавление нового кластера в конец
    void addCluster(const Cluster &, int, int);
    // Объединение всех кластеров
    void combineAll(int nX, int nY);
    // Получение числа кластеров
    int countOfClusters() const;
    // Получение числа признаков
    int countOfAttributes() const;

    // Удаление кластеров
    // Удаление i-ого кластера
    void removeCluster(int);
    // Удаление последнего кластера
    void removeCluster();
    // Удаление всех кластеров
    void clearAll();

    // Кластеризация
    QPair<QVector<Cluster>, QVector<Point>> clusterData(int, int, bool) const;

    // Добавление кластера центроидов (не имеет значения для модели)
    void addCentroids(const QVector<Point> &, int, int);
    // Удаление кластера центроидов (не имеет значения для модели)
    void removeCentroids();

    // Настройки отображения
    // Установка цвета фона графика
    void setBackgroundColor(const QBrush &);
    // Установка стиля
    void setStyle(int, const QBrush &, int size = 10);
    // Раскраска всех кластеров в разные цвета
    void
    colorizeAll(const QVector<QPair<QPen, QCPScatterStyle>> *colors = nullptr);

    // Сохранение графика в файл
    // Сохранение графика в файл PDF
    bool savePdf(const QString &) const;
    // Сохранение графика в файл PNG
    bool savePng(const QString &) const;
    // Сохранение графика в файл JPG
    bool saveJpg(const QString &) const;

    // Получение данных
    const QVector<Cluster> &data() const;
    // Установка данных
    void setData(const QVector<Cluster> &, int, int);

    // Стандартизация
    // Стандартизация по Z-оценке
    void zStd(int, int);
    // Стандартизация по среднеквадратическому отклонению
    void sigmaStd(int, int);
    // Стандартизация по минимальному и максимальному значениям
    void minMaxStd(int, int);

    // Генерация случайного целого числа [a, b]
    int randInt(int, int);

  protected:
    // Диапазоны цветов HSV для кластеров
    // Hue, Saturation, Value - Тон, Насыщенность, Значение (Яркость)
    const int Hmin = 0, Hmax = 359, Smin = 230, Smax = 255, Vmin = 230,
              Vmax = 255;

    // Размер центроида
    int centroidSize = 20;
    // Функция, возвращающая вектор стилей каждого кластера
    QVector<QPair<QPen, QCPScatterStyle>> getClustersColors();

  private:
    // Модель
    PlotModel *plotModel_ = nullptr;
    // Вид
    QCustomPlot *customPlot_ = nullptr;
    // Есть ли центроиды кластеров на графике или нет
    bool hasCentroids_ = false;
};

#endif // PLOTPRESENTER_H
