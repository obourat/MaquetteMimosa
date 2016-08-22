#include "modify.h"
#include "ui_modify.h"
#include "alllisting.h"
using namespace std;


Modify::Modify(AllListing *allListing,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Modify),
    allListing(allListing)

{
    ui->setupUi(this);
    QString updateNom, updateEdition, updateSocPri;
    updateNom = allListing->getDefaultNom();
    updateEdition = allListing->getDefaultEdition();
    updateSocPri = allListing->getDefaultSocPri();

    ui->updateNom->setText(updateNom);                                                                      //On rentre dans les input les valeurs connues des attributs
    ui->updateEdition->setText(updateEdition);
    ui->updateSocPri->setText(updateSocPri);

    //this->allListing = allListing;

    //ui->updateNom->text() = allListing.getDefaultNom();
}

Modify::~Modify()
{
    delete ui;
}

void Modify::on_updateButtonBox_rejected()                                                                  //Si on clique sur annuler
{
    close();
}

void Modify::on_updateButtonBox_accepted()                                                                  //Si on clique sur accepter
{
    if(ui->updateNom->text() == allListing->getDefaultNom() && ui->updateEdition->text() == allListing->getDefaultEdition() && ui->updateSocPri->text() == allListing->getDefaultSocPri())
    {
        QMessageBox::information(this, "Information", "Aucun attribut n'a ete change, le document n'a pas ete modifie");
    }

    else if (ui->updateNom->text() == "")
    {
        QMessageBox::warning(this, "Erreur", "Veuillez donner un nom au document");
    }

    else
    {
        QDomDocument dom("GDOcopy2generated");                                                            //Creation d'un QDomDocument à partir du fichier xml
        QFile file("GDOcopy2.xml");
        //QDomElement root = dom.documentElement();
        //qDebug() << root.text();
        //QString write_doc = dom.toString();                                                               //Stockage du fichier mis à jour

        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier XML");
            file.close();
            return;
        }

        if(!dom.setContent(&file)) {
            QMessageBox::critical(this, "erreur", "Impossible d'ouvrir le fichier XML");
            file.close();
            return;
        }
        file.close();


        QDomNode docElem = dom.documentElement().firstChild();                                              //ListDoc -> Doc

        while(!docElem.isNull())
        {
            QDomNode n = docElem.firstChild();                                                              //Doc ->Num...

            while (n.nodeName() != "id_CONST")                                                              //Tant que la balise est differente de id_const (bug XML mal formé)
            {

                QDomElement e = n.toElement();                                                              //on stocke les valeurs de chaque balise

                        if(allListing->getDefaultNom() == e.text())                                             //Si on se trouve dans le bon doc :
                        {

                            if(ui->updateNom->text() != allListing->getDefaultNom())                            //Si le nom a ete change :
                            {
                                QDomNode p = docElem.firstChild();
                                while ( p.nodeName() != "Titre")
                                {
                                    p = p.nextSibling();
                                }

                                e = p.toElement();


                                QDomElement newTitre = dom.createElement("Titre");
                                newTitre.appendChild(dom.createTextNode(ui->updateNom->text()));
                                e.parentNode().replaceChild(newTitre,e);
                            }

                            if(ui->updateEdition->text() != allListing->getDefaultEdition())                    //Si l'edition a ete change :
                            {
                                QDomNode q = docElem.firstChild();
                                while ( q.nodeName() != "Edition")
                                {
                                    q = q.nextSibling();
                                }

                                e = q.toElement();


                                QDomElement newEdition = dom.createElement("Edition");
                                newEdition.appendChild((dom.createTextNode(ui->updateEdition->text())));
                                e.parentNode().replaceChild(newEdition,e);
                            }
                            if(ui->updateSocPri->text() != allListing->getDefaultSocPri())                      //Si le SocPri a ete change :
                            {
                                QDomNode r = docElem.firstChild();
                                while ( r.nodeName() != "SocPri")
                                {
                                    r = r.nextSibling();
                                }

                                e = r.toElement();


                                QDomElement newSocPri = dom.createElement("SocPri");
                                newSocPri.appendChild((dom.createTextNode(ui->updateSocPri->text())));
                                e.parentNode().replaceChild(newSocPri,e);
                            }

                            break;                                                                           //On sors de la boucle si le bon doc a ete trouvé et traité

                        }




                n = n.nextSibling();                                       //On passe à la prochaine balise d'un doc
            }
        docElem = docElem.nextSibling();                                   //On passe au prochain doc
        }

        file.open(QIODevice::WriteOnly);
        file.write(dom.toByteArray());
        file.close();

        allListing->parser();




        //QTextStream out(&file);

        //out << dom.toString();                                                //ecriture du document

        //file.close();                                                     //fermeture du document
    }
}


//        while(!n.isNull())
//        {
//            QDomElement e=n.toElement();
//            if(!e.isNull())
//            {

//            }
//        }


        //if(ui->updateNom->text() != allListing->getDefaultNom())
//    }
//}

