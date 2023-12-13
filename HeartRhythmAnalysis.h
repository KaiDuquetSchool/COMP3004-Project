#ifndef HEARTRHYTHMANALYSIS_H
#define HEARTRHYTHMANALYSIS_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QTimer>


class HeartRhythmAnalysis : public QObject {
    Q_OBJECT

public:

    //constructor
    HeartRhythmAnalysis(QObject *parent = nullptr);

    void startRhythmAnalysis();                     // initiates the heart rhythm analysis
    void analyzeHeartRhythm();                      // analyzes type of rhythm

public slots:
    void electrodesPlacementConfirmed();            // signal for confirmation of electrodes placement 
    void rhythmAnalyzed();                           // signal emitted when rhythm analysis is complete

private:
    QTimer* timer;

    bool electrodesPlaced = false;
    QVector<float> electricalActivityData;          // storing electrical activity data
    QString currentRhythmType;

    QString determineRhythmType();                  // determining type of rhythm based on electrical activity
    bool checkForVentricularFibrillation();         // checking for ventricular fibrillation
    bool checkForVentricularTachycardia();          // checking for ventricular tachycardia
};

#endif 
