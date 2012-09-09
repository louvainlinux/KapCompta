#ifndef KCMAIN_H
#define KCMAIN_H

#include <QWidget>

class KCWelcomeScreen;
QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

class KCFileManager;

class KCMain : public QWidget
{
    Q_OBJECT
    
public:
    KCMain(QWidget *parent = 0);
    ~KCMain();
    void start();

public slots:
    void openProject(QString p);
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void buildGUI();

    KCWelcomeScreen *welcomeScreen;
    QString *projectPath;
    QListWidget *wList;
    QStackedWidget *sidePanel;
    KCFileManager *fileManager;
};

#endif // KCMAIN_H
