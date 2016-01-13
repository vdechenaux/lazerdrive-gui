#include "replaydialog.h"
#include "ui_replaydialog.h"

ReplayDialog::ReplayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplayDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
}

ReplayDialog::~ReplayDialog()
{
    delete ui;
}

void ReplayDialog::setColor(uint r, uint g, uint b)
{
    QString style = "background-color: rgb("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+");";
    ui->pushButton_replay->setStyleSheet(style);
}

void ReplayDialog::on_pushButton_replay_clicked()
{
    emit reviveClicked();
    accept();
}
