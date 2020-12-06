#include "rundialog.h"
#include "ui_rundialog.h"

// Конструктор
RunDialog::RunDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RunDialog) {
    ui->setupUi(this);
}

// Деструктор
RunDialog::~RunDialog() { delete ui; }

// Обновление информации на форме
void RunDialog::updateInfo(const QVector<Cluster> &data, int nClusters,
                           int nIterations, int nX, int nY) {
    int n = 1;
    ui->dataView->clear();
    for (auto &cluster : data) {
        for (auto &point : cluster) {
            QString s;
            QTextStream qts(&s);
            qts << n;
            for (auto x : point) {
                qts << ';' << x;
            }
            ui->dataView->addItem(s);
        }
        ++n;
    }
    int nPoints = ui->dataView->count();
    this->setWindowTitle("Начальные данные (число объектов: " +
                         QString::number(nPoints) + ")");
    ui->spinClusters->setValue(nClusters);
    ui->spinClusters->setMaximum(nPoints);
    ui->spinIterations->setValue(nIterations);
    if (!data.empty() && !data.first().empty() &&
        !data.first().front().empty()) {
        ui->spinX->setMaximum(data.first().front().size());
        ui->spinY->setMaximum(data.first().front().size());
    } else {
        ui->spinX->setMaximum(1);
        ui->spinY->setMaximum(1);
    }
    ui->spinX->setValue(nX + 1);
    ui->spinY->setValue(nY + 1);
}

// Получение числа кластеров из поля ввода формы
int RunDialog::getClusters() const { return ui->spinClusters->text().toInt(); }

// Получение числа итераций из поля ввода формы
int RunDialog::getIterations() const {
    return ui->spinIterations->text().toInt();
}

// Получение номера признака для координаты X
int RunDialog::getX() const { return ui->spinX->text().toInt() - 1; }

// Получение номера признака для координаты Y
int RunDialog::getY() const { return ui->spinY->text().toInt() - 1; }
