#ifndef CUSTOMISATIONWINDOW_H
#define CUSTOMISATIONWINDOW_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QTabBar>
#include <QPaintEvent>
#include <QStringList>
#include "questionstabwidget.h"
#include "fontcolorstabwidget.h"

class TabBar : public QTabBar
{
protected:
    QSize tabSizeHint(int) const;
    void paintEvent(QPaintEvent*);

public:
    explicit TabBar(QWidget *parent=0);
};

class TabWidget : public QTabWidget
{
public:
    explicit TabWidget(QWidget *parent=0);
};

class CustomisationWindow : public QDialog
{
    Q_OBJECT

private:
    QuestionsTabWidget* questionsTab;
    FontColorsTabWidget* fontColoursTab;
    TabWidget* tabWidget;
    QDialogButtonBox* boutonBox;

public:
    explicit CustomisationWindow(QWidget *parent = 0);
    void retraduisUI();
    // setter & unsetter
    void resetToOldValues(bool b);
    // accessors: set
    void setLife(int value);
    void setDust(int value);
    void setFreq(int value);
    void setHist(int value);
    void setCurrentFont(const QString &fontName);
    void setFontSize(const int &value);
    void setBold(const bool &b);
    void setItalic(const bool &b);
    void setCurrentLanguages(const QStringList &languages);
    void setLanguageColours(const QStringList &front, const QStringList &back);
    void setGenderColours(const QStringList &colourNames);
    void setGenders(const bool &b);
    // accessors: get
    int getLife() const;
    int getDust() const;
    int getFreq() const;
    int getHist() const;
    const QString& getCurrentFont() const;
    const int& getFontSize() const;
    const bool& getBold() const;
    const bool& getItalic() const;
    const QStringList& getLanguageColoursFront() const;
    const QStringList& getLanguageColoursBack() const;
    QStringList getLanguageColours(const QString &language) const;
    const QStringList& getGenderColours() const;
    const bool& getGenders() const;

signals:

public slots:
};

#endif // CUSTOMISATIONWINDOW_H
