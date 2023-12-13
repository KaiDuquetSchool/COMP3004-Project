#include "Cpr.h"
#include "AED.h"

Cpr::Cpr(AED* parent): QObject(parent) {
    flashTimer = new QTimer(this);
    flashTimer->setSingleShot(true);
    beatTimer = new QTimer(this);

    connect(beatTimer, SIGNAL (timeout()), this, SLOT (flashLabel()));
    connect(flashTimer, SIGNAL (timeout()), this, SLOT (unflashLabel()));

    aed = parent;
}

void Cpr::start() {

    emit aed->updateDisplay("Starting real-time CPR feedback.\n\nPress the Compression button to simulate a chest compression.\n"
                            "Aim for 100-120/minute (~2/second).\n\nFollow the rhythm of the flashing green light.\n\nPay attention"
                            " to the feedback given to you when doing chest compressions.\nStop at 30 compressions for 2 ventilations");

    prevCompressionTime = 0;
    runningAvg = 0;
    compressionCount = 0;
    doingCompressions = true;
    countLabel->setText("0 / 30");
    compressionTimingLabel->setStyleSheet(QString("background-color: rgb(199,199,199)"));

    while (!prevCompressions.empty()) prevCompressions.pop();

    beatTimer->start(500);
}

void Cpr::flashLabel() {
    compressionTimingLabel->setStyleSheet(QString("background-color: rgb(0, 255, 0)"));
    flashTimer->start(100);
}

void Cpr::unflashLabel() {
    compressionTimingLabel->setStyleSheet(QString("background-color: rgb(199,199,199)"));
}

void Cpr::setLabels(QLabel* compressionTimingLabel, QLabel* countLabel) {
    this->compressionTimingLabel = compressionTimingLabel;
    this->countLabel = countLabel;
}

void Cpr::abort() {
    beatTimer->stop();
    flashTimer->stop();

    emit aed->enableCprUI(false);
    emit aed->updateDisplay("Aborted real-time CPR feedback.");
}

void Cpr::compression() {
    qint64 currCompressionTime;

    if (!doingCompressions) {
        start();
        return;
    }

    if (!compressionTimer.isValid()) {
        compressionTimer.restart();
        currCompressionTime = 0;
    }
    else {
        currCompressionTime = compressionTimer.elapsed();
    }

    qint64 diff = currCompressionTime - prevCompressionTime;
    prevCompressionTime = currCompressionTime;

    if (compressionCount < 4) {
        runningAvg = (runningAvg * compressionCount + diff) / (compressionCount + 1);
    }
    else {
        runningAvg -= prevCompressions.front() / 4;
        runningAvg += diff / 4;
        prevCompressions.pop();
    }
    compressionCount += 1;
    prevCompressions.push(diff);

    countLabel->setText(QString("%1 / 30").arg(compressionCount));

    if (compressionCount >= 30) {
        emit aed->updateDisplay("Provide 2 ventilations to the patient.\n\nOnce completed, resume chest compressions simply by resuming clicking the compression button.");
        emit aed->freezeCompressionButton();
        doingCompressions = false;
        beatTimer->stop();
        flashTimer->stop();
        compressionTimer.invalidate();
        return;
    }

    if (compressionCount >= 4 && runningAvg > 750) {
        emit aed->updateDisplay("Compressions are too slow.\n\nAim for 2 compressions/second.\n\nFollow the rhythm of the blinking light.");
    }
    else if (compressionCount >= 4 && runningAvg < 350) {

        emit aed->updateDisplay("Compressions are too fast.\n\nAim for 2 compressions/second.\n\nFollow the rhythm of the blinking light.");
    }
    else {
        emit aed->updateDisplay("Good compression rate.\n\nKeep going.");
    }
}
