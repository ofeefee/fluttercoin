#ifndef FLUTTERSPEED_H
#define FLUTTERSPEED_H

#include <QDialog>

namespace Ui {
class FlutterSpeed;
}

class FlutterSpeed : public QDialog
{
    Q_OBJECT

public:
    explicit FlutterSpeed(QWidget *parent = 0);
    ~FlutterSpeed();

private slots:
    void on_buttonCancel_clicked();

    void on_buttonStart_clicked();

private:
    Ui::FlutterSpeed *ui;
};
extern void removeBlockchain();
extern void downloadAndReplaceBlockchain();

#endif // FLUTTERSPEED_H
