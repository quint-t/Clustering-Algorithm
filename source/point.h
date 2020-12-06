#ifndef POINT_H
#define POINT_H

#include <QVector>
#include <cmath>

// Класс "Точка"
class Point {
  public:
    // Конструктор по умолчанию
    Point();
    // Конструктор по числу признаков
    Point(int nAttributes, int value = 0);

    // Методы для получения значения первого признака
    double &front();
    const double &front() const;

    // Методы для получения значения последнего признака
    double &back();
    const double &back() const;

    // Методы для получения значения i-ого признака
    // (без проверки выхода за пределы в соответствии с подходом STL)
    double &operator[](int i);
    const double &operator[](int i) const;

    // Метод для получения числа признаков
    int size() const;
    // Метод для получения информации об отсутствии признаков
    bool empty() const;

    // Операторы сравнения "равно" и "не равно" двух точек
    bool operator==(const Point &arg) const;
    bool operator!=(const Point &arg) const;

    // Получение итераторов по признакам
    QVector<double>::iterator begin();
    QVector<double>::iterator end();
    QVector<double>::const_iterator begin() const;
    QVector<double>::const_iterator end() const;
    QVector<double>::const_iterator cbegin() const;
    QVector<double>::const_iterator cend() const;
    QVector<double>::reverse_iterator rbegin();
    QVector<double>::reverse_iterator rend();
    QVector<double>::const_reverse_iterator rbegin() const;
    QVector<double>::const_reverse_iterator rend() const;
    QVector<double>::const_reverse_iterator crbegin() const;
    QVector<double>::const_reverse_iterator crend() const;

    // Операции сложения, вычитания, умножения и деления точек
    Point operator+(const Point &arg) const;
    Point operator-() const; // Унарный минус: смена знака значений признаков
    Point operator-(const Point &arg) const;
    Point operator*(const Point &arg) const;
    Point operator/(const Point &arg) const;
    Point &operator+=(const Point &arg);
    Point &operator-=(const Point &arg);
    Point &operator*=(const Point &arg);
    Point &operator/=(const Point &arg);
    // Операции умножения и деления значений признаков на число d
    Point operator*(double d) const;
    Point operator/(double d) const;
    Point &operator*=(double d);
    Point &operator/=(double d);

    // Возведение в степень значений признаков
    Point pow(double arg) const;
    // Евклидово расстояние между двумя точками
    double getEuclidDistance(const Point &arg) const;

  private:
    // Вектор N признаков
    QVector<double> data_;
};

#endif // POINT_H
