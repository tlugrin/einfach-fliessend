#include <QHBoxLayout>
#include <QVBoxLayout>
#include "drilldialog.h"

DrillDialog::DrillDialog(QWidget *parent) :
    QDialog(parent)
{
    QHBoxLayout* boutonsLayout = new QHBoxLayout;
    drillBox = new QCheckBox;
    drillBox->setChecked(false);
    drillSizeSpin = new QSpinBox;
    drillSizeSpin->setRange(5,30);
    drillSizeSpin->setSingleStep(5);
    drillSizeSpin->setDisabled(true);
    boutonsLayout->addWidget(drillBox);
    boutonsLayout->addWidget(drillSizeSpin);

    boutonBox = new QDialogButtonBox(this);
    boutonBox->addButton(QDialogButtonBox::Ok);
    boutonBox->addButton(QDialogButtonBox::Cancel);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(boutonsLayout);
    mainLayout->addWidget(boutonBox);
    this->setLayout(mainLayout);
    this->adjustSize();

    retraduisUI();

    connect(drillBox, SIGNAL(clicked(bool)), this, SLOT(drillBoxClicked(bool)));
    connect(boutonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(boutonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void DrillDialog::drillBoxClicked(bool checked){
    drillSizeSpin->setEnabled(checked);
}

void DrillDialog::resetToOldValues(bool b){
    if(b){
        drillBox->setChecked(isChecked);
        drillSizeSpin->setEnabled(isChecked);
        drillSizeSpin->setValue(value);
    }else{
        isChecked = drillBox->isChecked();
        value = drillSizeSpin->value();
    }
}

/// interface
void DrillDialog::retraduisUI(){
    setWindowTitle(tr("Intensives Fragemodus wählen"));
    drillBox->setText(tr("Intensives Modus"));
}

/// accessors
void DrillDialog::setDrill(bool b){
    isChecked = b;
    drillBox->setChecked(b);
    drillSizeSpin->setEnabled(b);
}

void DrillDialog::setValue(int value){
    this->value = value;
    drillSizeSpin->setValue(value);
}

bool DrillDialog::getDrill() const{
    return isChecked;
}

int DrillDialog::getValue() const{
    return value;
}
