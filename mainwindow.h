#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void fetchData();
    void handleNetworkData(QNetworkReply *reply);

private:
    void populateTable(const QJsonObject &jsonObject);

    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QString apikey;
    QString trainNumber;
    QString date;
};

#endif // MAINWINDOW_H
