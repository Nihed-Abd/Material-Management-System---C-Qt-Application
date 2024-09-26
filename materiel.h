#ifndef MATERIEL_H
#define MATERIEL_H

#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

class materiel
{
public:

    int  id  ;
    QString matricule, nom , type , constructeur, date_fabrication, date_der , etat ;



public:

    materiel();
    materiel(int, QString, QString, QString, QString, QString, QString, QString);



    int getId() const { return id; }
    QString getMatricule() const { return matricule; }
    QString getNom() const { return nom; }
    QString getType() const { return type; }
    QString getConstructeur() const { return constructeur; }
    QString getDate_fabrication() const { return date_fabrication; }
    QString getDate_der() const { return date_der; }
    QString getEtat() const { return etat; }

    // Setters*
    void setId(int id_m) { id = id_m; }
    void setMatricule(QString matricule_c) { matricule = matricule_c; }
    void setNom(QString nom_c) { nom = nom_c; }
    void setType(QString type_c) { type = type_c; }
    void setConstructeur(QString constructeur_c) { constructeur = constructeur_c; }
    void setDate_fabrication(QString date_fabrication_c) { date_fabrication = date_fabrication_c; }
    void setDate_der(QString date_der_m) { date_der = date_der_m; }
    void setEtat(QString etat_c) { etat = etat_c; }


    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel * trierMaterielByDate(bool ascending);
    QSqlQueryModel * trierMaterielByNom(bool ascending);
    QSqlQueryModel * RechercherMateriel(QString);
    int calculateAge(int id);
    int calculateMaintenance(int id);



};

#endif // MATERIEL_H
