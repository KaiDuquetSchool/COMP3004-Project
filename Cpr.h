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
    void compression();
    void flashLabel();
    void unflashLabel();
    void abort();

signals:
    void cprActivated();                 // signal for cpr acitvation
    void cprStarted();                   // signal for cpr start
    void feedbackProvided(const QString &feedback);// signal for feedback
    void messageDisplayed(const QString &message); // signal for displaying message on display

private:
    bool doingCompressions = false;

    QTimer* flashTimer;
    QTimer* beatTimer;
    QElapsedTimer compressionTimer;

    int compressionCount;
    qint64 runningAvg;
    qint64 prevCompressionTime;

    std::queue<qint64> prevCompressions;

    QLabel* compressionTimingLabel;
    QLabel* countLabel;

    AED* aed;

};

#endif 
