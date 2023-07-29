#ifndef CDATADISPLAYSCREEN_H
#define CDATADISPLAYSCREEN_H

#include <QWidget>

namespace Ui {
class CDataDisplayScreen;
}

class CDataDisplayScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CDataDisplayScreen(QWidget *parent = nullptr);
    ~CDataDisplayScreen();


private:
    void InitView();

    void InitData();

    void InitConnect();

private:
    Ui::CDataDisplayScreen *ui;
};

#endif // CDATADISPLAYSCREEN_H
