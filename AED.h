#ifndef AED_H
#define AED_H

#include <QObject>

#include "HeartRhythmAnalysis.h"
#include "Cpr.h"
#include "ECG.h"

#define STARTING_BATTERY 100


// I made this class a Singleton for ease of access from any class without holding a reference.
class AED : public QObject {
    Q_OBJECT

public:
    // singleton instance getter
    static AED* Instance(QObject* parent = nullptr);

    bool turnOn();                            // turns on AED

    void guideThroughProcedure();             //  voice-guided instructions
    void analyzeEcg();                        // analyzes the patient's ECG rhythm
    void chargeForShock();                    // automatically charges itself if shock is required
    // void promptForShockDelivery();            // prompts when to deliver the shock
    void deliverShock();                      // delivers shock
    void promptCprResumption();               // prompts to resume CPR after shock
    void provideStandClearWarning();          // 'stand clear' warning
    void indicateAnalyzing();                 // 'analyzing'  heart rhythm
    void adviseOnShock();                     // 'shock advised' if necessary

    int getBattery() { return battery; }

    // The analysis and cpr references.
    HeartRhythmAnalysis* hra;
    Cpr* cpr;

public slots:
    void startRhythmAnalysis();
    void startShockDelivery();
    void startCpr();
    void stopCpr();

    void shockDelivered();

    void batteryTimerTimeout();
signals:

    void updateDiagnosisDisplay(QString diagnosis);
    void updateDisplay(QString message);
    void log(QString message);

    void promptElectrodes(QString message);
    void promptShock(QString message);

    void enableCprUI(bool enable);
    void freezeCompressionButton();

    void updateBattery(int newLevel);
    void batteryCriticallyLow();

private:
    // State variables
    bool isInRhythmAnalysis;
    bool isInShockDelivery;
    bool isInCpr;

    bool isOn = false;
    float battery = 100;

    // Timer to continuously drain battery
    QTimer* batteryTimer;

    // private constructors and static instance
    AED();
    AED(QObject* parent = nullptr);
    static AED* instance;
};

#endif 
