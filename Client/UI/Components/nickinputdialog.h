#ifndef NICKINPUTDIALOG_H
#define NICKINPUTDIALOG_H

#include <QDialog>
#include <QIntValidator>
#include <QVBoxLayout>
#include <QDialogButtonBox>

#include "UI/Components/customlineedit.h"
#include "UI/Components/customlabel.h"

class NickInputDialog : public QDialog {
    Q_OBJECT

    // Widgets
    CustomLineEdit *input;
    QVBoxLayout *vbox;
    QDialogButtonBox * buttonBox;

    // Atributes
    QString nickname;
    bool accepted;

public:
    explicit NickInputDialog(QString text, QWidget *parent = nullptr);

    bool isAccepted();
    QString getText();

    static QString getNickname(QString text = "", QWidget *parent = nullptr);

public slots:
    void onOkClicked();
    void onCloseClicked();
};

#endif // NICKINPUTDIALOG_H
