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
    // Redefined to allow the application to save its data before closing
    void closeEvent(QCloseEvent *event);
private slots:
    // Open the project at path p
    void openProject(QString p);
    // Called whenever the user selects another KCPanel in the QListWidget
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    // Build the GUI -- postponed from the constructor as it first needs to open a project
    void buildGUI();
    // Load addons from the plugins
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
