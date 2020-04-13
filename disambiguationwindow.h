#ifndef DISAMBIGUATIONWINDOW_H
#define DISAMBIGUATIONWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QVector>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>

class DisambiguationWindow : public QDialog
{
    Q_OBJECT

private:
    QSqlQuery* doublonQuery;
    QTableWidget* requetesTable;
    QDialogButtonBox* boutonBox;
    QCheckBox* resetBox;
    const QString language;
    void initialiseTable(const QStringList& currentEntry);

public:
    explicit DisambiguationWindow(QSqlQuery* query, const QString& language,
                                  const QStringList& currentEntry, QWidget *parent = 0);
    // accessors
    QVector<int> getSelection() const;
    bool getIsChecked() const;
    enum Outcome{
        Modified,
        Ignored,
        Cancelled
    };
};

#endif // DISAMBIGUATIONWINDOW_H
