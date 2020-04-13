#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include "loginwindow.h"
#include "loginwizard.h"

LoginWindow::LoginWindow(QMap<QString, QStringList> userParams, QWidget *parent) :
    QDialog(parent)
{
    this->userParams = userParams;
    QLabel* utilisateurLabel = new QLabel(tr("Wählen Sie einen Verwendername:"));
    utilisateursBox = new QComboBox;
    populateBox();
    // languages
    QVBoxLayout* radioLayout = new QVBoxLayout;
    radioGroup = new QButtonGroup(this);
    radioGroup->setExclusive(true);
    deRadio = new QRadioButton(tr("Französisch-Deutsch"));
    enRadio = new QRadioButton(tr("Französisch-Englisch"));
    shRadio = new QRadioButton(tr("Französisch-Albanisch"));
    radioGroup->addButton(deRadio, 1);
    radioGroup->addButton(enRadio, 2);
    radioGroup->addButton(shRadio, 3);
    deRadio->setChecked(true);
    radioLayout->addWidget(deRadio);
    radioLayout->addWidget(enRadio);
    radioLayout->addWidget(shRadio);
    choixBox = new QGroupBox(tr("Bitte wählen Sie eine Sprache:"));
    choixBox->setLayout(radioLayout);
    choixBox->setDisabled(true);
    // buttons
    nouveauButton = new QPushButton(tr("Neue(r) Verwender(in)?"));
    boutonsGroup = new QDialogButtonBox(this);
    boutonsGroup->addButton(QDialogButtonBox::Ok);
    boutonsGroup->button(QDialogButtonBox::Ok)->setDisabled(true);
    boutonsGroup->addButton(QDialogButtonBox::Cancel);
    // main layout
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(utilisateurLabel, 0, 0, 1, 2);
    layout->addWidget(utilisateursBox, 1, 0);
    layout->addWidget(nouveauButton, 1, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(choixBox, 2, 0, 1, 2);
    layout->addWidget(boutonsGroup, 3, 1, 1, 2, Qt::AlignRight);
    this->setLayout(layout);

    connect(boutonsGroup, SIGNAL(accepted()), this, SLOT(accept()));
    connect(boutonsGroup, SIGNAL(rejected()), this, SLOT(reject()));
    connect(nouveauButton, SIGNAL(clicked()), this, SLOT(createNewUser()));
    connect(utilisateursBox, SIGNAL(currentTextChanged(QString)), this, SLOT(okButtonState(QString)));
    connect(utilisateursBox, SIGNAL(currentTextChanged(QString)), this, SLOT(userChanged(QString)));
}

void LoginWindow::populateBox(){
    utilisateursBox->addItem(tr("-- Verwender wählen --"));
    utilisateursBox->setItemData(0, QBrush(Qt::gray), Qt::ForegroundRole);
    if(userParams.size() > 0){
        utilisateursBox->addItems(userParams.keys());
    }
}

void LoginWindow::okButtonState(QString text){
    if(text == tr("-- Verwender wählen --"))
        boutonsGroup->button(QDialogButtonBox::Ok)->setDisabled(true);
    else
        boutonsGroup->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void LoginWindow::userChanged(QString username){
    if(username == tr("-- Verwender wählen --")){
        choixBox->setDisabled(true);
    }else{
        choixBox->setEnabled(true);
        shRadio->setEnabled(userParams[username].contains("sh"));
        shRadio->setChecked(userParams[username].contains("sh"));
        enRadio->setEnabled(userParams[username].contains("en"));
        enRadio->setChecked(userParams[username].contains("en"));
        deRadio->setEnabled(userParams[username].contains("de"));
        deRadio->setChecked(userParams[username].contains("de"));
        if(utilisateursBox->findText(tr("-- Verwender wählen --")) > -1){
            int i = utilisateursBox->findText(tr("-- Verwender wählen --"));
            utilisateursBox->removeItem(i);
        }
    }
}

void LoginWindow::createNewUser(){
    LoginWizard* wizard = new LoginWizard(userParams.keys(), this);
    int ret = wizard->exec();
    if(ret == QDialog::Accepted){
        QString username = wizard->field("utilisateur").toString();
        QStringList languages;
        if(wizard->field("de").toBool()) languages << "de";
        if(wizard->field("en").toBool()) languages << "en";
        if(wizard->field("sh").toBool()) languages << "sh";
        userParams[username] = languages;
        utilisateursBox->insertItem(0, username);
    }
}

////////////////////////
///   __           __
///   ** ACCESSORS **
///
////////////////////////

QString LoginWindow::getCurrentUser() const{
    QString ret = utilisateursBox->currentText();
    if(utilisateursBox->currentText() == tr("-- Verwender wählen --"))
        ret = "";
    return ret;
}

QString LoginWindow::getCurrentLanguage() const{
    switch (radioGroup->checkedId()) {
    case 1:
        return "Fr_de";
    case 2:
        return "Fr_en";
    case 3:
        return "Fr_sh";
    default:
        return "Fr_de";
    }
}

void LoginWindow::setCurrentLanguage(const QString &language){
    if(language == "de")
        deRadio->setChecked(true);
    else if(language == "en")
        enRadio->setChecked(true);
    else
        shRadio->setChecked(true);
}

QStringList LoginWindow::getCurrentLanguages() const{
    QString currentUser = utilisateursBox->currentText();
    return userParams[currentUser];
}

QMap<QString,QStringList> LoginWindow::getUserParams() const{
    return userParams;
}
