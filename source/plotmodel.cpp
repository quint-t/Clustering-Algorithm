#include "plotmodel.h"

// Добавление нового кластера в i-ую позицию
void PlotModel::addCluster(int i, const Cluster &arg) {
    if (i >= 0 && i <= data_.size())
        data_.insert(i, arg);
}

// Добавление нового кластера в конец
void PlotModel::addCluster(const Cluster &arg) { data_.append(arg); }

// Получение числа кластеров
int PlotModel::countOfClusters() const { return data_.size(); }

// Получение числа признаков
int PlotModel::countOfAttributes() const {
    return (!data_.empty()) ? data_.front().front().size() : 0;
}

// Удаление i-ого кластера
void PlotModel::removeCluster(int i) {
    if (i >= 0 && i < data_.size())
        data_.remove(i);
}

// Удаление последнего кластера
void PlotModel::removeCluster() {
    if (!data_.empty())
        data_.pop_back();
}

// Удаление всех кластеров
void PlotModel::removeAll() { data_.clear(); }

// Получение данных
const QVector<Cluster> &PlotModel::data() const { return data_; }
