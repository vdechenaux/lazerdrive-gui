#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLazerDriveClient>
#include "usernamedialog.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    QLazerDriveClient *m_pClient;
    UsernameDialog *m_pUsernameDialog;
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
private slots:
    void clientConnected(QLazerDrivePlayer player);
};

#endif // DIALOG_H
