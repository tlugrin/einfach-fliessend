#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QDialogButtonBox>
#include <QDialog>
#include <QComboBox>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QStringList>
#include <QMap>
#include <QCloseEvent>

class LoginWindow : public QDialog
{
    Q_OBJECT

private:
    // user interface
    QComboBox* utilisateursBox;
    QGroupBox* choixBox;
    QRadioButton* deRadio;
    QRadioButton* enRadio;
    QRadioButton* shRadio;
    QButtonGroup* radioGroup;
    QPushButton* nouveauButton;
    QDialogButtonBox* boutonsGroup;
    // user: name and languages
    QMap<QString,QStringList> userParams;
    void populateBox();

public:
    explicit LoginWindow(QMap<QString,QStringList> userParams, QWidget *parent = 0);
    // accessors
    QString getCurrentUser() const;
    QString getCurrentLanguage() const;
    void setCurrentLanguage(const QString &language);
    QStringList getCurrentLanguages() const;
    QMap<QString,QStringList> getUserParams() const;

public slots:
    void createNewUser();// wizard pops up
    void okButtonState(QString text);// if combo box is on "choose user"
    void userChanged(QString username);
};

#endif // LOGINWINDOW_H
