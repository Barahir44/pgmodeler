#include "esquemawidget.h"

EsquemaWidget::EsquemaWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_SCHEMA)
{
 configurarLayouFormulario(NULL, OBJ_SCHEMA);

 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

 janela_pai->setMinimumSize(500, 220);
 janela_pai->setMaximumSize(16777215, 220);
}

void EsquemaWidget::definirAtributos(DatabaseModel *modelo, OperationList *lista_op, Schema *esquema)
{
 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, esquema);
}

void EsquemaWidget::aplicarConfiguracao(void)
{
 try
 {
  iniciarConfiguracao<Schema>();
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

