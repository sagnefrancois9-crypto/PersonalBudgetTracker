#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialiserBaseDeDonnees();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialiserBaseDeDonnees(){

    // creation du fichier SQLite
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("budget.db");

    if (!db.open()){
        qDebug() << "Erreur : impossible d'ouvrir la base de donnees" << db.lastError().text();
        return ;
    }

    qDebug()  << "Base de donnees connectees avec succes " ;

    // creation des tables si elles n'existent pas
    QSqlQuery query;

    // Table des Comptes
    query.exec("CREATE TABLE IF NOT EXISTS comptes ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "nom TEXT UNIQUE NOT NULL, "
               "solde_initial REAL NOT NULL)");

    // Table des Catégories
    query.exec("CREATE TABLE IF NOT EXISTS categories ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "nom TEXT NOT NULL, "
               "type TEXT NOT NULL)"); // 'Depense' ou 'Revenu'
    // Table des Budgets
    query.exec("CREATE TABLE IF NOT EXISTS budgets ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "mois INTEGER NOT NULL, "
               "annee INTEGER NOT NULL,"
               "montant_limite  REAL  NOT NULL, "
               "categorie_id INTEGER NOT NULL ") ;

    // Table des Transactions
    query.exec("CREATE TABLE IF NOT EXISTS transactions ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "date TEXT NOT NULL, "
               "montant REAL NOT NULL, "
               "description TEXT, "
               "compte_id INTEGER NOT NULL, "
               "categorie_id INTEGER NOT NULL)");
}

// ajouter compte
void MainWindow::on_pushButton_3_clicked()
{
    QString nom=ui->lineEdit_2->text();
    if (nom.isEmpty()){
        QMessageBox::warning(this, "Attention", "Le nom du compte ne peut pas être vide.");
        return ;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO comptes (nom, solde_initial) VALUES (:nom_compte, :solde)");

    query.bindValue(":nom_compte",nom);
    query.bindValue(":solde",ui->doubleSpinBox_3->value());

    if (!query.exec()) {
        // La règle UNIQUE a bloqué l'insertion (ou autre erreur SQL)
        QMessageBox::critical(this, "Erreur", "Impossible d'ajouter ce compte. Il existe peut-être déjà.");
    } else {
        // Succès
        QMessageBox::information(this, "Succès", "Compte ajouté avec succès !");

        //On vide les champs pour la prochaine saisie
        ui->lineEdit_2->clear();
        ui->doubleSpinBox_3->setValue(0.0);
    }
}

