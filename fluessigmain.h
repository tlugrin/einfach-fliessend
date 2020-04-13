#ifndef FLUESSIGMAIN_H
#define FLUESSIGMAIN_H

#include <QMainWindow>
#include <QToolBar>
#include <QLineEdit>
#include <QCompleter>
#include <QToolButton>
#include <QCheckBox>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QTranslator>
#include <QCloseEvent>
#include "centralwidget.h"
#include "drilldialog.h"
#include "customisationwindow.h"
#include "loginwizard.h"
#include "importdialog.h"
#include "namespaces.h"

class FluessigMain : public QMainWindow
{
    Q_OBJECT

private:
    QToolBar* chercherToolBar; // boîte d'outils "chercher"
    QLineEdit* chercherEdit;
    QCompleter* completeur;
    QAction* chercherAction;
    QString firstLetters;
    QVector<unsigned int> chercherIDs;
    int currentIDIndex;
    QToolButton* chercherPrecedantButton;
    QToolButton* chercherSuivantButton;
    QCheckBox* chercherCasseBox;
    QMenu* fichierMenu;      // fichier
    QAction* editerAction;
    QAction* interrogerAction;
    QActionGroup* modeGroup;
    QAction* blancAction;
    QAction* quitterAction;
    QAction* exporterAction;
    QAction* importerAction;
    QAction* loginAction;
    QMenu* parametresMenu;   // paramètres
    QMenu* languesAppMenu;
    QActionGroup* languesAppGroup;
    QAction* langueApp_frAct;
    QAction* langueApp_deAct;
    QAction* langueApp_enAct;
    QAction* drillAction;
    QAction* drillResetAction;
    QAction* persoAction;
    QMenu* languesMenu;
    QActionGroup* languesGroup;
    QAction* langues_frdeAct;
    QAction* langues_frenAct;
    QAction* langues_frshAct;
    QAction* swapAction;
    QAction* afficherChercherToolBarAct;
    QMenu* aProposMenu;     // aide
    QAction* aProposQtAction;
    CentralWidget* centralWidget;
    // languages
    QTranslator tradOwn;
    QTranslator tradQt;
    void retraduisUI();
    // user info
    LoginWizard* logWizard;
    DrillDialog* drillDialog;
    CustomisationWindow* persoWindow;
    // save, restore
    ImportDialog* importDialog;
    void saveSettings();
    void saveUserSettings();
    void restoreSettings();
    void restoreUserSettings();
    // helpful attributes & functions
    bool editingMode;
    void createActions();
    void createMenus();
    void createToolBars();
    void disableActions(bool b);
    void availableLanguagesChanged();
    void updateFromCustomised();
    void updateFromDrill();

protected:
    void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void modeGroupTriggered();
    void setCurrentLangueApp(Langue::LangueApp langue); // changed from login wizard
    void languesAppGroupTriggered();                    // changed from app menu
    void showLogin();
    void showDrillDialog();
    void showCustomisationWindow();
    void languesGroupTriggered();
    void exporterActionTriggered();
    void importerActionTriggered();
    void manageFindFocus(bool b);
    void manageCompleter();
    void caseSensitivityChanged(bool checked);
    void setChercherIDs();
    void showNextFound();
    void showPrevFound();

signals:
    void editingModeChanged(bool editing);

public:
    explicit FluessigMain(QWidget *parent = 0);

    //accessor
    Langue::LangueApp currentLangueApp() const;
    void setLangueApp(Langue::LangueApp langue);
};

#endif // FLUESSIGMAIN_H
