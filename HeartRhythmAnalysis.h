#ifndef HEARTRHYTHMANALYSIS_H
#define HEARTRHYTHMANALYSIS_H

#include <QObject>
#include <QString>
#include <QVector>

class HeartRhythmAnalysis : public QObject {
    Q_OBJECT

public:

    //constructor
     HeartRhythmAnalysis(QObject *parent = nullptr);

    void startRhythmAnalysis();                     // initiates the heart rhythm analysis 
    void placeElectrodes();                         //  placing electrodes
    void confirmElectrodesPlacement();              // confirming placement of electrodes
    QString analyzeHeartRhythm();                   // analyzes type of rhythm
    bool isRhythmShockable();                       // checking if rhythm is shockable 
    void recordElectricalActivity(float activity);  // records electrical activity data point to determine whether a shockable rhythm is present

signals:
    void electrodesPlacementConfirmed();            // signal for confirmation of electrodes placement 
    void rhythmAnalyzed(const QString &rhythmType); // signal emitted when rhythm analysis is complete
    void displayMessage(const QString &message);    // signal for displaying message on display

private:
    bool electrodesPlaced;
    QVector<float> electricalActivityData;          // storing electrical activity data
    QString currentRhythmType;

    QString determineRhythmType();                  // determining type of rhythm based on electrical activity
    bool checkForVentricularFibrillation();         // checking for ventricular fibrillation
    bool checkForVentricularTachycardia();          // checking for ventricular tachycardia
};

#endif 
