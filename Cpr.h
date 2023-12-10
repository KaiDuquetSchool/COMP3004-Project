#ifndef CPR_H
#define CPR_H

#include <QObject>
#include "HeartRhythmAnalysis.h"
#include "ShockDelivery.h"
#include "EmergencyResponse.h"


class Cpr : public QObject {
    Q_OBJECT

public:
    //constructor
   Cpr(QObject *parent = nullptr);

    void activateCprPrompt();            // activating prompt to start CPR
    void start();                        // starting cpr
    void cprConfirmed();                 // confirming cpr
    void cprFeedback();                  // feedback for cpr

signals:
    void cprActivated();                 // signal for cpr acitvation
    void cprStarted();                   // signal for cpr start
    void feedbackProvided(const QString &feedback);// signal for feedback
    void messageDisplayed(const QString &message); // signal for displaying message on display

private:
    int compressionRate;                 // rate of compressions
    int compressionDepth;                // depth of compressions
    void updatePatientResponse();        // updating patients response after Cpr

};

#endif 
