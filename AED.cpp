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

// If the AED is already on, use this as a power off if nothing is going on
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

// When the battery timer times out, update power by different usage (in intensive process, reduce more battery)
void AED::batteryTimerTimeout() {

    float usage = 0.1f;
    if (isInRhythmAnalysis) usage += 0.5;
    battery -= usage;

    emit updateBattery((int) battery);

    // If we're critically low, abort CPR feedback and notify UI so it can disable everything smoothly
    if (battery <= 5) {
        emit batteryCriticallyLow();
        if (isInCpr)
            cpr->abort();

        emit updateDisplay("BATTERY CRITICALLY LOW!\n\nThis device can no longer operate reliably, as it does not have enough power"
                           "to safely and reliably provide analysis and shocks.\n\nReplace/Charge the battery of this device.");
    }
}

// Only start a new Rhythm analysis if we're not in another procedure that was started by a previous analysis
void AED::startRhythmAnalysis() {
    if (isInRhythmAnalysis || isInShockDelivery || isInCpr) return;

    isInRhythmAnalysis = true;
    hra->startRhythmAnalysis();
}

// Similarly here, only start a shock delivery procedure if we're not in one or in CPR
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
