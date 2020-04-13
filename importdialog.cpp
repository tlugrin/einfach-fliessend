#include <QFileDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QTextStream>
#include <QMessageBox>
#include "importdialog.h"

ImportDialog::ImportDialog(QWidget *parent) :
    QDialog(parent)
{
    // select file
    selectFileLabel = new QLabel;
    fichierEdit = new ChooseFileLineEdit;
    fichierEdit->setReadOnly(true);
    fichierEdit->setText(QDir::homePath());
    importerButton = new QPushButton;
    importerButton->setDisabled(true);
    QVBoxLayout* selectFileLayout = new QVBoxLayout;
    selectFileLayout->addWidget(fichierEdit);
    selectFileLayout->addWidget(importerButton, 0, Qt::AlignRight);
    QFrame* selectFileFrame = new QFrame;
    selectFileFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);
    selectFileFrame->setLineWidth(1);
    selectFileFrame->setMidLineWidth(0);
    selectFileFrame->setLayout(selectFileLayout);
    // see import progress
    importProgressLabel = new QLabel;
    lusLabel = new QLabel;
    lusNbrLabel = new QLabel("0");
    nouveauxLabel = new QLabel;
    nouveauxNbrLabel = new QLabel("0");
    modifiesLabel = new QLabel;
    modifiesNbrLabel = new QLabel("0");
    progressBar = new QProgressBar;
    QGridLayout* importProgressLayout = new QGridLayout;
    importProgressLayout->addWidget(lusLabel, 0, 0);
    importProgressLayout->addWidget(lusNbrLabel, 0, 1);
    importProgressLayout->addWidget(nouveauxLabel, 1, 0);
    importProgressLayout->addWidget(nouveauxNbrLabel, 1, 1);
    importProgressLayout->addWidget(modifiesLabel, 2, 0);
    importProgressLayout->addWidget(modifiesNbrLabel, 2, 1);
    importProgressLayout->addWidget(progressBar, 3, 0, 1, 2);
    QFrame* importProgressFrame = new QFrame;
    importProgressFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    importProgressFrame->setLineWidth(1);
    importProgressFrame->setMidLineWidth(0);
    importProgressFrame->setLayout(importProgressLayout);

    boutonsBox = new QDialogButtonBox;
    boutonsBox->addButton(QDialogButtonBox::Cancel);
    boutonsBox->addButton(QDialogButtonBox::Ok);
    boutonsBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(selectFileLabel);
    layout->addWidget(selectFileFrame);
    layout->addWidget(importProgressLabel);
    layout->addWidget(importProgressFrame);
    layout->addWidget(boutonsBox);
    setLayout(layout);
    setMinimumWidth(400);

    retraduisUI();

    connect(fichierEdit,SIGNAL(clicked()),this,SLOT(lineEditClicked()));
    connect(importerButton,SIGNAL(clicked()),this,SLOT(disableTopFrame()));
    connect(boutonsBox,SIGNAL(rejected()),this,SLOT(reject()));
    connect(boutonsBox,SIGNAL(accepted()),this,SLOT(accept()));
}

void ImportDialog::retraduisUI(){
    selectFileLabel->setText(tr("Datei wählen:"));
    importerButton->setText(tr("Importieren"));
    importProgressLabel->setText(tr("Statistiken:"));
    lusLabel->setText(tr("Gelesene Eintrage:"));
    nouveauxLabel->setText(tr("Neue Eintrage:"));
    modifiesLabel->setText(tr("Beänderte Eintrage:"));
}

/// slots
void ImportDialog::lineEditClicked(){
    QString fichier = fichierEdit->text();
    fichier = QFileDialog::getOpenFileName(this, tr("Wörter importieren"), fichier,
                                            tr("Textdateien (*.csv *.txt);;Alle Dateien (*)"));
    if(fichier != ""){
        fichierEdit->setText(fichier);
        importerButton->setEnabled(true);
    }
}

void ImportDialog::disableTopFrame(){
    selectFileLabel->setDisabled(true);
    fichierEdit->setDisabled(true);
    importerButton->setDisabled(true);
    QString fileName=fichierEdit->text();
    QFile file(fileName);
    QTextStream in(&file);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, tr("Fehler bei der Datei"),
                              tr("Die Datei konnte leider nicht geöffnet werden."));
        return;
    }
    unsigned int nbrLines=0;
    while(in.readLineInto(0))
        nbrLines++;
    progressBar->setRange(0,nbrLines-1);
    emit importerButtonClicked(fileName);
}

void ImportDialog::nbrLusChanged(){
    int value = lusNbrLabel->text().toInt() + 1;
    lusNbrLabel->setText(QString("%1").arg(value));
    progressBar->setValue(value);
    if(value == progressBar->maximum()){
        boutonsBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        boutonsBox->button(QDialogButtonBox::Cancel)->setDisabled(true);
    }
}

void ImportDialog::nbrNouveauxChanged(){
    int value = nouveauxNbrLabel->text().toInt() + 1;
    nouveauxNbrLabel->setText(QString("%1").arg(value));
}

void ImportDialog::nbrModifiesChanged(){
    int value = modifiesNbrLabel->text().toInt() + 1;
    modifiesNbrLabel->setText(QString("%1").arg(value));
}
