#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <iostream>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    QPixmap pixmap(":/images/img1.png");
    QIcon ButtonIcon(pixmap);
    ui->analyzeHeartRhythmButton->setIcon(ButtonIcon);
    ui->analyzeHeartRhythmButton->setIconSize(pixmap.rect().size() * 0.75);

    QPixmap pixmap4(":images/img4.png");
    QIcon ButtonIcon4(pixmap4);
    ui->powerButton->setIcon(ButtonIcon4);
    ui->powerButton->setIconSize(pixmap4.rect().size() * 0.75);

    aed = AED::Instance(this);
    timer = new QTimer(this); // general use timer

    // attach all display signals and slots
    connect(aed, SIGNAL (log(QString)), this, SLOT (log(QString)));
    connect(aed, SIGNAL (updateDisplay(QString)), this, SLOT (updateDisplay(QString)));
    connect(aed, SIGNAL (updateDiagnosisDisplay(QString)), this, SLOT (updateDiagnosisDisplay(QString)));
    connect(aed, SIGNAL (enableCprUI(bool)), this, SLOT (enableCprUI(bool)));

    // prompt signals and slots
    connect(aed, SIGNAL (promptElectrodes(QString)), this, SLOT (promptElectrodes(QString)));
    connect(aed, SIGNAL (promptShock(QString)), this, SLOT (promptShock(QString)));

    // battery related signals
    connect(aed, SIGNAL (updateBattery(int)), this, SLOT (updateBattery(int)));
    connect(aed, SIGNAL (batteryCriticallyLow()), this, SLOT (batteryCriticallyLow()));

    // All the side buttons and compression button
    connect(ui->analyzeHeartRhythmButton, SIGNAL (clicked()), aed, SLOT (startRhythmAnalysis()));
    connect(ui->prepareShockButton, SIGNAL (clicked()), aed, SLOT (startShockDelivery()));
    connect(ui->prepareCprButton, SIGNAL (clicked()), aed, SLOT (startCpr()));
    connect(ui->stopCprButton, SIGNAL (clicked()), aed, SLOT (stopCpr()));
    connect(ui->compressionButton, SIGNAL (pressed()), aed->cpr, SLOT (compression()));

    // the compression freezing signals
    connect(aed, SIGNAL (freezeCompressionButton()), this, SLOT (freezeCompressionButton()));
    connect(timer, SIGNAL (timeout()), this, SLOT (unfreezeCompressionButton()));

    // initially (powered off state) set every side button to disabled
    ui->analyzeHeartRhythmButton->setDisabled(true);
    ui->analyzeHeartRhythmButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
    ui->prepareShockButton->setDisabled(true);
    ui->prepareShockButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
    ui->prepareCprButton->setDisabled(true);
    ui->prepareCprButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
    ui->stopCprButton->setDisabled(true);
    ui->stopCprButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));

    // hide confirmation buttons; they're only needed on prompt
    ui->confirmElectrodesButton->hide();
    ui->confirmShockButton->hide();

    // hide CPR specific UI
    enableCprUI(false);

    // give the label references to CPR module
    aed->cpr->setLabels(ui->compressionTimingLabel, ui->compressionCountLabel);

    ui->batteryBar->setValue(STARTING_BATTERY);
}

MainWindow::~MainWindow(){
    delete ui;
}

// Updates the center display with a message
void MainWindow::updateDisplay(QString message) {
    ui->aedDisplay->clear();
    ui->aedDisplay->append(message);
}

// System log. Probably unnecessary if we give proper visual feedback for everything
void MainWindow::log(QString message) {
    ui->systemLog->append(message);
}

// Depending on diagnosis from heart rhythm analysis, display instructions on whether we should shock or CPR
void MainWindow::updateDiagnosisDisplay(QString diagnosis) {
    QPixmap pixmap;
    bool shockable = false;

    QString s("Analysis Done. Resulting Diagnosis:\n");
    if (diagnosis.compare("vf") == 0) {
        s += "Ventricular Fibrillation\nRhythm is shockable. Shock delivery required.\nPlease press the Prepare Shock button.";
        pixmap.load(":images/vf.png");
        shockable = true;
    }
    else if (diagnosis.compare("vt") == 0) {
        s += "Ventricular Tachycardia\nRhythm is shockable. Shock delivery required.\nPlease press the Prepare Shock button.";
        pixmap.load(":images/vt.png");
        shockable = true;
    }
    else if (diagnosis.compare("pea") == 0) {
        s += "Sinus Rhythm\nRhythm is NOT shockable. CPR is advisory.\nPlease press the Prepare CPR Button.";
        pixmap.load(":images/pea.png");
    }
    else if (diagnosis.compare("asystole") == 0) {
        s += "Asystole\nRhythm is NOT shockable. CPR is advisory.\nPlease press the Prepare CPR Button.";
        pixmap.load(":images/asystole.png");
    }
    updateDisplay(s);

    // ----- ATTENTION HERE -----
    // Again this part is really bad; it's just a static image. We need live ECG data.
    ui->diagnosisResultLabel->setPixmap(pixmap);
    ui->diagnosisResultLabel->setMask(pixmap.mask());
    ui->diagnosisResultLabel->show();

    // If shockable enable prepare shock button
    if (shockable) {
        ui->prepareShockButton->setDisabled(false);
        ui->prepareShockButton->setStyleSheet(QString("background-color: rgb(255, 188, 196)"));
    }

    // Shockable or not, enable CPR buttons.
    ui->prepareCprButton->setDisabled(false);
    ui->prepareCprButton->setStyleSheet(QString("background-color: rgb(255, 188, 196)"));

    ui->stopCprButton->setDisabled(false);
    ui->stopCprButton->setStyleSheet(QString("background-color: rgb(255, 188, 196)"));
}

void MainWindow::updateBattery(int batteryLevel) {
    ui->batteryBar->setValue(batteryLevel);
    if (batteryLevel <= 20) {
        ui->batteryBar->setStyleSheet(QString("background-color: orange"));
    }
}

void MainWindow::batteryCriticallyLow() {
    enableCprUI(false);

    ui->analyzeHeartRhythmButton->setDisabled(true);
    ui->analyzeHeartRhythmButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
    ui->prepareShockButton->setDisabled(true);
    ui->prepareShockButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
    ui->prepareCprButton->setDisabled(true);
    ui->prepareCprButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
    ui->stopCprButton->setDisabled(true);
    ui->stopCprButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
}

// Prompt electrode placement
void MainWindow::promptElectrodes(QString message) {
    updateDisplay(message);

    ui->confirmElectrodesButton->show();
    connect(ui->confirmElectrodesButton, SIGNAL (clicked()), aed->hra, SLOT (electrodesPlacementConfirmed()));
    connect(ui->confirmElectrodesButton, SIGNAL (clicked()), this, SLOT (disablePrompts()));
}

void MainWindow::enableCprUI(bool enable) {
    if (enable) {
        ui->compressionButton->show();
        ui->compressionCountLabel->show();
        ui->compressionTimingLabel->show();
    }
    else {
        ui->compressionButton->hide();
        ui->compressionCountLabel->hide();
        ui->compressionTimingLabel->hide();
    }
}

void MainWindow::freezeCompressionButton() {
    ui->compressionButton->setDisabled(true);
    ui->compressionButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
    timer->start(1500);
}

void MainWindow::unfreezeCompressionButton() {
    ui->compressionButton->setDisabled(false);
    ui->compressionButton->setStyleSheet(QString("background-color: rgb(255, 188, 196)"));
}

void MainWindow::promptShock(QString message) {
    updateDisplay(message);

    ui->confirmShockButton->show();
    connect(ui->confirmShockButton, SIGNAL (clicked()), aed, SLOT (shockDelivered()));
    connect(ui->confirmShockButton, SIGNAL (clicked()), this, SLOT (disablePrompts()));
}

void MainWindow::disablePrompts() {
    ui->confirmElectrodesButton->hide();
    ui->confirmShockButton->hide();
    disconnect(ui->confirmElectrodesButton, SIGNAL (clicked()), aed->hra, SLOT (electrodesPlacementConfirmed()));
    disconnect(ui->confirmShockButton, SIGNAL (clicked()), aed, SLOT (shockDelivered()));
}

void MainWindow::on_powerButton_clicked() {
    bool isOn = aed->turnOn();
    if (isOn) {
        ui->analyzeHeartRhythmButton->setDisabled(false);
        ui->analyzeHeartRhythmButton->setStyleSheet(QString("background-color: rgb(255, 188, 196)"));
    }
    else {
        enableCprUI(false);

        ui->analyzeHeartRhythmButton->setDisabled(true);
        ui->analyzeHeartRhythmButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
        ui->prepareShockButton->setDisabled(true);
        ui->prepareShockButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
        ui->prepareCprButton->setDisabled(true);
        ui->prepareCprButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
        ui->stopCprButton->setDisabled(true);
        ui->stopCprButton->setStyleSheet(QString("background-color: rgb(64,64,64)"));
    }
}

