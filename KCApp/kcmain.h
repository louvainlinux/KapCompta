#ifndef KCMAIN_H
#define KCMAIN_H

#include <QWidget>
#include <QList>

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
class KCWelcomeScreen;

class KCMain : public QWidget
{
    /*
     * Provide main logic of the application
     */
    Q_OBJECT

public:
    KCMain(QWidget *parent = 0);
    ~KCMain();
    /*
     * Shows the welcome screen
     */
    void start();

public slots:
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
