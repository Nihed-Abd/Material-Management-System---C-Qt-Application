#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "materiel.h"
#include "notification.h"
#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QRegExp>
#include <QDateTime>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChart>
#include <QSystemTrayIcon>
#include <QMenu>





MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->tableView->setModel(m.afficher());
    ui->tableNotification->setModel(n.afficher());
    QPixmap pixmap(":/../../../notif.png");  // Load the image from the resource file
    QIcon icon(pixmap.scaled(64, 64));  // Scale the pixmap to the desired size (e.g., 64x64)
    trayIcon = new QSystemTrayIcon(icon, this);
    trayIcon->setVisible(true);

    QTimer *timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), this, SLOT(midnightReset()));
      QDateTime currentTime = QDateTime::currentDateTime();
          QDateTime midnight = QDateTime(currentTime.date(), QTime(0, 0));
          if (midnight < currentTime) {
              midnight = midnight.addDays(1); // Set to next midnight if current time is past midnight
          }

          int msecUntilMidnight = currentTime.msecsTo(midnight);
          timer->start(msecUntilMidnight);
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::containsOnlyLetters(const QString &str) {
    QRegExp regex("^[a-zA-Z]+$");
    return regex.exactMatch(str);
}

void MainWindow::on_ajouterMatBox_2_clicked()
{
    int id = ui->idBox_2->text().toInt();
    QString nom = ui->nomBoxMat_2->text();
    QString type = ui->typeBox_2->text();
    QString matricule = ui->matriculeBox_2->text();
    QString constructeur = ui->constructeurBox_2->text();
    QString date_fabrication = ui->dateEdit->text();
    QString date_der = ui->dateMaintenanceBox_2->text();
    QString etat = ui->etatBox_2->text();

    // Input validation
    if(matricule.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Matricule doit contenir exactement 8 caractères.");
        return;
    }

    if(!containsOnlyLetters(nom)) {
        QMessageBox::warning(this, "Erreur", "Nom doit contenir uniquement des lettres.");
        return;
    }

    if(!containsOnlyLetters(type)) {
        QMessageBox::warning(this, "Erreur", "Type doit contenir uniquement des lettres.");
        return;
    }

    QDate current_date = QDate::currentDate();
    QDate fabric_date = QDate::fromString(date_fabrication, "yyyy-MM-dd");
    QDate maint_date = QDate::fromString(date_der, "yyyy-MM-dd");

    if(fabric_date >= current_date) {
        QMessageBox::warning(this, "Erreur", "Date de fabrication doit être antérieure à la date actuelle.");
        return;
    }

    if(maint_date >= current_date) {
        QMessageBox::warning(this, "Erreur", "Date de dernière maintenance doit être antérieure à la date actuelle.");
        return;
    }

    if(!containsOnlyLetters(etat)) {
        QMessageBox::warning(this, "Erreur", "Etat doit contenir uniquement des lettres.");
        return;
    }

    materiel m(id, nom, type, matricule, constructeur, date_fabrication, date_der, etat);
    bool test = m.ajouter();

    if (test) {
        QString notif = "New Materiel With ID =" + QString::number(id) + " added";
               notification n;
               n.setDate(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
               n.setNotif(notif);
               bool notificationAdded = n.ajouter();
               if (notificationAdded) {
                         QMessageBox::information(this, "RDI Center", "Ajout effectué avec notification.");
                     } else {
                         QMessageBox::critical(this, "RDI Center", "Ajout effectué mais erreur dans l'ajout de la notification.");
                     }
        ui->tableView->setModel(m.afficher());
        ui->tableNotification->setModel(n.afficher());
        QMessageBox::information(this, "RDI Center", "Ajout effectué.");
        // Clearing input fields
        ui->idBox_2->clear();
        ui->nomBoxMat_2->clear();
        ui->typeBox_2->clear();
        ui->matriculeBox_2->clear();
        ui->constructeurBox_2->clear();
        ui->dateEdit->clear();
        ui->dateMaintenanceBox_2->clear();
        ui->etatBox_2->clear();
    } else {
        QMessageBox::critical(this, "RDI Center", "Ajout non effectué.");
    }
    updateLCDs();

}


void MainWindow::on_modifMatBox_clicked()
{
    int id = ui->idBox_4->text().toInt();
    QString nom = ui->nomBoxMat_4->text();
    QString type = ui->typeBox_4->text();
    QString matricule = ui->matriculeBox_4->text();
    QString constructeur = ui->constructeurBox_4->text();
    QString date_fabrication = ui->dateEdit_4->date().toString("yyyy-MM-dd");
    QString date_der = ui->dateMaintenanceBox_4->date().toString("yyyy-MM-dd");
    QString etat = ui->etatBox_4->text();

    // Input validation
    if(matricule.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Matricule doit contenir exactement 8 caractères.");
        return;
    }

    if(!containsOnlyLetters(nom)) {
        QMessageBox::warning(this, "Erreur", "Nom doit contenir uniquement des lettres.");
        return;
    }

    if(!containsOnlyLetters(type)) {
        QMessageBox::warning(this, "Erreur", "Type doit contenir uniquement des lettres.");
        return;
    }

    QDate current_date = QDate::currentDate();
    QDate fabric_date = ui->dateEdit_4->date();
    QDate maint_date = ui->dateMaintenanceBox_4->date();

    if(fabric_date >= current_date) {
        QMessageBox::warning(this, "Erreur", "Date de fabrication doit être antérieure à la date actuelle.");
        return;
    }

    if(maint_date >= current_date) {
        QMessageBox::warning(this, "Erreur", "Date de dernière maintenance doit être antérieure à la date actuelle.");
        return;
    }

    if(!containsOnlyLetters(etat)) {
        QMessageBox::warning(this, "Erreur", "Etat doit contenir uniquement des lettres.");
        return;
    }

    materiel m(id, nom, type, matricule, constructeur, date_fabrication, date_der, etat);
    bool check = m.modifier();

    if (check) {
        // Add notification after modifying material
        QString notif = "Materiel with ID = " + QString::number(id) + " modifié";
        notification n;
        n.setDate(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        n.setNotif(notif);
        bool notificationAdded = n.ajouter();

        if (notificationAdded) {
            QMessageBox::information(this, "RDI Center", "Modification effectuée avec notification.");
        } else {
            QMessageBox::critical(this, "RDI Center", "Modification effectuée mais erreur dans l'ajout de la notification.");
        }

        // Refresh
        ui->tableView->setModel(m.afficher());
        ui->tableNotification->setModel(n.afficher());

        // Clearing input fields
        ui->idBox_4->clear();
        ui->nomBoxMat_4->clear();
        ui->typeBox_4->clear();
        ui->matriculeBox_4->clear();
        ui->constructeurBox_4->clear();
        ui->dateEdit_4->clear();
        ui->dateMaintenanceBox_4->clear();
        ui->etatBox_4->clear();
    } else {
        QMessageBox::critical(this, "RDI Center", "Modification échouée.");
    }
    updateLCDs();
    trayIcon->showMessage("Materiel Modified", "Materiel modified successfully.", QSystemTrayIcon::Information, 5000);
}


void MainWindow::on_deleteBox_clicked()
{
    int id = ui->suppression_field->text().toInt();
    bool test = m.supprimer(id);

    if (test) {
        // Add notification after deleting material
        QString notif = "Materiel " + QString::number(id) + " supprimé";
        notification n;
        n.setDate(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        n.setNotif(notif);
        bool notificationAdded = n.ajouter();

        if (notificationAdded) {
            QMessageBox::information(nullptr, "RDI Center", "Suppression effectuée avec notification.", QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, "RDI Center", "Suppression effectuée mais erreur dans l'ajout de la notification.", QMessageBox::Cancel);
        }

        ui->tableView->setModel(m.afficher());
        ui->tableNotification->setModel(n.afficher());

    } else {
        QMessageBox::critical(nullptr, "RDI Center", "Suppression non effectuée.", QMessageBox::Cancel);
    }
    ui->suppression_field->clear();
}


void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    QString value = ui->tableView->model()->data(index).toString();
    ui->suppression_field->setText(value);
    ui->pdf_field->setText(value);
    QSqlQuery qry;

    qry.prepare("select * from materiel where id='" + value + "'");
    if (qry.exec()) {
        while (qry.next()) {
            ui->idBox_4->setText(qry.value(0).toString());
            ui->nomBoxMat_4->setText(qry.value(1).toString());
            ui->typeBox_4->setText(qry.value(2).toString());
            ui->matriculeBox_4->setText(qry.value(3).toString());
            ui->constructeurBox_4->setText(qry.value(4).toString());
            ui->dateEdit_4->setDate(QDate::fromString(qry.value(2).toString(), "yyyy-MM-dd"));
            ui->dateMaintenanceBox_4->setDate(QDate::fromString(qry.value(2).toString(), "yyyy-MM-dd"));
            ui->etatBox_4->setText(qry.value(7).toString());
        }
    }
    updateLCDs();

}

void MainWindow::on_ActualiserMatBox_clicked()
{
    ui->tableView->setModel(m.afficher());
    ui->tableNotification->setModel(n.afficher());
    updateLCDs();

}

void MainWindow::on_recherche_textChanged(const QString &arg1)
{
    QString rech=ui->recherche->text();
    ui->tableView->setModel(m.RechercherMateriel(rech));
}

void MainWindow::on_TrierBox_activated(const QString &arg1)
{
    bool ascending = true; // Default to ascending order

    if (arg1 == "A-Z") {
        ui->tableView->setModel(m.trierMaterielByNom(ascending));
    } else if (arg1 == "Date Fabrication") {
        ui->tableView->setModel(m.trierMaterielByDate(ascending));
    } else if (arg1 == "Trier") {
        ui->tableView->setModel(m.afficher());
    }
}

void MainWindow::on_imprimerBox_clicked()
{
    // Get the ID from the pdf_field
    int id = ui->pdf_field->text().toInt();

    // Query the database to retrieve the data corresponding to the ID
    QSqlQuery qry;
    qry.prepare("SELECT * FROM materiel WHERE id=:id");
    qry.bindValue(":id", id);
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to retrieve data from the database.");
        return;
    }

    // Create a PDF printer
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) // User canceled or didn't provide a file name
        return;
    printer.setOutputFileName(fileName);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Error", "Failed to create PDF.");
        return;
    }

    // Start drawing on the PDF
    int yPos = 20; // Initial y position
    while (qry.next()) {
        // Draw each field on the PDF
        painter.drawText(20, yPos, "ID: " + qry.value(0).toString());
        painter.drawText(20, yPos + 20, "Matricule: " + qry.value(1).toString());
        painter.drawText(20, yPos + 40, "Nom: " + qry.value(2).toString());
        painter.drawText(20, yPos + 60, "Type: " + qry.value(3).toString());
        painter.drawText(20, yPos + 80, "Constructeur: " + qry.value(4).toString());
        painter.drawText(20, yPos + 100, "Date de fabrication: " + qry.value(5).toString());
        painter.drawText(20, yPos + 120, "Date dernière révision: " + qry.value(6).toString());
        painter.drawText(20, yPos + 140, "Etat: " + qry.value(7).toString());

        yPos += 160; // Move to the next row
    }

    painter.end();

    QMessageBox::information(this, "PDF Created", "PDF has been generated successfully.");
}

void MainWindow::updateLCDs() {

    QDateTime currentTime = QDateTime::currentDateTime();
       QDateTime midnight = QDateTime(currentTime.date(), QTime(0, 0));
       if (midnight < currentTime) {
           midnight = midnight.addDays(1); // Set to next midnight if current time is past midnight
       }

       int minutesUntilMidnight = currentTime.secsTo(midnight) / 60; // Convert seconds to minutes

       // Update TIMER LCD
       ui->TIMER->display(minutesUntilMidnight);


    int materielCount = m.afficher()->rowCount(); // Get the number of rows in the model
       ui->lcdNumber->display(materielCount);
               int notifCount = n.afficher()->rowCount(); // Get the number of rows in the model
                  ui->notifLCD->display(notifCount);

    int id = ui->suppression_field->text().toInt(); // Get the selected ID from UI

    // Update LCDAGE
    int age = m.calculateAge(id);
    if (age != -1) {
        ui->LCDAGE->display(age);
    } else {
        ui->LCDAGE->display("Error");
    }

    // Update LCDMAINT
    int maint = m.calculateMaintenance(id);
    if (maint != -1) {
        ui->LCDMAINT->display(maint);
    } else {
        ui->LCDMAINT->display("Error");
    }
}

void MainWindow::on_tableNotification_activated(const QModelIndex &index)
{
        QSqlQueryModel *model = new QSqlQueryModel();

        model->setQuery("SELECT * FROM NOTIFICATION");
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("DATE_NOTIF"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOTIF"));

        ui->tableNotification->setModel(model);
        ui->tableNotification->resizeColumnsToContents();
        updateLCDs();

}

void MainWindow::on_pushButton_8_clicked()
{
    ui->tableNotification->setModel(n.afficher());
    updateLCDs();


}
void MainWindow::updateChart() {
    // Get the number of materials and notifications
    int materialCount = m.afficher()->rowCount(); // Assuming rowCount() returns the count of materials
    int notificationCount = n.afficher()->rowCount(); // Assuming rowCount() returns the count of notifications

    // Create a bar series and populate it with data
    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
    QtCharts::QBarSet *set1 = new QtCharts::QBarSet("Number of Materials");
    QtCharts::QBarSet *set2 = new QtCharts::QBarSet("Number of Notifications");

    // Add data to the bar sets
    *set1 << materialCount;
    *set2 << notificationCount;

    // Add the bar sets to the series
    series->append(set1);
    series->append(set2);

    // Create a chart and set its title
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Materials and Notifications Chart");

    // Create a chart view and set the chart
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Create a button to close the chart
    QPushButton *closeButton = new QPushButton("Close Chart", this);
    connect(closeButton, &QPushButton::clicked, [=]() {
        // Restore the original central widget
        setCentralWidget(ui->centralwidget);
    });

    // Create a layout to hold the chart view and the close button
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    layout->addWidget(closeButton);

    // Create a widget to hold the layout
    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    setCentralWidget(widget);
}


void MainWindow::on_pushButton_10_clicked()
{
    updateChart();

}
