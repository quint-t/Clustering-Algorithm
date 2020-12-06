#include "kmeans.h"

// Конструктор по числу кластеров, числу атрибутов и числу итераций
KMeansPlusPlus::KMeansPlusPlus(int nClusters, int nAttributes, int nIterations)
    : nClusters_(nClusters), nAttributes_(nAttributes),
      nIterations_(nIterations) {}

// Кластеризация (возвращает вектор кластеров и вектор центроидов)
QPair<QVector<Cluster>, QVector<Point>>
KMeansPlusPlus::compute(const Cluster &data, bool isPlusPlus) const {
    if (data.size() == 0)
        return {QVector<Cluster>(nClusters_), QVector<Point>()};
    // Вектор точек-центроидов
    QVector<Point> centroids(nClusters_, Point(nAttributes_));
    if (isPlusPlus) {
        // Инициализация K-Means++
        // 1. Выбираем в качестве первого центроида любую из точек входных
        // данных
        std::uniform_int_distribution<int> distN(0, data.size() - 1);
        centroids[0] = data[distN(rd)];
        // 2. Находим остальные центроиды
        qreal minDistanceFrom0 = calcDistance(data[0], centroids[0]);
        for (int i = 1; i < nClusters_; ++i) {
            // 2.1. Находим начальное значение для максимального расстояния
            // Находим расстояние от точки до ближайшего найденного центроида
            minDistanceFrom0 = std::min(
                minDistanceFrom0, calcDistance(data[0], centroids[i - 1]));
            int idx = 0; // Индекс максимально отдаленной точки
            // Расстояние до максимально отдаленной точки
            qreal maxDistance =
                minDistanceFrom0; // Начальное значение установлено
            // Теперь maxDistance можно сравнивать с остальными минимальными
            // значениями
            // 2.2. Находим maxDistance - максимальное из минимальных расстояний
            for (int j = 1, len = data.size(); j < len; ++j) {
                // Находим расстояние от точки до ближайшего найденного
                // центроида
                qreal minDistance = calcDistance(data[j], centroids[0]);
                for (int k = 1; k < i; ++k) {
                    qreal distance = calcDistance(data[j], centroids[k]);
                    if (minDistance > distance) {
                        minDistance = distance;
                    }
                }
                if (maxDistance < minDistance) {
                    maxDistance = minDistance;
                    // j-ая точка -- наиболее удаленная точка
                    idx = j;
                }
            }
            centroids[i] = data[idx]; // i-й центроид найден!
        }
    } else {
        // Инициализация K-Means
        QVector<int> numbers(data.size());
        std::iota(numbers.begin(), numbers.end(), 0);
        std::shuffle(numbers.begin(), numbers.end(), rd);
        for (int i = 0; i < nClusters_; ++i) {
            centroids[i] = data[numbers[i]];
        }
    }
    // Основной алгоритм K-Means
    // 1. Последовательно кластеризуем данные
    QVector<QVector<Point>> clusters(nClusters_);
    for (int iter = 0; iter < nIterations_; ++iter) {
        // Находим минимальную разницу для каждой точки и каждого кластера
        for (auto point : data) {
            qreal minDistance = calcDistance(point, centroids[0]);
            int clusterIdx = 0;
            for (int i = 1; i < nClusters_; ++i) {
                qreal currentDistance = calcDistance(point, centroids[i]);
                if (minDistance > currentDistance) {
                    minDistance = currentDistance;
                    clusterIdx = i;
                }
            }
            // Добавляем текущую точку к наиболее близкому кластеру
            clusters[clusterIdx].append(point);
        }
        // Вычисляем новые положения центроидов
        for (int i = 0; i < nClusters_; ++i) {
            centroids[i] = calcMean(clusters[i]);
            clusters[i].clear(); // Очищаем элементы кластера
        }
    }
    // Выполняем последний обход точек для определения минимальной разницы
    for (auto point : data) {
        qreal minDistance = calcDistance(point, centroids[0]);
        int clusterIdx = 0;
        for (int i = 1; i < nClusters_; ++i) {
            qreal currentDistance = calcDistance(point, centroids[i]);
            if (minDistance > currentDistance) {
                minDistance = currentDistance;
                clusterIdx = i;
            }
        }
        // Добавляем текущую точку к наиболее близкому кластеру
        clusters[clusterIdx].append(point);
    }
    // Вычисляем новые положения центроидов, не очищая элементы clusters
    for (int i = 0; i < nClusters_; ++i) {
        centroids[i] = calcMean(clusters[i]);
    }
    // Преобразование QVector<QVector<Point>> в QVector<Cluster>
    QVector<Cluster> result(nClusters_);
    auto it = result.begin();
    for (auto cluster : clusters) {
        *(it++) = std::move(cluster);
    }
    return {result, centroids};
}

// [private] Расстояние между двумя точками
qreal KMeansPlusPlus::calcDistance(const Point &point,
                                   const Point &mean) const {
    return point.getEuclidDistance(mean);
}

// [private] Точка со средними значениями признаков по всем точкам кластера
Point KMeansPlusPlus::calcMean(const Cluster &cluster) const {
    return cluster.sum() / cluster.size();
}
