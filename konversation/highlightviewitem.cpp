/***************************************************************************
                          highlightviewitem.cpp  -  description
                             -------------------
    begin                : Sat Jun 15 2002
    copyright            : (C) 2002 by Matthias Gierlings
    email                : gismore@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "highlightviewitem.h"

#include <kurl.h>

HighlightViewItem::HighlightViewItem(QListView* parent, Highlight* passed_Highlight)
  : QCheckListItem(parent, QString::null,QCheckListItem::CheckBox)
{
  setText(1,passed_Highlight->getPattern());
  itemColor = passed_Highlight->getColor();
  itemID = passed_Highlight->getID();
  setSoundURL(passed_Highlight->getSoundURL());
  setAutoText(passed_Highlight->getAutoText());
  setOn(passed_Highlight->getRegExp());
}

HighlightViewItem::~HighlightViewItem()
{
}

void HighlightViewItem::paintCell(QPainter* p, const QColorGroup &cg, int column, int width, int alignment)
{
  // copy all colors from cg and only then change needed colors
  itemColorGroup=cg;
  itemColorGroup.setColor(QColorGroup::Text, itemColor);
  QCheckListItem::paintCell(p, itemColorGroup, column, width, alignment);
}

HighlightViewItem* HighlightViewItem::itemBelow()
{
  return (HighlightViewItem*) QCheckListItem::itemBelow();
}

void HighlightViewItem::setPattern(const QString& newPattern) { setText(1,newPattern); }
QString HighlightViewItem::getPattern()                       { return text(1); }

void HighlightViewItem::setSoundURL(const KURL& url)
{
  soundURL = url;
  setText(2, soundURL.prettyURL());
}

void HighlightViewItem::setAutoText(const QString& newAutoText)
{
  autoText = newAutoText;
  setText(3,newAutoText);
}

bool HighlightViewItem::getRegExp()
{
  return isOn();
}

QString HighlightViewItem::getAutoText()
{
  return autoText;
}
