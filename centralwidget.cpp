/*************************************
 * TODO: enum type for languages
 * ***********************************/


#include <QGridLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QDate>
#include <QFileDialog>
#include <QProgressDialog>
#include <QApplication>
#include <QtDebug>
#include <QtSql/QSqlError>
#include <QtCore/qmath.h>
#include <stdlib.h>
#include "centralwidget.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent), nbrExemples(3), life(0), dust(0), freq(0), hist(0), isSwapping(false), isModifying(false)
{
    policeEdit = QFont();
    QFont policeGram = QFont("Century Schoolbook", 12, QFont::Normal, true);

    // user
    user = QString("");
    language = QString("");

    // Question/answer fields...
    motFrValidator = new QRegExpValidator(QRegExp("^[a-zéàâêôî\\(\\)]+[a-z\\(\\);,'éàâèêâîôïç¨ ]*"),this);
    motDeValidator = new QRegExpValidator(QRegExp("^[a-züäö\\(\\)]+[A-Za-züäö¨\\(\\)\"+,; ~-]*"),this);
    motEnValidator = new QRegExpValidator(QRegExp("^[a-z\\(\\)]+[a-z\\(\\)+,; ]*"),this);
    motShValidator = new QRegExpValidator(QRegExp("^[a-zçë¨\\(\\)]+[a-zëç\\(\\)\"+,;' ~-]*"),this);
    questionEdit = new QLineEdit;
    questionLabel = new QLabel;
    reponseEdit = new QLineEdit;
    reponseLabel = new QLabel;

    // ...buttons...
    reponseButton = new QPushButton;
    reponseButton->setMinimumWidth(100);
    fauxButton = new QPushButton;
    fauxButton->setCheckable(true);
    fauxButton->setChecked(false);
    reponseExemplesButton = new QPushButton;
    precedantButton = new QPushButton("<");
    suiteButton = new QPushButton;
    suivantButton = new QPushButton(">");
    navigation1Layout = new QStackedLayout;
    navigation1Layout->addWidget(suiteButton);
    navigation1Layout->addWidget(suivantButton);
    navigation2Layout = new QStackedLayout;
    navigation2Layout->addWidget(reponseExemplesButton);
    navigation2Layout->addWidget(precedantButton);

    // ...grammatical type...
    catGramLabel = new QLabel;
    catGramLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    catGramLabel->setFont(policeGram);
    catGramBox = new QComboBox;
    catGramBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    catGramLayout = new QStackedLayout;
    catGramLayout->addWidget(catGramLabel);
    catGramLayout->addWidget(catGramBox);

    // ...with additional info in case of noun...
    genreQLabel = new QLabel;
    genreALabel = new QLabel;
    genreQLabel->setFont(policeGram);
    genreALabel->setFont(policeGram);
    genreQBox = new QComboBox;
    genreABox = new QComboBox;
    genreQLayout = new QStackedLayout;
    genreALayout = new QStackedLayout;
    genreQLayout->addWidget(genreQLabel);
    genreALayout->addWidget(genreALabel);
    genreQLayout->addWidget(genreQBox);
    genreALayout->addWidget(genreABox);

    // ...or verb...
    verbeLabel = new QLabel;
    verbeLabel->setFont(policeGram);
    verbeBox = new QComboBox;
    verbeLayout = new QStackedLayout;
    verbeLayout->addWidget(verbeLabel);
    verbeLayout->addWidget(verbeBox);

    // ...examples...
    exempleFrValidator = new QRegExpValidator(QRegExp("^[A-Za-zéàèêîôâÉÀÊÎÔÂ]+[A-Za-zéàèêîôâÉÀÊÎ0-9+,.'!?% ~-]*"),this);
    exempleDeValidator = new QRegExpValidator(QRegExp("^[A-Za-züöäÜÖÄ]+[A-Za-züöäÜÖÄ0-9+,.'!?% ~-]*"),this);
    exempleEnValidator = new QRegExpValidator(QRegExp("^[A-Za-z]+[A-Za-z0-9+,.'!?% -]*"),this);
    exempleShValidator = new QRegExpValidator(QRegExp("^[A-Za-zëçËÇ]+[A-Za-zëçËÇ0-9+,.'!?% -]*"),this);
    exempleQEdits.clear();
    exempleAEdits.clear();
    exempleLabels.clear();
    for(unsigned int i=0; i<nbrExemples; i++){
        exempleQEdits.append(new QLineEdit);
        exempleAEdits.append(new QLineEdit);
        exempleLabels.append(new QLabel);
    }

    //...general layout
    QGridLayout* generalLayout = new QGridLayout();
    generalLayout->addWidget(questionLabel, 0, 0);
    generalLayout->addWidget(questionEdit, 0, 1, 1, 3);
    generalLayout->addLayout(genreQLayout, 0, 4);
    generalLayout->addWidget(reponseButton, 0, 6);

    generalLayout->addWidget(reponseLabel, 1, 0);
    generalLayout->addWidget(reponseEdit, 1, 1, 1, 3);
    generalLayout->addLayout(genreALayout, 1, 4);
    generalLayout->addWidget(fauxButton, 1, 6);

    generalLayout->addLayout(catGramLayout, 2, 3);
    generalLayout->addLayout(verbeLayout, 2, 4);
    generalLayout->addLayout(navigation1Layout, 2, 6);

    generalLayout->addLayout(navigation2Layout, 4, 6);
    for(unsigned int i=0; i<nbrExemples; i++){
        generalLayout->addWidget(exempleLabels[i], 4+2*i, 0);
        generalLayout->addWidget(exempleQEdits[i], 4+2*i, 1, 1, 3);
        generalLayout->addWidget(exempleAEdits[i], 5+2*i, 1, 1, 3);
    }

    generalLayout->setColumnMinimumWidth(1, 500);
    generalLayout->setColumnMinimumWidth(5, 20);
    generalLayout->setRowStretch(3, 1);
    generalLayout->setVerticalSpacing(20);
    this->setLayout(generalLayout);
    // ...order widgets on tab pressed...
    this->setTabOrder(questionEdit, reponseEdit);
    this->setTabOrder(reponseEdit, catGramBox);
    this->setTabOrder(catGramBox, exempleQEdits[0]);
    this->setTabOrder(exempleQEdits[0], exempleAEdits[0]);
    this->setTabOrder(exempleAEdits[0], exempleQEdits[1]);
    this->setTabOrder(exempleQEdits[1], exempleAEdits[1]);
    this->setTabOrder(exempleAEdits[1], exempleQEdits[2]);
    this->setTabOrder(exempleQEdits[2], exempleAEdits[2]);
    this->setTabOrder(exempleAEdits[2], reponseButton);
    // ...refresh GUI...
    retraduisUI();
    // ...open database
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("vocabulary");
    db.setUserName("fluessigeswort");
    db.setPassword("voc");
    db.setConnectOptions();
    if(!db.open())
        qDebug() << db.lastError().text();
    QSqlQuery query;
    query.exec("SET NAMES 'utf8'");

    connect(suiteButton,SIGNAL(clicked()),this,SLOT(suiteClicked()));
    connect(reponseButton,SIGNAL(clicked()),this,SLOT(dispatchClickReponse()));
    connect(reponseExemplesButton,SIGNAL(clicked()),this,SLOT(giveExamplesAnswer()));
    connect(catGramBox,SIGNAL(activated(int)),this,SLOT(setAdditionalInfo(int)));
    connect(reponseEdit,SIGNAL(textEdited(QString)),this,SLOT(guessCatGram(QString)));
    connect(reponseEdit,SIGNAL(editingFinished()),this,SLOT(guessCatGram()));
    connect(suivantButton,SIGNAL(clicked()),this,SLOT(nextEntry()));
    connect(precedantButton,SIGNAL(clicked()),this,SLOT(previousEntry()));
}

/// change window widget according to mode (editing vs voc training)
void CentralWidget::toggleEditingMode(bool editing){
    questionEdit->setReadOnly(!editing);
    reponseEdit->setReadOnly(!editing);
    for(unsigned int i=0; i<nbrExemples; i++){
        exempleQEdits[i]->setReadOnly(!editing);
        exempleAEdits[i]->setReadOnly(!editing);
    }
    isEditing = editing;
    emptyFields();
    // change mode visually
    if(editing){
        questionEdit->setValidator(motFrValidator);
        for(unsigned int i=0; i<nbrExemples; i++)
            exempleQEdits[i]->setValidator(exempleFrValidator);
        if(language.right(2) == "de"){
            reponseEdit->setValidator(motDeValidator);
            for(unsigned int i=0; i<nbrExemples; i++)
                exempleAEdits[i]->setValidator(exempleDeValidator);
        }else if(language.right(2) == "en"){
            reponseEdit->setValidator(motEnValidator);
            for(unsigned int i=0; i<nbrExemples; i++)
                exempleAEdits[i]->setValidator(exempleEnValidator);
        }else{
            reponseEdit->setValidator(motShValidator);
            for(unsigned int i=0; i<nbrExemples; i++)
                exempleAEdits[i]->setValidator(exempleShValidator);
        }
        reponseButton->setText(tr("Hinzufügen"));
        reponseButton->setEnabled(true);
        fauxButton->hide();
        navigation1Layout->setCurrentWidget(suivantButton);
        navigation2Layout->setCurrentWidget(precedantButton);
        catGramLayout->setCurrentWidget(catGramBox);
        catGramLayout->currentWidget()->show();
        catGramBox->setCurrentIndex(0);
        verbeLayout->setCurrentWidget(verbeBox);
        verbeLayout->currentWidget()->hide();
        genreQLayout->setCurrentWidget(genreQBox);
        genreQLayout->currentWidget()->show();
        genreQBox->setCurrentIndex(0);
        genreALayout->setCurrentWidget(genreABox);
        genreALayout->currentWidget()->show();
        genreABox->setCurrentIndex(0);
        coloursChanged();
    }else{
        questionEdit->setValidator(0);
        reponseEdit->setValidator(0);
        for(unsigned int i=0; i<nbrExemples; i++){
            exempleQEdits[i]->setValidator(0);
            exempleAEdits[i]->setValidator(0);
        }
        reponseButton->setText(tr("Antwort"));
        reponseButton->setDisabled(true);
        fauxButton->show();
        fauxButton->setChecked(false);
        navigation1Layout->setCurrentWidget(suiteButton);
        navigation2Layout->setCurrentWidget(reponseExemplesButton);
        reponseExemplesButton->setDisabled(true);
        catGramLayout->setCurrentWidget(catGramLabel);
        catGramLayout->currentWidget()->hide();
        verbeLayout->setCurrentWidget(verbeLabel);
        verbeLayout->currentWidget()->hide();
        genreQLayout->setCurrentWidget(genreQLabel);
        genreQLayout->currentWidget()->hide();
        genreALayout->setCurrentWidget(genreALabel);
        genreALayout->currentWidget()->hide();
        updateCurrentUserTableEntries();
    }
}

////////////////////////////////////////////////////////
///   __                                             __
///   ** EXPORT/IMPORT/LOOK FROM/(IN)TO THE DATABASE **
///
///////////////////////////////////////////////////////
/// export/import table to csv
void CentralWidget::exportTable(const QDate& from, const QString& tableName, const QString& fileName){
    QSqlQuery countQuery,fieldsQuery,exportQuery;
    unsigned int lastID=0,total;
    countQuery.prepare(QString("SELECT COUNT(*) FROM %1 WHERE created >= :from").arg(tableName));
    countQuery.bindValue(":from", from.toString("yyyy-MM-dd"));
    qDebug() << "exportTable(), countQuery:    " << countQuery.executedQuery();
    countQuery.exec();
    countQuery.first();
    total = countQuery.value(0).toUInt();
    if(total == 0){
        QMessageBox::critical(this, tr("Leere Datenbank"), tr("Kein Datum kann exportiert werden!"));
    }else{
        QFile file(fileName);
        QTextStream out(&file);
        if(!file.open(QIODevice::WriteOnly)){
            QMessageBox::critical(this, tr("Fehler bei der Datei"),
                                  tr("Die Datei konnte leider nicht geöffnet werden."));
            return;
        }
        fieldsQuery.exec(QString("SELECT COLUMN_NAME "
                                 "FROM information_schema.COLUMNS WHERE TABLE_NAME='%1'")
                         .arg(tableName));
        qDebug() << "exportTable(), fieldsQuery:   " << fieldsQuery.executedQuery();
        fieldsQuery.first();
        const unsigned int nbrFields=fieldsQuery.size();
        for(unsigned int v=0; v<nbrFields; v++){
            out << fieldsQuery.value(0).toString();
            if(v < nbrFields-1) out << " ; ";
            fieldsQuery.next();
        }
        QProgressDialog progress(tr("Datenbank wird exportiert..."),
                                 tr("Export abbrechen"), 0, total, this);
        progress.setWindowModality(Qt::ApplicationModal);
        for(unsigned int i=0; i<total; i++){
            progress.setValue(i);
            if(progress.wasCanceled()) break;
            exportQuery.prepare(QString("SELECT %1.id,word_from_1,word_from_2,word_from_3,"
                                         "word_to_1,word_to_2,word_to_3,"
                                         "Word_type.cat,irreg_verb,GdrQ.cat,GdrA.cat,"
                                         "ex_from_1,ex_from_2,ex_from_3,ex_to_1,ex_to_2,ex_to_3,"
                                         "created,modified "
                                     "FROM %1 "
                                     "INNER JOIN Cat_gram AS Word_type ON word_type = Word_type.id "
                                     "LEFT JOIN Gdr AS GdrQ ON gender_from = GdrQ.id "
                                     "LEFT JOIN Gdr AS GdrA ON gender_to = GdrA.id "
                                     "WHERE created >= :from AND %1.id >= :lastID "
                                     "ORDER BY %1.id ASC LIMIT 1")
                             .arg(tableName));
            exportQuery.bindValue(":from", from.toString("yyyy-MM-dd"));
            exportQuery.bindValue(":lastID",lastID);
            qDebug() << "exportTable(), exportQuery:   " << exportQuery.executedQuery();
            if(exportQuery.exec()){
                exportQuery.first();
                out << endl;
                for(unsigned int v=0; v<nbrFields; v++){
                    out << exportQuery.value(v).toString();
                    if(v<nbrFields-1) out << " ; ";
                }
                lastID = exportQuery.value(0).toUInt()+1;
            }
        }
        progress.setValue(total);
        QMessageBox::information(this, tr("Datenbankexport"),
                                 QString(tr("%1 Wörter erfolgreich exportiert!")).arg(total));
    }
}

void CentralWidget::importFile(QString fileName){
    QFile file(fileName);
    QTextStream in(&file);
    QString line;
    QStringList fields;
    QSqlQuery ignoreQuery,doublonQuery,importQuery;
    DisambiguationWindow::Outcome outcome;
    if(file.open(QIODevice::ReadOnly)){
        // read headers to determine which language is concerned
        if(in.readLineInto(&line)){
            const QString languageTmp = language;
            const bool isModifyingTmp = isModifying;
            isModifying = false;
            fields = line.split(" ; ", QString::KeepEmptyParts);
            language = "Fr_" + fields.at(4).mid(5,2);
            while(in.readLineInto(&line)){
                fields = line.split(" ; ", QString::KeepEmptyParts);
                ignoreQuery.prepare(QString("SELECT id FROM %1 "
                            "WHERE word_from_1=:wordQ_1 AND word_from_2=:wordQ_2 AND word_from_3=:wordQ_3 AND "
                                "word_to_1=:wordA_1 AND word_to_2=:wordA_2 AND word_to_3=:wordA_3 AND "
                                "ex_from_1=:exQ_1 AND ex_from_2=:exQ_2 AND ex_from_3=:exQ_3 AND "
                                "ex_to_1=:exA_1 AND ex_to_2=:exA_2 AND ex_to_3=:exA_3")
                                    .arg(language));
                for(unsigned int i=0; i<nbrExemples; i++){
                    ignoreQuery.bindValue(QString(":wordQ_%1").arg(i+1), fields.at(i+1));
                    ignoreQuery.bindValue(QString(":wordA_%1").arg(i+1), fields.at(i+4));
                    ignoreQuery.bindValue(QString(":exQ_%1").arg(i+1), fields.at(i+11));
                    ignoreQuery.bindValue(QString(":exA_%1").arg(i+1), fields.at(i+14));
                }
                qDebug() << "importFile(), ignoreQuery:   " << ignoreQuery.executedQuery();
                if(ignoreQuery.exec()){
                    emit newEntryRead();
                    if(ignoreQuery.size() == 0){// exact match not found
                        checkForDuplicates(&doublonQuery, fields.mid(1,3), fields.mid(4,3));
                        qDebug() << "importFile(), doublonQuery:  " << doublonQuery.executedQuery();
                        outcome = DisambiguationWindow::Ignored;
                        if(doublonQuery.size() > 0){
                            outcome = disambiguate(&doublonQuery, fields.mid(1,3), fields.mid(4,3),
                                                    fields.mid(11,3), fields.mid(14,3));
                        }
                        if(outcome == DisambiguationWindow::Ignored){
                            writeToDatabase(&importQuery, fields.mid(1,3), fields.mid(4,3), fields.at(7),
                                            fields.at(8).toInt(), fields.at(9), fields.at(10),
                                            fields.mid(11,3), fields.mid(14,3), fields.at(17), fields.at(18));
                            emit newEntryAdded();
                        }else if(outcome == DisambiguationWindow::Modified){
                            emit newEntryModified();
                        }
                    }
                }
            }
            language    = languageTmp;
            isModifying = isModifyingTmp;
        }else{
            QMessageBox::warning(this, tr("Leere Datei"), tr("Diese Datei scheint leer zu sein."));
        }
    }else{
        QMessageBox::critical(this, tr("Fehlerhafte Datei"),
                             tr("Diese Datei kann Fluessigeswort leider nicht lesen. "
                                "Bitte überprüfen Sie die Datei und probieren Sie nochmals."));
    }
}

/// look into database for QCompleter or final search (return hit in main window)
QVector<unsigned int> CentralWidget::find(const QString& word, Qt::CaseSensitivity sensitive){
    QVector<unsigned int> ids(0);
    QSqlQuery findQuery;
    QString binary="";
    if(sensitive == Qt::CaseSensitive)
        binary="BINARY";
    findQuery.exec(QString("SELECT id FROM %1 "
                              "WHERE word_from_1 LIKE %3 '%%2%' OR word_from_2 LIKE %3 '%%2%' "
                              "OR word_from_3 LIKE %3 '%%2%' OR word_to_1 LIKE %3 '%%2%' "
                              "OR word_to_2 LIKE %3 '%%2%' OR word_to_3 LIKE %3 '%%2%'")
                      .arg(language,word,binary));
    qDebug() << "find(), findQuery:   " << findQuery.executedQuery();
    while(findQuery.next())
        ids.push_back(findQuery.value(0).toUInt());
    if(ids.size() > 0){
        isModifying = true;
        reponseButton->setText(tr("Ändern"));
    }
    return ids;
}

QStringList CentralWidget::match(const QString &firstLetters) const{
    QStringList words;
    QSqlQuery findQuery;
    findQuery.exec(QString("SELECT word_from_1 FROM %1 WHERE word_from_1 LIKE '%%2%' "
                           "UNION "
                           "SELECT word_from_2 FROM %1 WHERE word_from_2 LIKE '%%2%' "
                           "UNION "
                           "SELECT word_from_3 FROM %1 WHERE word_from_3 LIKE '%%2%' "
                           "UNION "
                           "SELECT word_to_1 FROM %1 WHERE word_to_1 LIKE '%%2%' "
                           "UNION "
                           "SELECT word_to_2 FROM %1 WHERE word_to_2 LIKE '%%2%' "
                           "UNION "
                           "SELECT word_to_3 FROM %1 WHERE word_to_3 LIKE '%%2%' ")
                   .arg(language,firstLetters));
    qDebug() << "match(), findQuery:   " << findQuery.executedQuery();
    while(findQuery.next())
        words << findQuery.value(0).toString();
    return words;
}

/// show/hide additional grammatical info when 'nom' or 'verbe' is selected (editing mode)
void CentralWidget::setAdditionalInfo(int index){
    if(index == 0){// => noun
        genreQLayout->setCurrentWidget(genreQBox);
        genreQLayout->currentWidget()->show();
        genreALayout->setCurrentWidget(genreABox);
        genreALayout->currentWidget()->show();
        verbeLayout->currentWidget()->hide();
    }else if(index == 1){// => verb
        genreQLayout->currentWidget()->hide();
        genreALayout->currentWidget()->hide();
        verbeLayout->setCurrentWidget(verbeBox);
        verbeLayout->currentWidget()->show();
    }else{// make additional widgets invisible
        genreQLayout->currentWidget()->hide();
        genreALayout->currentWidget()->hide();
        verbeLayout->currentWidget()->hide();
    }
}

/// empty question-answer fields for user input
void CentralWidget::emptyFields(){
    questionEdit->setText("");
    reponseEdit->setText("");
    for(unsigned int i=0; i<nbrExemples; i++){
        exempleQEdits[i]->setText("");
        exempleAEdits[i]->setText("");
    }
    retraduisUI();
    isModifying = false;
    precedantButton->setEnabled(true);
    suivantButton->setDisabled(true);
}

/// differentiate when [reponseButton] is for "answer" or "input"
void CentralWidget::dispatchClickReponse(){
    if(isEditing)
        writeToDatabase();
    else
        giveAnswer();
}

/// show next question on [suiteButton] clicked
void CentralWidget::suiteClicked(){
    updateUserStats();
    nextQuestion();
    reponseButton->setEnabled(true);
    reponseExemplesButton->setEnabled(true);
}
////////////////////////////////////////////////////////
///   __                                      __
///   ** ADD & MODIFY ENTRIES IN THE DATABASE **
///
///////////////////////////////////////////////////////
/// detect 'der','die','das' for automatically setting gender
void CentralWidget::guessCatGram(const QString &text){//string is empty if signal was editingFinished()
    QString str, strEF=text.simplified();
    if(text == "")
        str = reponseEdit->text().simplified();
    else
        str = strEF;
    QString strSE = str.split(" ", QString::SkipEmptyParts).join("");//remove spaces
    if(language.right(2) == "de"){
        if(str.startsWith("der ")){
            setCatGramNom(tr("M."));
        }else if(str.startsWith("die ")){
            setCatGramNom(tr("F."));
        }else if(str.startsWith("das ")){
            setCatGramNom(tr("N."));
        }else if(strSE.endsWith("en,") or strSE.endsWith("en;") or
                 strEF.endsWith("en")){
            setCatGramVerbe(str);
        }
    }else if(language.right(2) == "en"){
        if(strSE.endsWith("on;")){
            setCatGramNom(tr("N."));
        }else if(str.startsWith("to ")){
            setCatGramVerbe(str);
        }
    }else{//shqip
        if(strSE.endsWith("oj,") or strSE.endsWith("aj,") or
           strSE.endsWith("oj;") or strSE.endsWith("aj;") or
           strEF.endsWith("oj") or strEF.endsWith("aj")){
            setCatGramVerbe(str);
        }else if(strSE.endsWith("isht")){
            catGramBox->setCurrentText(tr("Adverb"));
            genreQLayout->currentWidget()->hide();
            genreALayout->currentWidget()->hide();
            verbeLayout->currentWidget()->hide();
        }else if(str.startsWith("e ") or str.startsWith("i ")){
            catGramBox->setCurrentText(tr("Adjektiv"));
            genreQLayout->currentWidget()->hide();
            genreALayout->currentWidget()->hide();
            verbeLayout->currentWidget()->hide();
        }
    }
}

/// tools for guessCatGram()
void CentralWidget::setCatGramNom(const QString &genre){
    catGramBox->setCurrentText(tr("Nomen"));
    genreQLayout->currentWidget()->show();
    genreALayout->currentWidget()->show();
    verbeLayout->currentWidget()->hide();
    genreABox->setCurrentText(genre);
}

void CentralWidget::setCatGramVerbe(const QString &str){
    catGramBox->setCurrentText(tr("Verb"));
    verbeLayout->currentWidget()->show();
    genreQLayout->currentWidget()->hide();
    genreALayout->currentWidget()->hide();
    if(str.endsWith(",")) verbeBox->setCurrentText(tr("unreg."));
    if(str.endsWith(";")) verbeBox->setCurrentText(tr("reg."));
}

/// add an entry in the database based on user input
void CentralWidget::writeToDatabase(){
    // get input field contents
    QString motQ = questionEdit->text();
    QString motA = reponseEdit->text();
    QStringList exemplesQ, exemplesA;
    for(unsigned int i=0; i<nbrExemples; i++){
        exemplesQ << exempleQEdits[i]->text();
        exemplesA << exempleAEdits[i]->text();
    }

    // check input
    if(!isValidWord(motQ) or !isValidWord(motA)){
        QMessageBox::critical(this, tr("Fehler im Input"),
                    tr("Der Input beim französische oder fremde Wort "
                    "kann unter diesem Format leider nicht hinzufügen werden. "
                    "Bitte probieren Sie den Input zu ändern."),
                    QMessageBox::Ok);
        return;
    }
    for(unsigned int i=0; i<nbrExemples; i++){
        // non-empty entry containing bizarre characters:
        if(!isValidExample(exemplesQ[i]) or !isValidExample(exemplesA[i])){
            QMessageBox::critical(this, tr("Fehler im Input"),
                        QString(tr("Der Input beim französische oder fremde Beispiel Nummer %1 "
                                "kann unter diesem Format leider nicht hinzufügen werden. "
                                "Bitte probieren Sie den Input zu ändern.")).arg(i+1),
                        QMessageBox::Ok);
            return;
        // examples do not match:
        }else if((exemplesQ[i].simplified() != "" and exemplesA[i].simplified() == "") or
                 (exemplesQ[i].simplified() == "" and exemplesA[i].simplified() != "")){
            QMessageBox::critical(this, tr("Fehler im Input"),
                        QString(tr("Der Input beim Beispiel Nummer %1 kann unter diesem Format "
                                   "leider nicht hinzufügen werden. "
                                   "Bitte ergänzen Sie beide Sprache mit dem Beispiel.")).arg(i+1),
                        QMessageBox::Ok);
            return;
        // considered as OK:
        }else{
            exemplesQ[i] = exemplesQ[i].simplified();
            exemplesA[i] = exemplesA[i].simplified();
        }
    }
    QStringList motsQ = parseInput(motQ);
    QStringList motsA = parseInput(motA);
    if(!isModifying){
        QSqlQuery doublonQuery;
        checkForDuplicates(&doublonQuery, motsQ,motsA);
        qDebug() << "writeToDatabase(), doublonQuery:   " << doublonQuery.executedQuery();
        if(doublonQuery.size() > 0){
            DisambiguationWindow::Outcome outcome = disambiguate(&doublonQuery, motsQ, motsA,
                                                                 exemplesQ, exemplesA);
            if(outcome != DisambiguationWindow::Ignored) return;
        }
    }

    // retrieve word characteristics
    QString typeMot = toFrench(catGramBox->currentText());
    QString genreQ="f.", genreA="f.";
    int verbe=0;
    if(typeMot == "nom"){
        genreQ = toFrench(genreQBox->currentText());
        genreA = toFrench(genreABox->currentText());
    }else if(typeMot == "verbe"){
        verbe = verbeBox->currentIndex();// 0: reg., 1: irreg.
    }

    // automatically filled entries
    QString creation = QDate::currentDate().toString("yyyy-MM-dd");
    QString modif    = creation;

    // SQL insertion/update
    QSqlQuery insertQuery;
    if(writeToDatabase(&insertQuery, motsQ, motsA, typeMot, verbe, genreQ, genreA,
                       exemplesQ, exemplesA, modif, creation)){
        qDebug() << "writeToDatabase(), insertQuery:   " << insertQuery.executedQuery();
        if(isModifying){
            QMessageBox::information(this, tr("Erfolgreiche Änderung"),
                                     tr("Ein Wort wurde erfolgreich in der Datenbank beändert."),
                                     QMessageBox::Ok);
            isModifying = false;
            reponseButton->setText(tr("Hinzufügen"));
        }else{
            QMessageBox::information(this, tr("Erfolgreiche Hinzufügung"),
                                     tr("Ein Wort wurde erfolgreich in die Datenbank hinzugefügt."),
                                     QMessageBox::Ok);
        }
        emptyFields();
    }else{
        QMessageBox::warning(this, tr("Fehler bei der Hinzufügung"),
                             QString(tr("Die Worthinzufügung ist fehlgeschlagen. Bitte notieren Sie den "
                                "Fehlerskontext und berichten Sie ihn dem Developer.\n"
                                "Die Datenbank berichtete: %1")
                                .arg(insertQuery.lastError().text())),
                             QMessageBox::Ok);
    }
}

/// show disambiguation window and do what is appropriate
DisambiguationWindow::Outcome CentralWidget::disambiguate(QSqlQuery* query,
                                    const QStringList& motsQ, const QStringList& motsA,
                                    const QStringList& exemplesQ, const QStringList& exemplesA){
    QStringList currentEntry;
    currentEntry << motsQ.join(";").split(";", QString::SkipEmptyParts).join(" ; ");
    currentEntry << motsA.join(";").split(";", QString::SkipEmptyParts).join(" ; ");
    currentEntry << exemplesQ.join(";").split(";", QString::SkipEmptyParts).join(" ; ");
    currentEntry << exemplesA.join(";").split(";", QString::SkipEmptyParts).join(" ; ");
    DisambiguationWindow disambiguation(query, language, currentEntry, this);
    int ret = disambiguation.exec();
    if(ret == QDialog::Rejected){
        QMessageBox::information(this, tr("Wort nicht hinzugefügt"),
                                 tr("Das vorgeschlagene Wort wurde in der Datenbank nicht gespeichert."),
                                 QMessageBox::Ok);
        return DisambiguationWindow::Cancelled;// nothing further to be done
    }else{
        QVector<int> selection = disambiguation.getSelection();
        bool reinit = disambiguation.getIsChecked();
        if(selection.size() > 0){// otherwise continue saving this input
            mergeDuplicates(selection, query, motsQ, motsA,
                                 exemplesQ, exemplesA, reinit);
            return DisambiguationWindow::Modified;
        }
        return DisambiguationWindow::Ignored;// continue writing to database
    }
}

/// SQL insertion/update
bool CentralWidget::writeToDatabase(QSqlQuery *query, const QStringList &motsQ, const QStringList &motsA,
                                    const QString &typeMot, const int verbe,
                                    const QString &genreQ, const QString &genreA,
                                    const QStringList &exemplesQ, const QStringList &exemplesA,
                                    const QString &modif, const QString &creation) const{
    if(isModifying){
        QString queryStr="UPDATE %1 SET ";
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString("word_from_%1=:wordQ_%1,").arg(i+1);
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString("word_to_%1=:wordA_%1,").arg(i+1);
        queryStr += "word_type = (SELECT id FROM Cat_gram WHERE cat=:word_type),"
                    "irreg_verb=:irreg_verb,"
                    "gender_from = (SELECT id FROM Gdr WHERE cat=:genderQ),"
                    "gender_to = (SELECT id FROM Gdr WHERE cat=:genderA),";
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString("ex_from_%1=:exQ_%1,").arg(i+1);
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString("ex_to_%1=:exA_%1,").arg(i+1);
        queryStr += "modified=:modified WHERE id=:id";
        // TODO: check if query in use is equivalent to new query
        qDebug() << "writeToDatabase() - isModifying: check if query in use is equiv to new query\n";
        qDebug() << "New:    " << queryStr.arg(language);
        qDebug() << "In use: " << QString("UPDATE %1 "
                                          "SET word_from_1=:wordQ_1, word_from_2=:wordQ_2, word_from_3=:wordQ_3,"
                                          "word_to_1=:wordA_1, word_to_2=:wordA_2, word_to_3=:wordA_3,"
                                          "word_type = (SELECT id FROM Cat_gram WHERE cat=:word_type),"
                                          "irreg_verb=:irreg_verb,"
                                          "gender_from = (SELECT id FROM Gdr WHERE cat=:genderQ),"
                                          "gender_to = (SELECT id FROM Gdr WHERE cat=:genderA),"
                                          "ex_from_1=:exQ_1, ex_from_2=:exQ_2, ex_from_3=:exQ_3,"
                                          "ex_to_1=:exA_1, ex_to_2=:exA_2, ex_to_3=:exA_3, modified=:modified "
                                          "WHERE id=:id")
                                          .arg(language);

        /*query->prepare(queryStr.arg(language));*/
        query->prepare(QString("UPDATE %1 "
                            "SET word_from_1=:wordQ_1, word_from_2=:wordQ_2, word_from_3=:wordQ_3,"
                            "word_to_1=:wordA_1, word_to_2=:wordA_2, word_to_3=:wordA_3,"
                            "word_type = (SELECT id FROM Cat_gram WHERE cat=:word_type),"
                            "irreg_verb=:irreg_verb,"
                            "gender_from = (SELECT id FROM Gdr WHERE cat=:genderQ),"
                            "gender_to = (SELECT id FROM Gdr WHERE cat=:genderA),"
                            "ex_from_1=:exQ_1, ex_from_2=:exQ_2, ex_from_3=:exQ_3,"
                            "ex_to_1=:exA_1, ex_to_2=:exA_2, ex_to_3=:exA_3, modified=:modified "
                            "WHERE id=:id")
                            .arg(language));
        query->bindValue(":id", id);
    }else{
        QString queryStr="INSERT INTO %1 (";
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString("word_from_%1,").arg(i+1);
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString("word_to_%1,").arg(i+1);
        queryStr += "word_type,irreg_verb,gender_fr,gender_%2,";
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString("ex_from_%1,").arg(i+1);
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString("ex_to_%1,").arg(i+1);
        queryStr += "created,modified) SELECT ";
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString(":wordQ_%1,").arg(i+1);
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString(":wordA_%1,").arg(i+1);
        queryStr += "Word_type.id,:irreg_verb,GdrQ.id,GdrA.id,";
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString(":exQ_%1,").arg(i+1);
        for(unsigned int i=0; i<nbrExemples; i++)
            queryStr += QString(":exA_%1,").arg(i+1);
        queryStr += ":created,:modified "
                    "FROM "
                    "(SELECT id FROM Cat_gram WHERE cat = :word_type) AS Word_type, "
                    "(SELECT id FROM Gdr WHERE cat = :genderQ) AS GdrQ, "
                    "(SELECT id FROM Gdr WHERE cat = :genderA) AS GdrA";
        // TODO: check here as well
        qDebug() << "writeToDatabase() - adding new entry: check if query in use is equiv to new query";
        qDebug() << "New:    " << queryStr.arg(language);
        qDebug() << "In use: " << QString("INSERT INTO %1 (word_from_1,word_from_2,word_from_3,"
                                          "word_to_1,word_to_2,word_to_3,"
                                          "word_type,irreg_verb,gender_from,gender_to,"
                                          "ex_from_1,ex_from_2,ex_from_3,ex_to_1,ex_to_2,ex_to_3,created,modified) "
                       "SELECT :wordQ_1,:wordQ_2,:wordQ_3,:wordA_1,:wordA_2,:wordA_3,"
                       "Word_type.id,:irreg_verb,GdrQ.id,GdrA.id,"
                       ":exQ_1,:exQ_2,:exQ_3,:exA_1,:exA_2,:exA_3,:created,:modified "
                       "FROM "
                       "(SELECT id FROM Cat_gram WHERE cat = :word_type) AS Word_type, "
                       "(SELECT id FROM Gdr WHERE cat = :genderQ) AS GdrQ, "
                       "(SELECT id FROM Gdr WHERE cat = :genderA) AS GdrA")
                             .arg(language);

        /*query->prepare(queryStr.arg(language));*/
        query->prepare(QString("INSERT INTO %1 (word_from_1,word_from_2,word_from_3,"
                                         "word_to_1,word_to_2,word_to_3,"
                                         "word_type,irreg_verb,gender_from,gender_to,"
                                         "ex_from_1,ex_from_2,ex_from_3,ex_to_1,ex_to_2,ex_to_3,created,modified) "
                      "SELECT :wordQ_1,:wordQ_2,:wordQ_3,:wordA_1,:wordA_2,:wordA_3,"
                      "Word_type.id,:irreg_verb,GdrQ.id,GdrA.id,"
                      ":exQ_1,:exQ_2,:exQ_3,:exA_1,:exA_2,:exA_3,:created,:modified "
                      "FROM "
                      "(SELECT id FROM Cat_gram WHERE cat = :word_type) AS Word_type, "
                      "(SELECT id FROM Gdr WHERE cat = :genderQ) AS GdrQ, "
                      "(SELECT id FROM Gdr WHERE cat = :genderA) AS GdrA")
                            .arg(language));
        query->bindValue(":created", creation);
    }
    query->bindValue(":wordQ_1", motsQ[0]);
    query->bindValue(":wordQ_2", motsQ[1]);
    query->bindValue(":wordQ_3", motsQ[2]);
    query->bindValue(":wordA_1", motsA[0]);
    query->bindValue(":wordA_2", motsA[1]);
    query->bindValue(":wordA_3", motsA[2]);
    query->bindValue(":word_type", typeMot);
    query->bindValue(":irreg_verb", verbe);
    query->bindValue(":genderQ", genreQ);
    query->bindValue(":genderA", genreA);
    for(unsigned int i=0; i<nbrExemples; i++){
        query->bindValue(QString(":exQ_%1").arg(i+1), exemplesQ[i]);
        query->bindValue(QString(":exA_%1").arg(i+1), exemplesA[i]);
    }
    query->bindValue(":modified", modif);
    bool ret = query->exec();

    return ret;
}

/// check user input is valid for question/answer fields
bool CentralWidget::isValidWord(const QString &input) const{
    QString str = input;
    str.remove(QRegExp("[^A-Za-z]"));
    str = str.simplified();
    if(str == "") return false;
    return true;
}

/// check user input is valid for example fields
bool CentralWidget::isValidExample(const QString &input) const{
    if(input.simplified() != "")
        return isValidWord(input);
    return true;
}

/// split input at each ";" (different meanings of a word) and
/// give a 3-QString list with possibly empty strings
QStringList CentralWidget::parseInput(const QString &input){
    QStringList strList = input.split(';', QString::SkipEmptyParts);
    if(strList.size() > 3){
        QMessageBox::warning(this, tr("Zu viele Wortbedeutungen"),
                    tr("Nur die drei ersten hinzugefügt Wortbedeutungen werden berücksichtigt."),
                    QMessageBox::Ok);
        do{
            strList.removeLast();
        }while(strList.size() > 3);
    }
    // remove spaces of any kind at both ends + replace \t,\n,... by spaces:
    for(int i=0; i<strList.size(); i++)
        strList[i] = strList[i].simplified();
    while(strList.size() < 3)
        strList.append("");
    return strList;
}

/// look in the database if there are entries similar to user input
void CentralWidget::checkForDuplicates(QSqlQuery* query, const QStringList& motsQ,
                                                   const QStringList& motsA){
    query->exec(QString("SELECT id,word_from_1,word_from_2,word_from_3,word_to_1,word_to_2,word_to_3,"
                                   "ex_from_1,ex_from_2,ex_from_3,ex_to_1,ex_to_2,ex_to_3,created,modified "
                       "FROM %1 "
                       "WHERE word_from_1 IN ('%2') OR word_from_2 IN ('%2') OR word_from_3 IN ('%2') "
                       "OR word_to_1 IN ('%3') OR word_to_2 IN ('%3') OR word_to_3 IN ('%3') "
                       "ORDER BY created, modified")
                    .arg(language)
                    .arg(motsQ.join(",").split(",", QString::SkipEmptyParts).join("','"))
                    .arg(motsA.join(",").split(",", QString::SkipEmptyParts).join("','")));
}

/// merge entries marked as duplicates by the user
void CentralWidget::mergeDuplicates(QVector<int> selection, QSqlQuery *duplicates,
                                    QStringList motsQ, QStringList motsA,
                                    QStringList exemplesQ, QStringList exemplesA, bool reinit){
    // remove empty entries
    motsQ  = motsQ.join(",").split(",",QString::SkipEmptyParts);
    motsA  = motsA.join(",").split(",",QString::SkipEmptyParts);
    exemplesQ = exemplesQ.join(",").split(",",QString::SkipEmptyParts);
    exemplesA = exemplesA.join(",").split(",",QString::SkipEmptyParts);
    // retrieve all entries from [selection]
    QStringList selmotsQ, selmotsA, selexQ, selexA;
    duplicates->first();
    for(int i=0; i<selection.size(); i++){
        if(!duplicates->seek(selection[i])) qDebug() << "problem in doublon query...\n";
        for(unsigned int j=1; j<=nbrExemples; j++){
            QString mot = duplicates->value(j).toString();
            if(mot != "") selmotsQ << mot;
            mot = duplicates->value(j+nbrExemples).toString();
            if(mot != "") selmotsA << mot;
        }
        for(unsigned int j=1; j<=nbrExemples; j++){
            QString mot = duplicates->value(j+nbrExemples*2).toString();
            if(mot != "") selexQ << mot;
            mot = duplicates->value(j+nbrExemples*2+nbrExemples).toString();
            if(mot != "") selexA << mot;
        }
    }
    // complete lists with input
    selmotsQ << motsQ;
    selmotsA << motsA;
    selexQ << exemplesQ;
    selexA << exemplesA;
    // remove duplicates (order kept)
    QMessageBox::information(this, tr("Identische Eingaben"),
                             QString(tr("%1 französische und %2 deutsche Bedeutungen wurden als "
                                        "identische Eingaben festgestellt und gelöscht."))
                                .arg(selmotsQ.removeDuplicates())
                                .arg(selmotsA.removeDuplicates()),
                             QMessageBox::Ok);
    // remove newest entries
    QSqlQuery removeQuery;
    for(int i=1; i < selection.size(); i++){// keep first result
        duplicates->seek(selection[i]);
        removeQuery.prepare(QString("DELETE FROM %1 WHERE id=:id").arg(language));
        removeQuery.bindValue(":id", duplicates->value(0).toInt());
        qDebug() << "mergeDuplicates(), removeQuery:   " << removeQuery.executedQuery();
        removeQuery.exec();
    }
    // at least 3 entries
    while(selmotsQ.size() < 3) selmotsQ.append("");
    while(selmotsA.size() < 3) selmotsA.append("");
    while(selexQ.size() < 3) selexQ.append("");
    while(selexA.size() < 3) selexA.append("");
    // modify oldest in database
    duplicates->seek(selection[0]);
    QString modif = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery modifyQuery;
    modifyQuery.prepare(QString("UPDATE %1 "
                        "SET word_from_1=:wordQ_1, word_from_2=:wordQ_2, word_from_3=:wordQ_3,"
                        "word_to_1=:wordA_1, word_to_2=:wordA_2, word_to_3=:wordA_3,"
                        "ex_from_1=:exQ_1, ex_from_2=:exQ_2, ex_from_3=:exQ_3,"
                        "ex_to_1=:exA_1, ex_to_2=:exA_2, ex_to_3=:exA_3, modified=:modified "
                        "WHERE id=:id")
                        .arg(language).arg(language.right(2)));
    for(unsigned int i=0; i<nbrExemples; i++){
        modifyQuery.bindValue(QString(":wordQ_%1").arg(i+1), selmotsQ[i]);
        modifyQuery.bindValue(QString(":wordA_%1").arg(i+1), selmotsA[i]);
    }
    for(unsigned int i=0; i<nbrExemples; i++){
        modifyQuery.bindValue(QString(":exQ_%1").arg(i+1), selexQ[i]);
        modifyQuery.bindValue(QString(":exA_%1").arg(i+1), selexA[i]);
    }
    modifyQuery.bindValue(":modified", modif);
    modifyQuery.bindValue(":id", duplicates->value(0).toInt());
    qDebug() << "mergeDuplicates(), modifyQuery:   " << modifyQuery.executedQuery();
    modifyQuery.exec();
    if(reinit){
        modifyQuery.prepare(QString("UPDATE %1 "
                                    "SET last_asked=:last_asked, nbr_asked=0, nbr_wrong=0,"
                                    "last_wrong=:last_wrong, nbr_last_wrong=0, nbr_last_correct=0"
                                    "WHERE word_id=:id")
                            .arg(user+"_"+language));
        modifyQuery.bindValue(":last_asked", modif);
        modifyQuery.bindValue(":last_wrong", false);
        modifyQuery.bindValue(":id", duplicates->value(0).toInt());
        qDebug() << "mergeDuplicates(), modifyQuery (reinit==TRUE):   " << modifyQuery.executedQuery();
        modifyQuery.exec();
    }
}

void CentralWidget::previousEntry(){
    QSqlQuery previousQuery;
    if(isModifying){// schon zurückgegangen aber noch kein Wort geändert
        previousQuery.prepare(QString("SELECT id FROM %1 "
                                      "WHERE id < :id ORDER BY id DESC LIMIT 1")
                              .arg(language));
        previousQuery.bindValue(":id", id);
        qDebug() << "previousEntry(), previousQuery (isModifying==TRUE):   " << previousQuery.executedQuery();
        previousQuery.exec();
        if(!previousQuery.first()){
            QMessageBox::critical(this, tr("Anfang der Datenbank erreicht"),
                                  tr("Der Anfang der Datenbank wurde erreicht. Es ist nicht "
                                     "mehr möglich, rückwerts zu gehen."),
                                  QMessageBox::Ok);
            precedantButton->setDisabled(true);
            return;
        }
    }else{// erstes Mal zurückgegangen
        previousQuery.exec(QString("SELECT id FROM %1 "
                                   "ORDER BY id DESC LIMIT 1")
                           .arg(language));
        qDebug() << "previousEntry(), previousQuery (isModifying==FALSE):   " << previousQuery.executedQuery();
        if(!previousQuery.first()){
            QMessageBox::critical(this, tr("Leere Datenbank"),
                                  tr("Kein Datum konnte gefunden werden: die Datenbank enthält noch kein Wort.\n"
                                     "Bitte machen Sie einen Beitrag erst!"),
                                  QMessageBox::Ok);
            return;
        }else{
            suivantButton->setEnabled(true);
        }
    }
    id = previousQuery.value(0).toInt();
    reponseButton->setText(tr("Ändern"));
    isModifying = true;
    giveQuestion();
}

void CentralWidget::nextEntry(){
    QSqlQuery nextQuery;
    nextQuery.prepare(QString("SELECT id FROM %1 "
                           "WHERE id > :id ORDER BY id ASC LIMIT 1")
                        .arg(language));
    nextQuery.bindValue(":id", id);
    qDebug() << "nextEntry(), nextQuery:   " << nextQuery.executedQuery();
    nextQuery.exec();
    if(!nextQuery.first()){
        reponseButton->setText(tr("Hinzufügen"));
        emptyFields();// sets isModifying to false
        suivantButton->setDisabled(true);
    }else{
        id = nextQuery.value(0).toInt();
        giveQuestion();
    }
}

void CentralWidget::updateCurrentUserTableEntries() const{
    if(user == "" or language == "") return;
    QSqlQuery checkQuery;
    checkQuery.exec(QString("SELECT CheckExist.id FROM ("
                                "SELECT %1.id,word_id FROM %1 "
                                "LEFT JOIN %2 ON %1.id = %2.word_id ) AS CheckExist "
                            "WHERE CheckExist.word_id IS NULL")
                    .arg(language)
                    .arg(user+"_"+language));
    qDebug() << "updateCurrentUserTableEntries(), checkQuery:   " << checkQuery.executedQuery();
    while(checkQuery.next()){
        QSqlQuery updateQuery;
        updateQuery.prepare(QString("INSERT INTO %1 (word_id) VALUES (:word_id)")
                            .arg(user+"_"+language));
        updateQuery.bindValue(":word_id",checkQuery.value(0).toInt());
        qDebug() << "updateCurrentUserTableEntries(), updateQuery:   " << updateQuery.executedQuery();
        updateQuery.exec();
    }
}


//////////////////////////////////////////////////
///   __                                   __
///   ** ASKING QUESTIONS & GIVING ANSWERS **
///
//////////////////////////////////////////////////
/// update stats in user-related SQL table
void CentralWidget::updateUserStats(){
    if(questionEdit->text() != ""){
        // get current stats
        QSqlQuery updateQuery;
        updateQuery.prepare(QString("SELECT nbr_asked,nbr_wrong,last_wrong,nbr_last_wrong,nbr_last_correct "
                                    "FROM %1 WHERE word_id=:id")
                            .arg(user+"_"+language));
        updateQuery.bindValue(":id",id);
        qDebug() << "updateUserStats(), updateQuery (SELECT):   " << updateQuery.executedQuery();
        updateQuery.exec();
        updateQuery.first();
        int na  = updateQuery.value(0).toInt();
        int nw  = updateQuery.value(1).toInt();
        bool lw = updateQuery.value(2).toBool();
        int nlw = updateQuery.value(3).toInt();
        int nlc = updateQuery.value(4).toInt();
        // update stats
        na++;
        if(fauxButton->isChecked()){
            nw++;
            if(lw) nlw++;
            else nlw = 1;
        }else{
            if(lw) nlc = 1;
            else nlc++;
        }
        lw = fauxButton->isChecked();
        QString today = QDate::currentDate().toString("yyyy-MM-dd");
        updateQuery.prepare(QString("UPDATE %1 "
                                    "SET last_asked=:last_asked, nbr_asked=:nbr_asked,"
                                        "nbr_wrong=:nbr_wrong,last_wrong=:last_wrong,"
                                        "nbr_last_wrong=:nbr_last_wrong,nbr_last_correct=:nbr_last_correct "
                                    "WHERE word_id=:id")
                            .arg(user+"_"+language));
        updateQuery.bindValue(":last_asked",today);
        updateQuery.bindValue(":nbr_asked",na);
        updateQuery.bindValue(":nbr_wrong",nw);
        updateQuery.bindValue(":last_wrong",lw);
        updateQuery.bindValue(":nbr_last_wrong",nlw);
        updateQuery.bindValue(":nbr_last_correct",nlc);
        updateQuery.bindValue(":id",id);
        qDebug() << "updateUserStats(), updateQuery (UPDATE):   " << updateQuery.executedQuery();
        updateQuery.exec();
    }
}

/// show next question to user when 'next' button clicked
void CentralWidget::nextQuestion(){
    QVector<qreal> weight;
    qreal w,sumw=0;
    QSqlQuery getQuery;
    getQuery.exec(QString("SELECT word_id,created,modified,last_asked,nbr_asked,nbr_wrong,last_wrong,"
                                  "nbr_last_wrong,nbr_last_correct "
                           "FROM %1 "
                           "INNER JOIN %2 ON %2.id = word_id %3 "
                           "ORDER BY nbr_asked DESC")
                   .arg(user+"_"+language)
                   .arg(language)
                   .arg(drillQuery()));
    qDebug() << "nextQuestion(), getQuery:   " << getQuery.executedQuery();
    if(!getQuery.first()){
        QMessageBox::critical(this, tr("Leere Datenbank"),
                              tr("Keine Frage kann gestellt werden: die Datenbank enthält noch kein Wort.\n"
                                 "Bitte wählen Sie 'Wörter hinzufügen' unter dem 'Datei' Menü, um die "
                                 "Datenbank zu fühlen."), QMessageBox::Ok);
        return;
    }
    int maxAsked = getQuery.value(4).toInt();
    QDate lastAsked(2000,1,1);// never used if nbr_asked == 0
    do{
        if(getQuery.value(4).toInt() > 0){
            lastAsked = getQuery.value(3).toDate();
        }
        w = wordWeight(getQuery.value(1).toDate(), getQuery.value(2).toDate(), lastAsked,
                   getQuery.value(4).toInt(), maxAsked, getQuery.value(5).toInt(), getQuery.value(6).toInt(),
                   getQuery.value(7).toInt(), getQuery.value(8).toInt());
        weight.append(w);
        sumw += w;
    }while(getQuery.next());
    // randomisation procedure
    qreal u = qrand()*sumw/RAND_MAX; // in [0,sum of the weights]
    int i=-1;
    do{
        if(i >= weight.size()) qDebug() << "problem in the weights!\n";
        i++;
        u -= weight.at(i);
    }while(u > 0);
    if(!getQuery.seek(i)) qDebug() << "problem in multinomial sampling\n";
    id = getQuery.value(0).toInt();// word_id, i.e. id in Fr_de for example
    if(isDrilling and drillIDs.size() < drillSize and !drillIDs.contains(id)){
        drillIDs.append(id);
    }
    giveQuestion();
}

/// show question (question mode) or show some previously added entry (editing mode)
/// or show next entry in list of words found (editing mode)
void CentralWidget::giveQuestion(const unsigned int id){
    this->id = id;
    giveQuestion();
}

void CentralWidget::giveQuestion(){
    genreA = ""; verbe = "";
    QSqlQuery questQuery;
    int qEx=0;
    int aEx=qEx+nbrExemples;
    QString queryStr="SELECT ";
    for(unsigned int i=0; i<nbrExemples; i++)
        queryStr += QString("word_from_%1,").arg(i+1);
    for(unsigned int i=0; i<nbrExemples; i++)
        queryStr += QString("word_to_%1,").arg(i+1);
    queryStr += "Cat_gram.cat,irreg_verb,GdrQ.cat,GdrA.cat";
    for(unsigned int i=0; i<nbrExemples; i++)
        queryStr += QString(",ex_from_%1").arg(i+1);
    for(unsigned int i=0; i<nbrExemples; i++)
        queryStr += QString(",ex_to_%1").arg(i+1);
    queryStr += " FROM %1 "
                "LEFT JOIN Gdr AS GdrQ ON gender_from = GdrQ.id "
                "LEFT JOIN Gdr AS GdrA ON gender_to = GdrA.id "
                "INNER JOIN Cat_gram ON word_type = Cat_gram.id "
                "WHERE %1.id = :id";
    questQuery.prepare(queryStr.arg(language));
    /*questQuery.prepare(QString("SELECT word_from_1,word_from_2,word_from_3,word_to_1,word_to_2,word_to_3,"
                                 "Cat_gram.cat,irreg_verb,GdrQ.cat,GdrA.cat,"
                                 "ex_from_1,ex_from_2,ex_from_3,ex_to_1,ex_to_2,ex_to_3 "
                              "FROM %1 "
                              "LEFT JOIN Gdr AS GdrQ ON gender_from = GdrQ.id "
                              "LEFT JOIN Gdr AS GdrA ON gender_to = GdrA.id "
                              "INNER JOIN Cat_gram ON word_type = Cat_gram.id "
                              "WHERE %1.id = :id")
                       .arg(language));*/
    questQuery.bindValue(":id", id);
    qDebug() << "giveQuestion(), questQuery:   " << questQuery.executedQuery();
    qDebug() << "id = " << id;
    questQuery.exec();
    QStringList mots;
    QString param;
    questQuery.first();
    if(isSwapping and !isEditing)
        std::swap(aEx,qEx);
    qDebug() << "giveQuestion(): ok up to query execution";
    // first language : show content
    for(unsigned int q=qEx; q<nbrExemples; q++)
        mots << questQuery.value(q).toString();
    qDebug() << "giveQuestion(): ok up to French words extraction";
    questionEdit->setText(mots.join(";").split(";",QString::SkipEmptyParts).join(" ; "));
    param = questQuery.value(nbrExemples*2).toString();
    if(isEditing)
        catGramBox->setCurrentText(trDB(param));
    else
        catGramLabel->setText(trDB(param));
    catGramLayout->currentWidget()->show();
    if(param == "nom"){
        if(isEditing){
            genreQBox->setCurrentText(trDB(questQuery.value(8).toString()));
            genreABox->setCurrentText(trDB(questQuery.value(9).toString()));
        }else{
            int q=nbrExemples*2+2;
            int a=q+1;
            if(isSwapping) std::swap(q,a);
            genreQLabel->setText(trDB(questQuery.value(q).toString()));
            genreALabel->setText("");
            genreA = trDB(questQuery.value(a).toString());
        }
        genreQLayout->currentWidget()->show();
        genreALayout->currentWidget()->show();
        verbeLayout->currentWidget()->hide();
        qDebug() << "giveQuestion(): ok up to extraction of genre";
    }else if(param == "verbe"){
        if(isEditing){
            genreQLayout->currentWidget()->hide();
            genreALayout->currentWidget()->hide();
            switch(questQuery.value(nbrExemples*2+1).toInt()){
            case 0:
                verbeBox->setCurrentText(tr("reg."));
                break;
            default:
                verbeBox->setCurrentText(tr("unreg."));
            }
        }else{
            genreQLayout->currentWidget()->hide();
            genreALayout->currentWidget()->hide();
            verbeLabel->setText("");
            switch(questQuery.value(nbrExemples*2+1).toInt()){
            case 0:
                verbe = tr("reg.");
                break;
            default:
                verbe = tr("unreg.");
            }
        }
        verbeLayout->currentWidget()->show();
    }else{
        genreQLayout->currentWidget()->hide();
        genreALayout->currentWidget()->hide();
        verbeLayout->currentWidget()->hide();
    }
    for(unsigned int q=qEx; q<nbrExemples; q++)
        exempleQEdits[q]->setText(questQuery.value(q+nbrExemples*2+4).toString());
    qDebug() << "giveQuestion(): ok up to extraction of all French-related fields";
    // second language: fill fields but hide content
    mots.clear();
    for(unsigned int a=aEx; a<aEx+nbrExemples; a++)
        mots << questQuery.value(a).toString();
    qDebug() << "giveQuestion(): ok up to extraction of 'other language' words";
    if(!isEditing){
        reponseEdit->setEchoMode(QLineEdit::NoEcho);
        for(unsigned int a=aEx; a<aEx+nbrExemples; a++)
            exempleAEdits[a-aEx]->setEchoMode(QLineEdit::NoEcho);
        qDebug() << "giveQuestion(): ok, if not in editing mode";
    }
    qDebug() << "giveQuestion(): ok up to switching mode of LineEdits, if required";
    reponseEdit->setText(mots.join(";").split(";",QString::SkipEmptyParts).join(" ; "));
    for(unsigned int a=aEx; a<aEx+nbrExemples; a++)
        exempleAEdits[a-aEx]->setText(questQuery.value(a+nbrExemples*2+4).toString());
    qDebug() << "giveQuestion(): ok up to extraction of all 'other language'-related fields";
    coloursChanged();
    qDebug() << "giveQuestion(): ok up until the end";
}

/// show answer for words when upper 'answer' button clicked
void CentralWidget::giveAnswer(){
    reponseEdit->setEchoMode(QLineEdit::Normal);

}

/// show answer for examples when lower 'answer' button clicked
void CentralWidget::giveExamplesAnswer(){
    for(unsigned int i=0; i<nbrExemples; i++)
        exempleAEdits[i]->setEchoMode(QLineEdit::Normal);
}

/// compute weight (probability to be choosed) for a given db entry
qreal CentralWidget::wordWeight(const QDate &cr, const QDate &mod, const QDate &la,
                                 const int na, const int ma, const int nw, const int lw,
                                 const int nlw, const int nlc) const{
    qreal l,d,f,h; // life,dust,freq,hist related weights
    QDate cur = QDate::currentDate();
    // creation & modification dates (word's life)
    l = cur.daysTo(cr)+cur.daysTo(mod); // l < 0
    l = 1-qExp(l/2); // in [0,1)
    // last time asked date (dust laid on the word)
    switch(na){
    case 0:
        d = 1;
        break;
    default:
        d = cur.daysTo(la);// d < 0
        d = 1-qExp(d); // in [0,1]
    }
    // number of times asked (asking frequency for this word)
    switch(ma){
    case 0:
        f = 1;
        break;
    default:
        f = na/ma-1; // f < 0
        f = 1-qExp(f);
    }
    // number of wrong answers, last wrong? etc. (counters history)
    switch(na){
    case 0:
        h = 1;
        break;
    default:
        h = (nlw-nlc)/(nlw+nlc)/4;
        h+= qPow(-1, 1-lw)/2;// lw == FALSE <=> lw == 0
        h+= 1/2;
        h*= nw/na;
    }
    // return customised weight
    qreal ret = qExp(life)*l+qExp(dust)*d+qExp(freq)*f+qExp(hist)*h;
    ret /= qExp(life)+qExp(dust)+qExp(freq)+qExp(hist);
    qDebug() << "computed weight = " << ret;
    return ret;
}

QString CentralWidget::drillQuery(){
    if(isDrilling and drillIDs.size() == drillSize){
        QString drillStr(" WHERE word_id IN (");
        for(int i=0; i<drillSize; i++){
            drillStr += QString::number(drillIDs.at(i)) + ",";
        }
        drillStr.remove(drillStr.length()-1,1);
        drillStr += ") ";
        qDebug() << drillStr;
        return drillStr;
    }else{
        return QString("");
    }
}

/////////////////////////////////
///   __                  __
///   ** USER INTERACTION **
///
/////////////////////////////////
void CentralWidget::disableButtons(bool b){
    reponseButton->setDisabled(b);
    suiteButton->setDisabled(b);
    fauxButton->setDisabled(b);
    reponseExemplesButton->setDisabled(b);
    if(!b) toggleEditingMode(false);
}

void CentralWidget::coloursChanged(){
    if(isEditing){
        QString emptyStyleSheet = "QLineEdit {color : black; background : white}";
        questionEdit->setStyleSheet(emptyStyleSheet);
        reponseEdit->setStyleSheet(emptyStyleSheet);
        for(unsigned int i=0; i<nbrExemples; i++){
            exempleQEdits[i]->setStyleSheet(emptyStyleSheet);
            exempleAEdits[i]->setStyleSheet(emptyStyleSheet);
        }
    }else{
        qDebug() << "attention... " << questColours.size();
        qDebug() << "et ans " << ansColours.size();
        QStringList q=questColours;
        QStringList a=ansColours;
        if(isSwapping) std::swap(q,a);
        QString questStyleSheet = QString("QLineEdit {color : %1; background : %2}")
                .arg(q.at(0)).arg(q.at(1));
        QString ansStyleSheet = QString("QLineEdit {color : %1; background : %2}")
                .arg(a.at(0)).arg(a.at(1));
        questionEdit->setStyleSheet(questStyleSheet);
        reponseEdit->setStyleSheet(ansStyleSheet);
        for(unsigned int i=0; i<nbrExemples; i++){
            exempleQEdits[i]->setStyleSheet(questStyleSheet);
            exempleAEdits[i]->setStyleSheet(ansStyleSheet);
        }
        if(catGramLabel->text() == tr("Nomen")){
            qDebug() << "et pour les noms " << gdrColours.size();
            if(genreALabel->text() == tr("M.")){
                reponseEdit->setStyleSheet(QString("QLineEdit {color : %1; background : %2}")
                                           .arg(gdrColours.at(0)).arg(a.at(1)));
            }else if(genreALabel->text() == tr("F.")){
                reponseEdit->setStyleSheet(QString("QLineEdit {color : %1; background : %2}")
                                           .arg(gdrColours.at(1)).arg(a.at(1)));
            }else{
                reponseEdit->setStyleSheet(QString("QLineEdit {color : %1; background : %2}")
                                           .arg(gdrColours.at(2)).arg(a.at(1)));
            }
        }
    }
}


////////////////////////////////////
///   __                    __
///   ** LANGUAGES HANDLING **
///
////////////////////////////////////
/// translate text got from app to French for compatibility with database
/*static*/ QString CentralWidget::toFrench(const QString &text){
    QString ret;
    if(text == "noun" or text == "Nomen")
        ret = "nom";
    else if(text == "verb" or text == "Verb")
        ret =  "verbe";
    else if(text == "F.")
        ret = "f.";
    else if(text == "M.")
        ret = "m.";
    else if(text == "N.")
        ret = "n.";
    else
        ret = text;
    return ret;
}

/// give translated version of text retrieved from db
QString CentralWidget::trDB(const QString &text) const{
    if(text == "nom") return tr("Nomen");
    else if(text == "verbe") return tr("Verb");
    else if(text == "adjectif") return tr("Adjectiv");
    else if(text == "adverbe") return tr("Adverb");
    else if(text == "subordonnant") return tr("Subjunktion");
    else if(text == "pronom") return tr("Pronomen");
    else if(text == "préposition") return tr("Präposition");
    else if(text == "f.") return tr("F.");
    else if(text == "m.") return tr ("M.");
    else return tr("N.");
}

/// (re)set texts in the current language
void CentralWidget::retraduisUI(){
    questionLabel->setText(tr("Frage:"));
    reponseLabel->setText(tr("Antwort:"));
    if(isEditing){
        reponseButton->setText(tr("Hinzufügen"));
    }else{
        reponseButton->setText(tr("Antwort"));
    }
    fauxButton->setText(tr("Falsch"));
    suiteButton->setText(tr("Weiter"));
    reponseExemplesButton->setText(tr("Antwort"));
    catGramBox->clear();
    catGramBox->addItems(QStringList() << tr("Nomen") << tr("Verb") << tr("Adjectiv") <<
                              tr("Adverb") << tr("Subjunktion") << tr("Pronomen") << tr("Präposition"));
    genreQBox->clear();
    genreQBox->addItems(QStringList() << tr("F.") << tr("M.") << tr("N."));
    genreABox->clear();
    genreABox->addItems(QStringList() << tr("F.") << tr("M.") << tr("N."));
    verbeBox->clear();
    verbeBox->addItems(QStringList() << tr("reg.") << tr("unreg."));
    for(unsigned int i=0; i<nbrExemples; i++)
        exempleLabels[i]->setText(QString(tr("Beispiel %1:")).arg(i+1));
    if(isEditing){
        questionEdit->setPlaceholderText(tr("französisches Wort ; zweites Wort (max. 3)"));
        questionEdit->setFocus();
        exempleQEdits[0]->setPlaceholderText(tr("französisches Beispiel"));
        if(language == "Fr_de"){
            reponseEdit->setPlaceholderText(tr("deutsches Wort ; zweites Wort (max. 3)"));
            exempleAEdits[0]->setPlaceholderText(tr("deutsche Übersetzung"));
        }else if(language == "Fr_en"){
            reponseEdit->setPlaceholderText(tr("englisches Wort ; zweites Wort (max. 3)"));
            exempleAEdits[0]->setPlaceholderText(tr("englische Übersetzung"));
        }else{
            reponseEdit->setPlaceholderText(tr("albanisches Wort ; zweites Wort (max. 3)"));
            exempleAEdits[0]->setPlaceholderText(tr("albanische Übersetzung"));
        }
        verbeLayout->currentWidget()->hide();
        genreQLayout->currentWidget()->show();
        genreALayout->currentWidget()->show();
    }else{
        questionEdit->setPlaceholderText(tr("Bitte klicken Sie auf 'Weiter', um eine neue Frage anzuzeigen."));
        reponseEdit->setPlaceholderText("");
        exempleQEdits[0]->setPlaceholderText("");
        exempleAEdits[0]->setPlaceholderText("");
    }
}


////////////////////////
///   __           __
///   ** ACCESSORS **
///
////////////////////////
/// set parameters according to other dialog windows or when recovering user settings
void CentralWidget::setLife(int l){
    life = l;
}

void CentralWidget::setDust(int d){
    dust = d;
}

void CentralWidget::setFreq(int f){
    freq = f;
}

void CentralWidget::setHist(int h){
    hist = h;
}

void CentralWidget::setColours(const QStringList &quest, const QStringList &ans, const QStringList &gdr){
    questColours = quest;
    ansColours = ans;
    gdrColours = gdr;
    coloursChanged();
}

void CentralWidget::setUser(const QString &text){
    user = text;
}

void CentralWidget::setLanguage(const QString &text){
    language = text;
    // check if table for this user and this language already exists; if not, create it!
    QSqlQuery checkQuery;
    checkQuery.exec(QString("SHOW TABLES LIKE '%1'").arg(user+"_"+language));
    qDebug() << "setLanguage(), checkQuery:   " << checkQuery.executedQuery();
    checkQuery.first();
    if(checkQuery.size() == 0){
        qDebug() << "erreur ?";
        QSqlQuery createQuery;
        createQuery.exec(QString("CREATE TABLE %1 ("
                                 "id SMALLINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,"
                                 "word_id SMALLINT UNSIGNED NOT NULL,"
                                 "last_asked DATE,"
                                 "nbr_asked SMALLINT UNSIGNED NOT NULL DEFAULT 0,"
                                 "nbr_wrong SMALLINT UNSIGNED NOT NULL DEFAULT 0,"
                                 "last_wrong BOOLEAN NOT NULL DEFAULT FALSE,"
                                 "nbr_last_wrong SMALLINT UNSIGNED NOT NULL DEFAULT 0,"
                                 "nbr_last_correct SMALLINT UNSIGNED NOT NULL DEFAULT 0,"
                                 "CONSTRAINT fk_word_id_%1 "
                                    "FOREIGN KEY (word_id) "
                                    "REFERENCES %2(id) "
                                    "ON DELETE CASCADE"
                                 ")"
                                 "ENGINE=INNODB;")
                         .arg(user+"_"+language)
                         .arg(language));
        qDebug() << "setLanguage(), createQuery:   " << createQuery.executedQuery();
        qDebug() << createQuery.lastError().text();
        createQuery.exec(QString("ALTER TABLE %1 ADD UNIQUE index_word_id (word_id)")
                         .arg(user+"_"+language));
        QSqlQuery copyQuery;
        copyQuery.exec(QString("SELECT id FROM %1").arg(language));
        qDebug() << "setLanguage(), copyQuery:   " << copyQuery.executedQuery();
        QSqlQuery pasteQuery;
        while(copyQuery.next()){
            pasteQuery.prepare(QString("INSERT INTO %1 (word_id) VALUES (:word_id)")
                               .arg(user+"_"+language));
            pasteQuery.bindValue(":word_id",copyQuery.value(0).toInt());
            qDebug() << "setLanguage(), pasteQuery:   " << pasteQuery.executedQuery();
            pasteQuery.exec();
        }
    }
    qDebug() << "pas d'erreur !";
}

void CentralWidget::setDrill(bool b, int value){
    if(b) drillIDs.clear();
    isDrilling = b;
    drillSize = value;
}

void CentralWidget::updateFont(const QFont &police){
    policeEdit = police;
    questionEdit->setFont(policeEdit);
    reponseEdit->setFont(policeEdit);
    for(unsigned int i=0; i<nbrExemples; i++){
        exempleQEdits[i]->setFont(policeEdit);
        exempleAEdits[i]->setFont(policeEdit);
    }
}

void CentralWidget::drillReset(){
    drillIDs.clear();
}

void CentralWidget::swapChanged(bool b){
    isSwapping = b;
    qDebug() << isSwapping;
}

QVector<QDate> CentralWidget::getMinCreationDates(const QStringList& languages) const{
    QSqlQuery get;
    QVector<QDate> dates(3, QDate::currentDate());
    for(int i=0; i<languages.length(); i++){
        get.exec(QString("SELECT MIN(created) FROM %1").arg(languages.at(i)));
        qDebug() << "getMinCreationDates(), get:   " << get.executedQuery();
        get.first();
        QDate date = get.value(0).toDate();
        if(date.isValid()) dates[i] = date;
    }
    return dates;
}
