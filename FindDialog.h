#pragma once

#include <QtWidgets\qdialog.h>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class FindDialog : public QDialog
{
	Q_OBJECT
public:
	FindDialog(QWidget *parent = 0);
	~FindDialog();

	enum FindDirection {
		Forward, Backward
	};
	enum WholeWord {
		Normal, WholeOnly
	};

signals:
	void find(const QString &str, Qt::CaseSensitivity cs, FindDirection fd, WholeWord mw);
private slots:
	void findClicked();
	void enableFindButton(const QString &text);
private:
	QLabel *label = nullptr;
	QLineEdit *lineEdit = nullptr;
	QCheckBox *caseCheckBox = nullptr;
	QCheckBox *backwardCheckBox = nullptr;
	QCheckBox *wholeCheckBox = nullptr;
	QPushButton *findButton = nullptr;
	QPushButton *closeButton = nullptr;
};

