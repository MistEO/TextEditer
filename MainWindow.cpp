#include "mainwindow.h"
#include <QtWidgets\qaction.h>
#include <QtWidgets\qmenu.h>
#include <QtWidgets\qmenubar.h>
#include <QtWidgets\qtextedit.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qwidget.h>
#include <QtWidgets\qmessagebox.h>
#include <QtCore\qstring.h>
#include <QtCore\qtextstream.h>
#include <QtWidgets\qfiledialog.h>
#include <QtGui\qevent.h>
#include <QtGui\qfont.h>
#include <QtCore\qstringlist.h>
#include <QtCore\qfileinfo.h>
#include <QtCore\qsettings.h>
#include <QtGui\qtextcursor.h>
#include <QtCore\qtextstream.h>
#include <QtCore\qflags.h>
#include <QtGui\qpalette.h>
#include <QtWidgets\qfontdialog.h>
#include <QtCore\qlist.h>
#include <QtCore\qurl.h>
#include <qevent.h>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	createActions();
	createMenus();
	createMainWidget();

	setCurrentFile("");
	readSettings();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent * event)
{
	if (okToContinue()) {
		event->accept();
	}
	else {
		event->ignore();
	}
	writeSettings();
}

void MainWindow::createMenus()
{
	//File菜单
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	for (int i = 0; i != MaxRecentFiles; ++i) {
		fileMenu->addAction(recentFileActions[i]);
	}
	fileMenu->addAction(clearRecentFilesAction);
	recentFileSparatorAction = fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	//Edit菜单
	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(findAction);

	//Style菜单
	styleMenu = menuBar()->addMenu(tr("&Style"));
	styleMenu->addAction(fontAction);
}

void MainWindow::createActions()
{
	//File菜单项
	newAction = new QAction(tr("&New"), this);
	newAction->setShortcut(QKeySequence::New);
	connect(newAction, &QAction::triggered,
		this, &MainWindow::newFile);

	openAction = new QAction(tr("&Open"), this);
	openAction->setShortcut(QKeySequence::Open);
	connect(openAction, &QAction::triggered,
		this, &MainWindow::openFile);

	saveAction = new QAction(tr("&Save"), this);
	saveAction->setShortcut(QKeySequence::Save);
	connect(saveAction, &QAction::triggered,
		this, &MainWindow::saveFile);

	saveAsAction = new QAction(tr("Save &As"), this);
	saveAsAction->setShortcut(QKeySequence::SaveAs);
	connect(saveAsAction, &QAction::triggered,
		this, &MainWindow::saveAsFile);

	for (int i = 0; i != MaxRecentFiles; ++i) {
		recentFileActions[i] = new QAction(this);
		recentFileActions[i]->setVisible(true);
		connect(recentFileActions[i], &QAction::triggered,
			this, &MainWindow::openRecentFile);
	}

	clearRecentFilesAction = new QAction(tr("Clear &Recent Files"), this);
	connect(clearRecentFilesAction, &QAction::triggered,
		this, &MainWindow::clearRecentFiles);

	exitAction = new QAction(tr("&Exit"), this);
	exitAction->setShortcut(QKeySequence::Close);
	connect(exitAction, &QAction::triggered,
		this, &MainWindow::close);

	//Edit菜单项
	findAction = new QAction(tr("&Find"), this);
	findAction->setShortcut(QKeySequence::Find);
	connect(findAction, &QAction::triggered,
		this, &MainWindow::showFindDialog);

	//Style菜单项
	fontAction = new QAction(tr("&Font"), this);
	connect(fontAction, &QAction::triggered,
		this, &MainWindow::showFontDialog);
}

void MainWindow::createMainWidget()
{
	mainTextEdit = new QTextEdit;
	this->setCentralWidget(mainTextEdit);

	//QHBoxLayout * mainLayout = new QHBoxLayout;
	//mainLayout->addWidget(mainTextEdit);
	//mainWidget = new QWidget;
	//mainWidget->setLayout(mainLayout);
	//this->centralWidget()->setLayout(mainLayout);

	//拖拽事件交于MainWindow管理
	//setCentralWidget(mainTextEdit);
	//mainTextEdit->setAcceptDrops(false);
	//setAcceptDrops(true);
}

void MainWindow::newFile()
{
	if (okToContinue()) {
		mainTextEdit->clear();
		setCurrentFile("");
	}
}
void MainWindow::openFile()
{
	if (okToContinue()) {
		QString fileName = QFileDialog::getOpenFileName(
			this,
			tr("Open File"),			//FileDialogTitle
			".",						//默认打开路径
			tr("Text Files (*.txt)\n"	//文件类型
				"Cpp Files (*.c *.cpp)"));
		if (!fileName.isEmpty()) {
			load(fileName);
		}
	}
}

void MainWindow::saveFile()
{
	if (!curFile.isEmpty()) {
		save(curFile);
	}
	else {
		saveAsFile();
	}
}

void MainWindow::saveAsFile()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save As File"),
		".",
		tr("Text Files (*.txt)\n"
			"Cpp Files (*.c *.cpp)"));
	if (!fileName.isEmpty()) {
		save(fileName);
	}
}

void MainWindow::openRecentFile()
{
	if (okToContinue()) {
		QAction *action = qobject_cast<QAction *>(sender());
		if (action) {
			load(action->data().toString());
		}
	}
}

void MainWindow::clearRecentFiles()
{
	recentFiles.clear();
	updateRecentFileActions();
	writeSettings();
}

void MainWindow::showFindDialog()
{
	if (!findDialog) {
		findDialog = new FindDialog(this);
		connect(findDialog, &FindDialog::find,
			this, &MainWindow::findString);
	}
	findDialog->show();
	findDialog->raise();
	findDialog->activateWindow();
}

void MainWindow::findString(const QString &str, Qt::CaseSensitivity cs, FindDialog::FindDirection fd, FindDialog::WholeWord mw)
{
	QTextCursor mainTextCursor = mainTextEdit->textCursor();
	QFlags<QTextDocument::FindFlag> findOptions;
	if (cs == Qt::CaseSensitive) {
		findOptions.setFlag(QTextDocument::FindCaseSensitively);
	}
	if (mw == FindDialog::WholeOnly) {
		findOptions.setFlag(QTextDocument::FindWholeWords);
	}
	if (fd == FindDialog::Backward) {
		findOptions.setFlag(QTextDocument::FindBackward);
		int keyPos = mainTextEdit->document()->find(str, mainTextCursor.position(), findOptions).position();
		mainTextCursor.setPosition(keyPos, QTextCursor::MoveAnchor);
		mainTextCursor.setPosition(keyPos - str.length(), QTextCursor::KeepAnchor);
	}
	else {
		int keyPos = mainTextEdit->document()->find(str, mainTextCursor.position(), findOptions).position() - str.length();
		mainTextCursor.setPosition(keyPos, QTextCursor::MoveAnchor);
		mainTextCursor.setPosition(keyPos + str.length(), QTextCursor::KeepAnchor);
	}
	mainTextEdit->setTextCursor(mainTextCursor);
}

void MainWindow::showFontDialog()
{
	if (!fontDialog) {
		fontDialog = new QFontDialog(this);
		connect(fontDialog, &QFontDialog::fontSelected,
			this, &MainWindow::setFont);
		connect(fontDialog, &QFontDialog::fontSelected,
			this, [&]()-> void {font = fontDialog->selectedFont(); });
	}
	fontDialog->show();
	fontDialog->raise();
	fontDialog->activateWindow();
}

void MainWindow::setFont(const QFont & f)
{
	mainTextEdit->setFont(f);
}

bool MainWindow::okToContinue()
{
	if (mainTextEdit->document()->isModified()) {
		int r = QMessageBox::warning(this, tr("TextEditer"),
			tr("The document has been modified,\nDo you want ot save your changes?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (r == QMessageBox::Yes) {
			return save(curFile);
		}
		if (r == QMessageBox::Cancel) {
			return false;
		}
	}
	return true;
}

bool MainWindow::load(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly
		| QIODevice::Text)) {
		QMessageBox::critical(this, tr("Error"),
			"Cannt Open File!");
		return false;
	}
	QTextStream txtInput(&file);
	mainTextEdit->clear();
	while (!txtInput.atEnd()) {
		QString lineStr = txtInput.readLine();
		mainTextEdit->append(lineStr);
	}
	setCurrentFile(fileName);
	file.close();
	return true;
}

bool MainWindow::save(const QString & fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly
		| QIODevice::Text)) {
		QMessageBox::critical(this, tr("Error"),
			"Cannt Save File!");
		return false;
	}
	QTextStream txtOutput(&file);
	QString txt = mainTextEdit->document()->toPlainText();
	txtOutput << txt;
	setCurrentFile(fileName);
	file.close();
	return true;
}

void MainWindow::setCurrentFile(const QString & fileName)
{
	mainTextEdit->document()->setModified(false);
	curFile = fileName;
	QString showName;
	if (!fileName.isEmpty()) {
		showName = QFileInfo(fileName).fileName();
		recentFiles.removeAll(curFile);
		recentFiles.prepend(curFile);
		updateRecentFileActions();
	}
	else {
		showName = tr("Untitled");
	}
	setWindowTitle(tr("%1 - %2").arg(showName).arg(programName));
}

void MainWindow::updateRecentFileActions()
{
	for (int i = 0; i != MaxRecentFiles; ++i) {
		if (i < recentFiles.count()) {
			QString text = tr("%1: %2")
				.arg(i + 1)
				.arg(QFileInfo(recentFiles[i]).fileName());
			recentFileActions[i]->setText(text);
			recentFileActions[i]->setData(recentFiles[i]);
			recentFileActions[i]->setVisible(true);
		}
		else {
			recentFileActions[i]->setVisible(false);
		}
	}
	recentFileSparatorAction->setVisible(!recentFiles.isEmpty());
	clearRecentFilesAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::writeSettings()
{
	QSettings settings("MrEO Inc.", "Text Editer");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("recentFiles", recentFiles);
	settings.setValue("font", font);
}

void MainWindow::readSettings()
{
	QSettings settings("MrEO Inc.", "Text Editer");
	restoreGeometry(settings.value("geometry").toByteArray());

	recentFiles = settings.value("recentFiles").toStringList();
	updateRecentFileActions();

	font = settings.value("font", QVariant(QFont("新宋体", 20))).value<QFont>();
	mainTextEdit->setFont(font);
}