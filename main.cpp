#include <QApplication>
#include <QTimer>
#include <QRandomGenerator>
#include <Windows.h>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>
#include <QtTest/QTest>
#include <QDialog>
#include <QtDebug>

class Dialog : public QDialog
{

public:
    Dialog(QWidget *parent = nullptr) :
        QDialog(parent)
    {
        setWindowFlags(Qt::Dialog| Qt::Tool | Qt::FramelessWindowHint);
        auto sn = qApp->desktop()->screenNumber(this);
        auto g = qApp->desktop()->screenGeometry(sn);
        QScreen *screen = QGuiApplication::primaryScreen();
        resize(g.size());
        QTimer *timer = new QTimer(this);
        timer->setTimerType(Qt::VeryCoarseTimer);
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, [this, timer,screen]() {
            auto c = cursor();
            auto pos = c.pos();
            static bool negative = false;
            static int counter = 0;
            pos.setX(pos.x() + (negative ? -50 : 50));
            c.setPos(pos);
            c.setShape(negative ? Qt::WaitCursor : Qt::ArrowCursor);
            setCursor(c);
            negative = !negative;
            auto timeout = QRandomGenerator::system()->bounded(5);
            hide();
            shot = screen->grabWindow(0);
            show();
            repaint();
          QTest::mouseClick(this, Qt::LeftButton, Qt::NoModifier, pos);
            qDebug() << "I'm working" << counter++;
            timer->start(timeout * 1000);
        });
        timer->start(1000);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.drawPixmap(geometry(), shot);
        p.end();
    }

private:
    QPixmap shot;
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
