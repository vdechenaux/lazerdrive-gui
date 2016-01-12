#ifndef USERNAMEDIALOG_H
#define USERNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class UsernameDialog;
}

class UsernameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UsernameDialog(QWidget *parent = 0);
    ~UsernameDialog();
    void setUsername(QString username);

private slots:
    void on_pushButton_play_clicked();
    void on_pushButton_color_clicked();
    void on_checkBox_cheat_reversed_toggled(bool checked);

public slots:
    void setColor(uint r, uint g, uint b);
private:
    Ui::UsernameDialog *ui;
signals:
    void completed(QString username);
    void nextColor();
    void toogleReversedCheat(bool imune);
};

#endif // USERNAMEDIALOG_H
