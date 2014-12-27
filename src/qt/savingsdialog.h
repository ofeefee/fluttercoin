#ifndef SAVINGSDIALOG_H
#define SAVINGSDIALOG_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

namespace Ui {
    class AutoSavingsDialog;
}
class WalletModel;

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class AutoSavingsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AutoSavingsDialog(QWidget *parent = 0);
    ~AutoSavingsDialog();

    void setModel(WalletModel *model);
    void setAddress(const QString &address);
    void setAddress(const QString &address, QLineEdit *addrEdit);

private slots:
    void on_enableButton_clicked();
    void on_disableButton_clicked();
    void on_addressBookButton_clicked();
    void on_changeAddressBookButton_clicked();
    void on_pushButton_copy_clicked();
    void on_freeAddressBookButton_clicked();
    void on_freeRequestButton_clicked();
    void on_freeDisableButton_clicked();
    void replyFinished(QNetworkReply *reply);
    void freeDoHttpPost();

private:
    Ui::AutoSavingsDialog *ui;
    WalletModel *model;
    QNetworkAccessManager *nam;
    QTimer *freeTimer;
};

#endif // SAVINGSDIALOG_H
