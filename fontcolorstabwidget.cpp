#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QPainter>
#include <QPixmap>
#include <QColorDialog>
#include <QDebug>
#include "fontcolorstabwidget.h"

FontColorsTabWidget::FontColorsTabWidget(QWidget *parent) :
    QWidget(parent)
{
    // font : top
    policeBox = new QGroupBox;
    policeComboBox = new QFontComboBox;
    tailleComboBox = new QComboBox;
    QStringList tailles;
    tailles << "10" << "11" << "12" << "14" << "16" << "18";
    tailleComboBox->addItems(tailles);
    grasCheckBox = new QCheckBox;
    italiqueCheckBox = new QCheckBox;
    QHBoxLayout* policeLayout = new QHBoxLayout;
    policeLayout->addWidget(policeComboBox);
    policeLayout->addWidget(tailleComboBox);
    policeLayout->addWidget(grasCheckBox);
    policeLayout->addWidget(italiqueCheckBox);
    policeBox->setLayout(policeLayout);
    // colors for languages : middle
    languesBox = new QGroupBox;
    createLanguesTable();
    ppLabel = new QLabel;
    ppFrame = new ColourChoicePixmap;
    QGridLayout* languesCouleursLayout = new QGridLayout;
    languesCouleursLayout->addWidget(ppLabel,0,0);
    languesCouleursLayout->addWidget(ppFrame,0,1,Qt::AlignCenter);
    apLabel = new QLabel;
    apFrame = new ColourChoicePixmap;
    languesCouleursLayout->addWidget(apLabel, 1, 0);
    languesCouleursLayout->addWidget(apFrame, 1, 1,Qt::AlignCenter);

    QHBoxLayout* languesLayout = new QHBoxLayout;
    languesLayout->addWidget(languesTable);
    languesLayout->addLayout(languesCouleursLayout);
    languesBox->setLayout(languesLayout);
    // colors for genders : bottom
    genresBox = new QGroupBox;
    genresBox->setCheckable(true);
    masculinLabel = new QLabel;
    masculinFrame = new ColourChoicePixmap;
    femininLabel= new QLabel;
    femininFrame = new ColourChoicePixmap;
    neutreLabel = new QLabel;
    neutreFrame = new ColourChoicePixmap;
    QGridLayout* genresLayout = new QGridLayout;
    genresLayout->addWidget(masculinFrame,0,0);
    genresLayout->addWidget(masculinLabel,1,0);
    genresLayout->addWidget(femininFrame,0,1);
    genresLayout->addWidget(femininLabel,1,1);
    genresLayout->addWidget(neutreFrame,0,2);
    genresLayout->addWidget(neutreLabel,1,2);
    genresBox->setLayout(genresLayout);
    // finalise layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(policeBox);
    layout->addWidget(languesBox);
    layout->addWidget(genresBox);
    this->setLayout(layout);
    retraduisUI();

    connect(languesTable,SIGNAL(currentRowChanged(int)),this,SLOT(currentLanguageChanged(int)));
    connect(ppFrame,SIGNAL(colourChanged(QString)),this,SLOT(foregroundPixTriggered(QString)));
    connect(apFrame,SIGNAL(colourChanged(QString)),this,SLOT(backgroundPixTriggered(QString)));
}

/// interface
void FontColorsTabWidget::retraduisUI(){
    // font
    policeBox->setTitle(tr("Schrift"));
    grasCheckBox->setText(tr("Fett"));
    italiqueCheckBox->setText(tr("Kursiv"));
    // genders
    genresBox->setTitle(tr("Genus"));
    masculinLabel->setText(tr("Masculinum"));
    femininLabel->setText(tr("Femininum"));
    neutreLabel->setText(tr("Neutrum"));
    masculinFrame->setTitle(tr("Vordergrundfarbe für maskulinen Nomen"));
    femininFrame->setTitle(tr("Vordergrundfarbe für weiblichen Nomen"));
    neutreFrame->setTitle(tr("Vordergrundfarbe für sächlich Nomen"));
    // language table
    languesBox->setTitle(tr("Farbschema"));
    ppLabel->setText(tr("Vordergrund"));
    ppFrame->setTitle(tr("Vordergrundfarbe wählen"));
    apLabel->setText(tr("Hintergrund"));
    apFrame->setTitle(tr("Hintergrundfarbe wählen"));
    frItem->setText(tr("Französisch"));
    deItem->setText(tr("Deutsch"));
    enItem->setText(tr("Englisch"));
    shItem->setText(tr("Albanisch"));
}

void FontColorsTabWidget::createLanguesTable(){
    languesTable = new QListWidget;
    languesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    languesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    frItem = new QListWidgetItem;
    deItem = new QListWidgetItem;
    enItem = new QListWidgetItem;
    shItem = new QListWidgetItem;
    languesTable->addItem(frItem);
    languesTable->addItem(deItem);
    languesTable->addItem(enItem);
    languesTable->addItem(shItem);
}

/////////////////////////
///   __           __
///   ** ACCESSORS **
///
/////////////////////////

void FontColorsTabWidget::setCurrentFont(const QString &fontName){
    police = fontName;
    policeComboBox->setCurrentText(fontName);
}

void FontColorsTabWidget::setFontSize(const int &value){
    taille = value;
    tailleComboBox->setCurrentText(QString::number(value));
}

void FontColorsTabWidget::setBold(const bool &b){
    gras = b;
    grasCheckBox->setChecked(b);
}

void FontColorsTabWidget::setItalic(const bool &b){
    italique = b;
    italiqueCheckBox->setChecked(b);
}

void FontColorsTabWidget::setCurrentLanguages(const QStringList &languages){
    frItem->setHidden(false);
    deItem->setHidden(!languages.contains("de"));
    enItem->setHidden(!languages.contains("en"));
    shItem->setHidden(!languages.contains("sh"));
    languesTable->setCurrentRow(0, QItemSelectionModel::Select);
    this->languages = languages;
}

void FontColorsTabWidget::setLanguageColours(const QStringList &front, const QStringList &back){
    ppColours = front;
    apColours = back;
    qDebug() << "ppColours " << ppColours.size();
    qDebug() << "apColours " << apColours.size();
    for(int i=0; i<front.size(); i++){
        languesTable->item(i)->setForeground(QBrush(QColor(ppColours.at(i))));
        languesTable->item(i)->setBackground(QBrush(QColor(apColours.at(i))));
    }
    currentLanguageChanged(0);
}

void FontColorsTabWidget::setGenderColours(const QStringList &colourNames){
    gdrColours = colourNames;
    qDebug() << "colourNmes " << colourNames.size();
    for(int i=0; i<colourNames.size(); i++)
        qDebug() << "colourNames[" << i << "] = " << colourNames[i];
    masculinFrame->setColour(colourNames.at(0));
    femininFrame->setColour(colourNames.at(1));
    neutreFrame->setColour(colourNames.at(2));
}

void FontColorsTabWidget::setGenders(const bool &b){
    genres = b;
    genresBox->setChecked(b);
}

const QString& FontColorsTabWidget::getCurrentFont() const{
    return police;
}

const int& FontColorsTabWidget::getFontSize() const{
    return taille;
}

const bool& FontColorsTabWidget::getBold() const{
    return gras;
}

const bool& FontColorsTabWidget::getItalic() const{
    return italique;
}

const QStringList& FontColorsTabWidget::getLanguageColoursFront() const{
    return ppColours;
}

const QStringList& FontColorsTabWidget::getLanguageColoursBack() const{
    return apColours;
}

QStringList FontColorsTabWidget::getLanguageColours(const QString &language) const{
    qDebug() << "get language colours pp " << ppColours.size();
    qDebug() << "get language colours ap " << apColours.size();
    QStringList ret;
    if(language == "fr"){
        ret << ppColours.at(0);
        ret << apColours.at(0);
    }else{
        int row = languages.indexOf(QRegExp(language.right(2)));
        qDebug() << "row = " << row;
        qDebug() << "language = " << language;
        for(int i=0; i<languages.size(); i++)
            qDebug() << languages[i];
        ret << ppColours.at(row+1);
        ret << apColours.at(row+1);
    }
    qDebug() << "ok getLanguageColours";
    return ret;
}

const QStringList& FontColorsTabWidget::getGenderColours() const{
    return gdrColours;
}

const bool& FontColorsTabWidget::getGenders() const{
    return genres;
}

void FontColorsTabWidget::currentLanguageChanged(int row){
    ppFrame->setColour(languesTable->item(row)->foreground().color().name());
    apFrame->setColour(languesTable->item(row)->background().color().name());
}

void FontColorsTabWidget::foregroundPixTriggered(QString colourName){
    int row = languesTable->currentRow();
    languesTable->item(row)->setForeground(QBrush(QColor(colourName)));
}

void FontColorsTabWidget::backgroundPixTriggered(QString colourName){
    int row = languesTable->currentRow();
    languesTable->item(row)->setBackground(QBrush(QColor(colourName)));
}

void FontColorsTabWidget::resetToOldValues(bool b){
    if(b){
        policeComboBox->setCurrentFont(police);
        tailleComboBox->setCurrentText(QString::number(taille));
        grasCheckBox->setChecked(gras);
        italiqueCheckBox->setChecked(italique);
        for(int i=0; i<ppColours.size(); i++){
            languesTable->item(i)->setForeground(QBrush(QColor(ppColours.at(i))));
            languesTable->item(i)->setBackground(QBrush(QColor(apColours.at(i))));
            currentLanguageChanged(languesTable->currentRow());
        }
        masculinFrame->setColour(gdrColours.at(0));
        femininFrame->setColour(gdrColours.at(1));
        neutreFrame->setColour(gdrColours.at(2));
        genresBox->setChecked(genres);
    }else{
        police   = policeComboBox->currentFont().family();
        taille   = tailleComboBox->currentText().toInt();
        gras     = grasCheckBox->isChecked();
        italique = italiqueCheckBox->isChecked();
        for(int i=0; i<ppColours.size(); i++){
            ppColours.replace(i, languesTable->item(i)->foreground().color().name());
            apColours.replace(i, languesTable->item(i)->background().color().name());
        }
        gdrColours.replace(0, masculinFrame->getColour());
        gdrColours.replace(1, femininFrame->getColour());
        gdrColours.replace(2, neutreFrame->getColour());
        genres = genresBox->isChecked();
    }
}

/////////////////////////////////////////
///   __                           __
///   ** COLOR CHOICE PIXMAP CLASS **
///
/////////////////////////////////////////

ColourChoicePixmap::ColourChoicePixmap(const QString &colourDialogTitle, QWidget *parent)
    : QWidget(parent), rect(1,1,20,20), clicked(false), title(colourDialogTitle)
{
    pen = QPen(QColor(Qt::black));
    pen.setWidth(2);
    brush = QBrush(QColor(Qt::white));
    update();
}

QSize ColourChoicePixmap::minimumSizeHint() const{
    return QSize(22,22);// 20 + 2*half-width of pen
}

QSize ColourChoicePixmap::sizeHint() const{
    return QSize(22,22);
}

void ColourChoicePixmap::mousePressEvent(QMouseEvent *event){
    /*if(event->button() == Qt::LeftButton
            and this->pixmap()->rect().contains(event->pos()))*/
    if(event->button() == Qt::LeftButton
            and rect.contains(event->pos()))
        clicked = true;
}

void ColourChoicePixmap::mouseReleaseEvent(QMouseEvent *event){
    /*if(event->button() == Qt::LeftButton
            and clicked
            and this->pixmap()->rect().contains(event->pos())){*/
    if(event->button() == Qt::LeftButton
            and rect.contains(event->pos())
            and clicked){
        QColor col = QColorDialog::getColor(brush.color(), this, title);
        if(col.isValid()){
            brush.setColor(col);
            update();
            emit colourChanged(col.name());
        }
    }
    clicked = false;
}

void ColourChoicePixmap::paintEvent(QPaintEvent *event){
    /*QWidget::paintEvent(event);
    QPainter p(&carreCouleurPixmap);*/
    QPainter p(this);
    p.setBrush(brush);
    p.setPen(pen);
    /*p.drawRect(carreCouleurPixmap.rect());
    this->setPixmap(carreCouleurPixmap);*/
    p.drawRect(rect);
    event->accept();
}

void ColourChoicePixmap::setTitle(const QString &colourDialogTitle){
    title = colourDialogTitle;
}

void ColourChoicePixmap::setColour(const QString &colour){
    brush.setColor(QColor(colour));
    update();
}

QString ColourChoicePixmap::getColour() const{
    return brush.color().name();
}
