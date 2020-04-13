#include <QVBoxLayout>
#include <QGridLayout>
#include <QFont>
#include <QRegExp>
#include <QButtonGroup>
#include <QDebug>
#include "loginwizard.h"

LoginWizard::LoginWizard(const QMap<QString, QStringList>& userParams, QWidget* parent) :
    QWizard(parent), userParams(userParams)
{
    introPage = new IntroPage;
    setPage(Page_Intro, introPage);
    usernamePage = new UsernamePage(userParams.keys());
    setPage(Page_Username, usernamePage);
    userChoicePage = new UserChoicePage(userParams.keys());
    setPage(Page_UserChoice, userChoicePage);
    languagesRegisterPage = new LanguagesRegisterPage(userParams);
    setPage(Page_LanguagesRegister, languagesRegisterPage);
    languagesChoicePage = new LanguagesChoicePage;
    setPage(Page_LanguagesChoice, languagesChoicePage);
    confirmationPage = new ConfirmationPage;
    setPage(Page_Confirmation, confirmationPage);

    setStartId(Page_Intro);
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/pictures/books.png"));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/pictures/icon.png"));// TODO: does not show (style?)

    connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(managePageButtons(int)));
    connect(userChoicePage,SIGNAL(completeChanged()),this,SLOT(setAvailableLanguages()));
    connect(introPage,SIGNAL(appLanguageChanged(Langue::LangueApp)),this,SIGNAL(appLanguageChanged(Langue::LangueApp)));
}

void LoginWizard::managePageButtons(int id){
    if(id == Page_LanguagesChoice){
        if(field("nouveau").toBool()){
            button(QWizard::BackButton)->setDisabled(true);
            registerNewUser();
        }else if(field("modifier").toBool()){
            button(QWizard::BackButton)->setDisabled(true);
            updateExistingUser();
        }
    }
}

void LoginWizard::registerNewUser(){
    QString newUser = field("utilisateur").toString();
    QStringList newLanguages;
    if(field("de").toBool()) newLanguages << "de";
    if(field("en").toBool()) newLanguages << "en";
    if(field("sh").toBool()) newLanguages << "sh";
    userParams.insert(newUser, newLanguages);
    userChoicePage->setNewUser(newUser);
    languagesRegisterPage->setNewUser(newUser, newLanguages);
}

void LoginWizard::updateExistingUser(){
    QString existingUser  = field("utilisateurs").toString();
    QStringList languages = userParams.value(existingUser);
    if(field("de").toBool()) languages.append("de");
    if(field("en").toBool()) languages.append("en");
    if(field("sh").toBool()) languages.append("sh");
    languages.removeDuplicates();
    userParams.insert(existingUser, languages);
}

void LoginWizard::setAvailableLanguages(){
    QString existingUser  = field("utilisateurs").toString();
    QStringList languages = userParams.value(existingUser);
    setField("de", languages.contains("de"));
    setField("en", languages.contains("en"));
    setField("sh", languages.contains("sh"));
}


/////////////////////////////////
///   __                   __
///   ** INTRODUCTORY PAGE **
///
/////////////////////////////////
IntroPage::IntroPage(QWidget *parent) :
    QWizardPage(parent)
{
    descriptionLabel = new QLabel;
    descriptionLabel->setWordWrap(true);
    langueApplicationGroupBox = new QGroupBox;
    langueApplicationCombo    = new QComboBox;
    langueApplicationCombo->addItems(QStringList() << "Deutsch" << "Français" << "English");
    langueApplicationCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    QVBoxLayout* langueLayout = new QVBoxLayout;
    langueLayout->addWidget(langueApplicationCombo);
    langueApplicationGroupBox->setLayout(langueLayout);
    statutUtilisateurGroupBox = new QGroupBox;
    nouveauButton             = new QRadioButton;
    nouveauButton->setChecked(true);
    existantButton            = new QRadioButton;
    modifierButton            = new QRadioButton;
    QButtonGroup* radioGroup = new QButtonGroup(this);
    radioGroup->setExclusive(true);
    radioGroup->addButton(nouveauButton);
    radioGroup->addButton(existantButton);
    radioGroup->addButton(modifierButton);
    QVBoxLayout* statutLayout = new QVBoxLayout;
    statutLayout->addWidget(nouveauButton);
    statutLayout->addWidget(existantButton);
    statutLayout->addWidget(modifierButton);
    statutUtilisateurGroupBox->setLayout(statutLayout);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(descriptionLabel);
    layout->addSpacing(10);
    layout->addWidget(langueApplicationGroupBox);
    layout->addSpacing(10);
    layout->addWidget(statutUtilisateurGroupBox);
    setLayout(layout);
    // keep link for user modification and wizard management
    registerField("nouveau", nouveauButton);
    registerField("modifier", modifierButton);
    retraduisUI();

    connect(langueApplicationCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(appLanguageChanged(int)));
}

int IntroPage::nextId() const{
    if(nouveauButton->isChecked()){
        return LoginWizard::Page_Username;
    }else{
        return LoginWizard::Page_UserChoice;
    }
}

void IntroPage::retraduisUI(){
    setTitle(tr("Wilkommen!"));
    descriptionLabel->setText(tr("Mit diesem Anmeldungassistent können Sie sich entweder "
                                      "anmelden, oder als neuer Verwender hinzufügen werden, oder "
                                      "Ihren Profil verändern. Wählen Sie eine Option und klicken Sie auf 'Weiter'."));
    langueApplicationGroupBox->setTitle(tr("Sprache"));
    statutUtilisateurGroupBox->setTitle(tr("Benutzer"));
    nouveauButton->setText(tr("Neuer Benutzer"));
    existantButton->setText(tr("Vorhandener Benutzer"));
    modifierButton->setText(tr("Benutzereinstellungen ändern"));
}

void IntroPage::appLanguageChanged(int index){
    Langue::LangueApp langue = static_cast<Langue::LangueApp>(index);
    qDebug() << "IntroPage::appLanguageChanged(): langue vaut " << langue;
    emit appLanguageChanged(langue);
}


///////////////////////////////////////
///   __                         __
///   ** CHOICE OF USERNAME PAGE **
///
///////////////////////////////////////
UsernamePage::UsernamePage(const QStringList &users, QWidget *parent) :
    QWizardPage(parent)
{
    this->users = users;
    descriptionLabel = new QLabel;
    utilisateurEdit = new QLineEdit;
    validateur = new QRegExpValidator(QRegExp("^[A-Z].[A-Za-z-]*"),this);
    utilisateurEdit->setValidator(validateur);
    userExistsLabel = new QLabel;
    userExistsLabel->setStyleSheet("QLabel { color : red; }");
    userExistsLabel->hide();
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(descriptionLabel);
    layout->addWidget(utilisateurEdit);
    layout->addWidget(userExistsLabel);
    setLayout(layout);
    // keep links for confirmation page
    registerField("utilisateur",utilisateurEdit);
    retraduisUI();

    connect(utilisateurEdit,SIGNAL(textEdited(QString)),this,SLOT(showUserExists()));
    connect(utilisateurEdit,SIGNAL(textEdited(QString)),this,SIGNAL(completeChanged()));
}

int UsernamePage::nextId() const{
    return LoginWizard::Page_LanguagesRegister;
}

bool UsernamePage::userExists() const{
    if(users.contains(utilisateurEdit->text())) return true;
    else return false;
}

void UsernamePage::showUserExists(){
    if(userExists())
        userExistsLabel->show();
    else
        userExistsLabel->hide();
}

bool UsernamePage::isComplete() const{
    if(userExists() or utilisateurEdit->text()=="")
        return false;
    else
        return true;
}

void UsernamePage::retraduisUI(){
    setTitle(tr("Benutzername"));
    descriptionLabel->setText(tr("Benutzername:"));
    utilisateurEdit->setPlaceholderText(tr("Name"));
    userExistsLabel->setText(tr("Dieser Benutzername wurde leider schon gewählt!"));
}


//////////////////////////////////////////////////
///   __                                    __
///   ** CHOOSE AMONGST EXISTING USERS PAGE **
///
//////////////////////////////////////////////////
UserChoicePage::UserChoicePage(const QStringList &usernames, QWidget *parent) :
    QWizardPage(parent)
{
    descriptionLabel = new QLabel;
    descriptionLabel->setWordWrap(true);
    utilisateursBox = new QComboBox;
    utilisateursBox->addItem("");// completed in retraduisUI()
    utilisateursBox->setItemData(0, QBrush(Qt::gray), Qt::ForegroundRole);
    utilisateursBox->addItems(usernames);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(descriptionLabel);
    layout->addWidget(utilisateursBox);
    setLayout(layout);
    // keep link for languages choice page
    registerField("utilisateurs", utilisateursBox, "currentText");
    retraduisUI();

    connect(utilisateursBox,SIGNAL(currentIndexChanged(int)),this,SIGNAL(completeChanged()));
}

bool UserChoicePage::isComplete() const{
    if(utilisateursBox->currentIndex() > 0) return true;
    else return false;
}

int UserChoicePage::nextId() const{
    if(field("modifier").toBool()){
        return LoginWizard::Page_LanguagesRegister;
    }else{
        return LoginWizard::Page_LanguagesChoice;
    }
}

void UserChoicePage::setNewUser(QString const &user){
    utilisateursBox->addItem(user);
    utilisateursBox->setCurrentText(user);
}

void UserChoicePage::retraduisUI(){
    setTitle(tr("Benutzer wählen"));
    descriptionLabel->setText(tr("Wählen Sie einen der folgenden Benutzernamen."));
    utilisateursBox->setItemText(0,tr("-- Verwender wählen --"));
}


//////////////////////////////////////////
///   __                             __
///   ** REGISTER FOR LANGUAGES PAGE **
///
//////////////////////////////////////////
LanguagesRegisterPage::LanguagesRegisterPage(QMap<QString, QStringList> userParams, QWidget *parent) :
    QWizardPage(parent)
{
    this->userParams = userParams;
    descriptionLabel = new QLabel;
    descriptionLabel->setWordWrap(true);
    deBox = new QCheckBox;
    enBox = new QCheckBox;
    shBox = new QCheckBox;
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(descriptionLabel);
    layout->addWidget(deBox);
    layout->addWidget(enBox);
    layout->addWidget(shBox);
    setLayout(layout);
    // keep links for confirmation page and for registering new user
    registerField("de",deBox);
    registerField("en",enBox);
    registerField("sh",shBox);
    retraduisUI();

    connect(deBox,SIGNAL(clicked()),this,SIGNAL(completeChanged()));
    connect(enBox,SIGNAL(clicked()),this,SIGNAL(completeChanged()));
    connect(shBox,SIGNAL(clicked()),this,SIGNAL(completeChanged()));
}

void LanguagesRegisterPage::initializePage(){
    QString curUser = field("utilisateurs").toString();
    if(!field("nouveau").toBool()){
        if(userParams[curUser].contains("de")){
            deBox->setChecked(true);
            deBox->setDisabled(true);
        }
        if(userParams[curUser].contains("en")){
            enBox->setChecked(true);
            enBox->setDisabled(true);
        }
        if(userParams[curUser].contains("sh")){
            shBox->setChecked(true);
            shBox->setDisabled(true);
        }
    }
}

bool LanguagesRegisterPage::isComplete() const{
    if(deBox->isChecked() or enBox->isChecked() or shBox->isChecked())
        return true;
    else
        return false;
}

int LanguagesRegisterPage::nextId() const{
    return LoginWizard::Page_Confirmation;
}

void LanguagesRegisterPage::setNewUser(const QString &user, const QStringList &languages){
    userParams.insert(user, languages);
}

void LanguagesRegisterPage::retraduisUI(){
    setTitle(tr("Sprachen"));
    descriptionLabel->setText(tr("Welche Sprachen wünschen Sie verbessern?"));
    deBox->setText(tr("Französisch-Deutsch"));
    enBox->setText(tr("Französisch-Englisch"));
    shBox->setText(tr("Französisch-Albanisch"));
}


////////////////////////////////////
///   __                       __
///   ** CHOOSE LANGUAGES PAGE **
///
////////////////////////////////////
LanguagesChoicePage::LanguagesChoicePage(QWidget *parent) :
    QWizardPage(parent)
{
    descriptionLabel = new QLabel;
    descriptionLabel->setWordWrap(true);
    deButton = new QRadioButton;
    enButton = new QRadioButton;
    shButton = new QRadioButton;
    QButtonGroup* radioGroup = new QButtonGroup(this);
    radioGroup->setExclusive(true);
    radioGroup->addButton(deButton);
    radioGroup->addButton(enButton);
    radioGroup->addButton(shButton);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(descriptionLabel);
    layout->addWidget(deButton);
    layout->addWidget(enButton);
    layout->addWidget(shButton);
    setLayout(layout);
    // keep link for accessors
    registerField("de courante",deButton);
    registerField("en courante",enButton);
    registerField("sh courante",shButton);
    retraduisUI();
}

void LanguagesChoicePage::initializePage(){
    bool state;
    state = field("sh").toBool();
    shButton->setChecked(state);
    shButton->setEnabled(state);
    state = field("en").toBool();
    enButton->setChecked(state);
    enButton->setEnabled(state);
    state = field("de").toBool();
    deButton->setChecked(state);
    deButton->setEnabled(state);
}

int LanguagesChoicePage::nextId() const{
    return -1;
}

void LanguagesChoicePage::retraduisUI(){
    setTitle(tr("Sprachen"));
    descriptionLabel->setText(tr("Welche Sprachen wünschen Sie nun verbessern?"));
    deButton->setText(tr("Französisch-Deutsch"));
    enButton->setText(tr("Französisch-Englisch"));
    shButton->setText(tr("Französisch-Albanisch"));
}


//////////////////////////////////////////////////
///   __                                 __
///   ** SUMMARY PAGE AFTER REGISTRATION **
///
//////////////////////////////////////////////////
ConfirmationPage::ConfirmationPage(QWidget *parent) :
    QWizardPage(parent)
{
    oui = QPixmap(":/pictures/ouiicon.png");
    oui = oui.scaled(15,15);
    non = QPixmap(":/pictures/nonicon.png");
    non = non.scaled(15,15);
    descriptionLabel = new QLabel;
    descriptionLabel->setWordWrap(true);
    userLabel = new QLabel;
    utilisateurLabel = new QLabel;
    QFont police;
    police.setItalic(true);
    utilisateurLabel->setFont(police);
    langLabel = new QLabel;
    deLabel = new QLabel;
    enLabel = new QLabel;
    shLabel = new QLabel;
    dePixLabel = new QLabel;
    enPixLabel = new QLabel;
    shPixLabel = new QLabel;
    dePixLabel->setPixmap(non);
    enPixLabel->setPixmap(non);
    shPixLabel->setPixmap(non);
    finLabel = new QLabel;
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(userLabel,0,0,1,2);
    layout->addWidget(utilisateurLabel,1,0,1,2);

    layout->addWidget(langLabel,3,0,1,2);
    layout->addWidget(dePixLabel,4,0);
    layout->addWidget(enPixLabel,5,0);
    layout->addWidget(shPixLabel,6,0);
    layout->addWidget(deLabel,4,1);
    layout->addWidget(enLabel,5,1);
    layout->addWidget(shLabel,6,1);
    layout->addWidget(finLabel,8,0);
    layout->setHorizontalSpacing(5);
    layout->setRowMinimumHeight(2,5);
    setLayout(layout);
    setFixedSize(minimumSizeHint());
    retraduisUI();
}

int ConfirmationPage::nextId() const{
    return LoginWizard::Page_LanguagesChoice;
}

void ConfirmationPage::initializePage(){
    QString user;
    if(field("nouveau").toBool()){
        user = field("utilisateur").toString();
    }else if(field("modifier").toBool()){
        user = field("utilisateurs").toString();
    }
    utilisateurLabel->setText(user);
    bool de = field("de").toBool();
    if(de){
        dePixLabel->setPixmap(oui);
    }else{
        dePixLabel->setPixmap(non);
    }
    bool en = field("en").toBool();
    if(en){
        enPixLabel->setPixmap(oui);
    }else{
        enPixLabel->setPixmap(non);
    }
    bool sh = field("sh").toBool();
    if(sh){
        shPixLabel->setPixmap(oui);
    }else{
        shPixLabel->setPixmap(non);
    }
}

void ConfirmationPage::retraduisUI(){
    setTitle(tr("Bestätigung"));
    descriptionLabel->setText(tr("Wilkommen beim Einfach fließend %1! "
                                 "Sie können nun Ihren Wortschatz verbessern "
                                 "oder neue Wörter in der Datenbank hinzufügen."));
    userLabel->setText(tr("Benutzername:"));
    langLabel->setText(tr("Gewählte Sprachen:"));
    deLabel->setText(tr("Französich-Deutsch"));
    enLabel->setText(tr("Französisch-Englisch"));
    shLabel->setText(tr("Französisch-Albanien"));
    finLabel->setText(tr("Klicken Sie auf 'Fertig', um Ihre Wahlen zu bestätigen."));
}


////////////////////////
///   __           __
///   ** ACCESSORS **
///
////////////////////////
QString LoginWizard::getCurrentUser() const{
    qDebug() << "LoginWizard::getCurrentUser(): enter";
    QString curUser = field("utilisateurs").toString();
    if(curUser == tr("-- Verwender wählen --")){
        return "";
    }else{
        return curUser;
    }
}

QString LoginWizard::getCurrentLanguage() const{
    if(field("de courante").toBool()){
        return "Fr_de";
    }else if(field("en courante").toBool()){
        return "Fr_en";
    }else if(field("sh courante").toBool()){
        return "Fr_sh";
    }else{
        qDebug() << "Error in LoginWizard::getCurrentLanguage()";
        return "Fr_de";
    }
}

void LoginWizard::setCurrentLanguage(const QString &language){
    if(language == "de"){
        setField("de courante",true);
    }else if(language == "en"){
        setField("en courante",true);
    }else if(language == "sh"){
        setField("sh courante",true);
    }else{
        qDebug() << "Error in LoginWizard::setCurrentLanguage()";
    }
}

QStringList LoginWizard::getCurrentLanguages() const{
    QString user = getCurrentUser();
    if(user != ""){
        return userParams[user];
    }else{
        qDebug() << "Error in LoginWizard::getCurrentLanguages(): unknown current user";
        return QStringList("");
    }
}

QMap<QString,QStringList> LoginWizard::getUserParams() const{
    return userParams;
}

void LoginWizard::retraduisUI(){
    qDebug() << "LoginWizard::retraduisUI(): enter";
    setWindowTitle(tr("Einfach fließend: Anmeldung"));
    introPage->retraduisUI();
    usernamePage->retraduisUI();
    userChoicePage->retraduisUI();
    languagesRegisterPage->retraduisUI();
    languagesChoicePage->retraduisUI();
    confirmationPage->retraduisUI();
}

void LoginWizard::setCurrentAppLanguage(const Langue::LangueApp &language){
    introPage->setCurrentAppLanguage(language);
}

void IntroPage::setCurrentAppLanguage(const Langue::LangueApp &language){
    langueApplicationCombo->setCurrentIndex(language);
}
