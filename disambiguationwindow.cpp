#include "disambiguationwindow.h"
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>
#include <QFont>
#include <QDebug>

DisambiguationWindow::DisambiguationWindow(QSqlQuery* query, const QString& language,
                                           const QStringList& currentEntry, QWidget *parent) :
    QDialog(parent), language(language)
{
    this->setWindowTitle(tr("Gleiche Wörter in der Datenbank"));
    doublonQuery = query;
    initialiseTable(currentEntry);
    QLabel* introLabel = new QLabel(tr("Wählen Sie eine oder mehrere Einträge, die mit Ihrem "
                                    "vorgeschlagene Wort zusammengestellt werden.\n"
                                    "Wenn Sie keine den folgenden Einträge wählen, dann wird "
                                    "Ihres Wort als neue Eintrag betrachtet.\n"));
    introLabel->setWordWrap(true);
    resetBox = new QCheckBox(tr("Trainingsdaten zurückstellen?"));
    resetBox->setToolTip(tr("Wählen Sie diese Option, um das Wort im nächsten Training\n"
                           "als neues Wort zu betrachten."));
    // bottom layout with buttons
    boutonBox = new QDialogButtonBox(this);
    boutonBox->addButton(QDialogButtonBox::Ok);
    boutonBox->addButton(QDialogButtonBox::Cancel);
    // fill main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(introLabel);
    mainLayout->addWidget(requetesTable);
    mainLayout->addWidget(resetBox);
    mainLayout->addWidget(boutonBox);
    setLayout(mainLayout);
    adjustSize();

    connect(boutonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(boutonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

/// initialise table using SQL query result
void DisambiguationWindow::initialiseTable(const QStringList& currentEntry){
    QStringList horizHeader;
    QString aWords,aExamples;
    if(language=="Fr_de"){
        aWords    = tr("deutsche Wörter");
        aExamples = tr("deutsche Beispiele");
    }else if(language =="Fr_en"){
        aWords    = tr("englische Wörter");
        aExamples = tr("englische Beispiele");
    }else{
        aWords    = tr("albanische Wörter");
        aExamples = tr("albanische Beispiele");
    }
    horizHeader << tr("französische Wörter") << aWords
                << tr("französische Beispiele") << aExamples
                << tr("Hinzufügungsdatum") << tr("Änderungsdatum");
    requetesTable = new QTableWidget(doublonQuery->size()+1, 6, this);
    requetesTable->setHorizontalHeaderLabels(horizHeader);
    requetesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    requetesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    requetesTable->setSelectionMode(QAbstractItemView::MultiSelection);// toggle rows (Ctrl/Maj not needed)
    // first row: current entry to be added
    QFont font;
    font.setItalic(true);
    for(unsigned int i=0; i<4; i++){
        QTableWidgetItem* item = new QTableWidgetItem(currentEntry.at(i));
        item->setFont(font);
        item->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
        requetesTable->setItem(0, i, item);
    }
    // next rows: entries similar to current entry
    while(doublonQuery->next()){
        // columns 1-4
        QStringList qList,aList,qExList,aExList;
        for(unsigned int i=1; i<=3; i++){
            qList << doublonQuery->value(i).toString();
            aList << doublonQuery->value(i+3).toString();
            qExList << doublonQuery->value(i+6).toString().left(20);// truncate examples
            aExList << doublonQuery->value(i+9).toString().left(20);
        }
        QString qJoin = qList.join(";").split(";", QString::SkipEmptyParts).join(" ; ");
        QString aJoin = aList.join(";").split(";", QString::SkipEmptyParts).join(" ; ");
        QString qExJoin = qExList.join(";").split(";", QString::SkipEmptyParts).join("... ; ");
        QString aExJoin = aExList.join(";").split(";", QString::SkipEmptyParts).join("... ; ");
        if(qExJoin != ""){
            qExJoin += "..."; aExJoin += "...";
        }
        QTableWidgetItem* qItem = new QTableWidgetItem(qJoin);
        QTableWidgetItem* aItem = new QTableWidgetItem(aJoin);
        QTableWidgetItem* qExItem = new QTableWidgetItem(qExJoin);
        QTableWidgetItem* aExItem = new QTableWidgetItem(aExJoin);
        // columns 5-6
        QTableWidgetItem* creationItem = new QTableWidgetItem(doublonQuery->value(13)
                                                              .toDate().toString("dd MMMM yyyy"));
        QTableWidgetItem* modifItem    = new QTableWidgetItem(doublonQuery->value(14)
                                                              .toDate().toString("dd MMMM yyyy"));
        const int recPos = doublonQuery->at()+1;// retrieve record position
        qDebug() << "recPos = " << recPos;
        qDebug() << "aJoin = " << aJoin << "(" << creationItem->text() << ")";
        requetesTable->setItem(recPos, 0, qItem);
        requetesTable->setItem(recPos, 1, aItem);
        requetesTable->setItem(recPos, 2, qExItem);
        requetesTable->setItem(recPos, 3, aExItem);
        requetesTable->setItem(recPos, 4, creationItem);
        requetesTable->setItem(recPos, 5, modifItem);
    }
    requetesTable->resizeColumnsToContents();
    qDebug() << requetesTable->rowCount();
}

/// access the selected rows
QVector<int> DisambiguationWindow::getSelection() const{
    QVector<int> selection;
    selection.clear();
    for(int i=1; i<requetesTable->rowCount(); i++){
        if(requetesTable->item(i, 1)->isSelected()){
            selection.append(i-1);
        }
    }
    return selection;
}

/// access checkbox state: reset counters?
bool DisambiguationWindow::getIsChecked() const{
    return resetBox->isChecked();
}
