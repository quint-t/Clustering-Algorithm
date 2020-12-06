#include <point.h>

// Конструктор по умолчанию
Point::Point() { data_.squeeze(); }

// Конструктор по числу признаков
Point::Point(int nAttributes, int value) : data_(nAttributes, value) {
    data_.squeeze(); // Освободить лишнюю зарезервированную память
}

// Методы для получения значения первого признака
double &Point::front() { return data_.front(); }
const double &Point::front() const { return data_.front(); }

// Методы для получения значения последнего признака
double &Point::back() { return data_.back(); }
const double &Point::back() const { return data_.back(); }

// Методы для получения значения i-ого признака
// (без проверки выхода за пределы в соответствии с подходом STL)
double &Point::operator[](int i) { return data_[i]; }
const double &Point::operator[](int i) const { return data_[i]; }

// Метод для получения числа признаков
int Point::size() const { return data_.size(); }
// Метод для получения информации об отсутствии признаков
bool Point::empty() const { return data_.empty(); }

// Операторы сравнения "равно" и "не равно" двух точек
bool Point::operator==(const Point &arg) const { return data_ == arg.data_; }
bool Point::operator!=(const Point &arg) const { return data_ != arg.data_; }

// Получение итераторов по признакам
QVector<double>::iterator Point::begin() { return data_.begin(); }
QVector<double>::iterator Point::end() { return data_.end(); }
QVector<double>::const_iterator Point::begin() const { return data_.begin(); }
QVector<double>::const_iterator Point::end() const { return data_.end(); }
QVector<double>::const_iterator Point::cbegin() const { return data_.cbegin(); }
QVector<double>::const_iterator Point::cend() const { return data_.cend(); }
QVector<double>::reverse_iterator Point::rbegin() { return data_.rbegin(); }
QVector<double>::reverse_iterator Point::rend() { return data_.rend(); }
QVector<double>::const_reverse_iterator Point::rbegin() const {
    return data_.rbegin();
}
QVector<double>::const_reverse_iterator Point::rend() const {
    return data_.rend();
}
QVector<double>::const_reverse_iterator Point::crbegin() const {
    return data_.crbegin();
}
QVector<double>::const_reverse_iterator Point::crend() const {
    return data_.crend();
}

// Операции сложения, вычитания, умножения и деления точек
Point Point::operator+(const Point &arg) const {
    Point r(*this);
    auto p1 = r.begin();
    for (auto p2 = arg.begin(); p1 != r.end(); ++p1, ++p2) {
        *p1 += *p2;
    }
    return r;
}
Point Point::operator-() const { // Унарный минус
    Point r(*this);
    for (auto &x : r) {
        x = -x;
    }
    return r;
}
Point Point::operator-(const Point &arg) const { return *this + (-arg); }
Point Point::operator*(const Point &arg) const {
    Point r(*this);
    auto p1 = r.begin();
    for (auto p2 = arg.begin(); p1 != r.end(); ++p1, ++p2) {
        *p1 *= *p2;
    }
    return r;
}
Point Point::operator/(const Point &arg) const {
    Point r(*this);
    auto p1 = r.begin();
    for (auto p2 = arg.begin(); p1 != r.end(); ++p1, ++p2) {
        *p1 /= *p2;
    }
    return r;
}
Point &Point::operator+=(const Point &arg) { return *this = (*this + arg); }
Point &Point::operator-=(const Point &arg) { return *this = (*this - arg); }
Point &Point::operator*=(const Point &arg) { return *this = (*this * arg); }
Point &Point::operator/=(const Point &arg) { return *this = (*this / arg); }

// Операции умножения и деления значений признаков на число d
Point Point::operator*(double d) const {
    Point r(*this);
    for (auto &x : r) {
        x *= d;
    }
    return r;
}
Point Point::operator/(double d) const {
    Point r(*this);
    for (auto &x : r) {
        x /= d;
    }
    return r;
}
Point &Point::operator*=(double d) { return *this = (*this * d); }
Point &Point::operator/=(double d) { return *this = (*this / d); }

// Возведение в степень значений признаков
Point Point::pow(double arg) const {
    Point r(*this);
    for (double &x : r) {
        x = std::pow(x, arg);
    }
    return r;
}

// Расстояние Евклида между двумя точками
double Point::getEuclidDistance(const Point &arg) const {
    double r = 0;
    auto p1 = begin();
    for (auto p2 = arg.begin(); p1 != end(); ++p1, ++p2) {
        r += std::pow(*p2 - *p1, 2.0);
    }
    return std::sqrt(r);
}
