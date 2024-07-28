#include "CUtils.h"

#include <QPoint>
#include <QPointF>
#include <QColor>
#include <QFont>
#include <QTextStream>
#include <QIODevice>

#include <cmath>


QVariant CUtils::textToVariant(const QString& text, QMetaType::Type type)
{
    switch (type)
    {
	case QMetaType::QStringList:
		return text.split('|', Qt::SkipEmptyParts);

    case QMetaType::Int:
        return text.toInt();

    case QMetaType::Double:
        return text.toDouble();

	case QMetaType::Float:
		return text.toFloat();

    case QMetaType::Bool:
        if (text.toLower() == "true")
            return true;
        else
            return false;

	case QMetaType::QColor:
		return QColor(text);

	case QMetaType::QFont:
	{
		QFont f;
		f.fromString(text);
		return f;
	}

    default:
        return text;    // string
    }
}


QString CUtils::variantToText(const QVariant& v, QMetaType::Type type)
{
	if (type == 0)
		type = static_cast<QMetaType::Type>(v.metaType().id());

	switch (type)
	{
	case QMetaType::QPoint:
		return QString("%1;%2").arg(v.toPoint().x()).arg(v.toPoint().y());

	case QMetaType::QPointF:
		return QString("%1;%2").arg(v.toPointF().x()).arg(v.toPointF().y());

	case QMetaType::QSize:
		return QString("%1:%2").arg(v.toSize().width()).arg(v.toSize().height());

	case QMetaType::QSizeF:
		return QString("%1:%2").arg(v.toSizeF().width()).arg(v.toSizeF().height());

	case QMetaType::Bool:
		return v.toBool() ? "true" : "false";

	case QMetaType::Double:
		return QString::number(v.toDouble(), 'f', 4);

	case QMetaType::Float:
		return QString::number(v.value<float>(), 'f', 4);

	case QMetaType::QStringList:
		return v.toStringList().join('|');

	default:;
        return v.toString();
    }
}


Qt::PenStyle CUtils::textToPenStyle(const QString& text, Qt::PenStyle def)
{
	static QMap<QString, Qt::PenStyle> s_penStyles =
	{	{ "none", Qt::NoPen }, 
		{ "solid", Qt::SolidLine }, 
		{ "dashed", Qt::DashLine },
		{ "dotted", Qt::DotLine },
		{ "dashdot", Qt::DashDotLine },
		{ "dashdotdot", Qt::DashDotDotLine } 
	};

	if (s_penStyles.contains(text))
		return s_penStyles[text];
	else
        return def;
}


QString CUtils::penStyleToText(int style)
{
	switch (style)
	{
		case Qt::SolidLine:         return QStringLiteral("solid");
		case Qt::DashLine:          return QStringLiteral("dashed");
		case Qt::DotLine:           return QStringLiteral("dotted");
		case Qt::DashDotLine:       return QStringLiteral("dashdot");
		case Qt::DashDotDotLine:    return QStringLiteral("dashdotdot");
		default:					return QStringLiteral("none");
	}
}


QString CUtils::visToString(const QSet<QByteArray>& visIds)
{
	return QList<QByteArray>(visIds.begin(), visIds.end()).join('|');
}


QSet<QByteArray> CUtils::visFromString(const QString& text)
{
	auto split = text.toUtf8().split('|');
	return QSet<QByteArray>(split.begin(), split.end());
}


QStringList CUtils::byteArraySetToStringList(const QSet<QByteArray>& ids)
{
	QStringList sl;
	for (const auto& id : ids)
		sl << id;
	return sl;
}


QString CUtils::pointsToString(const QList<QPointF>& points)
{
	QString s;
	QTextStream ds(&s, QIODevice::WriteOnly);
	for (const auto& p : points)
	{
		float x = p.x();
		float y = p.y();
		ds << x << " " << y << " ";
	}

	return s;
}


QList<QPointF> CUtils::pointsFromString(const QString& text)
{
	QList<QPointF> pl;

	QString str(text.trimmed());
	if (str.size())
	{
		float x = 0, y = 0;
		QTextStream ds(&str);
		while (!ds.atEnd())
		{
			ds >> x;
			ds >> y;	// to do: check validity
			pl.append(QPointF(x, y));
		}
	}

	return pl;
}


QPointF CUtils::closestIntersection(const QLineF& line, const QPolygonF& endPolygon)
{
	QPointF intersectPoint;
	QPointF p1 = endPolygon.first();
	QPointF p2;

	for (int i = 1; i < endPolygon.count(); ++i) 
	{
		p2 = endPolygon.at(i);
		QLineF polyLine = QLineF(p1, p2);
		QLineF::IntersectType intersectType = polyLine.intersects(line, &intersectPoint);
		if (intersectType == QLineF::BoundedIntersection)
			return intersectPoint;

		p1 = p2;
	}

	return QPointF();
}


QString CUtils::cutLastSuffix(const QString& fileName)
{
	int idx = fileName.lastIndexOf(".");
	if (idx < 0)
		return fileName;
	else
		return fileName.left(idx);
}


QRectF CUtils::getBoundingRect(const QList<QGraphicsItem*>& items)
{
	QRectF r;

	for (const auto item : items)
		r |= item->sceneBoundingRect();

	return r;
}


QLineF CUtils::extendLine(const QLineF& line, float fromStart, float fromEnd)
{
	QPointF v(line.p2().x() - line.p1().x(), line.p2().y() - line.p1().y());

    float l = std::sqrt(v.x() * v.x() + v.y() * v.y());

	v.setX(v.x() / l);
	v.setY(v.y() / l);

	return QLineF(
		line.p1().x() - v.x() * fromStart, 
		line.p1().y() - v.y() * fromStart,
		line.p2().x() - v.x() * fromEnd,
		line.p2().y() - v.y() * fromEnd
	);
}

