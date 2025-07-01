#include "splashscreen.h"
#include "ui_splashscreen.h"

#include <QPainter>
#include <QGuiApplication>
#include <QScreen>

Splashscreen::Splashscreen(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Splashscreen),
    m_background(":/SplashScreen.png")   // load your resource
{
    ui->setupUi(this);

    // 1) Window flags for a proper splash look
    setWindowFlags(windowFlags()
                   | Qt::FramelessWindowHint
                   | Qt::SplashScreen);

    // 2) Force a fixed size of 800×680
    const QSize targetSize(800, 680);
    setFixedSize(targetSize);

    // 3) Center on primary screen (optional)
    QRect screenGeom = QGuiApplication::primaryScreen()->geometry();
    move(screenGeom.center() - rect().center());
}

Splashscreen::~Splashscreen()
{
    delete ui;
}

void Splashscreen::paintEvent(QPaintEvent * /*event*/)
{
    QPainter p(this);
    // Draw the pixmap scaled to exactly fill our 800×680 widget
    p.drawPixmap(rect(), m_background);
}
