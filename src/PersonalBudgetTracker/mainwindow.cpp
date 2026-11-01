#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    QSqlDatabase db= QSqlDatabase::addDataBase("QSQLITE");
    db.setDataBaseName("budget.db");

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
               "nom TEXT NOT NULL, "
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
