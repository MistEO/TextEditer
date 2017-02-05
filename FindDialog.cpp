#include "FindDialog.h"
#include <QtWidgets\qcheckbox.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qlineedit.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qboxlayout.h>


FindDialog::FindDialog(QWidget *parent)
	:QDialog(parent)
{
	label = new QLabel("Find :");
	lineEdit = new QLineEdit;
	label->setBuddy(lineEdit);

	caseCheckBox = new QCheckBox(tr("Match &case")); //tr()函数为将字符串翻译为其他语言的标记
	backwardCheckBox = new QCheckBox(tr("Search &backward"));
	wholeCheckBox = new QCheckBox(tr("Match &whole word"));

	findButton = new QPushButton(tr("&Find"));
	findButton->setDefault(true);
	findButton->setEnabled(false);

	closeButton = new QPushButton(tr("Close"));

	connect(lineEdit, &QLineEdit::textChanged,
		this, &FindDialog::enableFindButton);
	connect(findButton, &QPushButton::clicked, this, &FindDialog::findClicked);
	connect(closeButton, &QPushButton::clicked, this, &FindDialog::close);

	//左上部分布局
	QHBoxLayout *topLeftLayout = new QHBoxLayout;
	topLeftLayout->addWidget(label);
	topLeftLayout->addWidget(lineEdit);

	//左半部分布局
	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addLayout(topLeftLayout);
	leftLayout->addWidget(caseCheckBox);
	leftLayout->addWidget(backwardCheckBox);
	leftLayout->addWidget(wholeCheckBox);

	//右半部分布局
	QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout->addWidget(findButton);
	rightLayout->addWidget(closeButton);
	rightLayout->addStretch(); //添加分隔符（伸展器）

	//主布局
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);
	setLayout(mainLayout);

	setWindowTitle(tr("Find"));
	setFixedHeight(sizeHint().height()); //固定窗口高度
}

FindDialog::~FindDialog()
{
}


void FindDialog::findClicked()
{
	QString text = lineEdit->text();
	Qt::CaseSensitivity cs =
		caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
	FindDirection fd = backwardCheckBox->isChecked() ? FindDialog::Backward : FindDialog::Forward;
	WholeWord mw = wholeCheckBox->isChecked() ? FindDialog::WholeOnly : FindDialog::Normal;
	emit find(text, cs, fd, mw);
}

void FindDialog::enableFindButton(const QString &text)
{
	findButton->setEnabled(!text.isEmpty());
}