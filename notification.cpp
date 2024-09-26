#include "notification.h"
#include <QDebug>

notification::notification()
{

}
notification::notification(QString date_notif,QString notif)
{
    this->date_notif=date_notif;
    this->notif=notif;

}
bool notification::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO NOTIFICATION (DATE_NOTIF, NOTIF) "
                  "VALUES (:date_notif, :notif)");

    query.bindValue(":date_notif", date_notif);
    query.bindValue(":notif", notif);
    return query.exec();
}
QSqlQueryModel *notification::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM NOTIFICATION");
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("DATE_NOTIF"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOTIF"));


    return model;
}
void notification::midnightReset() {
    QSqlQuery query;
    bool success = query.exec("DELETE FROM NOTIFICATION");

    if (success) {
        qDebug() << "Midnight reset: Notification table cleared.";
    } else {
        qDebug() << "Midnight reset: Failed to clear Notification table.";
    }
}

