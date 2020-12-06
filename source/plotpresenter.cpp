#include "plotpresenter.h"

// Конструктор
PlotPresenter::PlotPresenter(PlotModel &pm, QCustomPlot &qcp)
    : plotModel_(&pm), customPlot_(&qcp) {}

// Добавление нового кластера в i-ую позицию
void PlotPresenter::addCluster(int i, const Cluster &arg, int nX, int nY) {
    int nClusters = plotModel_->countOfClusters();
    int nAttributes = plotModel_->countOfAttributes();
    if (nClusters == 0 ||
        (nClusters != 0 && nAttributes == arg.front().size())) {
        removeCentroids();
        plotModel_->addCluster(i, arg);
        customPlot_->addGraph();
        QVector<qreal> x, y;
        for (auto &attr : arg) {
            x.append(attr[nX]);
            y.append(attr[nY]);
        }
        customPlot_->graph()->addData(x, y);
        customPlot_->replot();
    } else {
        QMessageBox::critical(customPlot_, "Ошибка",
                              "Число признаков текущего объекта: " +
                                  QString::number(arg.size()) +
                                  "\nНеобходимое число признаков: " +
                                  QString::number(nAttributes));
    }
}

// Добавление нового кластера в конец
void PlotPresenter::addCluster(const Cluster &arg, int nX, int nY) {
    addCluster(plotModel_->countOfClusters(), arg, nX, nY);
}

// Объединение всех кластеров
void PlotPresenter::combineAll(int nX, int nY) {
    QVector<Point> points;
    for (auto cluster : data()) {
        for (auto point : cluster) {
            points.append(point);
        }
    }
    clearAll();
    addCluster(Cluster(points), nX, nY);
    colorizeAll();
}

// Получение числа кластеров
int PlotPresenter::countOfClusters() const {
    return plotModel_->countOfClusters();
}

// Получение числа признаков
int PlotPresenter::countOfAttributes() const {
    return plotModel_->countOfAttributes();
}

// Удаление i-ого кластера
void PlotPresenter::removeCluster(int i) {
    if (plotModel_->countOfClusters() != 0) {
        removeCentroids();
        plotModel_->removeCluster(i);
        customPlot_->removeGraph(i);
        customPlot_->replot();
    } else {
        QMessageBox::critical(
            customPlot_, "Ошибка",
            QString::number(i) +
                " кластер не существует. Удаление невозможно.");
    }
}

// Удаление последнего кластера
void PlotPresenter::removeCluster() {
    removeCluster(plotModel_->countOfClusters() - 1);
}

// Удаление всех кластеров
void PlotPresenter::clearAll() {
    removeCentroids();
    plotModel_->removeAll();
    customPlot_->clearGraphs();
    customPlot_->replot();
}

// Кластеризация
QPair<QVector<Cluster>, QVector<Point>>
PlotPresenter::clusterData(int nClusters, int nIterations,
                           bool isPlusPlus) const {
    if (nClusters == 0 || nIterations == 0)
        return QPair<QVector<Cluster>, QVector<Point>>();
    QVector<Point> points;
    for (auto cluster : data()) {
        for (auto point : cluster) {
            points.append(point);
        }
    }
    if (points.size() == 0) {
        QMessageBox::critical(customPlot_, "Ошибка", "Число точек равно нулю");
        return QPair<QVector<Cluster>, QVector<Point>>();
    }
    if (points.size() < nClusters) {
        QMessageBox::critical(customPlot_, "Ошибка",
                              "Число объектов должно быть больше или равно "
                              "числу кластеров!\nЧисло объектов: " +
                                  QString::number(points.size()) +
                                  "\nЗапрашиваемое число кластеров: " +
                                  QString::number(nClusters));
        return QPair<QVector<Cluster>, QVector<Point>>();
    }
    KMeansPlusPlus km(nClusters, this->countOfAttributes(), nIterations);
    return km.compute(points, isPlusPlus);
}

// Добавление кластера центроидов (не имеет значения для модели)
void PlotPresenter::addCentroids(const QVector<Point> &data, int attr1,
                                 int attr2) {
    hasCentroids_ = true;
    customPlot_->addGraph();
    customPlot_->graph()->setPen(QPen(QBrush(Qt::black), 0, Qt::SolidLine));
    customPlot_->graph()->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssDiamond, centroidSize));
    for (auto point : data) {
        customPlot_->graph()->addData(point[attr1], point[attr2]);
    }
    customPlot_->replot();
}

// Удаление кластера центроидов (не имеет значения для модели)
void PlotPresenter::removeCentroids() {
    if (hasCentroids_) {
        customPlot_->removeGraph(customPlot_->graphCount() - 1);
        hasCentroids_ = false;
    }
}

// Установка цвета фона графика
void PlotPresenter::setBackgroundColor(const QBrush &arg) {
    customPlot_->setBackground(arg);
    customPlot_->replot();
}

// Установка стиля
void PlotPresenter::setStyle(int i, const QBrush &brush, int size) {
    customPlot_->graph(i)->setPen(QPen(brush, 0, Qt::NoPen));
    customPlot_->graph(i)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssDisc, size));
    customPlot_->replot();
}

// Раскраска всех кластеров в разные цвета
void PlotPresenter::colorizeAll(
    const QVector<QPair<QPen, QCPScatterStyle>> *colors) {
    if (colors) {
        for (int i = 0, len = data().size(); i < len; ++i) {
            customPlot_->graph(i)->setPen((*colors)[i].first);
            customPlot_->graph(i)->setScatterStyle((*colors)[i].second);
        }
    } else {
        for (int i = 0, len = data().size(); i < len; ++i) {
            setStyle(i, QBrush(QColor().fromHsv(randInt(Hmin, Hmax),
                                                randInt(Smin, Smax),
                                                randInt(Vmin, Vmax))));
        }
    }
    customPlot_->replot();
}

// Сохранение графика в файл PDF
bool PlotPresenter::savePdf(const QString &filename) const {
    return customPlot_->savePdf(filename);
}

// Сохранение графика в файл PNG
bool PlotPresenter::savePng(const QString &filename) const {
    return customPlot_->savePng(filename);
}

// Сохранение графика в файл JPG
bool PlotPresenter::saveJpg(const QString &filename) const {
    return customPlot_->saveJpg(filename);
}

// Получение данных
const QVector<Cluster> &PlotPresenter::data() const {
    return plotModel_->data();
}

// Установка данных
void PlotPresenter::setData(const QVector<Cluster> &newData, int nX, int nY) {
    if (&newData != &data()) {
        clearAll();
        for (auto cluster : newData) {
            addCluster(cluster, nX, nY);
        }
    } else {
        setData(QVector<Cluster>(data()), nX, nY);
    }
}

// Стандартизация по Z-оценке
void PlotPresenter::zStd(int nX, int nY) {
    removeCentroids();
    auto clustersColors = getClustersColors();
    QVector<Cluster> newData(data());
    for (auto &cluster : newData) {
        int nAttributes = plotModel_->countOfAttributes();
        Point mean(nAttributes), sigma(nAttributes);
        for (auto point : cluster) {
            mean += point;
        }
        mean /= cluster.size();
        for (auto point : cluster) {
            sigma += (point - mean).pow(2);
        }
        sigma = (sigma / (cluster.size() - (cluster.size() != 1))).pow(0.5);
        for (auto attr : sigma) {
            if (attr == 0) {
                QMessageBox::critical(customPlot_, "Внимание!",
                                      "Размах значений одного из "
                                      "признаков близок к нулю, поэтому "
                                      "стандартизация выполнена не будет.");
                return;
            }
        }
        for (auto &point : cluster) {
            point = (point - mean) / sigma;
        }
    }
    setData(newData, nX, nY);
    colorizeAll(&clustersColors);
    customPlot_->replot();
}

// Стандартизация по среднеквадратическому отклонению
void PlotPresenter::sigmaStd(int nX, int nY) {
    removeCentroids();
    auto clustersColors = getClustersColors();
    QVector<Cluster> newData(data());
    for (auto &cluster : newData) {
        int nAttributes = plotModel_->countOfAttributes();
        Point mean(nAttributes), sigma(nAttributes);
        for (auto point : cluster) {
            mean += point;
        }
        mean /= cluster.size();
        for (auto point : cluster) {
            sigma += (point - mean).pow(2);
        }
        sigma = (sigma / (cluster.size() - (cluster.size() != 1))).pow(0.5);
        for (auto attr : sigma) {
            if (attr == 0) {
                QMessageBox::critical(customPlot_, "Внимание!",
                                      "Размах значений одного из "
                                      "признаков близок к нулю, поэтому "
                                      "стандартизация выполнена не будет.");
                return;
            }
        }
        for (auto &point : cluster) {
            point /= sigma;
        }
    }
    setData(newData, nX, nY);
    colorizeAll(&clustersColors);
    customPlot_->replot();
}

// Стандартизация по минимальному и максимальному значениям
void PlotPresenter::minMaxStd(int nX, int nY) {
    removeCentroids();
    auto clustersColors = getClustersColors();
    QVector<Cluster> newData(data());
    for (auto &cluster : newData) {
        int nAttributes = plotModel_->countOfAttributes();
        Point minPoint(nAttributes), maxPoint(nAttributes);
        for (auto point : cluster) {
            for (int attr = 0; attr < nAttributes; ++attr) {
                auto pointAttr = point[attr];
                minPoint[attr] = std::min(minPoint[attr], pointAttr);
                maxPoint[attr] = std::max(maxPoint[attr], pointAttr);
            }
        }
        Point minMaxPoint(maxPoint - minPoint);
        for (auto attr : minMaxPoint) {
            if (attr == 0) {
                QMessageBox::critical(customPlot_, "Внимание!",
                                      "Размах значений одного из "
                                      "признаков близок к нулю, поэтому "
                                      "стандартизация выполнена не будет.");
                return;
            }
        }
        for (auto &point : cluster) {
            point = (point - minPoint) / minMaxPoint;
        }
    }
    setData(newData, nX, nY);
    colorizeAll(&clustersColors);
    customPlot_->replot();
}

// [protected] Функция, возвращающая вектор стилей каждого кластера
QVector<QPair<QPen, QCPScatterStyle>> PlotPresenter::getClustersColors() {
    QVector<QPair<QPen, QCPScatterStyle>> clustersColors(data().size());
    for (int i = 0, len = data().size(); i < len; ++i) {
        clustersColors[i] =
            QPair<QPen, QCPScatterStyle>(customPlot_->graph(i)->pen(),
                                         customPlot_->graph(i)->scatterStyle());
    }
    return clustersColors;
}

// [protected] Генерация случайного целого числа [a, b]
int PlotPresenter::randInt(int a, int b) { return qrand() % (b - a + 1) + a; }
