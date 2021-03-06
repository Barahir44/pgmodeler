#include "listaobjetoswidget.h"

ListaObjetosWidget::ListaObjetosWidget(QWidget *parent): ObjetoBaseWidget(parent)
{
 Ui_ListaObjetosWidget::setupUi(this);
 configurarLayouFormulario(listaobjetos_grid, BASE_OBJECT);

 //Define os atributos do formulários e da janela pai
 janela_pai->setWindowTitle(trUtf8("Dependeces / References of Object"));
 janela_pai->definirBotoes(CaixaMensagem::BOTAO_OK);
 janela_pai->setMinimumSize(550, 300);

 //Conecta o botão ok do formulário pai com o método de fechamento do formulário
 connect(janela_pai->aplicar_ok_btn, SIGNAL(clicked(bool)), janela_pai, SLOT(close(void)));
}

void ListaObjetosWidget::definirAtributos(DatabaseModel *modelo, BaseObject *objeto, BaseObject *objeto_pai)
{
 vector<BaseObject *> vet_objs;

 ObjetoBaseWidget::definirAtributos(modelo, NULL, objeto, objeto_pai);

 this->nome_edt->setReadOnly(true);
 this->janela_pai->aplicar_ok_btn->setEnabled(true);
 this->obj_protegido_frm->setVisible(false);
 this->comentario_edt->setVisible(false);
 this->comentario_lbl->setVisible(false);

 //Configura o icone do objeto a ser exibido
 iconeobj_lbl->setPixmap(QPixmap(QString(":/icones/icones/") +
                                 BaseObject::getSchemaName(objeto->getObjectType()) + QString(".png")));

 //Carrega as dependências e referências do objeto
 modelo->getObjectDependecies(objeto, vet_objs);
 this->atualizarListaObjetos(vet_objs, dependencias_tbw);

 modelo->getObjectReferences(objeto, vet_objs);
 this->atualizarListaObjetos(vet_objs, referencias_tbw);
}

void ListaObjetosWidget::hideEvent(QHideEvent *evento)
{
 tabWidget->setCurrentIndex(0);

 //Limpa as tabelas de objetos
 while(dependencias_tbw->rowCount() > 0)
  dependencias_tbw->removeRow(0);

 while(referencias_tbw->rowCount() > 0)
  referencias_tbw->removeRow(0);

 ObjetoBaseWidget::hideEvent(evento);
}

void ListaObjetosWidget::atualizarListaObjetos(vector<BaseObject *> &objetos, QTableWidget *listaobjetos_tbw)
{
 int qtd, id_lin, i;
 QTableWidgetItem *item_tab=NULL;
 BaseObject *obj_pai=NULL;

 qtd=objetos.size();

 //Varre a lista de objtes inserindo cada um na lista (tabela)
 for(id_lin=0, i=0; i < qtd; i++)
 {
  /* Caso o objeto da lista for o mesmo do objeto de origem (o qual ser que exibir
     as refs. e deps.) este será ignorado */
  if(objetos[i]!=this->objeto)
  {
   listaobjetos_tbw->insertRow(id_lin);

   //Aloca o item de nome do objeto de ref/dep
   item_tab=new QTableWidgetItem;
   item_tab->setText(QString::fromUtf8(objetos[i]->getName()));
   item_tab->setIcon(QPixmap(QString(":/icones/icones/") +
                     BaseObject::getSchemaName(objetos[i]->getObjectType())+ QString(".png")));
   listaobjetos_tbw->setItem(id_lin, 0, item_tab);

   //Aloca o item de tipo do objeto de ref/dep
   item_tab=new QTableWidgetItem;
   item_tab->setText(QString::fromUtf8(objetos[i]->getTypeName()));
   listaobjetos_tbw->setItem(id_lin, 1, item_tab);

   //Aloca o item do objeto pai do objeto de ref/dep
   item_tab=new QTableWidgetItem;
   objeto=objetos[i];
   if(dynamic_cast<TableObject *>(objetos[i]))
    obj_pai=dynamic_cast<TableObject *>(objetos[i])->getParentTable();
   else if(objetos[i]->getSchema())
    obj_pai=objetos[i]->getSchema();
   else
    obj_pai=this->modelo;

   //Aloca o item de tipo do objeto pai do objeto de ref/dep
   item_tab->setText(QString::fromUtf8(obj_pai->getName()));
   item_tab->setIcon(QPixmap(QString(":/icones/icones/") +
                     BaseObject::getSchemaName(obj_pai->getObjectType())+ QString(".png")));
   listaobjetos_tbw->setItem(id_lin, 2, item_tab);

   item_tab=new QTableWidgetItem;
   item_tab->setText(QString::fromUtf8(obj_pai->getTypeName()));
   listaobjetos_tbw->setItem(id_lin, 3, item_tab);

   id_lin++;
  }
 }
}

