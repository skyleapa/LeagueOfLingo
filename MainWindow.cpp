#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      inputBox(new QLineEdit(this)),
      selectButton(new QPushButton("Search", this)),
      termList(new QListWidget(this)),
      responseDisplay(new QTextEdit(this)),
      riotApi(new RiotApi("")),
      leagueDictionary(new LeagueDictionary()) {

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setStyleSheet("background-color: rgb(195, 177, 225);");

    auto *titleLabel = new QLabel("Welcome to the League Of Lingo!", this);
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QLabel* termListLabel = new QLabel("Available League Terms:");
    mainLayout->addWidget(termListLabel);

    // removing the extra space under a qlistwidget
    termList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    termList->setStyleSheet("background-color: rgb(230, 230, 250);");
    mainLayout->addWidget(termList);

    for (const auto& term : leagueDictionary->getAllTerms()) {
        termList->addItem(QString::fromStdString(term));
    }

    QHBoxLayout* inputLayout = new QHBoxLayout;
    inputBox->setStyleSheet("background-color: rgb(230, 230, 250);");
    selectButton->setStyleSheet("background-color: rgb(230, 230, 250);");
    inputLayout->addWidget(inputBox);
    inputLayout->addWidget(selectButton);

    mainLayout->addLayout(inputLayout);

    responseDisplay->setReadOnly(true);
    responseDisplay->setStyleSheet("background-color: rgb(230, 230, 250);");
    mainLayout->addWidget(responseDisplay);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(selectButton, &QPushButton::clicked, this, &MainWindow::onSelectClicked);
}

MainWindow::~MainWindow() {
    delete riotApi;
    delete leagueDictionary;
}

void MainWindow::onSelectClicked() {
    QString keyword = inputBox->text().trimmed();
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please search for a non-empty league term.");
        return;
    }

    std::string result = leagueDictionary->lookup(keyword.toStdString(), *riotApi, puuid, gameName, tagLine);
    responseDisplay->setText(QString::fromStdString(result));
}
