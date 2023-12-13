#include "HeartRhythmAnalysis.h"
#include "AED.h"

#include <QTimer>
#include <sstream>

HeartRhythmAnalysis::HeartRhythmAnalysis(QObject* parent): QObject(parent) {
    timer = new QTimer(this);
}

void HeartRhythmAnalysis::startRhythmAnalysis() {
    AED* aed = AED::Instance();

    emit aed->log("Starting Rhythm Analysis");
    emit aed->promptElectrodes("Please place electrodes on patient");
}

void HeartRhythmAnalysis::electrodesPlacementConfirmed() {
    AED *aed = AED::Instance();
    emit aed->log("Electrode Placement Confirmed");
    analyzeHeartRhythm();
}

// This function and the one below need to be worked on. This just gives a static image
// and doesn't do any analyzing. This part needs work.
void HeartRhythmAnalysis::analyzeHeartRhythm() {
    AED *aed = AED::Instance();

    emit aed->updateDisplay("Analyzing Patient's Heart Rhythm.\n\nSTAND CLEAR of patient.");

    connect(timer, SIGNAL (timeout()), this, SLOT (rhythmAnalyzed()));
    timer->setSingleShot(true);
    timer->start(5000);
}

void HeartRhythmAnalysis::rhythmAnalyzed() {
    timer->stop();
    disconnect(timer, SIGNAL (timeout()), this, SLOT (rhythmAnalyzed()));

    AED *aed = AED::Instance();

    QString diagnosis = "vf";
    emit aed->updateDiagnosisDisplay(diagnosis);
}
