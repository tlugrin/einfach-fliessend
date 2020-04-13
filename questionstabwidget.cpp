#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "questionstabwidget.h"

QuestionsTabWidget::QuestionsTabWidget(QWidget *parent) :
    QWidget(parent)
{
    explicationLabel = new QLabel;
    explicationLabel->setWordWrap(true);
    int nbrSliders = 4;
    values.resize(nbrSliders);
    // initialise sliders:
    for(int i=0; i<nbrSliders; i++){
        QSlider* slider = new QSlider(Qt::Horizontal);
        slider->setTickPosition(QSlider::TicksAbove);
        slider->setTickInterval(1);
        slider->setMinimum(-3);
        slider->setMaximum(3);
        slider->setMinimumWidth(280);
        persoSlider.append(slider);
        QLabel* label = new QLabel("0");
        label->setMinimumWidth(20);
        label->setAlignment(Qt::AlignRight);
        persoLabel.append(label);
    }
    // corresonding labels:
    lifeLabel = new QLabel;
    dustLabel = new QLabel;
    freqLabel = new QLabel;
    histLabel = new QLabel;
    // fill user input layout:
    QGridLayout* grilleLayout = new QGridLayout;
    grilleLayout->addWidget(lifeLabel, 1, 0);
    grilleLayout->addWidget(dustLabel, 2, 0);
    grilleLayout->addWidget(freqLabel, 3, 0);
    grilleLayout->addWidget(histLabel, 4, 0);
    grilleLayout->setColumnMinimumWidth(0,120);
    for(int i=0; i<nbrSliders; i++){
        grilleLayout->addWidget(persoSlider.at(i), i+1, 1, 1, 7);
        grilleLayout->addWidget(persoLabel.at(i), i+1, 8);
    }
    grilleLayout->setVerticalSpacing(30);
    // fill main layout:
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(40);
    layout->addWidget(explicationLabel);
    layout->addLayout(grilleLayout);
    this->setLayout(layout);
    this->setFixedSize(sizeHint());
    retraduisUI();

    connect(persoSlider.at(0),SIGNAL(valueChanged(int)),this,SLOT(lifeValueChanged(int)));
    connect(persoSlider.at(1),SIGNAL(valueChanged(int)),this,SLOT(dustValueChanged(int)));
    connect(persoSlider.at(2),SIGNAL(valueChanged(int)),this,SLOT(freqValueChanged(int)));
    connect(persoSlider.at(3),SIGNAL(valueChanged(int)),this,SLOT(histValueChanged(int)));
}

/// interface
void QuestionsTabWidget::retraduisUI(){
    explicationLabel->setText(tr("Mit diesen Schiebern können Sie den Wortwahl Prozess "
                                 "beeinflussen. Zum Beispiel, wenn Sie den 'Alter' "
                                 "Schieber um '2' anbringen, dann wird das Hinzufügungsdatum "
                                 "7 Male mehr bedeutend im Wortwahl Prozess als es wäre, "
                                 "wenn der Schieber um '0' liegen würde."));
    lifeLabel->setText(tr("Alter:"));
    dustLabel->setText(tr("Schlafzeit:"));
    freqLabel->setText(tr("Häufigkeit:"));
    histLabel->setText(tr("Werdegang:"));
}

/// slots on slider changed
void QuestionsTabWidget::lifeValueChanged(int value){
        persoLabel.at(0)->setText(QString("%1").arg(value));
}

void QuestionsTabWidget::dustValueChanged(int value){
        persoLabel.at(1)->setText(QString("%1").arg(value));
}

void QuestionsTabWidget::freqValueChanged(int value){
        persoLabel.at(2)->setText(QString("%1").arg(value));
}

void QuestionsTabWidget::histValueChanged(int value){
        persoLabel.at(3)->setText(QString("%1").arg(value));
}

/// accessors
int QuestionsTabWidget::getLife() const{
    return persoSlider.at(0)->value();
}

int QuestionsTabWidget::getDust() const{
    return persoSlider.at(1)->value();
}

int QuestionsTabWidget::getFreq() const{
    return persoSlider.at(2)->value();
}

int QuestionsTabWidget::getHist() const{
    return persoSlider.at(3)->value();
}

void QuestionsTabWidget::setLife(int value){
    values.replace(0, value);
    persoSlider.at(0)->setValue(value);
}

void QuestionsTabWidget::setDust(int value){
    values.replace(1, value);
    persoSlider.at(1)->setValue(value);
}

void QuestionsTabWidget::setFreq(int value){
    values.replace(2, value);
    persoSlider.at(2)->setValue(value);
}

void QuestionsTabWidget::setHist(int value){
    values.replace(3, value);
    persoSlider.at(3)->setValue(value);
}

void QuestionsTabWidget::resetToOldValues(bool b){
    if(b){
        for(int i=0; i<values.size(); i++)
            persoSlider.at(i)->setValue(values.at(i));
    }else{
        for(int i=0; i<values.size(); i++)
            values.replace(i, persoSlider.at(i)->value());
    }
}
