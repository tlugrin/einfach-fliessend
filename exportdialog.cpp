#include "exportdialog.h"
#include<QFrame>
#include<QVBoxLayout>
#include<QFileDialog>
#include<QDebug>

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent)
{
    // choose file to which to export
    fichierLabel = new QLabel;
    fichierEdit = new ChooseFileLineEdit;
    fichierEdit->setReadOnly(true);
    fichierEdit->setText(QDir::homePath() + "/fluessigexport.csv");
    // standard separator between fields
    QFrame* separator1 = new QFrame;
    separator1->setFrameStyle(QFrame::Sunken | QFrame::HLine);
    separator1->setLineWidth(1);
    separator1->setMidLineWidth(1);
    // choose table
    languesLabel = new QLabel;
    deButton = new QRadioButton;
    enButton = new QRadioButton;
    shButton = new QRadioButton;
    radioGroup = new QButtonGroup(this);
    radioGroup->setExclusive(true);
    radioGroup->addButton(deButton, 0);
    radioGroup->addButton(enButton, 1);
    radioGroup->addButton(shButton, 2);
    // standard separator between fields
    QFrame* separator2 = new QFrame;
    separator2->setFrameStyle(QFrame::Sunken | QFrame::HLine);
    separator2->setLineWidth(1);
    separator2->setMidLineWidth(1);
    // choose date from which to export
    fromLabel = new QLabel;
    fromDate = new QDateEdit;
    fromDate->setDisplayFormat("d MMMM yyyy");
    fromDate->setMaximumDate(QDate::currentDate());
    fromDate->setCalendarPopup(true);
    minDates.fill(QDate::currentDate(), 3);
    // bottom buttons
    boutonBox = new QDialogButtonBox(this);
    exporterButton = new QPushButton;
    boutonBox->addButton(exporterButton, QDialogButtonBox::AcceptRole);
    boutonBox->addButton(QDialogButtonBox::Cancel);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(fichierLabel);
    layout->addWidget(fichierEdit);
    layout->addWidget(separator1);
    layout->addWidget(languesLabel);
    layout->addWidget(deButton);
    layout->addWidget(enButton);
    layout->addWidget(shButton);
    layout->addWidget(separator2);
    layout->addWidget(fromLabel);
    layout->addWidget(fromDate);
    layout->addWidget(boutonBox);
    setLayout(layout);

    retraduisUI();
    setMinimumWidth(450);

    connect(fichierEdit,SIGNAL(clicked()),this,SLOT(lineEditClicked()));
    connect(radioGroup, SIGNAL(buttonClicked(int)),this,SLOT(selectedLanguageChanged(int)));
    connect(boutonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(boutonBox,SIGNAL(rejected()),this,SLOT(reject()));
}

void ExportDialog::retraduisUI(){
    fichierLabel->setText(tr("Wählen Sie den Dateiname:"));
    languesLabel->setText(tr("Wählen Sie die Sprache:"));
    deButton->setText(tr("Französisch-Deutsch"));
    enButton->setText(tr("Französisch-Englisch"));
    shButton->setText(tr("Französisch-Albanisch"));
    fromLabel->setText(tr("Wörter hinzugefügt seit:"));
    exporterButton->setText(tr("Exportieren"));
}

/// accessors
void ExportDialog::setAvailableLanguages(const QStringList &languages){
    bool b;
    b = languages.contains("sh");
    shButton->setEnabled(b);
    shButton->setChecked(b);
    b = languages.contains("en");
    enButton->setEnabled(b);
    enButton->setChecked(b);
    b = languages.contains("de");
    deButton->setEnabled(b);
    deButton->setChecked(b);

    selectedLanguageChanged(radioGroup->checkedId());
}

void ExportDialog::setMinDates(const QVector<QDate> &dates){
    minDates = dates;

    selectedLanguageChanged(radioGroup->checkedId());
}

QString ExportDialog::getFileName() const{
    return fichierEdit->text();
}

QDate ExportDialog::getFromDate() const{
    return fromDate->date();
}

QString ExportDialog::getTableName() const{
    if(deButton->isChecked())      return "Fr_de";
    else if(enButton->isChecked()) return "Fr_en";
    else                           return "Fr_sh";
}

/// slots
void ExportDialog::selectedLanguageChanged(int id){
    fromDate->setMinimumDate(minDates.at(id));
    fromDate->setDate(minDates.at(id));
}

void ExportDialog::lineEditClicked(){
    QString fichier = fichierEdit->text();
    fichier = QFileDialog::getSaveFileName(this, tr("Datenbank exportieren"), fichier,
                                            tr("Textdateien (*.csv *.txt);;Alle Dateien (*)"));
    if(fichier != "") fichierEdit->setText(fichier);
}

////////////////////////////////////////////////////////
///   __                                    __
///   ** CHOOSE FILE WHEN LINE EDIT CLICKED **
///
///////////////////////////////////////////////////////
ChooseFileLineEdit::ChooseFileLineEdit(QWidget *parent) :
    QLineEdit(parent), isClicked(false)
{}

void ChooseFileLineEdit::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        isClicked = true;
        qDebug() << "isClicked = " << isClicked;
    }
}

void ChooseFileLineEdit::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton
            and isClicked
            and rect().contains(event->pos())){
        isClicked = false;
        emit clicked();
    }
}
