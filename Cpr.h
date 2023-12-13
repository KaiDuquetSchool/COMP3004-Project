#ifndef CPR_H
#define CPR_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include <queue>

class AED;

class Cpr : public QObject {
    Q_OBJECT

public:
    //constructor
    Cpr(AED *parent = nullptr);

    void start();                        // starting cpr

    void setLabels(QLabel* compressionTimingLabel, QLabel* countLabel);

public slots:
    void compression();                 // Slot every time a compression happens
    void flashLabel();                  // Slot to flash the light green
    void unflashLabel();                // Slot to turn off the flash
    void abort();                       // Aborting the CPR real-time feedback

private:
    bool doingCompressions = false;

    QTimer* flashTimer;                 // This timer is just to turn off the green light after it flashes
    QTimer* beatTimer;                  // This timer goes off every 500 milliseconds; this is the pace the CPR deliverer should follow
    QElapsedTimer compressionTimer;     // This is an elapsed timer which is used to measure how closely the actual compressions are to the recommended pace

    int compressionCount;
    qint64 runningAvg;                  // The running average of the time between each compression
    qint64 prevCompressionTime;         // The most recent compression time

    std::queue<qint64> prevCompressions;// The queue for the running average

    QLabel* compressionTimingLabel;     // The green light label
    QLabel* countLabel;                 // The compression count label

    AED* aed;

};

#endif 
