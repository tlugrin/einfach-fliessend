#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDateEdit>
#include <QVector>
#include <QDate>
#include <QDialogButtonBox>
#include <QPushButton>

class ChooseFileLineEdit : public QLineEdit
{
    Q_OBJECT

private:
    bool isClicked;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

public:
    explicit ChooseFileLineEdit(QWidget* parent = 0);

signals:
    void clicked();
};

class ExportDialog : public QDialog
{
    Q_OBJECT

private:
    QLabel* fichierLabel;
    ChooseFileLineEdit* fichierEdit;
    QLabel* languesLabel;
    QButtonGroup* radioGroup;
    QRadioButton* deButton;
    QRadioButton* enButton;
    QRadioButton* shButton;
    QLabel* fromLabel;
    QDateEdit* fromDate;
    QVector<QDate> minDates;
    QDialogButtonBox* boutonBox;
    QPushButton* exporterButton;

public:
    explicit ExportDialog(QWidget *parent = 0);
    void retraduisUI();
    // accessors
    void setAvailableLanguages(const QStringList& languages);
    void setMinDates(const QVector<QDate>& dates);
    QString getFileName() const;
    QString getTableName() const;
    QDate getFromDate() const;

public slots:
    void selectedLanguageChanged(int id);// reset data range of [fromDate]
    void lineEditClicked(); // choose file to save to
};

#endif // EXPORTDIALOG_H
