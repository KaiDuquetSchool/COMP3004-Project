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

// Flash the light green, start a timer for 100 ms to turn it off
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

// Main meat of the code
void Cpr::compression() {
    qint64 currCompressionTime;

    // If we're not doing compressions, but the compression button was pressed, we came from ventilations; Restart
    if (!doingCompressions) {
        start();
        return;
    }

    // If its the first compression, start elapsing timer.
    if (!compressionTimer.isValid()) {
        compressionTimer.restart();
        currCompressionTime = 0;
    }
    else {
        // Take the elapsed time
        currCompressionTime = compressionTimer.elapsed();
    }

    // Get the time between compressions (current minus previous)
    qint64 diff = currCompressionTime - prevCompressionTime;
    prevCompressionTime = currCompressionTime;

    // If we do not yet have 4 compressions, just add to average
    if (compressionCount < 4) {
        runningAvg = (runningAvg * compressionCount + diff) / (compressionCount + 1);
    }
    else {
        // Else make it a running average; remove the least recent compression
        runningAvg -= prevCompressions.front() / 4;
        runningAvg += diff / 4;
        prevCompressions.pop();
    }
    compressionCount += 1;
    prevCompressions.push(diff);

    // Update count
    countLabel->setText(QString("%1 / 30").arg(compressionCount));

    // If we're at 30 compressions, we need 2 ventilations (30:2 ratio). Freeze the compression button to avoid misclick
    if (compressionCount >= 30) {
        emit aed->updateDisplay("Provide 2 ventilations to the patient.\n\nOnce completed, resume chest compressions simply by resuming clicking the compression button.");
        emit aed->freezeCompressionButton();
        doingCompressions = false;
        beatTimer->stop();
        flashTimer->stop();
        compressionTimer.invalidate();
        return;
    }

    // If we're too fast or too slow (small or large running average), notify the person.
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
