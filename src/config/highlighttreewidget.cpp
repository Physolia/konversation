/*
    SPDX-License-Identifier: GPL-2.0-or-later

    SPDX-FileCopyrightText: 2017 Peter Simonsson <peter.simonsson@gmail.com>
*/

#include "highlighttreewidget.h"

#include <QDropEvent>

HighlightTreeWidget::HighlightTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
}

void HighlightTreeWidget::dropEvent(QDropEvent *event)
{
    QTreeWidget::dropEvent(event);

    if (event->isAccepted())
        emit itemDropped();
}
