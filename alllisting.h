#ifndef ALLLISTING_H
#define ALLLISTING_H

#include <QDialog>
#include <QtGui>
#include <QtXml>
#include <QFile>
#include <QString>
#include <QTextStream>
#include "Xml_Dom.h"
#include <QXmlStreamReader>
#include <QListWidget>
#include <QTreeWidget>
#include <QTreeView>
#include <QObject>
#include <QBrush>
#include <QStandardItemModel>


class Modify;

namespace Ui {
class AllListing;
}

class AllListing : public QDialog
{
    Q_OBJECT

public:
    explicit AllListing(QWidget *parent = 0);
    ~AllListing();
    bool parser(); //fonction permettant de parser et de faire le listing des elements du fichier

    const QString& getDefaultNom() const;
    void setDefaultNom(const QString& value);

    const QString getDefaultSocPri() const;
    void setDefaultSocPri(const QString &value);

    const QString getDefaultEdition() const;
    void setDefaultEdition(const QString &value);


private slots:

    void on_searchLine_textChanged(const QString &text);
    void on_neContientPasBox_released();
    //void uninstallAppletCliquedSlot();
    void myContMenu(const QPoint &pos);
    //void itemClicked();


    void modifyAttributes();


private:
    Ui::AllListing *ui;

    void addTreeRoot(QString name, QString edition, QString socpri);
    void addTreeChild(QTreeWidgetItem *parent, QString name, QString description);
    void parcoursArbre(QTreeWidget *tw, QString search);
    void parcoursArbreRecursif(QTreeWidgetItem *item, QString search);


    QString *m_txt;
    QStandardItemModel* model;
    QMenu *contextMenu;         //
    QAction *uninstallAction;   //
    QTreeView *treeView;        //
    QAction *myAction;
    QString defaultNom;
    QString defaultEdition;
    QString defaultSocPri;



    Modify *modify;

    QMap<QString, QStringList> map;




};

#endif // ALLLISTING_H
