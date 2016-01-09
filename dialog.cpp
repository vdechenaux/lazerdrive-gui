#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    m_pUsernameDialog = new UsernameDialog(this);

    m_pClient = new QLazerDriveClient(this);
    connect(m_pClient, SIGNAL(connected(QLazerDrivePlayer)), this, SLOT(clientConnected(QLazerDrivePlayer)));
    connect(m_pClient, SIGNAL(nextColorReceived(uint,uint,uint)), m_pUsernameDialog, SLOT(setColor(uint, uint, uint)));

    connect(m_pUsernameDialog, SIGNAL(completed(QString)), m_pClient, SLOT(enterTheGame(QString)));
    connect(m_pUsernameDialog, SIGNAL(nextColor()), m_pClient, SLOT(nextColor()));

    m_pClient->connectToServer("one.eu.lazerdrive.io");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::clientConnected(QLazerDrivePlayer player)
{
    m_pUsernameDialog->setUsername(player.name());
    m_pUsernameDialog->setColor(player.r(), player.g(), player.b());
    m_pUsernameDialog->show();
}
