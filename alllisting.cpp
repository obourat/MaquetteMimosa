#include "alllisting.h"
#include "ui_alllisting.h"
#include "modify.h"


using namespace std;




AllListing::AllListing(QWidget *parent) :                                                   //Constructeur
    QDialog(parent),
    ui(new Ui::AllListing)
{
    ui->setupUi(this);                                                                      //Affichage de la fenêtre

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);                            //On sélectionne un menu personnalisé pour le context menu

    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(myContMenu(const QPoint &)));


    parser();                                                                               //Parse le fichier et affiche la fenêtre
}



bool AllListing::parser()                                                                   //Fonction qui parse le fichier + lance une fenêtre TreeWidget avec les éléments + recherche implémentée
{
    QXmlStreamReader reader;                                                                //Parsage avec QXmlStreamReader
    QString fileXmlName = "GDOcopy2.xml";
    QFile fileXml(fileXmlName);                                                             //Déclaration du fichier xml
    QString element = "Doc";
    QString liseur;
    QString titre;
    QString edition;
    QString socpri;


    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1);                                                          //Ajoute le nombre de colonnes dans l'arbre



    //Set le menu

    //    ui->treeWidget->setModel(model);
    //    contextMenu = new QMenu(ui->treeWidget);
    //    ui->treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    //    uninstallAction = new QAction("Uninstall TA", contextMenu);
    //    ui->treeWidget->addAction(uninstallAction);
    //    connect(uninstallAction, SIGNAL(triggered()), this, SLOT(uninstallAppletCliquedSlot()));




#warning probleme taille colonne                                                            //Set la fenetre
#if 1
    ui->treeWidget->setColumnWidth(0,1250);
    ui->treeWidget->setColumnWidth(1,300);
#else
    for (int i = 0; i < ui->treeWidget->columnCount(); ++i)
    {
        ui->treeWidget->resizeColumnToContents(i);
    }
#endif



    ui->treeWidget->setHeaderLabels(QStringList() << "Titres" << "Arguments");              //Ajoute le header



    if(!fileXml.open(QFile::ReadOnly | QFile::Text))                                        //Ouverture du fichier XML en lecture seule et en mode texte (sort de la fonction si le fichier ne peut pas être ouvert
        return false;


    reader.setDevice(&fileXml);                                                             //Initialise l'instance reader avec le flux XML venant de file


    reader.readNext();                                                                      //On parcours le fichier et on vérifie si l'on est a des éléments "Titre" et "Edition" et "SocPri"

    while (!reader.atEnd())                                                                 //tant que le fichier n'est pas lu entièrement...
    {
        if(reader.isStartElement())
        {
            if(reader.name() == element)                                                    //Si l'élement est celui recherché (Doc)
            {

                if(element == "Doc")                                                        //Si on vient de trouver un Doc, il faut chercher son Titre
                {
                    element = "Titre";
                }


                else if (element == "Titre")                                                //Si on vient de trouver le Titre du doc, il faut chercher son Edition  après avec renseigné le fichier txt
                {
                    liseur = reader.readElementText();
                    if (liseur == "")
                    {
                        titre="Pas de Titre";
                        element = "Edition";
                    }
                    else
                    {
                        titre=liseur;
                        element = "Edition";
                    }

                }


                else if (element == "Edition")                                              //Si on a trouvé l'Edition du doc, il faut chercher son SocPri après avoir renseigné le fichier txt
                {
                    liseur = reader.readElementText();
                    if (liseur == "")
                    {
                        edition="Pas d'Edition";
                        element = "SocPri";
                    }
                    else
                    {
                        edition=liseur;
                        element = "SocPri";
                    }

                }


                else if (element == "SocPri")                                               //Si on a trouvé le SocPri du doc, on peut chercher l'existence d'un nouveau doc
                {
                    liseur = reader.readElementText();
                    if (liseur == "")
                    {
                        socpri="Pas de SocPri";
                        addTreeRoot(titre, edition, socpri);
                        element = "Doc";
                    }
                    else
                    {
                        socpri=liseur;
                        addTreeRoot(titre, edition, socpri);
                        element = "Doc";
                    }


                }

            }

        }


        reader.readNext();                                                                  //On va au prochain element
    }

    fileXml.close();
    return true;
}



AllListing::~AllListing()
{
    delete ui;
}



void AllListing::addTreeRoot(QString name, QString edition, QString socpri)                 //Fonction permettant d'ajouter des noeuds
{

    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

    treeItem->setText(0, name);
    addTreeChild(treeItem, edition , "Edition");
    addTreeChild(treeItem, socpri, "SocPri");

}



void AllListing::addTreeChild(QTreeWidgetItem *parent,                                      //Fonction permettant d'ajouter des enfants aux noeuds
                              QString name, QString description)
{

    QTreeWidgetItem *treeItem = new QTreeWidgetItem();


    treeItem->setText(0, name);
    treeItem->setText(1, description);

    treeItem->setFlags(treeItem->flags() | Qt::ItemIsEditable);                             //Rend l'élément éditable


    parent->addChild(treeItem);                                                             //Ajoute l'enfant à l'arbre
}



//void AllListing::on_pushButton_released()
//{
//    // Cacher une ligne
//    //ui->treeWidget->setRowHidden(0, QModelIndex(), true);


//    // Affichage données premier parent
//    qDebug() << ui->treeWidget->invisibleRootItem()->child(0)->data(0, Qt::DisplayRole);
//    // Affichage des donnes des enfants du premier parent

//    QTreeWidgetItem *item;
//    item = ui->treeWidget->invisibleRootItem()->child(0)->child(0);

//    qDebug() << ui->treeWidget->invisibleRootItem()->childCount();

//    if(item != NULL)
//    {
//        qDebug() <<item->data(1, Qt::DisplayRole).toString();
//    }

//}



void AllListing::parcoursArbre(QTreeWidget *tw, QString search)                             //Fonction qui parcrours l'ensemble des éléments parents de l'arbre (recherche)
{
    // ui->treeWidget->findItems()

    for(int i = 0;i<tw->topLevelItemCount();i++)                                            //On parcours l'ensemble des Parents
    {

        parcoursArbreRecursif(tw->topLevelItem(i), search);                                 //Le cas où on recherche ce qui contient

    }
}



void AllListing::parcoursArbreRecursif(QTreeWidgetItem *item, QString search)               //Fonction de parcours des éléments enfants et parents et hide ou laisse afficher en fonction de la recherche et du mode choisi
{
    QString dataSelect = item->data(0, Qt::DisplayRole).toString();
    if(!dataSelect.contains(search))                                                        //Si le contenu de l'item parent ne contient pas l'input :
    {

        for (int i = 0; i < item->childCount(); ++i)                                        //On parcours l'ensembles des Attributs enfant de chaque élément parent
        {

            for (int j = 0; j < item->child(i)->columnCount(); ++j)
            {
                QString dataSelectChild = item->child(i)->data(j, Qt::DisplayRole).toString();

                if(!dataSelectChild.contains(QRegExp(search)) && i == item->childCount() - 1)
                {
                    if(ui->neContientPasBox->isChecked())
                    {
                        item->setHidden(false);
                    }
                    else
                    {
                        item->setHidden(true);
                    }
                }
                else if(dataSelectChild.contains(QRegExp(search)))
                {
                    if(ui->neContientPasBox->isChecked())
                    {
                        item->setHidden(true);

                    }
                    else
                    {
                        item->setHidden(false);

                    }
                    return;
                }
            }
        }

    }
    else
    {
        if(ui->neContientPasBox->isChecked())
        {
            item->setHidden(true);
        }
        else
        {
            item->setHidden(false);
        }

    }


}
const QString AllListing::getDefaultEdition() const                                                 //Getters and setters des titres et attributs
{
    return defaultEdition;
}

void AllListing::setDefaultEdition(const QString &value)
{
    defaultEdition = value;
}

const QString AllListing::getDefaultSocPri() const
{
    return defaultSocPri;
}

void AllListing::setDefaultSocPri(const QString &value)
{
    defaultSocPri = value;
}

const QString& AllListing::getDefaultNom() const
{
    return defaultNom;
}

void AllListing::setDefaultNom(const QString &value)
{
    defaultNom = value;
}




void AllListing::on_searchLine_textChanged(const QString &text)                                 //Slot de la recherche dynamique
{
    parcoursArbre(ui->treeWidget, text);
#if 0
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        // Parent
        QTreeWidgetItem *item;
        item = ui->treeWidget->topLevelItem(i);

        // Titre
        QString title = item->data(0, Qt::DisplayRole).toString();

        if(!dataSelect.contains(QRegExp(search)))
        {
            // Items enfants
            QList<QTreeWidgetItem*> items;
            // Affectation
            items = ui->treeWidget->find("Pas d'Edition", Qt::MatchContains | Qt::MatchRecursive);

        }
        else
        {
            item->setHidden(false);
        }
    }

    //parcoursArbre(ui->treeWidget, text);

    // Items de l'arbre
    QList<QTreeWidgetItem*> items;
    // Affectation
    items = ui->treeWidget->findChildren("Pas d'Edition", Qt::MatchContains | Qt::MatchRecursive);

    for (int i = 0; i < items.count(); ++i)
    {
        ui->treeWidget->
    }
#endif

}



void AllListing::on_neContientPasBox_released()                                                 //Slot du bouton "ne contient pas"
{
    QString text;
    text = ui->searchLine->text();
    parcoursArbre(ui->treeWidget, text);
}


//void AllListing::mySlot()
//{
//    modify = new Modify();
//    modify->exec();
//}



//void AllListing:: prepareMenu(const QPointnt & pos)
//{
//    QTreeWidget *tree = treeWid;
//    QTreeWidgetItem *nd = tree->itemAt(pos);
//    QDebug()<<pos<<nd->text(0);

//    QAction *newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&New"),this);
//    newAct->setStatusTip(tr("new sth"));
//    connect(newAct, SIGNAL(triggered()), this, SLOT(newDev()));

//    QMenu menu(this);
//    menu.addAction(newAct);

//    QPoint pt(pos);
//    menu.exec(tree->mapToGlobal(pt));
//}



//void AllListing:: uninstallAppletCliquedSlot()
//{

//}


void AllListing::myContMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);                                            //Selection du WidgetItem en fonction de la position de la souris
    if(!item)
    {
        return;
    }

    QMenu *menu = new QMenu(ui->treeWidget);                                                        //Ajout du menu

    myAction = menu->addAction("Editer le document");

    connect(myAction, SIGNAL(triggered()), this, SLOT(modifyAttributes()));

    //Ajout d'une action sur le menu, du raccourci et de la def...
    myAction->setShortcut(tr("Ctrl+E"));
    myAction->setStatusTip(tr("Modifier le document séléctionné"));
    menu->exec(ui->treeWidget->viewport()->mapToGlobal(pos));                                       //Execution du menu sur le treeWidget

#if 0
#warning signal du bouton modifié buggé
    if(myAction->isEnabled())                                                                       //Si on clique sur le bouton, on affiche la fenêtre de modification
    {
        foreach(QTreeWidgetItem *item, ui->treeWidget->selectedItems())
        {
            if(item->parent())                                                                     //Si l'item possède un parent...
            {
                item=item->parent();
            }

            //qDebug() << "item Text  [" <<  item->text(0);


            //QStringList items;

            defaultNom = item->text(0);
            defaultRef = item->child(0)->text(0);
            defaultSocPri = item->child(1)->text(0);

            //items << titleFather;

            //map.insert(item->text(0), items);                                                       //On stocke le nom du parent avec le QMap

            //            for(int line = 0; line<item->childCount();++line)
            //            {
            //                //qDebug() << "itemChild Text  [" << item->child(line) << "]: " << item->child(line)->text(0);

            //                QString titleChild = item->child(line)->text(0);// + " " + QString::number(line);
            //                items << titleChild;
            //            }

            //map.insert(item->text(0), items);                                                   //On utilise un Qmap pour stocker la valeur des enfants
            modify = new Modify(this);
            modify->exec();


        }




    }
#endif
    //connect(myAction, SIGNAL(isEnabled()),this, SLOT(itemClicked(QTreeWidgetItem*, int)));
    //modify = new Modify;
    //modify->exec();




}

void AllListing::modifyAttributes()
{
    foreach(QTreeWidgetItem *item, ui->treeWidget->selectedItems())
    {
        if(item->parent())                                                                     //Si l'item possède un parent...
        {
            item=item->parent();
        }

        //qDebug() << "item Text  [" <<  item->text(0);


        //QStringList items;

        defaultNom = item->text(0);
        defaultEdition = item->child(0)->text(0);
        defaultSocPri = item->child(1)->text(0);

        //items << titleFather;

        //map.insert(item->text(0), items);                                                       //On stocke le nom du parent avec le QMap

        //            for(int line = 0; line<item->childCount();++line)
        //            {
        //                //qDebug() << "itemChild Text  [" << item->child(line) << "]: " << item->child(line)->text(0);

        //                QString titleChild = item->child(line)->text(0);// + " " + QString::number(line);
        //                items << titleChild;
        //            }

        //map.insert(item->text(0), items);                                                   //On utilise un Qmap pour stocker la valeur des enfants
        modify = new Modify(this);
        modify->exec();


    }

}

//void AllListing::itemClicked()
//{
//    foreach(QTreeWidgetItem *item, ui->treeWidget->selectedItems())
//    {
//        for(int col = 0; col<item->childCount();++col)
//        {
//            qDebug() << "item Text  [" << col << "]: " << item->text(col);
//        }
//    }
//      modify = new Modify;
//      modify->exec();

//}
