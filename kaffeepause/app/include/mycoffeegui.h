#pragma once
#include <QMainWindow>

namespace Ui {
class splashRoot;
}

class MyCoffeeGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyCoffeeGUI(QWidget *parent = nullptr);
    ~MyCoffeeGUI();

private slots:
    void on_pushButton_clicked();

private:
    Ui::splashRoot *ui;
};
