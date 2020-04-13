#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QDialogButtonBox>
#include "exportdialog.h"

class ImportDialog : public QDialog
{
    Q_OBJECT

private:
    QLabel* selectFileLabel;
    ChooseFileLineEdit* fichierEdit;
    QPushButton* importerButton;
    QLabel* importProgressLabel;
    QLabel* lusLabel;
    QLabel* lusNbrLabel;
    QLabel* nouveauxLabel;
    QLabel* nouveauxNbrLabel;
    QLabel* modifiesLabel;
    QLabel* modifiesNbrLabel;
    QProgressBar* progressBar;
    QDialogButtonBox* boutonsBox;

public:
    explicit ImportDialog(QWidget *parent = 0);
    void retraduisUI();

public slots:
    void lineEditClicked();
    void disableTopFrame();
    void nbrLusChanged();
    void nbrNouveauxChanged();
    void nbrModifiesChanged();

signals:
    void importerButtonClicked(QString filename);
};

#endif // IMPORTDIALOG_H
