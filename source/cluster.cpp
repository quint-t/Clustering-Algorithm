#include <cluster.h>

// Конструктор по умолчанию
Cluster::Cluster() { data_.squeeze(); }

// Конструктор по числу точек и числу признаков
Cluster::Cluster(int nPoints, int nAttributes, int value)
    : data_(nPoints, Point(nAttributes, value)) {
    data_.squeeze(); // Освободить лишнюю зарезервированную память
}

// Конструктор по вектору точек
// (точки должны иметь одинаковое число признаков, иначе исключение
// std::invalid_argument)
Cluster::Cluster(const QVector<Point> &arg) : data_(arg) {
    if (!data_.empty()) {
        auto nAttributes = data_.front().size();
        for (auto &point : data_) {
            if (point.size() != nAttributes) {
                throw std::invalid_argument(
                    "QVector points have a different number of attributes!");
            }
        }
        data_.squeeze();
    }
}

// Оператор приведения кластера к вектору точек
Cluster::operator QVector<Point>() const { return data_; }

// Методы для получения первой точки кластера
Point &Cluster::front() { return data_.front(); }
const Point &Cluster::front() const { return data_.front(); }

// Методы для получения последней точки кластера
Point &Cluster::back() { return data_.back(); }
const Point &Cluster::back() const { return data_.back(); }

// Методы для получения i-ой точки кластера
// (без проверки выхода за пределы в соответствии с подходом STL)
Point &Cluster::operator[](int i) { return data_[i]; }
const Point &Cluster::operator[](int i) const { return data_[i]; }

// Метод для получения числа точек кластера
int Cluster::size() const { return data_.size(); }

// Метод для получения информации об отсутствии точек в кластере
bool Cluster::empty() const { return data_.empty(); }

// Операторы сравнения "равно" и "не равно" двух кластеров
bool Cluster::operator==(const Cluster &arg) const {
    return data_ == arg.data_;
}
bool Cluster::operator!=(const Cluster &arg) const {
    return data_ != arg.data_;
}

// Получение итераторов по точкам
QVector<Point>::iterator Cluster::begin() { return data_.begin(); }
QVector<Point>::iterator Cluster::end() { return data_.end(); }
QVector<Point>::const_iterator Cluster::begin() const { return data_.begin(); }
QVector<Point>::const_iterator Cluster::end() const { return data_.end(); }
QVector<Point>::const_iterator Cluster::cbegin() const {
    return data_.cbegin();
}
QVector<Point>::const_iterator Cluster::cend() const { return data_.cend(); }
QVector<Point>::reverse_iterator Cluster::rbegin() { return data_.rbegin(); }
QVector<Point>::reverse_iterator Cluster::rend() { return data_.rend(); }
QVector<Point>::const_reverse_iterator Cluster::rbegin() const {
    return data_.rbegin();
}
QVector<Point>::const_reverse_iterator Cluster::rend() const {
    return data_.rend();
}
QVector<Point>::const_reverse_iterator Cluster::crbegin() const {
    return data_.crbegin();
}
QVector<Point>::const_reverse_iterator Cluster::crend() const {
    return data_.crend();
}

// Прямая сумма всех точек кластера
Point Cluster::sum() const {
    if (empty())
        return Point();
    Point r(data_.first().size());
    for (auto &point : data_) {
        r += point;
    }
    return r;
}
