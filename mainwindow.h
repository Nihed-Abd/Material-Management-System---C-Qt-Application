#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include "materiel.h"
#include "notification.h"
#include <QSystemTrayIcon>
#include <QMenu>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouterMatBox_2_clicked();
    void on_modifMatBox_clicked();
    void on_deleteBox_clicked();
    void on_tableView_activated(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);
    void on_ActualiserMatBox_clicked();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_recherche_textChanged(const QString &arg1);
    void on_TrierBox_activated(const QString &arg1);
    void on_imprimerBox_clicked();

    void on_tableNotification_activated(const QModelIndex &index);

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::MainWindow *ui;
    materiel m;
    notification n;
    QSystemTrayIcon *trayIcon;


    bool containsOnlyLetters(const QString &str); // Declaration of the function

    // Additional LCD widget declarations
    QLCDNumber *lcdNumber;
    QLCDNumber *LCDMAINT;
    QLCDNumber *LCDAGE;

    // Function to update LCD widgets
    void updateLCDs();
    void updateChart();
};

#endif // MAINWINDOW_H
