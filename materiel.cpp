#include "materiel.h"
#include <QDate>
#include <QSqlQuery>



materiel::materiel() {}

materiel::materiel(int id ,  QString matricule,QString nom ,QString type ,QString constructeur,QString date_fabrication,QString date_der ,QString etat )
{
    this->id=id;
    this->matricule=matricule;
    this->nom=nom;
    this->type=type;
    this->constructeur=constructeur;
    this->date_fabrication=date_fabrication;
    this->date_der=date_der;
    this->etat=etat;


}
bool materiel::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO MATERIEL (ID, MATRICULE, NOM, TYPE, CONSTRUCTEUR, DATE_FABRICATION, DATE_DER, ETAT) "
                  "VALUES (:id, :matricule, :nom, :type, :constructeur, :date_fabrication, :date_der, :etat)");

    query.bindValue(":id", id);
    query.bindValue(":matricule", matricule);
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":constructeur", constructeur);
    query.bindValue(":date_fabrication", date_fabrication);
    query.bindValue(":date_der", date_der);
    query.bindValue(":etat", etat);

    return query.exec();
}

bool materiel::modifier()
{
    QSqlQuery query;

    query.prepare("UPDATE MATERIEL SET MATRICULE=:matricule, NOM=:nom, TYPE=:type, "
                  "CONSTRUCTEUR=:constructeur, DATE_FABRICATION=:date_fabrication, DATE_DER=:date_der, ETAT=:etat WHERE ID=:id");

    query.bindValue(":id", id);
    query.bindValue(":matricule", matricule);
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":constructeur", constructeur);
    query.bindValue(":date_fabrication", date_fabrication);
    query.bindValue(":date_der", date_der);
    query.bindValue(":etat", etat);

    return query.exec();
}

QSqlQueryModel *materiel::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM MATERIEL");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Matricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Constructeur"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date de fabrication"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date dernière révision"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Etat"));

    return model;
}

bool materiel::supprimer(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM MATERIEL WHERE ID=:id");
    query.bindValue(":id", id);

    return query.exec();
}



QSqlQueryModel* materiel::RechercherMateriel(QString recherche)
{
     QSqlQueryModel * model= new QSqlQueryModel();
     model->setQuery("SELECT * FROM MATERIEL WHERE NOM LIKE '"+recherche+"%' OR MATRICULE LIKE '"+recherche+"%' ");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("Matricule"));
     model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
     model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
     model->setHeaderData(4, Qt::Horizontal, QObject::tr("Constructeur"));
     model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date de fabrication"));
     model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date dernière révision"));
     model->setHeaderData(7, Qt::Horizontal, QObject::tr("Etat"));

return model;
}



QSqlQueryModel *materiel::trierMaterielByDate(bool ascending)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sortOrder = ascending ? "ASC" : "DESC";

    model->setQuery("SELECT * FROM MATERIEL ORDER BY DATE_FABRICATION " + sortOrder);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Matricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Constructeur"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date de fabrication"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date dernière révision"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Etat"));

    return model;
}

QSqlQueryModel *materiel::trierMaterielByNom(bool ascending)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sortOrder = ascending ? "ASC" : "DESC";

    model->setQuery("SELECT * FROM MATERIEL ORDER BY NOM " + sortOrder);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Matricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Constructeur"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date de fabrication"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date dernière révision"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Etat"));

    return model;
}
int materiel::calculateAge(int id) {
    QSqlQuery query;
    query.prepare("SELECT DATE_FABRICATION, DATE_DER FROM MATERIEL WHERE ID=:id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        QDate fabric_date = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
        QDate maint_date = QDate::fromString(query.value(1).toString(), "yyyy-MM-dd");
        QDate current_date = QDate::currentDate();

        return fabric_date.daysTo(current_date); // Age in days
    } else {
        return -1; // Error occurred
    }
}

int materiel::calculateMaintenance(int id) {
    QSqlQuery query;
    query.prepare("SELECT DATE_DER FROM MATERIEL WHERE ID=:id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        QDate maint_date = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
        QDate current_date = QDate::currentDate();

        return maint_date.daysTo(current_date); // Maintenance days
    } else {
        return -1; // Error occurred
    }
}

