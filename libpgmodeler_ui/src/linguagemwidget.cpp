#include "linguagemwidget.h"

LinguagemWidget::LinguagemWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_LANGUAGE)
{
 try
 {
  map<QString, vector<QWidget *> > mapa_campos;
  QFrame *frame=NULL;
  QString chave_ver;

  Ui_LinguagemWidget::setupUi(this);

  sel_func_handler=NULL;
  sel_func_validator=NULL;
  sel_func_inline=NULL;

  sel_func_handler=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
  sel_func_validator=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
  sel_func_inline=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);

  linguagem_grid->addWidget(sel_func_handler,1,1,1,2);
  linguagem_grid->addWidget(sel_func_validator,2,1,1,2);
  linguagem_grid->addWidget(sel_func_inline,3,1,1,2);

  configurarLayouFormulario(linguagem_grid, OBJ_LANGUAGE);

  //Gera o frame de informação
  frame=gerarFrameInformacao(trUtf8("The functions to be assigned to the language should have, respectively, the following signatures:<br/><br/>  <strong>Handler Function:</strong> <em>language_handler function()</em><br/>  <strong>Validator Function:</strong> <em>void function(oid)</em><br/>  <strong>Inline Function:</strong> <em>void function(internal)</em>"));
  linguagem_grid->addWidget(frame, linguagem_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  //Define os campos exclusivos para cada versão
  chave_ver=gerarIntervaloVersoes(APOS_VERSAO, SchemaParser::PGSQL_VERSION_84);
  mapa_campos[chave_ver].push_back(func_inline_lbl);
  //Gera o frame de alerta
  frame=gerarFrameAlertaVersao(mapa_campos);
  linguagem_grid->addWidget(frame, linguagem_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->setMinimumSize(550, 510);
  janela_pai->setMaximumSize(16777215, 510);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void LinguagemWidget::hideEvent(QHideEvent *evento)
{
 sel_func_handler->removerObjetoSelecionado();
 sel_func_validator->removerObjetoSelecionado();
 sel_func_inline->removerObjetoSelecionado();
 confiavel_chk->setChecked(false);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}

void LinguagemWidget::definirAtributos(DatabaseModel *modelo, OperationList *lista_op, Language *linguagem)
{
 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, linguagem);

 /* Obtém as funções da linguagem e exibe suas assinaturas nas caixas
    de texto respectivas */
 sel_func_handler->definirModelo(modelo);
 sel_func_validator->definirModelo(modelo);
 sel_func_inline->definirModelo(modelo);

 if(linguagem)
 {
  //Marca o checkbox de confiável de acordo com o que está definido na linguagem
  confiavel_chk->setChecked(linguagem->isTrusted());
  sel_func_handler->definirObjeto(linguagem->getFunction(Language::HANDLER_FUNC));
  sel_func_validator->definirObjeto(linguagem->getFunction(Language::VALIDATOR_FUNC));
  sel_func_inline->definirObjeto(linguagem->getFunction(Language::INLINE_FUNC));
 }
}

void LinguagemWidget::aplicarConfiguracao(void)
{
 try
 {
  Language *linguagem=NULL;

  iniciarConfiguracao<Language>();

  //Converte o objeto para linguagem
  linguagem=dynamic_cast<Language *>(this->objeto);
  linguagem->setTrusted(confiavel_chk->isChecked());

  linguagem->setFunction(dynamic_cast<Function *>(sel_func_handler->obterObjeto()), Language::HANDLER_FUNC);
  linguagem->setFunction(dynamic_cast<Function *>(sel_func_validator->obterObjeto()), Language::VALIDATOR_FUNC);
  linguagem->setFunction(dynamic_cast<Function *>(sel_func_inline->obterObjeto()), Language::INLINE_FUNC);

  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Exception &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

