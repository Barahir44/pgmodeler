#include "parametrowidget.h"

ParametroWidget::ParametroWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_PARAMETER)
{
 try
 {
  QGridLayout *parametro_grid=NULL;
  QSpacerItem *hspacer=NULL;

  Ui_ParametroWidget::setupUi(this);

  tipo_pgsql=NULL;
  tipo_pgsql=new TipoPgSQLWidget(this);
  parametro_grid=new QGridLayout(this);
  hspacer=new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  parametro_grid->setContentsMargins(0, 0, 0, 0);

  parametro_grid->addWidget(valorpadrao_lbl, 0, 0, 1, 1);
  parametro_grid->addWidget(valorpadrao_edt, 0, 1, 1, 3);
  parametro_grid->addWidget(modo_lbl, 1, 0, 1, 1);
  parametro_grid->addWidget(param_in_chk, 1, 1, 1, 1);
  parametro_grid->addWidget(param_out_chk, 1, 2, 1, 1);
  parametro_grid->addItem(hspacer, 1, 3, 1, 1);
  parametro_grid->addWidget(tipo_pgsql,2,0,1,4);

  configurarLayouFormulario(parametro_grid, OBJ_PARAMETER);
  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->setMinimumSize(500, 270);
  janela_pai->setMaximumSize(16777215, 270);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ParametroWidget::hideEvent(QHideEvent *evento)
{
 //Reseta os campos do formulário ao esconder o mesmo
 param_in_chk->setChecked(false);
 param_out_chk->setChecked(false);
 valorpadrao_edt->clear();
 ObjetoBaseWidget::hideEvent(evento);
}

void ParametroWidget::definirAtributos(Parameter param, DatabaseModel *modelo)
{
 this->parametro=param;

 //Configura os campos do formulário com os valores do parâmetro passado
 param_in_chk->setChecked(param.isIn());
 param_out_chk->setChecked(param.isOut());
 valorpadrao_edt->setText(QString::fromUtf8(param.getDefaultValue()));
 tipo_pgsql->definirAtributos(param.getType(), modelo);

 ObjetoBaseWidget::definirAtributos(modelo,NULL,&this->parametro);
}

void ParametroWidget::aplicarConfiguracao(void)
{
 try
 {
  //iniciarConfiguracao<Parametro>();

  //Configura o parâmetro com os valores do formulário
  parametro.setIn(param_in_chk->isChecked());
  parametro.setOut(param_out_chk->isChecked());
  parametro.setDefaultValue(valorpadrao_edt->text());
  parametro.setType(tipo_pgsql->obterTipoPgSQL());

  /* Efetiva as configurações chamando o método de
     aplicação das configurações da classe pai */
  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Exception &e)
 {
  cancelarConfiguracao();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

Parameter ParametroWidget::obterParametro(void)
{
 return(parametro);
}

