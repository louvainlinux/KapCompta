#ifndef KCMAIN_H
#define KCMAIN_H

#include <QWidget>
#include <QList>

class KCWelcomeScreen;
QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QGroupBox;
class KCPlugin;
class KCPanel;
class QCloseEvent;
QT_END_NAMESPACE

class KCFileManager;

class KCMain : public QWidget
{
    Q_OBJECT

public:
    KCMain(QWidget *parent = 0);
    ~KCMain();
    void start();
    void saveAllPanels();

protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void openProject(QString p);
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void buildGUI();
    void extractPluginList();

    KCWelcomeScreen *welcomeScreen;
    QString *projectPath;
    QListWidget *wList;
    QStackedWidget *sidePanel;
    KCFileManager *fileManager;
    QGroupBox *box;
    QList<KCPlugin*> plugins;
    QList<KCPanel*> panels;
};

#endif // KCMAIN_H
