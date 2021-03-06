#include "confaparenciawidget.h"

ConfAparenciaWidget::ConfAparenciaWidget(QWidget * parent) : QWidget(parent)
{
 setupUi(this);

 //Armazena cada id de elemento no arquivo de configuração
 QString ids_confs[]={
  ParsersAttributes::GLOBAL, ParsersAttributes::CONSTRAINTS, ParsersAttributes::OBJ_SELECTION,
  ParsersAttributes::POSITION_INFO, ParsersAttributes::POSITION_INFO,
  ParsersAttributes::OBJECT_TYPE, ParsersAttributes::LOCKER_ARC, ParsersAttributes::LOCKER_BODY,
  ParsersAttributes::TABLE_SCHEMA_NAME, ParsersAttributes::TABLE_NAME, ParsersAttributes::TABLE_BODY,
  ParsersAttributes::TABLE_EXT_BODY, ParsersAttributes::TABLE_TITLE, BaseObject::getSchemaName(OBJ_RULE),
  BaseObject::getSchemaName(OBJ_RULE), BaseObject::getSchemaName(OBJ_INDEX),
  BaseObject::getSchemaName(OBJ_INDEX), BaseObject::getSchemaName(OBJ_TRIGGER),
  BaseObject::getSchemaName(OBJ_TRIGGER), ParsersAttributes::VIEW_SCHEMA_NAME, ParsersAttributes::VIEW_NAME,
  ParsersAttributes::VIEW_BODY, ParsersAttributes::VIEW_TITLE, ParsersAttributes::ALIAS,
  ParsersAttributes::REF_COLUMN, ParsersAttributes::REF_TABLE, ParsersAttributes::REFERENCE,
  BaseObject::getSchemaName(OBJ_TEXTBOX), ParsersAttributes::COLUMN, ParsersAttributes::COLUMN,
  ParsersAttributes::INH_COLUMN, ParsersAttributes::PROT_COLUMN, ParsersAttributes::PK_COLUMN,
  ParsersAttributes::PK_COLUMN, ParsersAttributes::FK_COLUMN, ParsersAttributes::FK_COLUMN,
  ParsersAttributes::UQ_COLUMN, ParsersAttributes::UQ_COLUMN, ParsersAttributes::NN_COLUMN,
  ParsersAttributes::NN_COLUMN, ParsersAttributes::RELATIONSHIP, ParsersAttributes::LABEL,
  ParsersAttributes::LABEL, ParsersAttributes::ATTRIBUTE, ParsersAttributes::ATTRIBUTE };
  int i, qtd=elemento_cmb->count(),
     //Este vetor armazena os índices dos elementos os quais referem-se a configuraçao de cor de objetos
     vet_ids_aux[]={ 2, 4, 6, 7, 10, 11, 12, 14, 16, 18, 21, 22,
                     26, 27, 29, 33, 35, 37, 39, 40, 42, 44 };
 vector<int> ids_confs_obj(vet_ids_aux, vet_ids_aux + sizeof(vet_ids_aux) / sizeof(int));

 //Aloca o vetor de itens de configuração e atribui cada id de configuração aos elementos
 itens_conf.resize(qtd);
 for(i=0; i < qtd; i++)
 {
  itens_conf[i].id_conf=ids_confs[i];
  itens_conf[i].conf_obj=(std::find(ids_confs_obj.begin(), ids_confs_obj.end(), i)!=ids_confs_obj.end());
 }

 modelo=new DatabaseModel;
 cena=new ObjectsScene;
 cena->setSceneRect(QRectF(0,0,500,500));

 //Aloca o viewport com algumas opções de otimização na renderização
 viewp=new QGraphicsView(cena);
 viewp->setEnabled(false);
 viewp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
 viewp->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
 viewp->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

 //Ativa o antialiasing de fonte e objetos
 viewp->setRenderHint(QPainter::Antialiasing);
 viewp->setRenderHint(QPainter::TextAntialiasing);
 viewp->setRenderHint(QPainter::SmoothPixmapTransform);

 //Força a cena a ser desenhada da esquerda para a direita e de cima para baixo
 viewp->setAlignment(Qt::AlignLeft | Qt::AlignTop);

 //Otimizações: Cache do background (grade) e atualização mínima do viewport
 viewp->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
 viewp->centerOn(0,0);

 confaparencia_grid->addWidget(viewp, confaparencia_grid->count()-1, 0, 1, 5);

 connect(elemento_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(habilitarElemConfiguracao(void)));
 connect(cor1_tb, SIGNAL(clicked(void)), this, SLOT(aplicarCorElemento(void)));
 connect(cor2_tb, SIGNAL(clicked(void)), this, SLOT(aplicarCorElemento(void)));
 connect(cor3_tb, SIGNAL(clicked(void)), this, SLOT(aplicarCorElemento(void)));

 connect(fonte_cmb, SIGNAL(currentFontChanged(QFont)), this, SLOT(aplicarEstiloFonte(void)));
 connect(tam_fonte_spb, SIGNAL(valueChanged(double)), this, SLOT(aplicarEstiloFonte(void)));
 connect(negrito_chk, SIGNAL(toggled(bool)), this, SLOT(aplicarEstiloFonte(void)));
 connect(sublinhado_chk, SIGNAL(toggled(bool)), this, SLOT(aplicarEstiloFonte(void)));
 connect(italico_chk, SIGNAL(toggled(bool)), this, SLOT(aplicarEstiloFonte(void)));
}

ConfAparenciaWidget::~ConfAparenciaWidget(void)
{
 delete(viewp);
 delete(cena);
 delete(modelo);
}

void ConfAparenciaWidget::criarObjetosExemplo(void)
{
 try
 {
  RelationshipView *rel=NULL;
  TextboxView *caixa=NULL;
  TableView *tab=NULL;
  GraphicalView *visao=NULL;
  unsigned qtd, i;

  //Caso não existam objetos no modelo faz o carregamento do arquivo
  if(modelo->getObjectCount()==0)
  {
   modelo->loadModel(GlobalAttributes::CONFIGURATIONS_DIR +
                          GlobalAttributes::DIR_SEPARATOR +
                          GlobalAttributes::EXAMPLE_MODEL);

   qtd=modelo->getObjectCount(OBJ_TABLE);
   for(i=0; i < qtd; i++)
   {
    tab=new TableView(modelo->getTable(i));
    tab->setSelected(i==1);
    cena->addItem(tab);
   }

   qtd=modelo->getObjectCount(OBJ_VIEW);
   for(i=0; i < qtd; i++)
   {
    visao=new GraphicalView(modelo->getView(i));
    cena->addItem(visao);
   }

   qtd=modelo->getObjectCount(OBJ_RELATIONSHIP);
   for(i=0; i < qtd; i++)
   {
    rel=new RelationshipView(modelo->getRelationship(i, OBJ_RELATIONSHIP));
    cena->addItem(rel);
   }

   qtd=modelo->getObjectCount(BASE_RELATIONSHIP);
   for(i=0; i < qtd; i++)
   {
    rel=new RelationshipView(modelo->getRelationship(i, BASE_RELATIONSHIP));
    cena->addItem(rel);
   }

   qtd=modelo->getObjectCount(OBJ_TEXTBOX);
   for(i=0; i < qtd; i++)
   {
    caixa=new TextboxView(modelo->getTextbox(i));
    caixa->setSelected(i==0);
    cena->addItem(caixa);
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConfAparenciaWidget::carregarConfiguracao(void)
{
 try
 {
  int i, qtd=itens_conf.size();

  //Carrega o arquivo de estilo de objetos
  BaseObjectView::loadObjectsStyle();

  //Cria os objetos de exemplo
  this->criarObjetosExemplo();

  //Obtém cada estilo carregado do arquivo e os atribui ao itens de configuração
  for(i=0; i < qtd; i++)
  {
   //Caso o item de configuração atual refere-se a cores de um objeto
   if(itens_conf[i].conf_obj)
   {
    //Obtém o estilo de preenchimento e da borda e os armazena no item atual
    BaseObjectView::getFillStyle(itens_conf[i].id_conf,
                                            itens_conf[i].cores[0], itens_conf[i].cores[1]);
    itens_conf[i].cores[2]=BaseObjectView::getBorderStyle(itens_conf[i].id_conf).color();
   }
   else
    //Caso o item atual seja um elemento de configuração de fonte, obtém o estilo de fonte respectivo
    itens_conf[i].fmt_fonte=BaseObjectView::getFontStyle(itens_conf[i].id_conf);
  }

  //Inicializa o formulário de configuração de aparência
  this->habilitarElemConfiguracao();

  //Marca no combo de fontes, a fonte global
  fonte_cmb->setCurrentFont(BaseObjectView::getFontStyle(ParsersAttributes::GLOBAL).font());

  //Define todos os objetos do modelo de exemplo como modificados, forçando seu redesenho
  modelo->setObjectsModified();
  cena->update();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConfAparenciaWidget::salvarConfiguracao(void)
{
 try
 {
  map<QString, QString> atribs;
  vector<ItemConfAparencia>::iterator itr, itr_end;
  ItemConfAparencia item;
  QString id_atrib;
  QFont fonte;

  itr=itens_conf.begin();
  itr_end=itens_conf.end();

  while(itr!=itr_end)
  {
   item=(*itr);
   itr++;

   //Caso o item atual refere-se a um elemento de cor de objeto
   if(item.conf_obj)
   {
    //Cria um atributo o qual armazena a cor de preenchimento
    id_atrib=item.id_conf + QString("-color");
    if(item.cores[0]==item.cores[1])
     atribs[id_atrib]=item.cores[0].name();
    else
     atribs[id_atrib]=item.cores[0].name() + QString(",") + item.cores[1].name();

    //Cria um atributo o qual armazena a cor da borda
    id_atrib=item.id_conf + QString("-bcolor");
    atribs[id_atrib]=item.cores[2].name();
   }
   /* Caso o item atual seja um elemento de fonte do objeto porém não seja o
      elemento de configuração global de fonte */
   else if(item.id_conf!=ParsersAttributes::GLOBAL && !item.conf_obj)
   {
    //Obtém a fonte do item
    fonte=item.fmt_fonte.font();

    //Cria um atributo que armazena a cor da fonte
    id_atrib=item.id_conf + QString("-fcolor");
    atribs[id_atrib]=item.fmt_fonte.foreground().color().name();

    //Cria um atributo que armazena se a fonte está em itálico, negrito e sublinhado
    id_atrib=item.id_conf + QString("-") + ParsersAttributes::ITALIC;
    atribs[id_atrib]=(fonte.italic() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

    id_atrib=item.id_conf + QString("-") + ParsersAttributes::BOLD;
    atribs[id_atrib]=(fonte.bold() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

    id_atrib=item.id_conf + QString("-") + ParsersAttributes::UNDERLINE;
    atribs[id_atrib]=(fonte.underline() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
   }
   //Caso o item atual seja o elemento global de fonte
   else
   {
    //Cria dois atributos que armazenam o nome e o tamanho da fonte global do modelo
    atribs["font-name"]=QFontInfo(item.fmt_fonte.font()).family();
    atribs["font-size"]=QString("%1").arg(item.fmt_fonte.font().pointSizeF());
   }
  }

  //Especifica aos parâmetros de configuração de estilo de objetos os atributos configurados acima
  params_config[GlobalAttributes::OBJECTS_STYLE_CONF]=atribs;

  //Salva a configuração em arquivo
  ConfBaseWidget::salvarConfiguracao(GlobalAttributes::OBJECTS_STYLE_CONF);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConfAparenciaWidget::habilitarElemConfiguracao(void)
{
 QPalette pal;
 int idx=elemento_cmb->currentIndex();

 //Widgets que ficam habilitados somente quando o elemento global de fonte está selecionado
 fonte_cmb->setEnabled(idx==0);
 fonte_lbl->setEnabled(idx==0);
 tam_fonte_spb->setEnabled(idx==0);
 unidade_lbl->setEnabled(idx==0);

 /* Widgets que ficam habilitados somente quando o elemento atual não é o global e o
    mesmo também não é elemento de cor de objetos */
 sublinhado_chk->setEnabled(idx!=0 && !itens_conf[idx].conf_obj);
 negrito_chk->setEnabled(idx!=0 && !itens_conf[idx].conf_obj);
 italico_chk->setEnabled(idx!=0 && !itens_conf[idx].conf_obj);

 //Estes elementos ficam visíveis quando o elemento global não está selecionado
 cores_lbl->setVisible(idx!=0);
 cor1_tb->setVisible(idx!=0);

 //Estes widgets ficam visíveis somente quando o elemento atual é referente a cor de objetos
 cor2_tb->setVisible(itens_conf[idx].conf_obj);
 cor3_tb->setVisible(itens_conf[idx].conf_obj);

 //Bloqueia os sinais de todos os widgets para evitar que slots sejam executados antes do tempo
 cor1_tb->blockSignals(true);
 cor2_tb->blockSignals(true);
 cor3_tb->blockSignals(true);
 sublinhado_chk->blockSignals(true);
 italico_chk->blockSignals(true);
 negrito_chk->blockSignals(true);
 fonte_cmb->blockSignals(true);
 tam_fonte_spb->blockSignals(true);

 /* Caso o elemento atual refere-se a uma configuração de fonte,
    preenche os widgets com os dados da fonte */
 if(!itens_conf[idx].conf_obj)
 {
  QTextCharFormat fmt=BaseObjectView::getFontStyle(itens_conf[idx].id_conf);
  pal.setColor(QPalette::Button, fmt.foreground());
  cor1_tb->setPalette(pal);
  sublinhado_chk->setChecked(fmt.font().underline());
  italico_chk->setChecked(fmt.font().italic());
  negrito_chk->setChecked(fmt.font().bold());
  fonte_cmb->setCurrentFont(fmt.font());
  tam_fonte_spb->setValue(fmt.font().pointSizeF());
 }
 /* Caso o elemento atual seja de configuração de cor do objeto,
    preenche os botões de cores com as cores configuradas ao elemento */
 else
 {
  QColor cor1, cor2;

  BaseObjectView::getFillStyle(itens_conf[idx].id_conf, cor1, cor2);

  pal.setColor(QPalette::Button, cor1);
  cor1_tb->setPalette(pal);

  pal.setColor(QPalette::Button, cor2);
  cor2_tb->setPalette(pal);

  pal.setColor(QPalette::Button, BaseObjectView::getBorderStyle(itens_conf[idx].id_conf).color());
  cor3_tb->setPalette(pal);

  sublinhado_chk->setChecked(false);
  italico_chk->setChecked(false);
  negrito_chk->setChecked(false);
 }

 //Desbloqueia os sinais dos widgets
 cor1_tb->blockSignals(false);
 cor2_tb->blockSignals(false);
 cor3_tb->blockSignals(false);
 sublinhado_chk->blockSignals(false);
 italico_chk->blockSignals(false);
 negrito_chk->blockSignals(false);
 fonte_cmb->blockSignals(false);
 tam_fonte_spb->blockSignals(false);
}

void ConfAparenciaWidget::aplicarCorElemento(void)
{
 QToolButton *btn=dynamic_cast<QToolButton *>(sender());

 if(btn)
 {
  QPalette pal;
  unsigned idx_cor;

  //Executa o diálogo de seleção de cores
  pal=btn->palette();
  cor_dlg.setCurrentColor(pal.color(QPalette::Button));
  cor_dlg.exec();

  //Caso o usuário selecionou uma cor
  if(cor_dlg.result()==QDialog::Accepted)
  {
   //Preenche o botão acionado com a cor escolhida do diálogo de cores
   pal.setColor(QPalette::Button, cor_dlg.selectedColor());
   btn->setPalette(pal);

   //Caso seja uma configuração de cor de objetos
   if(itens_conf[elemento_cmb->currentIndex()].conf_obj)
   {
    //Conforme o botão acionado define-se o índice da cor a ser configurada
    if(btn==cor1_tb) idx_cor=0;
    else if(btn==cor2_tb) idx_cor=1;
    else idx_cor=3;

    //Atribui a cor configurada ao elemento atual
    itens_conf[elemento_cmb->currentIndex()].cores[idx_cor]=cor_dlg.selectedColor();
    BaseObjectView::setElementColor(itens_conf[elemento_cmb->currentIndex()].id_conf, cor_dlg.selectedColor(), idx_cor);
   }
   //Caso seja uma configuração de fonte
   else
   {
    //Atribui a cor selecionada   cor da fonte do elemento atual
    itens_conf[elemento_cmb->currentIndex()].fmt_fonte.setForeground(cor_dlg.selectedColor());
    BaseObjectView::setFontStyle(itens_conf[elemento_cmb->currentIndex()].id_conf,
                                      itens_conf[elemento_cmb->currentIndex()].fmt_fonte);
   }

   //Atualiza o modelo de exemplo para exibir as modificações de aparência
   modelo->setObjectsModified();
   cena->update();
  }
 }
}

void ConfAparenciaWidget::aplicarEstiloFonte(void)
{
 QFont fonte;

 //Configura uma fonte com os dados configurados no formulário
 fonte=fonte_cmb->currentFont();
 fonte.setBold(negrito_chk->isChecked());
 fonte.setItalic(italico_chk->isChecked());
 fonte.setUnderline(sublinhado_chk->isChecked());
 fonte.setPointSizeF(tam_fonte_spb->value());

 //Atribui a fonte configurada ao elemento
 itens_conf[elemento_cmb->currentIndex()].fmt_fonte.setFont(fonte);
 BaseObjectView::setFontStyle(itens_conf[elemento_cmb->currentIndex()].id_conf,
                                   itens_conf[elemento_cmb->currentIndex()].fmt_fonte);

 //Atualiza o modelo de exemplo para exibir as modificações de aparência
 modelo->setObjectsModified();
 cena->update();
}

void ConfAparenciaWidget::restaurarPadroes(void)
{
 try
 {
  //Restaura as configurações padrão e recarrega o arquivo restaurado
  ConfBaseWidget::restaurarPadroes(GlobalAttributes::OBJECTS_STYLE_CONF);
  this->carregarConfiguracao();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

