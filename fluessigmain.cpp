#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <QMessageBox>
#include <QtDebug>
#include <QApplication>
#include <QLibraryInfo>
#include <QSettings>
#include <QGuiApplication>
#include <QStringListModel>
#include "fluessigmain.h"
#include "exportdialog.h"

FluessigMain::FluessigMain(QWidget *parent)
    : QMainWindow(parent), firstLetters(""), currentIDIndex(0)
{
    // General settings
    QCoreApplication::setOrganizationName("Dsobiblu");
    QCoreApplication::setApplicationName("Einfach fließend!");
    QFont policeWindow;
    policeWindow.setPointSize(12);
    setWindowTitle("Einfach fließend 1.0");
    setMinimumSize(800, 400);
    setWindowIcon(QIcon(":/pictures/icon.png"));
    setFont(policeWindow);
    // Components: centre...
    centralWidget = new CentralWidget;
    setCentralWidget(centralWidget);
    // ...menu bar...
    createToolBars();
    createActions();
    createMenus();
    // ...prepare last features (language mainly)
    centralWidget->disableButtons(true);
    disableActions(true);
    restoreSettings();
    editingMode = false;
    emit editingModeChanged(editingMode);

    connect(importDialog,SIGNAL(importerButtonClicked(QString)),centralWidget,SLOT(importFile(QString)));
    connect(centralWidget,SIGNAL(newEntryRead()),importDialog,SLOT(nbrLusChanged()));
    connect(centralWidget,SIGNAL(newEntryAdded()),importDialog,SLOT(nbrNouveauxChanged()));
    connect(centralWidget,SIGNAL(newEntryModified()),importDialog,SLOT(nbrModifiesChanged()));
    connect(logWizard,SIGNAL(appLanguageChanged(Langue::LangueApp)),this,SLOT(setCurrentLangueApp(Langue::LangueApp)));
    showLogin();
}

void FluessigMain::closeEvent(QCloseEvent *event){
    saveSettings();
    event->accept();
}

void FluessigMain::createActions(){
    modeGroup = new QActionGroup(this);
    modeGroup->setExclusive(true);
    editerAction = new QAction(modeGroup);
    editerAction->setCheckable(true);
    editerAction->setIcon(QIcon::fromTheme("document-new"));
    interrogerAction = new QAction(modeGroup);
    interrogerAction->setCheckable(true);
    interrogerAction->setChecked(true);
    interrogerAction->setIcon(QIcon::fromTheme("accessories-dictionary"));
    blancAction = new QAction(this);
    blancAction->setShortcut(QKeySequence("Ctrl+Z"));
    blancAction->setEnabled(false);
    exporterAction = new QAction(this);
    exporterAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    exporterAction->setIcon(QIcon::fromTheme("document-save"));
    importerAction = new QAction(this);
    importerAction->setShortcut(QKeySequence("Ctrl+Shift+O"));
    importerAction->setIcon(QIcon::fromTheme("document-open"));
    loginAction = new QAction(this);
    quitterAction = new QAction(this);
    quitterAction->setShortcut(QKeySequence("Ctrl+Q"));
    quitterAction->setIcon(QIcon::fromTheme("application-exit"));
    languesAppGroup = new QActionGroup(this);
    languesAppGroup->setExclusive(true);
    langueApp_deAct = new QAction(languesAppGroup);
    langueApp_deAct->setCheckable(true);
    langueApp_deAct->setChecked(true);
    langueApp_frAct = new QAction(languesAppGroup);
    langueApp_frAct->setCheckable(true);
    langueApp_enAct = new QAction(languesAppGroup);
    langueApp_enAct->setCheckable(true);
    languesGroup = new QActionGroup(this);
    languesGroup->setExclusive(true);
    langues_frdeAct = new QAction(languesGroup);
    langues_frdeAct->setIcon(QIcon(":/flags/de.png"));
    langues_frdeAct->setCheckable(true);
    langues_frdeAct->setChecked(true);
    langues_frenAct = new QAction(languesGroup);
    langues_frenAct->setIcon(QIcon(":/flags/en.png"));
    langues_frenAct->setCheckable(true);
    langues_frshAct = new QAction(languesGroup);
    langues_frshAct->setIcon(QIcon(":/flags/sh.png"));
    langues_frshAct->setCheckable(true);
    drillAction = new QAction(this);
    drillAction->setIcon(QIcon::fromTheme("object-rotate-right"));
    drillResetAction = new QAction(this);
    drillResetAction->setDisabled(true);
    persoAction = new QAction(this);
    persoAction->setIcon(QIcon::fromTheme("preferences-other"));
    swapAction = new QAction(this);
    swapAction->setCheckable(true);
    afficherChercherToolBarAct->setShortcut(QKeySequence("Ctrl+F"));
    aProposQtAction = new QAction(this);
    aProposQtAction->setIcon(QIcon::fromTheme("help-about"));

    connect(this,SIGNAL(editingModeChanged(bool)),centralWidget,SLOT(toggleEditingMode(bool)));
    connect(modeGroup,SIGNAL(triggered(QAction*)),this,SLOT(modeGroupTriggered()));
    connect(blancAction,SIGNAL(triggered()),centralWidget,SLOT(emptyFields()));
    connect(exporterAction,SIGNAL(triggered()),this,SLOT(exporterActionTriggered()));
    connect(importerAction,SIGNAL(triggered()),this,SLOT(importerActionTriggered()));
    connect(loginAction,SIGNAL(triggered()),this,SLOT(showLogin()));
    connect(quitterAction,SIGNAL(triggered()),this,SLOT(close()));
    connect(languesAppGroup,SIGNAL(triggered(QAction*)),this,SLOT(languesAppGroupTriggered()));
    connect(drillAction, SIGNAL(triggered()),this,SLOT(showDrillDialog()));
    connect(drillResetAction,SIGNAL(triggered()),centralWidget,SLOT(drillReset()));
    connect(persoAction,SIGNAL(triggered()),this,SLOT(showCustomisationWindow()));
    connect(languesGroup,SIGNAL(triggered(QAction*)),this,SLOT(languesGroupTriggered()));
    connect(swapAction,SIGNAL(triggered(bool)),centralWidget,SLOT(swapChanged(bool)));
    connect(aProposQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
}

void FluessigMain::createMenus(){
    fichierMenu = new QMenu(this);
    fichierMenu->addAction(interrogerAction);
    fichierMenu->addAction(editerAction);
    fichierMenu->addAction(blancAction);
    fichierMenu->addSeparator();
    fichierMenu->addAction(loginAction);
    fichierMenu->addAction(exporterAction);
    fichierMenu->addAction(importerAction);
    fichierMenu->addSeparator();
    fichierMenu->addAction(quitterAction);
    parametresMenu = new QMenu(this);
    languesAppMenu = new QMenu(this);
    parametresMenu->addMenu(languesAppMenu);
    languesAppMenu->addAction(langueApp_frAct);
    languesAppMenu->addAction(langueApp_deAct);
    languesAppMenu->addAction(langueApp_enAct);
    parametresMenu->addSeparator();
    parametresMenu->addAction(drillAction);
    parametresMenu->addAction(drillResetAction);
    parametresMenu->addSeparator();
    parametresMenu->addAction(persoAction);
    languesMenu = new QMenu(this);
    parametresMenu->addMenu(languesMenu);
    languesMenu->addAction(langues_frdeAct);
    languesMenu->addAction(langues_frenAct);
    languesMenu->addAction(langues_frshAct);
    parametresMenu->addAction(swapAction);
    parametresMenu->addSeparator();
    parametresMenu->addAction(afficherChercherToolBarAct);
    aProposMenu = new QMenu(this);
    aProposMenu->addAction(aProposQtAction);
    menuBar()->addMenu(fichierMenu);
    menuBar()->addMenu(parametresMenu);
    menuBar()->addMenu(aProposMenu);
}

void FluessigMain::createToolBars(){
    chercherToolBar = addToolBar(tr("Finden..."));
    chercherToolBar->setMovable(false);
    chercherEdit = new QLineEdit;
    chercherEdit->setPlaceholderText(tr("finden"));
    chercherEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    completeur = new QCompleter(this);
    chercherEdit->setCompleter(completeur);
    chercherAction = chercherEdit->addAction(QIcon(":/pictures/loupe.png"), QLineEdit::TrailingPosition);
    chercherPrecedantButton = new QToolButton;
    chercherPrecedantButton->setDisabled(true);
    chercherPrecedantButton->setIcon(QIcon(":/pictures/precedant.png"));
    chercherSuivantButton = new QToolButton;
    chercherSuivantButton->setDisabled(true);
    chercherSuivantButton->setIcon(QIcon(":/pictures/suivant.png"));
    chercherCasseBox = new QCheckBox;
    chercherToolBar->addWidget(chercherEdit);
    chercherToolBar->addWidget(chercherPrecedantButton);
    chercherToolBar->addWidget(chercherSuivantButton);
    chercherToolBar->addWidget(chercherCasseBox);
    afficherChercherToolBarAct = chercherToolBar->toggleViewAction();
    afficherChercherToolBarAct->setDisabled(true);
    chercherToolBar->hide();

    connect(afficherChercherToolBarAct,SIGNAL(toggled(bool)),this,SLOT(manageFindFocus(bool)));
    connect(chercherEdit,SIGNAL(textEdited(QString)),this,SLOT(manageCompleter()));
    connect(chercherEdit,SIGNAL(returnPressed()),chercherAction,SIGNAL(triggered()));
    connect(chercherAction,SIGNAL(triggered()),this,SLOT(setChercherIDs()));
    connect(chercherCasseBox,SIGNAL(toggled(bool)),this,SLOT(caseSensitivityChanged(bool)));
    connect(chercherPrecedantButton,SIGNAL(clicked()),this,SLOT(showPrevFound()));
    connect(chercherSuivantButton,SIGNAL(clicked()),this,SLOT(showNextFound()));
}

void FluessigMain::modeGroupTriggered(){
    bool editing = editerAction->isChecked();
    if(editing and !editingMode){
        int clic = QMessageBox::information(this, tr("Bearbeitungsmodus"),
                    tr("Achten Sie bitte darauf, daß Sie nun in die Bearbeitungsmodus eintreten. Jetzt "
                    "kann den Datenbank beändert werden.\nVielen dank für Ihren Beitrag!"),
                    QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok);
        if(clic == QMessageBox::Cancel){
            editerAction->setChecked(false);
            interrogerAction->setChecked(true);
        }else{
            editingMode = editing;
            blancAction->setEnabled(true);
            afficherChercherToolBarAct->setEnabled(true);
            emit editingModeChanged(editing);
        }
    }else if(editing and editingMode){
        QMessageBox::information(this, tr("Bearbeitungsmodus"),
           tr("Bearbeitungsmodus ist schon aktiv!"), QMessageBox::Ok);
    }else if(!editing and !editingMode){
        QMessageBox::information(this, tr("Anfragemodus"),
           tr("Anfragemodus ist schon akitv!"), QMessageBox::Ok);
    }else{
        editingMode = editing;
        blancAction->setDisabled(true);
        afficherChercherToolBarAct->setDisabled(true);
        chercherToolBar->hide();
        emit editingModeChanged(editing);
    }
}


//////////////////////////////////
///   __                    __
///   ** HANDLING LANGUAGES **
///
//////////////////////////////////
void FluessigMain::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        this->retraduisUI();
    else
        QMainWindow::changeEvent(event);
}

void FluessigMain::setCurrentLangueApp(Langue::LangueApp langue){
    switch(langue){
    case Langue::de:
        langueApp_deAct->setChecked(true);
        break;
    case Langue::fr:
        langueApp_frAct->setChecked(true);
        break;
    default:
        langueApp_enAct->setChecked(true);
    }
    languesAppGroupTriggered();
}

void FluessigMain::languesAppGroupTriggered(){
//    qDebug() << "remove own translator : " << qApp->removeTranslator(&tradOwn);
//    qDebug() << "remove qt translator  : " << qApp->removeTranslator(&tradQt);
    if(langueApp_frAct->isChecked()){
        if(tradOwn.load(":/translations/fluessig_fr"))
            qApp->installTranslator(&tradOwn);
        if(tradQt.load(QLocale::French,"qt","_",QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
            qApp->installTranslator(&tradQt);
    }else if(langueApp_deAct->isChecked()){
        if(tradQt.load(QLocale::German,"qt","_",QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
            qApp->installTranslator(&tradQt);
            qDebug() << "JA! Deutsche Sprache erfolgreich installiert!";
        }else{
            qDebug() << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
        }
    }else if(langueApp_enAct->isChecked()){
        if(tradQt.load(QLocale::English,"qt","_",QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
            qApp->installTranslator(&tradQt);
            qDebug() << "YEAH! English locale installed successfully!";
        }else{
            qDebug() << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
        }
        if(tradOwn.load(":/translations/fluessig_en"))
            qApp->installTranslator(&tradOwn);
    }
}

void FluessigMain::retraduisUI(){
    qDebug() << "FluessigMain::retraduisUI(): enter";
    logWizard->retraduisUI();
    chercherCasseBox->setText(tr("Groß-/Kleinschreibung beachten"));
    editerAction->setText(tr("Wörter &hinzufügen"));
    interrogerAction->setText(tr("&Abfragen beginnen"));
    blancAction->setText(tr("Datenfelder &löschen"));
    if(logWizard->getCurrentUser() == "")
        loginAction->setText(tr("&Anmelden..."));
    else
        loginAction->setText(tr("Benutzer &wechseln..."));
    exporterAction->setText(tr("Datenbank &exportieren..."));
    importerAction->setText(tr("Datei &importieren..."));
    quitterAction->setText(tr("&Beenden"));
    langueApp_frAct->setText(tr("&Französisch"));
    langueApp_deAct->setText(tr("&Deutsch"));
    langueApp_enAct->setText(tr("&Englisch"));
    drillAction->setText(tr("&Intensivmodus..."));
    drillResetAction->setText(tr("&Neustart"));
    persoAction->setText(tr("&Anpassen..."));
    aProposQtAction->setText(tr("Über &Qt..."));
    fichierMenu->setTitle(tr("&Datei"));
    parametresMenu->setTitle(tr("&Einstellungen"));
    languesAppMenu->setTitle(tr("Anwendungs&sprache"));
    languesMenu->setTitle(tr("Sprache &lernen"));
    langues_frdeAct->setText(tr("Französisch-&Deutsch"));
    langues_frenAct->setText(tr("Französisch-&Englisch"));
    langues_frshAct->setText(tr("Französisch-&Albanisch"));
    swapAction->setText(tr("Sprachen &wechseln"));
    afficherChercherToolBarAct->setText(tr("Finden..."));
    aProposMenu->setTitle(tr("&Hilfe"));
    centralWidget->retraduisUI();
    drillDialog->retraduisUI();
    persoWindow->retraduisUI();
    importDialog->retraduisUI();
}

void FluessigMain::languesGroupTriggered(){
    if(langues_frdeAct->isChecked()){
        logWizard->setCurrentLanguage("de");
        centralWidget->setLanguage("Fr_de");
    }else if(langues_frenAct->isChecked()){
        logWizard->setCurrentLanguage("en");
        centralWidget->setLanguage("Fr_en");
    }else{
        logWizard->setCurrentLanguage("sh");
        centralWidget->setLanguage("Fr_sh");
    }
    if(editingMode) centralWidget->retraduisUI();// change placeholder text!
}

//accessor
Langue::LangueApp FluessigMain::currentLangueApp() const{
    const QList<QAction*> listAct = languesAppGroup->actions();
    QAction* currentAct     = languesAppGroup->checkedAction();
    const Langue::LangueApp langue= static_cast<Langue::LangueApp>(listAct.indexOf(currentAct));
    return langue;
}

////////////////////////////
///   __               __
///   ** SAVE, RESTORE **
///
////////////////////////////
void FluessigMain::saveSettings(){
    QSettings settings;

    settings.beginGroup("Users");
    QMap<QString,QVariant> userVariant;
    QMap<QString,QStringList> userParams = logWizard->getUserParams();
    for(int i=0; i < userParams.size(); i++){
        QString username = userParams.keys().at(i);
        QVariant languages = userParams.values().at(i);
        userVariant[username] = languages;
    }
    settings.setValue("parameters",userVariant);
    settings.setValue("default app language",currentLangueApp());
    settings.endGroup();
    saveUserSettings();
}

void FluessigMain::saveUserSettings(){
    QSettings settings;

    QString user = logWizard->getCurrentUser();
    if(user != ""){
        settings.beginGroup(user);
        settings.setValue("drill/checked", drillDialog->getDrill());
        settings.setValue("drill/value", drillDialog->getValue());
        settings.setValue("perso/life", persoWindow->getLife());
        settings.setValue("perso/dust", persoWindow->getDust());
        settings.setValue("perso/freq", persoWindow->getFreq());
        settings.setValue("perso/hist", persoWindow->getHist());
        settings.setValue("perso/police/famille", persoWindow->getCurrentFont());
        settings.setValue("perso/police/taille", persoWindow->getFontSize());
        settings.setValue("perso/police/gras", persoWindow->getBold());
        settings.setValue("perso/police/italique", persoWindow->getItalic());
        settings.setValue("perso/col/front", persoWindow->getLanguageColoursFront());
        settings.setValue("perso/col/back", persoWindow->getLanguageColoursBack());
        settings.setValue("perso/col/genres", persoWindow->getGenderColours());
        settings.setValue("perso/genres", persoWindow->getGenders());
        settings.setValue("langue/fr", langueApp_frAct->isChecked());
        settings.setValue("langue/de", langueApp_deAct->isChecked());
        settings.setValue("langue/en", langueApp_enAct->isChecked());
        settings.endGroup();
    }
}

void FluessigMain::restoreSettings(){
    QSettings settings;

    settings.beginGroup("Users");
    QMap<QString,QVariant> userVariant = settings.value("parameters").toMap();
    QMap<QString,QStringList> userParams;
    for(int i=0; i < userVariant.size(); i++){
        QString username      = userVariant.keys().at(i);
        QStringList languages = userVariant.values().at(i).toStringList();
        userParams[username] = languages;
    }
    settings.endGroup();
    // update & login
    logWizard    = new LoginWizard(userParams, this);
    drillDialog  = new DrillDialog(this);
    persoWindow  = new CustomisationWindow(this);
    importDialog = new ImportDialog(this);
    // update language
    settings.beginGroup("Users");
    QVariant defaultVariant = settings.value("default app language",Langue::en);
    Langue::LangueApp defaultLang = defaultVariant.value<Langue::LangueApp>();
    setCurrentLangueApp(defaultLang);
    settings.endGroup();
}

void FluessigMain::restoreUserSettings(){
    QSettings settings;

    QString user = logWizard->getCurrentUser();
    if(user != ""){
        int value;
        QStringList front,back;
        QString str;
        bool b;
        // retrieve from storage
        settings.beginGroup(user);
        // drill params
        b = settings.value("drill/checked", false).toBool();
        value = settings.value("drill/value", 10).toInt();
        drillDialog->setDrill(b);
        drillDialog->setValue(value);
        // slider values
        value = settings.value("perso/life", 0).toInt();
        centralWidget->setLife(value);
        persoWindow->setLife(value);
        value = settings.value("perso/dust", 0).toInt();
        centralWidget->setDust(value);
        persoWindow->setDust(value);
        value = settings.value("perso/freq", 0).toInt();
        centralWidget->setFreq(value);
        persoWindow->setFreq(value);
        value = settings.value("perso/hist", 0).toInt();
        centralWidget->setHist(value);
        persoWindow->setHist(value);
        // font
        str = settings.value("perso/police/famille", "Arial").toString();
        persoWindow->setCurrentFont(str);
        value = settings.value("perso/police/taille", 14).toInt();
        persoWindow->setFontSize(value);
        b = settings.value("perso/police/gras", true).toBool();
        persoWindow->setBold(b);
        b = settings.value("perso/police/italique", false).toBool();
        persoWindow->setItalic(b);
        // colours
        value = logWizard->getCurrentLanguages().size();
        front << "#000000";
        back << "#FFFFFF";
        front = settings.value("perso/col/front", front).toStringList();
        back  = settings.value("perso/col/back", back).toStringList();
        for(int i=front.size(); i<value+1; i++){
            front << "#000000";
            back << "#FFFFFF";
        }
        persoWindow->setLanguageColours(front,back);
        front.clear();
        front << "#000000" << "#000000" << "#000000";
        front = settings.value("perso/col/genres", front).toStringList();
        persoWindow->setGenderColours(front);
        b = settings.value("perso/genres", true).toBool();
        persoWindow->setGenders(b);
        persoWindow->setCurrentLanguages(logWizard->getCurrentLanguages());
        langueApp_frAct->setChecked(settings.value("langue/fr", false).toBool());
        langueApp_deAct->setChecked(settings.value("langue/de", true).toBool());
        langueApp_enAct->setChecked(settings.value("langue/en", false).toBool());
        updateFromDrill();
        updateFromCustomised();
        settings.endGroup();
    }
    languesAppGroupTriggered();
}

void FluessigMain::disableActions(bool b){
    editerAction->setDisabled(b);
    exporterAction->setDisabled(b);
    importerAction->setDisabled(b);
    languesGroup->setDisabled(b);
    persoAction->setDisabled(b);
    drillAction->setDisabled(b);
    swapAction->setDisabled(b);
}

void FluessigMain::availableLanguagesChanged(){
    QStringList languages = logWizard->getCurrentLanguages();
    QString language = logWizard->getCurrentLanguage();
    langues_frdeAct->setEnabled(languages.contains("de"));
    langues_frdeAct->setChecked(language == "Fr_de");
    langues_frenAct->setEnabled(languages.contains("en"));
    langues_frenAct->setChecked(language == "Fr_en");
    langues_frshAct->setEnabled(languages.contains("sh"));
    langues_frshAct->setChecked(language == "Fr_sh");
}

void FluessigMain::exporterActionTriggered(){
    ExportDialog exportDialog(this);
    QStringList allLanguages;
    allLanguages << "Fr_de" << "Fr_en" << "Fr_sh";
    exportDialog.setAvailableLanguages(logWizard->getCurrentLanguages());
    exportDialog.setMinDates(centralWidget->getMinCreationDates(allLanguages));
    if(exportDialog.exec() == QDialog::Accepted){
        QDate fromDate = exportDialog.getFromDate();
        QString tableName = exportDialog.getTableName();
        QString fileName = exportDialog.getFileName();
        centralWidget->exportTable(fromDate, tableName, fileName);
    }
}

void FluessigMain::importerActionTriggered(){
    importDialog->exec();
}

///////////////////////////////////////////////////
///   __                                      __
///   ** LOGIN, DRILL & CUSTOMISATION WINDOWS **
///
///////////////////////////////////////////////////
/// show login window (choose existing user/create new user)
void FluessigMain::showLogin(){
    saveUserSettings();
    logWizard->restart();
    // sync app language before showing wizard
    logWizard->setCurrentAppLanguage(currentLangueApp());
    int ret = logWizard->exec();
    if(ret == QDialog::Accepted){
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QString user = logWizard->getCurrentUser();
        centralWidget->setUser(user);
        availableLanguagesChanged();
        languesGroupTriggered();
        centralWidget->disableButtons(false);
        disableActions(false);
        retraduisUI();// update loginAction
        restoreUserSettings();
        centralWidget->updateCurrentUserTableEntries();
        emit editingModeChanged(editingMode);
        QApplication::restoreOverrideCursor();
    }
}

/// show drill dialog (yes/no and how many words in a row)
void FluessigMain::showDrillDialog(){
    int ret = drillDialog->exec();
    if(ret == QDialog::Accepted){
        drillDialog->resetToOldValues(false);
        updateFromDrill();
    }else{
        drillDialog->resetToOldValues(true);
    }
}

void FluessigMain::updateFromDrill(){
    bool b = drillDialog->getDrill();
    int value = drillDialog->getValue();
    centralWidget->setDrill(b, value);
    drillResetAction->setEnabled(b);
}

/// show customisation window (sliders)
void FluessigMain::showCustomisationWindow(){
    int ret = persoWindow->exec();
    if(ret == QDialog::Accepted){
        persoWindow->resetToOldValues(false);
        updateFromCustomised();
    }else{
        persoWindow->resetToOldValues(true);
    }
}

void FluessigMain::updateFromCustomised(){
    centralWidget->setLife(persoWindow->getLife());
    centralWidget->setDust(persoWindow->getDust());
    centralWidget->setFreq(persoWindow->getFreq());
    centralWidget->setHist(persoWindow->getHist());
    QStringList quest,ans,gdr;
    quest = persoWindow->getLanguageColours("fr");
    ans   = persoWindow->getLanguageColours(logWizard->getCurrentLanguage());
    if(persoWindow->getGenders())
        gdr   = persoWindow->getGenderColours();
    else
        gdr << ans.at(0) << ans.at(0) << ans.at(0);
    centralWidget->setColours(quest, ans, gdr);
    QFont police(persoWindow->getCurrentFont(), persoWindow->getFontSize());
    police.setBold(persoWindow->getBold());
    police.setItalic(persoWindow->getItalic());
    centralWidget->updateFont(police);
}

///////////////////////////////////////////////////
///   __                       __
///   ** SEARCH & FIND TOOLBAR **
///
///////////////////////////////////////////////////
void FluessigMain::manageFindFocus(bool b){
    if(b) chercherEdit->setFocus();
}

void FluessigMain::manageCompleter(){
    chercherPrecedantButton->setDisabled(true);
    chercherSuivantButton->setDisabled(true);
    QString currentLetters=chercherEdit->text();
    if(currentLetters.length() < 3){
        chercherEdit->setStyleSheet("QLineEdit { color: black }");
        firstLetters = "";
    }
    if(currentLetters.length() >= 3 and currentLetters.left(3) != firstLetters){
        firstLetters = currentLetters.left(3);
        QStringList list = centralWidget->match(firstLetters);
        if(list.empty()){
            chercherEdit->setStyleSheet("QLineEdit { color: red }");
        }else{
            chercherEdit->setStyleSheet("QLineEdit { color: black }");
            delete completeur;
            completeur = new QCompleter(this);
            completeur->setMaxVisibleItems(8);
            completeur->setFilterMode(Qt::MatchContains);
            list.removeDuplicates();
            QStringListModel* model = new QStringListModel(list,completeur);
            completeur->setModel(model);
            chercherEdit->setCompleter(completeur);
        }
    }
}

void FluessigMain::setChercherIDs(){
    QString word=chercherEdit->text();
    Qt::CaseSensitivity sensitive=Qt::CaseInsensitive;
    if(chercherCasseBox->isChecked())
        sensitive = Qt::CaseSensitive;
    chercherIDs = centralWidget->find(word, sensitive);
    if(chercherIDs.size() == 0){
        chercherEdit->setStyleSheet("QLineEdit { color: red }");
    }else{
        if(chercherIDs.size() > 1)
            chercherSuivantButton->setEnabled(true);
        currentIDIndex = 0;
        centralWidget->giveQuestion(chercherIDs[currentIDIndex]);
    }
}

void FluessigMain::showPrevFound(){
    centralWidget->giveQuestion(chercherIDs[--currentIDIndex]);
    if(currentIDIndex == 0)
        chercherPrecedantButton->setDisabled(true);
    if(currentIDIndex < chercherIDs.size()-1)
        chercherSuivantButton->setEnabled(true);
}

void FluessigMain::showNextFound(){
    centralWidget->giveQuestion(chercherIDs[++currentIDIndex]);
    if(currentIDIndex == chercherIDs.size()-1)
        chercherSuivantButton->setDisabled(true);
    if(currentIDIndex > 0)
        chercherPrecedantButton->setEnabled(true);
}

void FluessigMain::caseSensitivityChanged(bool checked){
    Qt::CaseSensitivity sensitivity=Qt::CaseInsensitive;
    if(checked)
        sensitivity = Qt::CaseSensitive;
    completeur->setCaseSensitivity(sensitivity);
}

