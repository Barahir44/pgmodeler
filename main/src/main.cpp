#include "formprincipal.h"
#include "application.h"
#include <QTranslator>

#ifndef Q_OS_WIN
 #include "execinfo.h"
#endif

void startCrashHandler(int)
{
 ofstream output;
 QString lin, cmd;

 /** At the moment the backtrace function does not exists on MingW (Windows) this way
     the code that generates the stacktrace is available only on Linux/Unix systems */
 #ifndef Q_OS_WIN
  void *stack[20];
  size_t stack_size, i;
  char **symbols=NULL;

  stack_size = backtrace(stack, 20);
  symbols = backtrace_symbols(stack, stack_size);
  cmd="crashhandler";
 #else
  cmd="crashhandler.exe";
 #endif

 //Creates the stacktrace file
 output.open(GlobalAttributes::TEMPORARY_DIR +
            GlobalAttributes::DIR_SEPARATOR +
            GlobalAttributes::STACKTRACE_FILE);

 if(output.is_open())
 {
  lin=QString("** pgModeler [v%1] crashed after receive signal: %2 **\n\nDate/Time:%3\n\n")
      .arg(GlobalAttributes::PGMODELER_VERSION)
      .arg("SIGSEGV")
      .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

  output.write(lin.toStdString().c_str(), lin.size());

  #ifndef Q_OS_WIN
   for(i=0; i < stack_size; i++)
   {
    lin=QString(symbols[i]) + QString("\n");
    output.write(lin.toStdString().c_str(), lin.size());
   }
   free(symbols);
  #else
    lin=QString("** Stack trace unavailable on Windows system **");
    saida.write(lin.toStdString().c_str(), lin.size());
  #endif

  output.close();
 }

 //Executes the crashhandler command (which must be on the same directory as the pgModeler executable)
 cmd=QApplication::applicationDirPath() + GlobalAttributes::DIR_SEPARATOR + cmd;
 system(cmd.toStdString().c_str());
 exit(1);
}


int main(int argc, char **argv)
{
 try
 {
  //Install a signal handler to start crashhandler when SIGSEGV is emitted
  signal(SIGSEGV, startCrashHandler);

  Application app(argc,argv);
  QTranslator translator;

  app.addLibraryPath(GlobalAttributes::PLUGINS_DIR);

  //Tries to load the ui translation according to the system's locale
  translator.load(QLocale::system().name(), GlobalAttributes::LANGUAGES_DIR);

  //Installs the translator on the application
  app.installTranslator(&translator);

  //Loading the application splash screen
  QPixmap pixmap(":imagens/imagens/pgmodeler_logo.png");
  QPixmap alfa(":imagens/imagens/pgmodeler_logo_alfa.png");
  pixmap.setAlphaChannel(alfa);

  //Draws the current version code on the splash
  QFont fnt;
  QPainter p;
  fnt.setFamily("Dejavu Sans");
  fnt.setBold(true);
  fnt.setPointSize(7.5f);

  QFontMetrics fm(fnt);
  QString str_ver=QString("v%1").arg(GlobalAttributes::PGMODELER_VERSION);
  QRect ret=fm.boundingRect(str_ver);

  p.begin(&pixmap);
  p.setFont(fnt);
  p.setPen(QColor(255,255,255));
  p.drawText(QPointF((pixmap.size().width()*0.55f)-(ret.width()/2),
                      pixmap.size().width()-17), str_ver);
  p.end();

  QSplashScreen splash(pixmap);
  splash.show();
  splash.repaint();

  //Creates the main form
  FormPrincipal fmain;
  app.setMainWidget(&fmain);

  //Indicating that the splash screen must be closed when the main window is shown
  splash.finish(&fmain);

  fmain.showMaximized();
  app.exec();

  return(0);
 }
 catch(Exception &e)
 {
  return(e.getErrorType());
 }
}
