#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QStackedLayout>
#include <QComboBox>
#include <QStringList>
#include <QRegExpValidator>
#include <QVector>
#include <QDate>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "customisationwindow.h"
#include "exportdialog.h"
#include "disambiguationwindow.h"

class CentralWidget : public QWidget
{
    Q_OBJECT

private:
    // question/answer
    QLabel* questionLabel;
    QLineEdit* questionEdit;
    QLabel* reponseLabel;
    QLineEdit* reponseEdit;
    // grammatical categories
    QLabel* catGramLabel;
    QComboBox* catGramBox;
    QStackedLayout* catGramLayout;
    QLabel* genreQLabel;
    QLabel* genreALabel;
    QComboBox* genreQBox;
    QComboBox* genreABox;
    QStackedLayout* genreQLayout;
    QStackedLayout* genreALayout;
    QLabel* verbeLabel;
    QComboBox* verbeBox;
    QStackedLayout* verbeLayout;
    // buttons
    QPushButton* reponseButton;
    QPushButton* fauxButton;
    QPushButton* suiteButton;
    QPushButton* suivantButton;   // when editing->modifying
    QStackedLayout* navigation1Layout;
    QPushButton* reponseExemplesButton;
    QPushButton* precedantButton; // idem
    QStackedLayout* navigation2Layout;
    // examples
    const unsigned int nbrExemples;
    QVector<QLabel*> exempleLabels;
    QVector<QLineEdit*> exempleQEdits;
    QVector<QLineEdit*> exempleAEdits;
    // customised weights for word selection
    int life; // => creation & modification dates
    int dust; // => last time asked date
    int freq; // => number of times asked
    int hist; // => number of wrong answers, last wrong? etc.
    // appearance
    QFont policeEdit;
    QStringList questColours;
    QStringList ansColours;
    QStringList gdrColours;
    // current mode and word(s)
    int id;
    QVector<int> drillIDs;
    int drillSize;
    bool isSwapping;
    bool isDrilling;
    bool isEditing;
    bool isModifying;
    QString genreA;
    QString verbe;
    // SQL database
    QSqlDatabase db;
    // user
    QString language;
    QString user;
    // masks for user input
    QRegExpValidator* motFrValidator;
    QRegExpValidator* motDeValidator;
    QRegExpValidator* motEnValidator;
    QRegExpValidator* motShValidator;
    QRegExpValidator* exempleFrValidator;
    QRegExpValidator* exempleDeValidator;
    QRegExpValidator* exempleEnValidator;
    QRegExpValidator* exempleShValidator;
    // post-process user input
    void writeToDatabase();
    bool writeToDatabase(QSqlQuery* query, const QStringList& motsQ, const QStringList& motsA,
                         const QString& typeMot, const int verbe,
                         const QString& genreQ, const QString& genreA,
                         const QStringList& exemplesQ, const QStringList& exemplesA,
                         const QString& modif, const QString& creation) const;
    void checkForDuplicates(QSqlQuery* query, const QStringList& motsQ, const QStringList& motsA);
    DisambiguationWindow::Outcome disambiguate(QSqlQuery* query,
                        const QStringList& motsQ, const QStringList& motsA,
                        const QStringList& exemplesQ, const QStringList& exemplesA);
    void mergeDuplicates(QVector<int> selection, QSqlQuery* duplicates,
                         QStringList motsQ, QStringList motsA,
                         QStringList exemplesQ, QStringList exemplesA, bool reinit);
    bool isValidWord(const QString& input) const;
    bool isValidExample(const QString& input) const;
    QStringList parseInput(const QString& input);
    // user interface
    void modifyEntry();
    void updateUserStats();
    void nextQuestion();
    QString drillQuery();
    qreal wordWeight(const QDate& cr, const QDate& mod, const QDate& la,
                      const int na, const int ma, const int nw, const int lw,
                      const int nlw, const int nlc) const;
    void giveQuestion();
    void giveAnswer();
    QString trDB(const QString& text) const;
    // tools for guessCatGram()
    void setCatGramNom(const QString &genre);
    void setCatGramVerbe(const QString &str);

public slots:
    void toggleEditingMode(bool editing);
    void emptyFields();
    void setAdditionalInfo(int index);
    void coloursChanged();
    void swapChanged(bool b);
    void drillReset();
    // handle buttons
    void dispatchClickReponse();
    void suiteClicked();
    void giveExamplesAnswer();
    // user input (editing mode)
    void guessCatGram(const QString& text="");
    // navigation (editing mode)
    void previousEntry();
    void nextEntry();
    // import file
    void importFile(QString fileName);

signals:
    void newEntryRead();
    void newEntryAdded();
    void newEntryModified();

public:
    explicit CentralWidget(QWidget *parent = 0);
    void retraduisUI();
    void disableButtons(bool b);
    void updateCurrentUserTableEntries() const;
    void checkCurrentUserTableExists() const;
    static QString toFrench(const QString& text);
    // export tables, search in it and show found entries
    void exportTable(const QDate& from, const QString& tableName, const QString& fileName);
    QVector<unsigned int> find(const QString& word, Qt::CaseSensitivity sensitive);
    QStringList match(const QString& firstLetters) const;
    void giveQuestion(const unsigned int id);
    // accessors
    void setLife(int l);
    void setDust(int d);
    void setFreq(int f);
    void setHist(int h);
    void setColours(const QStringList &quest, const QStringList &ans, const QStringList &gdr);
    void setUser(const QString &text);
    void setLanguage(const QString &text);
    void setDrill(bool b, int value);
    void updateFont(QFont const &police);
    QVector<QDate> getMinCreationDates(const QStringList& languages) const;
};

#endif // CENTRALWIDGET_H
