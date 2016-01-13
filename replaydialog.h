#ifndef REPLAYDIALOG_H
#define REPLAYDIALOG_H

#include <QDialog>

namespace Ui {
class ReplayDialog;
}

class ReplayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplayDialog(QWidget *parent = 0);
    ~ReplayDialog();

private:
    Ui::ReplayDialog *ui;
public slots:
    void setColor(uint r, uint g, uint b);
private slots:
    void on_pushButton_replay_clicked();
signals:
    void reviveClicked();
};

#endif // REPLAYDIALOG_H
