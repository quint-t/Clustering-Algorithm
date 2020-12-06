#ifndef CLUSTER_H
#define CLUSTER_H

#include "point.h"
#include <QVector>

// Класс "Кластер"
class Cluster {
  public:
    // Конструктор по умолчанию
    Cluster();
    // Конструктор по числу точек и числу признаков
    Cluster(int nPoints, int nAttributes, int value = 0);
    // Конструктор по вектору точек
    // (точки должны иметь одинаковое число признаков, иначе исключение
    // std::invalid_argument)
    Cluster(const QVector<Point> &);

    // Оператор приведения кластера к вектору точек
    operator QVector<Point>() const;

    // Методы для получения первой точки кластера
    Point &front();
    const Point &front() const;

    // Методы для получения последней точки кластера
    Point &back();
    const Point &back() const;

    // Методы для получения i-ой точки кластера
    // (без проверки выхода за пределы в соответствии с подходом STL)
    Point &operator[](int i);
    const Point &operator[](int i) const;

    // Метод для получения числа точек кластера
    int size() const;
    // Метод для получения информации об отсутствии точек в кластере
    bool empty() const;

    // Операторы сравнения "равно" и "не равно" двух кластеров
    bool operator==(const Cluster &) const;
    bool operator!=(const Cluster &) const;

    // Получение итераторов по точкам
    QVector<Point>::iterator begin();
    QVector<Point>::iterator end();
    QVector<Point>::const_iterator begin() const;
    QVector<Point>::const_iterator end() const;
    QVector<Point>::const_iterator cbegin() const;
    QVector<Point>::const_iterator cend() const;
    QVector<Point>::reverse_iterator rbegin();
    QVector<Point>::reverse_iterator rend();
    QVector<Point>::const_reverse_iterator rbegin() const;
    QVector<Point>::const_reverse_iterator rend() const;
    QVector<Point>::const_reverse_iterator crbegin() const;
    QVector<Point>::const_reverse_iterator crend() const;

    // Прямая сумма всех точек кластера
    Point sum() const;

  private:
    // Вектор M точек
    QVector<Point> data_;
};

#endif // CLUSTER_H
