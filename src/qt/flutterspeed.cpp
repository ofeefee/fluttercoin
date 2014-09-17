#include "flutterspeed.h"
#include "ui_flutterspeed.h"

#include "init.h"

FlutterSpeed::FlutterSpeed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlutterSpeed)
{
    ui->setupUi(this);
}

FlutterSpeed::~FlutterSpeed()
{
    delete ui;
}

void FlutterSpeed::on_buttonCancel_clicked()
{
    close();
}



void FlutterSpeed::on_buttonStart_clicked()
{
    ShutdownandDeleteChain(NULL);
}

