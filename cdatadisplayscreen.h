#ifndef CDATADISPLAYSCREEN_H
#define CDATADISPLAYSCREEN_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class CDataDisplayScreen;
}

class CDataDisplayScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CDataDisplayScreen(QWidget *parent = nullptr);
    ~CDataDisplayScreen();

    CDataDisplayScreen* GetInstance();

private:
    void InitView();

    void InitData();

    void InitConnect();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::CDataDisplayScreen *ui;
};

#endif // CDATADISPLAYSCREEN_H
