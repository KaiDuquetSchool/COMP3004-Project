#ifndef ECG_H
#define ECG_H

#include <QObject>
#include <QString>
#include <QVector>

class ECG : public QObject {
    Q_OBJECT

public:
    //contructor
     ECG(QObject *parent = nullptr);

    void startECGTraining();                           // start training 
    void generateECGReadings();                        // generate readings 
    void showReading(const QString& readingFile);      // display reading 
    void selectResponse(const QString& promptMessage); // prompt for user response based on reading (cpr..shock)
    void receiveSelection(const QString& selection);   //  user response
    void provideFeedback();                            //  feedback for user selection
    void provideDetailedPerformanceReview();           //  detailed performance review
 
signals:
    void ecgReadingGenerated(const QString& readingFile);  // signal for ECG reading is generated
    void responsePrompted(const QString& promptMessage);   // signal for prompt response
    void selectionReceived(const QString& selection);      //  signal for selection recieved 
    void feedbackProvided(const QString& feedback);        // signla for feedback provided
    void detailedReviewProvided(const QString& review);    // signal for review provided

private:
    QVector<QString> ecgReadings;                 // stores ECG readings
    int currentEcgReadingIndex;                   // index of the current ECG reading
    bool correctDiagnosisMade;
};

#endif 
