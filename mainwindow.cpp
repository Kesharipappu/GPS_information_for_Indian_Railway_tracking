#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkRequest>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this))
    , apikey("your_api_key_here")
    , trainNumber("12345")
    , date("2024-07-31")
{
    ui->setupUi(this);
    connect(ui->fetchButton, &QPushButton::clicked, this, &MainWindow::fetchData);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::handleNetworkData);

    // Set up table widget
    ui->tableWidget->setColumnCount(10);
    QStringList headers = {"Station Name", "Actual Arrival", "Actual Departure", "Day", "Delay In Arrival", "Delay In Departure", "Schedule Arrival", "Schedule Departure", "Serial No", "Station Code"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::fetchData() {
    // Load JSON data from file
    QFile file("data.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Could not open data.json");
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(fileData);
    if (jsonResponse.isNull() || !jsonResponse.isObject()) {
        QMessageBox::critical(this, "Error", "Invalid JSON data in data.json");
        return;
    }

    QJsonObject jsonObject = jsonResponse.object();
    populateTable(jsonObject);
}

void MainWindow::populateTable(const QJsonObject &jsonObject) {
    // Clear any existing rows
    ui->tableWidget->setRowCount(0);

    // Get TrainRoute array
    QJsonArray trainRoute = jsonObject.value("TrainRoute").toArray();

    // Populate table widget with data
    for (int i = 0; i < trainRoute.size(); ++i) {
        QJsonObject station = trainRoute[i].toObject();

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(station.value("StationName").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(station.value("ActualArrival").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(station.value("ActualDeparture").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(station.value("Day").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(station.value("DelayInArrival").toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(station.value("DelayInDeparture").toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(station.value("ScheduleArrival").toString()));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(station.value("ScheduleDeparture").toString()));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(station.value("SerialNo").toString()));
        ui->tableWidget->setItem(row, 9, new QTableWidgetItem(station.value("StationCode").toString()));
    }
}

void MainWindow::handleNetworkData(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Error", reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
    if (jsonResponse.isNull() || !jsonResponse.isObject()) {
        QMessageBox::critical(this, "Error", "Invalid JSON data received from API");
        reply->deleteLater();
        return;
    }

    QJsonObject jsonObject = jsonResponse.object();
    populateTable(jsonObject);

    reply->deleteLater();
}
