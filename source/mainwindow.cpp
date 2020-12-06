#include "mainwindow.h"
#include "ui_mainwindow.h"

// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // Определение представителя
    QCustomPlot *customPlot = ui->graphWidget;
    plotPresenter = new PlotPresenter(plotModel, *customPlot);
    // Включение режима взаимодействия с графиком
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                QCP::iSelectPlottables | QCP::iMultiSelect);
    // Отображение делений правой и верхней оси
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    // Удаление числовых меток правой и верхней оси
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);
    // Передача диапазонов левой и нижней оси на правую и верхнюю оси
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
            customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),
            customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // Установка фона графика в белый
    customPlot->setBackground(QBrush(QColor(Qt::white)));
    // Установка начальных диапазонов
    customPlot->xAxis->setRangeLower(0);
    customPlot->xAxis->setRangeUpper(10);
    customPlot->yAxis->setRangeLower(0);
    customPlot->yAxis->setRangeUpper(10);
    // Перерисовка графика
    customPlot->replot();
    // Переустановка глобального генератора случайных чисел
    qsrand(QTime().secsTo(QTime::currentTime()));
}

// Генерация случайного целого числа [a, b]
int MainWindow::randInt(int a, int b) { return plotPresenter->randInt(a, b); }

// Событие изменения размера MainWindow
void MainWindow::resizeEvent(QResizeEvent *) {
    // Пропорциональное отображение данных (убирает растяжение графика)
    QCPAxis *x, *y;
    x = ui->graphWidget->axisRect()->axis(QCPAxis::atBottom);
    y = ui->graphWidget->axisRect()->axis(QCPAxis::atLeft);
    x->setScaleRatio(y, 1.0);
    ui->graphWidget->replot();
}

// Деструктор
MainWindow::~MainWindow() { delete ui; }

// Генерация исходных данных
void MainWindow::on_genData_triggered() {
    // Запускаем диалоговое окно
    QCPRange rx = ui->graphWidget->xAxis->range(),
             ry = ui->graphWidget->yAxis->range();
    genDialog.updateInfo(genDialog.getNumberOfPoints(), genDialog.getKind(),
                         rx.lower, rx.upper, ry.lower, ry.upper);
    if (genDialog.exec() != GenDialog::Accepted) {
        return;
    }
    int nPoints = genDialog.getNumberOfPoints();
    if (nPoints == 0)
        return;
    int isCircle = (genDialog.getKind() == 0);
    QPair<qreal, qreal> rangeX = genDialog.getRangeX(),
                        rangeY = genDialog.getRangeY();
    if (rangeX.first > rangeX.second || rangeY.first > rangeY.second) {
        QMessageBox::critical(this, "Ошибка!",
                              "Нижние границы должны быть не больше верхних!");
        return;
    }
    // Добавляем кластер с точками
    Cluster cluster(nPoints, /* nAttributes */ 2);
    if (isCircle) {
        double xMean = rangeX.first + (rangeX.second - rangeX.first) / 2,
               yMean = rangeY.first + (rangeY.second - rangeY.first) / 2;
        // 6 = 3 (правило трех сигм) * 2 (половина от диапазона)
        double xSigma = (rangeX.second - rangeX.first) / 6,
               ySigma = (rangeY.second - rangeY.first) / 6;
        std::normal_distribution<qreal> distX(xMean, xSigma),
            distY(yMean, ySigma);
        for (int i = 0; i < nPoints; ++i) {
            cluster[i][/* id X */ 0] = distX(rd);
            cluster[i][/* id Y */ 1] = distY(rd);
            // Выход за диапазон нежелателен, поэтому ставим ограничения
            if (cluster[i][0] < rangeX.first) {
                cluster[i][0] = rangeX.first;
            }
            if (cluster[i][0] > rangeX.second) {
                cluster[i][0] = rangeX.second;
            }
            if (cluster[i][1] < rangeY.first) {
                cluster[i][1] = rangeY.first;
            }
            if (cluster[i][1] > rangeY.second) {
                cluster[i][1] = rangeY.second;
            }
        }
    } else {
        std::uniform_real_distribution<qreal> distX(rangeX.first,
                                                    rangeX.second),
            distY(rangeY.first, rangeY.second);
        for (int i = 0; i < nPoints; ++i) {
            cluster[i][/* id X */ 0] = distX(rd);
            cluster[i][/* id Y */ 1] = distY(rd);
        }
    }
    plotPresenter->addCluster(cluster, nX_, nY_);
    plotPresenter->setStyle(
        plotPresenter->countOfClusters() - 1,
        QBrush(QColor().fromHsv(randInt(Hmin, Hmax), randInt(Smin, Smax),
                                randInt(Vmin, Vmax))));
}

// Сохранение графика в PNG формате
void MainWindow::on_saveAsPNG_triggered() {
    plotPresenter->savePng(QFileDialog::getSaveFileName(
        this, "Сохранить как...", ".", "PNG files (*.png)"));
}

// Сохранение графика в JPG формате
void MainWindow::on_saveAsJPG_triggered() {
    plotPresenter->saveJpg(QFileDialog::getSaveFileName(
        this, "Сохранить как...", ".", "JPG files (*.jpg)"));
}

// Сохранение графика в PDF формате
void MainWindow::on_saveAsPDF_triggered() {
    plotPresenter->savePdf(QFileDialog::getSaveFileName(
        this, "Сохранить как...", ".", "PDF files (*.pdf)"));
}

// Выход
void MainWindow::on_quit_triggered() { QCoreApplication::quit(); }

// Кластеризация данных
void MainWindow::on_clusterDataRnd_triggered(bool isPlusPlus) {
    if (!on_showData_triggered(true)) { // Если нажата кнопка отмены
        return;
    }
    auto result =
        plotPresenter->clusterData(nClusters_, nIterations_, isPlusPlus);
    if (result.second.empty()) // Если центроидов нет, то нет и кластеров
        return;
    // Обновление данных (старые удаляются, новые добавляются)
    plotPresenter->setData(result.first, nX_, nY_);
    for (int i = 0, cnt = plotPresenter->countOfClusters(); i < cnt; ++i) {
        plotPresenter->setStyle(
            i, QBrush(QColor().fromHsv(randInt(Hmin, Hmax), randInt(Smin, Smax),
                                       randInt(Vmin, Vmax))));
    }
    // Добавление центроидов
    plotPresenter->addCentroids(result.second, nX_, nY_);
}

void MainWindow::on_clusterDataPlusPlus_triggered() {
    on_clusterDataRnd_triggered(true);
}

// О программе
void MainWindow::on_about_triggered() {
    QMessageBox::information(this, "О программе", "Практика в СПбГУТ 2020.\n"
                                                  "Студент: Коваленко Леонид.\n"
                                                  "C++, QT, QCustomPlot.");
}

// Краткое описание программы
void MainWindow::on_description_triggered() {
    QMessageBox::information(
        this, "Краткое описание программы",
        "Программа для кластеризации данных.\nУправление "
        "осуществляется при помощи интуитивно понятной "
        "панели меню.\nОсновное:\n- Файл исходных данных "
        "CSV содержит строки, содержащие значения "
        "признаков безымянных объектов через запятую или точку с запятой. "
        "Комментарии идут после знака косой черты '/'.\n- Данные могут быть "
        "загружены или сгенерированы, а впоследствии сохранены, но не могут "
        "быть отредактированы в самой программе.\n- Стандартизация происходит "
        "для каждого кластера отдельно.\n- Кластеризация предполагает "
        "предварительное объединение всех кластеров в один.\n- "
        "Кластеризация проходит по всем признакам.\n- На графике изображаются "
        "точки данных по двум признакам (первому X и второму Y/... - "
        "настраивается в 'Посмотреть внутренние данные').\n- Цвета кластеров "
        "генерируются случайно.\n- Мера расстояния: Евклидово расстояние.");
}

// Открытие CSV-файла данных
void MainWindow::on_loadData_triggered() {
    // Открываем файл для чтения
    int nAttributes = -1, linePos = 0;
    QString filename = QFileDialog::getOpenFileName(this, "Открыть csv-файл",
                                                    ".", "CSV files (*.csv)");
    if (filename.size() == 0) // Если была нажата кнопка отмены
        return;
    QFile fileIn(filename);
    if (fileIn.exists() && fileIn.open(QIODevice::ReadOnly)) {
        QTextStream qts(&fileIn);
        QVector<Point> points;
        while (!qts.atEnd()) {
            QString line = qts.readLine();
            ++linePos;
            int pos = line.indexOf('/');
            if (pos >= 0) {
                line = line.left(pos);
            }
            line = line.simplified();
            QStringList s = line.split(QRegExp("[;,]+"));
            if (line.size() == 0)
                continue;
            if (nAttributes == -1)
                nAttributes = s.size();
            if (nAttributes == s.size()) {
                Point point(nAttributes);
                bool isOk = true;
                for (int i = 0; i < nAttributes; ++i) {
                    double temp = s[i].toDouble(&isOk);
                    if (isOk)
                        point[i] = temp;
                    else {
                        QMessageBox::critical(
                            this, "Ошибка в строке " + QString::number(linePos),
                            "Неверный формат файла: одно из "
                            "чисел написано неверно");
                        return;
                    }
                }
                points.append(point);
            } else {
                QMessageBox::critical(
                    this, "Ошибка в строке " + QString::number(linePos),
                    "Неверный формат файла: в каждой строке "
                    "должно быть одинаковое количество чисел, "
                    "характеризующих значения признаков каждого объекта");
                return;
            }
        }
        if (!points.empty()) {
            plotPresenter->addCluster(points, nX_,
                                      (nAttributes >= 2) ? nY_ : nX_);
            plotPresenter->setStyle(
                plotPresenter->countOfClusters() - 1,
                QBrush(QColor().fromHsv(randInt(Hmin, Hmax),
                                        randInt(Smin, Smax),
                                        randInt(Vmin, Vmax))));
        } else {
            QMessageBox::warning(this, "Внимание!", "Файл не содержит данных!");
        }
        fileIn.close();
    } else {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось открыть файл\n" + filename);
        return;
    }
}

// Сохранение внутренних данных в файл
void MainWindow::on_saveData_triggered() {
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить как...",
                                                    ".", "All files (*.*)");
    if (filename.size() == 0) // Если была нажата кнопка отмены
        return;
    QFile fileOut(filename);
    if (fileOut.open(QIODevice::WriteOnly)) {
        QMessageBox::StandardButton answer =
            QMessageBox::question(this, "Ошибка", "Добавлять метки кластеров?");
        bool clusterLabel = (answer == QMessageBox::Yes);
        QTextStream qts(&fileOut);
        qts << "/ This file was generated by the ClusterData program\n";
        int n = 1;
        for (const auto &cluster : plotPresenter->data()) {
            if (cluster.size() > 0) {
                for (auto point : cluster) {
                    if (clusterLabel) {
                        qts << n;
                        for (auto x : point) {
                            qts << ';' << x;
                        }
                        qts << '\n';
                    } else {
                        qts << point.front();
                        for (auto it = point.begin() + 1; it != point.end();
                             ++it) {
                            qts << ';' << *it;
                        }
                        qts << '\n';
                    }
                }
                ++n;
            }
        }
        fileOut.close();
    } else {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось открыть файл\n" + filename);
        return;
    }
}

// Отображение внутренних данных
// bNoWarning -- если true, то предупреждений никаких не будет
bool MainWindow::on_showData_triggered(bool bNoWarning) {
    int oldX = nX_, oldY = nY_;
    runDialog.updateInfo(plotPresenter->data(), nClusters_, nIterations_, nX_,
                         nY_);
    if (runDialog.exec() == RunDialog::Accepted) {
        nClusters_ = runDialog.getClusters();
        nIterations_ = runDialog.getIterations();
        nX_ = runDialog.getX();
        nY_ = runDialog.getY();
        if (bNoWarning || (nX_ == oldX && nY_ == oldY)) {
            return true;
        }
        QMessageBox::Button answer = QMessageBox::question(
            this, "Внимание!", "Номера признаков были изменены! Вы уверены?");
        if (answer == QMessageBox::No) {
            nX_ = oldX;
            nY_ = oldY;
            return true;
        }
        // Обновление данных (старые удаляются, новые добавляются)
        plotPresenter->setData(plotPresenter->data(), nX_, nY_);
        for (int i = 0, cnt = plotPresenter->countOfClusters(); i < cnt; ++i) {
            plotPresenter->setStyle(
                i, QBrush(QColor().fromHsv(randInt(Hmin, Hmax),
                                           randInt(Smin, Smax),
                                           randInt(Vmin, Vmax))));
        }
        return true;
    }
    return false;
}

// Стандартизация по Z-оценке
void MainWindow::on_stdZ_triggered() { plotPresenter->zStd(nX_, nY_); }

// Стандартизация по среднеквадратическому отклонению
void MainWindow::on_stdSigma_triggered() { plotPresenter->sigmaStd(nX_, nY_); }

// Стандартизация по минимальному и максимальному значениям
void MainWindow::on_stdMinMax_triggered() {
    plotPresenter->minMaxStd(nX_, nY_);
}

// Объединение всех кластеров
void MainWindow::on_combineData_triggered() {
    plotPresenter->combineAll(nX_, nY_);
}

// Удаление всех кластеров
void MainWindow::on_clearData_triggered() { plotPresenter->clearAll(); }
