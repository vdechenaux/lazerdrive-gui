#include "usernamedialog.h"
#include "ui_usernamedialog.h"

UsernameDialog::UsernameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsernameDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
}

UsernameDialog::~UsernameDialog()
{
    delete ui;
}

void UsernameDialog::setUsername(QString username)
{
    ui->lineEdit_username->setText(username);
}

void UsernameDialog::on_pushButton_play_clicked()
{
    emit completed(ui->lineEdit_username->text());
    accept();
}

void UsernameDialog::on_pushButton_color_clicked()
{
    emit nextColor();
}

void UsernameDialog::setColor(uint r, uint g, uint b)
{
    QString style = "background-color: rgb("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+");";
    ui->pushButton_play->setStyleSheet(style);
    ui->pushButton_color->setStyleSheet(style);
}
