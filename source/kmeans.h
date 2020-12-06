#ifndef KMEANS_H
#define KMEANS_H

#include "cluster.h"
#include <QPair>
#include <QVector>
#include <random>

// Класс "K-Means алгоритм"
class KMeansPlusPlus {
  public:
    // Конструктор по числу кластеров, числу атрибутов и числу итераций
    KMeansPlusPlus(int nClusters, int nAttributes, int nIterations);

    // Кластеризация (возвращает вектор кластеров и вектор центроидов)
    QPair<QVector<Cluster>, QVector<Point>> compute(const Cluster &,
                                                    bool) const;

  protected:
    // Расстояние между двумя точками
    qreal calcDistance(const Point &, const Point &) const;

    // Точка со средними значениями признаков по всем точкам кластера
    Point calcMean(const Cluster &) const;

  private:
    // Число кластеров
    int nClusters_;
    // Число атрибутов
    int nAttributes_;
    // Число итераций
    int nIterations_;

    // Недетерминированный генератор случайных чисел с использованием
    // аппаратного источника энтропии
    mutable std::random_device rd;
};

#endif // KMEANS_H
