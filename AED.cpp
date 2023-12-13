#include "AED.h"
#include <iostream>

AED* AED::instance = nullptr;

AED* AED::Instance(QObject* parent) {
    if (instance == nullptr)
        instance = new AED(parent);
    return instance;
}

AED::AED(QObject* parent): QObject(parent) {
    this->hra = new HeartRhythmAnalysis(this);
    this->cpr = new Cpr(this);
    isInRhythmAnalysis = false;
    isInShockDelivery = false;
    isInCpr = false;

    battery = STARTING_BATTERY;
    batteryTimer = new QTimer(this);
    connect(batteryTimer, SIGNAL (timeout()), this, SLOT (batteryTimerTimeout()));
}

bool AED::turnOn() {
    if (isOn) {
        if (!isInRhythmAnalysis && !isInShockDelivery && !isInCpr) {
            emit log("AED Powering Off");
            isOn = false;
            batteryTimer->stop();
        }
    }
    else {
        emit log("AED Powered On");
        batteryTimer->start(1000);
        isOn = true;
    }
    return isOn;
}

void AED::batteryTimerTimeout() {

    float usage = 0.1f;
    if (isInRhythmAnalysis) usage += 0.5;
    battery -= usage;

    emit updateBattery((int) battery);

    if (battery <= 5) {
        emit batteryCriticallyLow();
        if (isInCpr)
            cpr->abort();

        emit updateDisplay("BATTERY CRITICALLY LOW!\n\nThis device can no longer operate reliably, as it does not have enough power"
                           "to safely and reliably provide analysis and shocks.\n\nReplace/Charge the battery of this device.");
    }
}

void AED::startRhythmAnalysis() {
    if (isInRhythmAnalysis || isInShockDelivery || isInCpr) return;

    isInRhythmAnalysis = true;
    hra->startRhythmAnalysis();
}

void AED::startShockDelivery() {
    if (isInShockDelivery || isInCpr) return;
    isInShockDelivery = true;
    isInRhythmAnalysis = false;

    emit log("Starting shock delivery");
    emit promptShock("Please STAND CLEAR of the patient.\nDo not come in contact with them.\n\nWhen ready, press the Confirm Shock button to deliver a shock.");
}

void AED::shockDelivered() {
    if(!isInShockDelivery) return;

    emit log("Shock successfully administered.");
    emit updateDisplay("Shock successfully delivered.\n\nPlease press the Prepare CPR Button to commence resuscitation.");
    isInShockDelivery = false;
}

void AED::startCpr() {
    if (isInCpr) return;
    isInCpr = true;

    emit enableCprUI(true);
    cpr->start();
}

void AED::stopCpr() {
    if (!isInCpr) return;

    cpr->abort();
    isInCpr = false;
}
