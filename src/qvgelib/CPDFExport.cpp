/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2020 Ars L.Masiuk(ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include <QPainter> 
#include <QPdfWriter> 
#include <QPageSize> 
#include <QPageLayout> 
#include <QMarginsF> 
#include <QDebug> 
#include <QScopedPointer>

#include "CPDFExport.h"
#include "CEditorScene.h"


CPDFExport::CPDFExport()
{
#ifdef Q_OS_WIN32
	m_printer = m_pageDialog.printer();
#else
    m_printer = new QPrinter;
#endif

    m_printer->setOutputFormat(QPrinter::NativeFormat);
}


CPDFExport::~CPDFExport()
{
#ifdef Q_OS_WIN32
#else
	delete m_printer;
#endif
}


// impl: setup interface

void CPDFExport::readSettings(QSettings& settings)
{
	settings.beginGroup("PDFExport");

	auto size = settings.value("PaperSize").toSize();
	QString paperName = settings.value("PaperName").toString();

	m_printer->setPageSize(QPageSize(size, paperName));

	QMarginsF mm;
	mm.setLeft(settings.value("MarginLeft").toDouble());
	mm.setRight(settings.value("MarginRight").toDouble());
	mm.setTop(settings.value("MarginTop").toDouble());
	mm.setBottom(settings.value("MarginBottom").toDouble());
	m_printer->setPageMargins(mm);

	//QMarginsF mmf(mm.left, mm.top, mm.right, mm.bottom);
	//QPageLayout pl(pageSize, QPageLayout::Portrait, mmf);
	//m_printer->setPageLayout(pl);

	settings.endGroup();
}


void CPDFExport::writeSettings(QSettings& settings)
{
	settings.beginGroup("PDFExport");

	auto size = m_printer->pageLayout().pageSize().sizePoints();
	settings.setValue("PaperSize", size);

	QString paper = m_printer->pageLayout().pageSize().name();
	settings.setValue("PaperName", paper);

	auto mm = m_printer->pageLayout().margins();
	settings.setValue("MarginLeft", mm.left());
	settings.setValue("MarginRight", mm.right());
	settings.setValue("MarginTop", mm.top());
	settings.setValue("MarginBottom", mm.bottom());
	settings.endGroup();
	settings.sync();
}


bool CPDFExport::setupDialog(CEditorScene& scene)
{
	auto bbox = scene.itemsBoundingRect();
	if (bbox.width() > bbox.height())
		m_printer->pageLayout().setOrientation(QPageLayout::Landscape);
	else
		m_printer->pageLayout().setOrientation(QPageLayout::Portrait);

#ifdef Q_OS_WIN32
	if (m_pageDialog.exec() == QDialog::Rejected)
		return false;
#else
    QPageSetupDialog pd(m_printer);
    if (pd.exec() == QDialog::Rejected)
        return false;
#endif

	return true;
}


// impl: IFileSerializer

bool CPDFExport::save(const QString& fileName, CEditorScene& scene, QString* /*lastError*/) const
{
	Q_ASSERT(m_printer);

	QScopedPointer<CEditorScene> tempScene(scene.clone());

	tempScene->crop();

	QPdfWriter writer(fileName);
	writer.setPageSize(m_printer->pageLayout().pageSize());
	writer.setPageOrientation(m_printer->pageLayout().orientation());
	writer.pageLayout().setMargins(m_printer->pageLayout().margins());

	QPainter painter(&writer);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	tempScene->render(&painter);
	painter.end();

	return true;
}
