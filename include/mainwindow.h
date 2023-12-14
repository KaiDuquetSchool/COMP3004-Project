#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AED.h"

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
    void on_powerButton_clicked();

    void updateDiagnosisDisplay(QString diagnosis);
    void updateDisplay(QString message);
    void log(QString message);

    void promptElectrodes(QString message);
    void promptShock(QString message);
    void disablePrompts();

    void enableCprUI(bool enable);
    void freezeCompressionButton();
    void unfreezeCompressionButton();

    void updateBattery(int batteryLevel);

    void batteryCriticallyLow();

private:
    Ui::MainWindow *ui;

    QTimer* timer;
    AED* aed;
};
#endif // MAINWINDOW_H
