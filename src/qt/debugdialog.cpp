#include "debugdialog.h"
#include "ui_debugdialog.h"

#include "util.h"
#include "tail.h"

#include <boost/filesystem.hpp>

#include <QFile>
#include <QString>

debugDialog::debugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::debugDialog)
{
    ui->setupUi(this);
    startTail();
}

debugDialog::~debugDialog()
{
    delete ui;
}

void debugDialog::on_buttonBox_clicked()
{
    worker->stopProcess();
    close();
}

void debugDialog::startTail()
{
    boost::filesystem::path debugFile = GetDataDir() / "debug.log";
    QString filename = QString::fromStdString(debugFile.string());

    worker = new Tail(filename,this);
    if (worker != NULL)
    {
        connect(worker,SIGNAL(sendLine(QString)),this,SLOT(recieveLine(QString)),Qt::QueuedConnection);
        worker->start();
    }

}

void debugDialog::recieveLine(QString line)
{
    ui->textBrowser->insertPlainText(line);
}
