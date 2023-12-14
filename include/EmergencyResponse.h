#ifndef EMERGENCYRESPONSE_H
#define EMERGENCYRESPONSE_H

#include <QObject>
#include "Cpr.h"
#include "ShockDelivery.h"
#include "HeartRhythmAnalysis.h"

class EmergencyResponse : public QObject {
    // Cpr *cpr;
    // ShockDelivery *shockDelivery;
    // HeartRhythmAnalysis *heartRhythmAnalysis;

    Q_OBJECT

public:
    //contructor
     EmergencyResponse(QObject *parent = nullptr);

    void startEmergencyResponseTraining();     
    void createScenario();                     // create emergency scenario
    void activate();                           
    void confirm();                            
    void simulateHeartRhythmAnalysis();        
    void shock();                              
    void startCpr();                           // starting cpr
    void cprFeedback();                        // cpr feedback
    void updatePatientResponse();              // updating patient response

signals:
    void scenarioCreated();                    // signal for creation of scenario
    void scenarioActivated();                  // signal for activation
    void actionConfirmed();                    // signal for confirmed
    void heartRhythmAnalyzed();                // signal when heart rhythm analyzed 
    void shockDelivered();                     // signal for shock delivery 
    void cprStarted();                         // signal for cpr start
    void feedbackProvided(const QString &feedback); // signal for feedback
    void patientResponseUpdated();             // signal for patience response 
    void displayMessage(const QString &message); // signal for displaying message on display

private:
    bool emergencyScenarioStarted;   // should be set to true once the scenario has started 
    bool patientAssessed;            // should be set to true after assessment
};

#endif 
