#include "nicknameinputdialog.h"

NicknameInputDialog::NicknameInputDialog(QString text, QWidget *parent)
    : QDialog{parent}, nickname(text), accepted(false) {
    // Window proprieties
    this->setFixedSize(400, 175);
    this->setStyleSheet("QDialog {"
                            "color: qlineargradient(spread:pad, x1:0 y1:1, x2:1 y2:0,"
                            "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"
                            "background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 #7d20bd, stop:1 #8165f0);"
                        "}" );
    this->setWindowTitle("Nickname");

    // Box Layout
    vbox = new QVBoxLayout;
    vbox->setGeometry(this->geometry());

    // Text input
    input = new CustomLineEdit(this);

    input->setPlaceholderText("Nickname");

    if (!nickname.isEmpty()) {
        input->setText(nickname);
    }

    int inputWidth = this->width() - 10, inputHeight = this->height() * 0.08;
    int inputX = 10, inputY = 10;
    input->setGeometry(inputX, inputY, inputWidth, inputHeight);
    vbox->addWidget(input);

    // Accepts only Alphanumeric and underscore. Max of 12 chars
    QRegularExpression validNickname("^[a-zA-Z0-9_]*$");
    QValidator *validator= new QRegularExpressionValidator(validNickname, this);
    input->setValidator(validator);
    input->setMaxLength(12);

    // ButtonBox
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);
    buttonBox->setStyleSheet("* {"
                               "background-color: white; "
                               "border-radius: 15px; "
                               "border: 0px;"
                               "font-weight: bold;"
                               "color: #110C5A;"
                           "}");
    vbox->addWidget(buttonBox);
    this->setLayout(vbox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &NicknameInputDialog::onOkClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &NicknameInputDialog::onCloseClicked);
}

bool NicknameInputDialog::isAccepted() { return accepted; }

QString NicknameInputDialog::getText() { return nickname; }

QString NicknameInputDialog::getNickname(QString text, QWidget *parent) {
    NicknameInputDialog nicknameDialog(text, parent);
    nicknameDialog.exec();

    if (nicknameDialog.isAccepted()) {
        return nicknameDialog.getText();
    }

    return "*CLOSED"; // The input doesn't accept especial characters, so I used * to handle the close button
}

void NicknameInputDialog::onOkClicked() {
    nickname = input->text();
    accepted = true;
    this->close();
}

void NicknameInputDialog::onCloseClicked() { this->close(); }

