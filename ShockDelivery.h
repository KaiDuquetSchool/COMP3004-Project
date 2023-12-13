#ifndef SHOCKDELIVERY_H
#define SHOCKDELIVERY_H

#include <QObject>
#include "HeartRhythmAnalysis.h"
#include "EmergencyResponse.h"


class ShockDelivery : public QObject {
    Q_OBJECT

public:
    //constructor 
    ShockDelivery(QObject *parent = nullptr);

    void deliverShock();                        // delivering a shock 

signals:
    void electrodesPlacementRequested();       // signal for electrode placement
    void shockDeliveryRequested();             // signal for requesting shock delivery
    void shockDelivered();                     // signal for when a shock has been delivered
    void displayMessage(const QString &message); // signal to display message 

private:
    void simulateShock();                      // simulating the shock delivery to the patient
    void updatePatientResponse();              // updating the system with the response to the shock

    bool safetyCheckComplete; //checking if safety check has been successfully performed (should be set  to true if so)
    bool shockDeliveredCheck; ///checking if the shock has successfully been delievered (should be set  to true if so)
};

#endif 
