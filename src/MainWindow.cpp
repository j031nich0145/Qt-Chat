#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Supa LLM Client");
    resize(900, 650);

    auto *central = new QWidget(this);
    auto *layout  = new QVBoxLayout(central);

    m_chatDisplay = new QTextEdit(this);
    m_chatDisplay->setReadOnly(true);
    m_chatDisplay->setPlaceholderText("Conversation will appear here...");

    auto *inputRow = new QHBoxLayout();
    m_inputField   = new QLineEdit(this);
    m_inputField->setPlaceholderText("Type a message...");
    m_sendButton   = new QPushButton("Send", this);

    inputRow->addWidget(m_inputField);
    inputRow->addWidget(m_sendButton);

    layout->addWidget(m_chatDisplay);
    layout->addLayout(inputRow);

    setCentralWidget(central);
}