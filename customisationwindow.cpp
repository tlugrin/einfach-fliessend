#include <QVBoxLayout>
#include <QStylePainter>
#include <QStyleOptionTab>
#include <QPainter>
#include <QFont>
#include "customisationwindow.h"

TabBar::TabBar(QWidget *parent) :
    QTabBar(parent)
{
    setIconSize(QSize(80,80));
}

QSize TabBar::tabSizeHint(int) const{
    return QSize(90,100);
}

// customised tab with icon and text below
void TabBar::paintEvent(QPaintEvent *){
    QStylePainter stylePainter(this);
    for(int i=0; i<count(); i++){
        QStyleOptionTab tab;
        initStyleOption(&tab, i);
        QIcon icon = tab.icon;
        QString label = tab.text;
        tab.icon = QIcon();
        tab.text = QString();
        stylePainter.drawControl(QStyle::CE_TabBarTab, tab);

        QPainter painter;
        QFont police;
        police.setBold(true);
        painter.begin(this);
        painter.setFont(police);
        QRect tabrect = tabRect(i);
        painter.drawText(tabrect, Qt::AlignBottom | Qt::AlignHCenter, label);
        icon.paint(&painter, 0, tabrect.top(), tab.iconSize.width(), tab.iconSize.height(),
                   Qt::AlignTop | Qt::AlignHCenter);
        painter.end();
    }
}

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setTabBar(new TabBar);
}

CustomisationWindow::CustomisationWindow(QWidget *parent) :
    QDialog(parent)
{
    // central layout
    questionsTab = new QuestionsTabWidget;
    fontColoursTab = new FontColorsTabWidget;
    tabWidget = new TabWidget;
    tabWidget->setTabPosition(QTabWidget::West);
    tabWidget->addTab(questionsTab, QIcon(":/pictures/wuerfel.png"), tr("Fragen"));
    tabWidget->addTab(fontColoursTab, QIcon(":/pictures/a.png"), tr("Schriftformat"));
    // bottom layout
    boutonBox = new QDialogButtonBox(this);
    boutonBox->addButton(QDialogButtonBox::Cancel);
    boutonBox->addButton(QDialogButtonBox::Ok);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(tabWidget);
    layout->addWidget(boutonBox);
    this->setLayout(layout);
    retraduisUI();

    connect(boutonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(boutonBox,SIGNAL(rejected()),this,SLOT(reject()));
}

/// interface
void CustomisationWindow::retraduisUI(){
    this->setWindowTitle(tr("Personalisieren"));
    tabWidget->setTabText(0, tr("Fragen"));
    tabWidget->setTabText(1,tr("Schriftformat"));
    questionsTab->retraduisUI();
    fontColoursTab->retraduisUI();
}

/// accessors
int CustomisationWindow::getLife() const{
    return questionsTab->getLife();
}

int CustomisationWindow::getDust() const{
    return questionsTab->getDust();
}

int CustomisationWindow::getFreq() const{
    return questionsTab->getFreq();
}

int CustomisationWindow::getHist() const{
    return questionsTab->getHist();
}

void CustomisationWindow::setLife(int value){
    questionsTab->setLife(value);
}

void CustomisationWindow::setDust(int value){
    questionsTab->setDust(value);
}

void CustomisationWindow::setFreq(int value){
    questionsTab->setFreq(value);
}

void CustomisationWindow::setHist(int value){
    questionsTab->setHist(value);
}

void CustomisationWindow::setCurrentFont(const QString &fontName){
    fontColoursTab->setCurrentFont(fontName);
}

void CustomisationWindow::setFontSize(const int &value){
    fontColoursTab->setFontSize(value);
}

void CustomisationWindow::setBold(const bool &b){
    fontColoursTab->setBold(b);
}

void CustomisationWindow::setItalic(const bool &b){
    fontColoursTab->setItalic(b);
}

void CustomisationWindow::setCurrentLanguages(const QStringList &languages){
    fontColoursTab->setCurrentLanguages(languages);
}

void CustomisationWindow::setLanguageColours(const QStringList &front, const QStringList &back){
    fontColoursTab->setLanguageColours(front, back);
}

void CustomisationWindow::setGenderColours(const QStringList &colourNames){
    fontColoursTab->setGenderColours(colourNames);
}

void CustomisationWindow::setGenders(const bool &b){
    fontColoursTab->setGenders(b);
}

const QString& CustomisationWindow::getCurrentFont() const{
    return fontColoursTab->getCurrentFont();
}

const int& CustomisationWindow::getFontSize() const{
    return fontColoursTab->getFontSize();
}

const bool& CustomisationWindow::getBold() const{
    return fontColoursTab->getBold();
}

const bool& CustomisationWindow::getItalic() const{
    return fontColoursTab->getItalic();
}

const QStringList& CustomisationWindow::getLanguageColoursFront() const{
    return fontColoursTab->getLanguageColoursFront();
}

const QStringList& CustomisationWindow::getLanguageColoursBack() const{
    return fontColoursTab->getLanguageColoursBack();
}

QStringList CustomisationWindow::getLanguageColours(const QString &language) const{
    return fontColoursTab->getLanguageColours(language);
}

const QStringList& CustomisationWindow::getGenderColours() const{
    return fontColoursTab->getGenderColours();
}

const bool& CustomisationWindow::getGenders() const{
    return fontColoursTab->getGenders();
}

void CustomisationWindow::resetToOldValues(bool b){
    questionsTab->resetToOldValues(b);
    fontColoursTab->resetToOldValues(b);
}
