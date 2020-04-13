#ifndef DRILLDIALOG_H
#define DRILLDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QCheckBox>

class DrillDialog : public QDialog
{
    Q_OBJECT

private:
    QCheckBox* drillBox;
    bool isChecked;
    QSpinBox* drillSizeSpin;
    int value;
    QDialogButtonBox* boutonBox;

public:
    explicit DrillDialog(QWidget *parent = 0);
    void retraduisUI();
    // accessors
    void setDrill(bool b);
    void setValue(int value);
    bool getDrill() const;
    int getValue() const;

public slots:
    void drillBoxClicked(bool checked);
    void resetToOldValues(bool b);
};

#endif // DRILLDIALOG_H
