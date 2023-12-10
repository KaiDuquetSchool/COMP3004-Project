#ifndef AED_H
#define AED_H

#include <QObject>

#include "HeartRhythmAnalysis.h"
#include "ECG.h"


class AED : public QObject {
    Q_OBJECT

public:
     //contructor
     AED(QObject *parent = nullptr);

    void turnOn();                            // turns on AED
    void guideThroughProcedure();             // Provides voice-guided instructions
    void promptElectrodePlacement();          // Prompts to place electrodes on the patient's chest
    void analyzeEcg();                        // Analyzes the patient's ECG rhythm
    void chargeForShock();                    // automatically charges itself if shock is required
    // void promptForShockDelivery();            // prompts when to deliver the shock
    void deliverShock();                      // delivers shock
    void promptCprResumption();               // prompts to resume CPR after shock
    void provideStandClearWarning();          // 'stand clear' warning
    void indicateAnalyzing();                 // 'analyzing'  heart rhythm
    void adviseOnShock();                     // 'shock advised' if necessary

signals:
    void turnedOn();                          
    void electrodePlacementPrompted();        
    void ecgAnalysisStarted();                
    void shockCharged();                      
    void shockDelivered();                    
    void cprPrompted();                       
    void standClearWarningIssued();           
    void analyzingIndicated();                
    void shockAdvised();                      

private:
    bool isOn;                               
    bool electrodePadsPlaced;                 
    bool isAnalyzing;                         
    bool shockAdvised;                        
    bool shockDelivered;                      
    bool isInCprMode;                         // AED is in CPR mode post-shock
    QString currentVoicePrompt;               // stores the voice prompt to be played
    QString currentVisualMessage;             // stores the visual message to be displayed

    
};

#endif 
