#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

class notification
{
public:

    QString date_notif, notif;


    notification();
    notification(QString, QString);


    QString getDate() const { return date_notif; }
    QString getNotif() const { return notif; }

    void setDate(QString date_n) { date_notif = date_n; }
    void setNotif(QString notification) { notif = notification; }

    bool ajouter();
    QSqlQueryModel * afficher();
    void midnightReset();


};

#endif // NOTIFICATION_H
