#pragma once

#include <QtWidgets/QWidget>
#include "ui_xscreen.h"

class XScreen : public QWidget
{
    Q_OBJECT

public:
    XScreen(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent *e);
public slots://�ۺ���
	void Record();

private:
    Ui::XScreenClass ui;
};
