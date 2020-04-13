/*
 * Wizard's structure & possible paths
 *              .> UsernamePage (new user)            -> LanguagesRegisterPage -> ConfirmationPage
 *             /
 * IntroPage -|--> UserChoicePage (existing user)     -> LanguagesChoicePage
 *             \
 *              '> UserChoicePage (modif exist. user) -> LanguagesRegisterPage -> ConfirmationPage -> LanguagesChoicePage
 * */

#ifndef LOGINWIZARD_H
#define LOGINWIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QStringList>
#include <QComboBox>
#include <QPixmap>
#include <QMap>
#include "namespaces.h"

class IntroPage;
class UsernamePage;
class UserChoicePage;
class LanguagesRegisterPage;
class LanguagesChoicePage;
class ConfirmationPage;

class LoginWizard : public QWizard
{
    Q_OBJECT

private:
    // user: name and languages
    IntroPage* introPage;
    UsernamePage* usernamePage;
    UserChoicePage* userChoicePage;
    LanguagesRegisterPage* languagesRegisterPage;
    LanguagesChoicePage* languagesChoicePage;
    ConfirmationPage* confirmationPage;
    QMap<QString,QStringList> userParams;
    void registerNewUser();
    void updateExistingUser();

public:
    enum{Page_Intro, Page_Username, Page_UserChoice, Page_LanguagesRegister, Page_LanguagesChoice, Page_Confirmation};

    explicit LoginWizard(const QMap<QString,QStringList> &userParams, QWidget *parent = 0);
    // accessors
    QString getCurrentUser() const;
    QString getCurrentLanguage() const;
    void setCurrentLanguage(const QString &language);
    QStringList getCurrentLanguages() const;
    QMap<QString,QStringList> getUserParams() const;
    void setCurrentAppLanguage(const Langue::LangueApp &language);
    void retraduisUI();

public slots:
    void managePageButtons(int id);
    void setAvailableLanguages();

signals:
    void appLanguageChanged(Langue::LangueApp);
};

class IntroPage : public QWizardPage
{
    Q_OBJECT

private:
    QLabel* descriptionLabel;
    QGroupBox* langueApplicationGroupBox;
    QComboBox* langueApplicationCombo;
    QGroupBox* statutUtilisateurGroupBox;
    QRadioButton* nouveauButton;
    QRadioButton* existantButton;
    QRadioButton* modifierButton;

public:
    explicit IntroPage(QWidget *parent = 0);
    int nextId() const;
    void retraduisUI();
    // accessor
    void setCurrentAppLanguage(const Langue::LangueApp &language);

public slots:
    void appLanguageChanged(int index);

signals:
    void appLanguageChanged(Langue::LangueApp);
};

class UsernamePage : public QWizardPage
{
    Q_OBJECT

private:
    QLabel* descriptionLabel;
    QStringList users;
    QLineEdit* utilisateurEdit;
    QRegExpValidator* validateur;
    QLabel* userExistsLabel;
    bool userExists() const;

public:
    explicit UsernamePage(const QStringList &users, QWidget *parent = 0);
    bool isComplete() const;
    int nextId() const;
    void retraduisUI();

public slots:
    void showUserExists();
};

class UserChoicePage : public QWizardPage
{
    Q_OBJECT

private:
    QLabel* descriptionLabel;
    QComboBox* utilisateursBox;

public:
    explicit UserChoicePage(QStringList const &usernames, QWidget *parent = 0);
    bool isComplete() const;
    int nextId() const;
    void retraduisUI();
    // accessor
    void setNewUser(const QString &user);
};

class LanguagesRegisterPage : public QWizardPage
{
    Q_OBJECT

private:
    QLabel* descriptionLabel;
    QCheckBox* deBox;
    QCheckBox* enBox;
    QCheckBox* shBox;
    QMap<QString,QStringList> userParams;

protected:
    void initializePage();

public:
    explicit LanguagesRegisterPage(QMap<QString,QStringList> userParams, QWidget *parent = 0);
    bool isComplete() const;
    int nextId() const;
    void retraduisUI();
    // accessor
    void setNewUser(QString const &user, QStringList const &languages);
};

class LanguagesChoicePage : public QWizardPage
{
    Q_OBJECT

private:
    QLabel* descriptionLabel;
    QRadioButton* deButton;
    QRadioButton* enButton;
    QRadioButton* shButton;

protected:
    void initializePage();

public:
    explicit LanguagesChoicePage(QWidget *parent = 0);
    int nextId() const;
    void retraduisUI();
};

class ConfirmationPage : public QWizardPage
{
    Q_OBJECT

private:
    QLabel* descriptionLabel;
    QLabel* userLabel;
    QLabel* utilisateurLabel;
    QLabel* langLabel;
    QLabel* deLabel;
    QLabel* enLabel;
    QLabel* shLabel;
    QLabel* dePixLabel;
    QLabel* enPixLabel;
    QLabel* shPixLabel;
    QLabel* finLabel;
    QPixmap oui;
    QPixmap non;

protected:
    void initializePage();

public:
    explicit ConfirmationPage(QWidget *parent = 0);
    int nextId() const;
    void retraduisUI();

public slots:
};

#endif // LOGINWIZARD_H
