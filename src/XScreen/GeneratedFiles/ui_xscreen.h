/********************************************************************************
** Form generated from reading UI file 'xscreen.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XSCREEN_H
#define UI_XSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XScreenClass
{
public:
    QWidget *back;
    QPushButton *minButton;
    QPushButton *closeButton;
    QWidget *all;
    QPushButton *recordButton;
    QLineEdit *urlEdit;
    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
    QLabel *timelabel;
    QLabel *infolabel;
    QLabel *infolabel_2;
    QLineEdit *fpsEdit;
    QLabel *title;
    QLabel *logo;

    void setupUi(QWidget *XScreenClass)
    {
        if (XScreenClass->objectName().isEmpty())
            XScreenClass->setObjectName(QStringLiteral("XScreenClass"));
        XScreenClass->resize(674, 350);
        QIcon icon;
        icon.addFile(QStringLiteral(":/XScreen/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        XScreenClass->setWindowIcon(icon);
        XScreenClass->setStyleSheet(QString::fromUtf8("#back\n"
"{\n"
"background-color:rgb(53,53,53);\n"
"border-radius:15px;\n"
"}\n"
"#all\n"
"{\n"
"background-color:rgb(35,35,35);\n"
"}\n"
"#title\n"
"{\n"
"color:rgb(0, 170, 255);\n"
"font:75 15pt \"\351\273\221\344\275\223\";\n"
"}\n"
"#timelabel\n"
"{\n"
"color:rgb(0, 170, 255);\n"
"font:75 20pt \"\351\273\221\344\275\223\";\n"
"}"));
        back = new QWidget(XScreenClass);
        back->setObjectName(QStringLiteral("back"));
        back->setGeometry(QRect(0, 0, 800, 1000));
        minButton = new QPushButton(back);
        minButton->setObjectName(QStringLiteral("minButton"));
        minButton->setGeometry(QRect(610, 0, 31, 31));
        minButton->setStyleSheet(QStringLiteral("background-image: url(:/XScreen/mini.png);"));
        minButton->setFlat(true);
        closeButton = new QPushButton(back);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        closeButton->setGeometry(QRect(640, 0, 31, 23));
        closeButton->setStyleSheet(QStringLiteral("background-image: url(:/XScreen/close.png);"));
        closeButton->setFlat(true);
        all = new QWidget(back);
        all->setObjectName(QStringLiteral("all"));
        all->setGeometry(QRect(0, 40, 671, 311));
        recordButton = new QPushButton(all);
        recordButton->setObjectName(QStringLiteral("recordButton"));
        recordButton->setGeometry(QRect(550, 30, 101, 91));
        recordButton->setStyleSheet(QLatin1String("QPushButton:!hover\n"
"{\n"
"	background-image: url(:/XScreen/record_normal.png);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"	background-image: url(:/XScreen/record_hot.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"	background-image: url(:/XScreen/record_pressed.png);\n"
"	background-color: rgba(255, 255, 255, 0);\n"
"}"));
        recordButton->setFlat(true);
        urlEdit = new QLineEdit(all);
        urlEdit->setObjectName(QStringLiteral("urlEdit"));
        urlEdit->setGeometry(QRect(50, 50, 481, 61));
        widthEdit = new QLineEdit(all);
        widthEdit->setObjectName(QStringLiteral("widthEdit"));
        widthEdit->setGeometry(QRect(50, 210, 71, 41));
        heightEdit = new QLineEdit(all);
        heightEdit->setObjectName(QStringLiteral("heightEdit"));
        heightEdit->setGeometry(QRect(130, 210, 71, 41));
        timelabel = new QLabel(all);
        timelabel->setObjectName(QStringLiteral("timelabel"));
        timelabel->setGeometry(QRect(460, 140, 91, 20));
        infolabel = new QLabel(all);
        infolabel->setObjectName(QStringLiteral("infolabel"));
        infolabel->setGeometry(QRect(400, 249, 81, 31));
        infolabel->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        infolabel_2 = new QLabel(all);
        infolabel_2->setObjectName(QStringLiteral("infolabel_2"));
        infolabel_2->setGeometry(QRect(240, 210, 31, 31));
        infolabel_2->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        fpsEdit = new QLineEdit(all);
        fpsEdit->setObjectName(QStringLiteral("fpsEdit"));
        fpsEdit->setGeometry(QRect(270, 210, 71, 41));
        title = new QLabel(back);
        title->setObjectName(QStringLiteral("title"));
        title->setGeometry(QRect(50, 10, 261, 31));
        logo = new QLabel(back);
        logo->setObjectName(QStringLiteral("logo"));
        logo->setGeometry(QRect(10, 10, 31, 31));
        logo->setStyleSheet(QStringLiteral("background-image: url(:/XScreen/logo.png);"));

        retranslateUi(XScreenClass);
        QObject::connect(closeButton, SIGNAL(clicked()), XScreenClass, SLOT(close()));
        QObject::connect(minButton, SIGNAL(clicked()), XScreenClass, SLOT(showMinimized()));
        QObject::connect(recordButton, SIGNAL(clicked()), XScreenClass, SLOT(Record()));

        QMetaObject::connectSlotsByName(XScreenClass);
    } // setupUi

    void retranslateUi(QWidget *XScreenClass)
    {
        XScreenClass->setWindowTitle(QApplication::translate("XScreenClass", "XScreen", Q_NULLPTR));
        minButton->setText(QString());
        closeButton->setText(QString());
        recordButton->setText(QString());
        urlEdit->setText(QApplication::translate("XScreenClass", "D:\\video", Q_NULLPTR));
        widthEdit->setText(QApplication::translate("XScreenClass", "1280", Q_NULLPTR));
        heightEdit->setText(QApplication::translate("XScreenClass", "720", Q_NULLPTR));
        timelabel->setText(QApplication::translate("XScreenClass", "000:00", Q_NULLPTR));
        infolabel->setText(QApplication::translate("XScreenClass", "\347\273\264\346\212\244\357\274\232\345\244\217\344\275\263\346\235\203", Q_NULLPTR));
        infolabel_2->setText(QApplication::translate("XScreenClass", "\345\270\247\347\216\207", Q_NULLPTR));
        fpsEdit->setText(QApplication::translate("XScreenClass", "10", Q_NULLPTR));
        title->setText(QApplication::translate("XScreenClass", "XScreen \345\261\217\345\271\225\345\275\225\345\203\217\346\234\272 v1.0", Q_NULLPTR));
        logo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class XScreenClass: public Ui_XScreenClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XSCREEN_H
