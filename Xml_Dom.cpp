#include "Xml_Dom.h"
#include <iostream>
using namespace std;


Xml_Dom::Xml_Dom() : QWidget()
{
QDomDocument dom("mon_xml");
QFile xml_doc("list_applis_refapp.xml");
if(!xml_doc.open(QIODevice::ReadOnly))
{
QMessageBox::warning(this, "Erreur à l'ouverture du document XML", "Le document XML n'a pas pu être ouvert. Vérifiez que le nom est le bon et que le document est bien placé");
return;
}
if (!dom.setContent(&xml_doc))
{
xml_doc.close();
QMessageBox::warning(this, "Erreur à l'ouverture du document XML", "Le document XML n'a pas pu être attribué à l'objet QDomDocument.");
return;
}
QDomElement dom_element = dom.documentElement();
QDomNode noeud = dom_element.firstChild();
while(!noeud.isNull())
{
if(!dom_element.isNull())
QMessageBox::information(this, "Nom de la balise", "Le nom de la balise est " + dom_element.tagName());
noeud = noeud.nextSibling();
}
}
Xml_Dom::~Xml_Dom()
{
}
