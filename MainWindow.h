#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "FindDialog.h"

class QAction;
class QMenu;
class QTextEdit;
class QWidget;
class QString;
class QStringList;
class QFont;
class QFontDialog;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void newFile();
	void openFile();
	void saveFile();
	void saveAsFile();
	void openRecentFile();
	void clearRecentFiles();

	void showFindDialog();
	void findString(const QString &str, 
		Qt::CaseSensitivity cs, 
		FindDialog::FindDirection fd, 
		FindDialog::WholeWord mw);

	void showFontDialog();
	void setFont(const QFont & f);

private:
	void createMenus();
	void createActions();
	void createMainWidget();
	void writeSettings();
	void readSettings();

	bool okToContinue();
	bool load(const QString &fileName);
	bool save(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	void updateRecentFileActions();

	//QWidget * mainWidget = nullptr;
	QTextEdit * mainTextEdit = nullptr;
	const QString programName = tr("Text Editer");

	//File²Ëµ¥
	QMenu * fileMenu = nullptr;
	QAction * newAction = nullptr;
	QAction * openAction = nullptr;
	QAction * saveAction = nullptr;
	QAction * saveAsAction = nullptr;
	static const int MaxRecentFiles = 5;
	QAction * recentFileActions[MaxRecentFiles] = { nullptr };
	QAction * recentFileSparatorAction = nullptr;
	QAction * clearRecentFilesAction = nullptr;
	QAction * exitAction = nullptr;

	QStringList recentFiles;
	QString curFile;

	//Edit²Ëµ¥
	QMenu * editMenu = nullptr;
	QAction * findAction = nullptr;
	FindDialog * findDialog = nullptr;

	//Style²Ëµ¥
	QMenu *styleMenu = nullptr;
	QAction * fontAction = nullptr;
	QFontDialog * fontDialog = nullptr;
	QFont font;
};

#endif // MAINWINDOW_H
