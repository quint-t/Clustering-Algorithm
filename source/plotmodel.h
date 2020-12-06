#ifndef PLOTMODEL_H
#define PLOTMODEL_H

#include "cluster.h"
#include <QVector>

// Класс "Модель"
class PlotModel {
  public:
    // Добавление кластеров
    // Добавление нового кластера в i-ую позицию
    void addCluster(int, const Cluster &);
    // Добавление нового кластера в конец
    void addCluster(const Cluster &);
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
    void removeAll();

    // Получение данных
    const QVector<Cluster> &data() const;

  private:
    // Список из кластеров точек
    QVector<Cluster> data_;
};

#endif // PLOTMODEL_H
