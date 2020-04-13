#ifndef FONTCOLORSTABWIDGET_H
#define FONTCOLORSTABWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QCheckBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QStringList>
#include <QMouseEvent>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QPaintEvent>

/*class ColourChoicePixmap : public QLabel
{
    Q_OBJECT

private:
    QPen pen;
    QBrush brush;
    QPixmap carreCouleurPixmap;
    bool clicked;
    QString title;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

public:
    explicit ColourChoicePixmap(const QString &colorDialogTitle="", QWidget *parent=0);
    void setTitle(const QString &colorDialogTitle);
    void setColour(const QString &colourName);
    QString getColour() const;

signals:
    void colourChanged(QString colourName);
};*/

class ColourChoicePixmap : public QWidget
{
    Q_OBJECT

private:
    QPen pen;
    QBrush brush;
    const QRect rect;
    bool clicked;
    QString title;

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

public:
    explicit ColourChoicePixmap(const QString &colourDialogTitle="", QWidget *parent=0);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void setTitle(const QString &colourDialogTitle);
    void setColour(const QString &colourName);
    QString getColour() const;

signals:
    void colourChanged(QString colourName);
};

class FontColorsTabWidget : public QWidget
{
    Q_OBJECT

private:
    /// font family
    QGroupBox* policeBox;
    QFontComboBox* policeComboBox;
    QComboBox* tailleComboBox;
    QCheckBox* grasCheckBox;
    QCheckBox* italiqueCheckBox;
    QString police;
    int taille;
    bool gras;
    bool italique;
    /// language colours
    QGroupBox* languesBox;
    QListWidget* languesTable;
    QListWidgetItem* frItem;
    QListWidgetItem* deItem;
    QListWidgetItem* enItem;
    QListWidgetItem* shItem;
    QLabel* ppLabel;
    QLabel* apLabel;
    ColourChoicePixmap* ppFrame;
    ColourChoicePixmap* apFrame;
    QStringList ppColours;
    QStringList apColours;
    QStringList languages;
    void createLanguesTable();
    /// gender colours
    QGroupBox* genresBox;
    QLabel* masculinLabel;
    QLabel* femininLabel;
    QLabel* neutreLabel;
    ColourChoicePixmap* masculinFrame;
    ColourChoicePixmap* femininFrame;
    ColourChoicePixmap* neutreFrame;
    QStringList gdrColours;
    bool genres;

public:
    explicit FontColorsTabWidget(QWidget *parent = 0);
    void retraduisUI();
    // setter & unsetter
    void resetToOldValues(bool b);
    // accessors
    void setCurrentFont(const QString &fontName);
    void setFontSize(const int &value);
    void setBold(const bool &b);
    void setItalic(const bool &b);
    void setCurrentLanguages(const QStringList &languages);
    void setLanguageColours(const QStringList &front, const QStringList &back);
    void setGenderColours(const QStringList &colourNames);
    void setGenders(const bool &b);
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
    void currentLanguageChanged(int row);
    void foregroundPixTriggered(QString colourName);
    void backgroundPixTriggered(QString colourName);
};

#endif // FONTCOLORSTABWIDGET_H
