#ifndef QUESTIONSTABWIDGET_H
#define QUESTIONSTABWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QVector>
#include <QLabel>

class QuestionsTabWidget : public QWidget
{
    Q_OBJECT

private:
    QVector<QSlider*> persoSlider;
    QVector<QLabel*> persoLabel;
    QVector<int> values;
    QLabel* explicationLabel;
    QLabel* lifeLabel;
    QLabel* dustLabel;
    QLabel* freqLabel;
    QLabel* histLabel;

public:
    explicit QuestionsTabWidget(QWidget *parent = 0);
    void retraduisUI();
    // setter & unsetter
    void resetToOldValues(bool b);
    // accessors
    int getLife() const;
    int getDust() const;
    int getFreq() const;
    int getHist() const;
    void setLife(int value);
    void setDust(int value);
    void setFreq(int value);
    void setHist(int value);

signals:

public slots:
    void lifeValueChanged(int value);
    void dustValueChanged(int value);
    void freqValueChanged(int value);
    void histValueChanged(int value);
};

#endif // QUESTIONSTABWIDGET_H
