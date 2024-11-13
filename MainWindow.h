#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include "LeagueDictionary.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    private slots:
        void onSelectClicked();

private:
    QLineEdit* inputBox;
    QPushButton* selectButton;
    QListWidget* termList;
    QTextEdit* responseDisplay;

    RiotApi *riotApi;
    LeagueDictionary *leagueDictionary;

    const std::string puuid = "lpm4glcFwzpMNPDlAo7EPjtzAn84v5HlGxScVciLiq0GO8959Y4ZWEl42WcWF0NDg-78Gj5jCJYTKA";
    const std::string gameName = "skyleapa";
    const std::string tagLine = "NA1";
};

#endif // MAINWINDOW_H
