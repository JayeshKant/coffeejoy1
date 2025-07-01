#pragma once

#include <QWidget>
#include <QPixmap>

namespace Ui {
class Splashscreen;
}

class Splashscreen : public QWidget {
    Q_OBJECT

public:
    explicit Splashscreen(QWidget *parent = nullptr);
    ~Splashscreen() override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Splashscreen *ui;
    QPixmap m_background;
};
