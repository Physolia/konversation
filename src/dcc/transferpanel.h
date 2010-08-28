/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  begin:     Mit Aug 7 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com
*/
/*
  Copyright (C) 2004-2007 Shintaro Matsuoka <shin@shoegazed.org>
  Copyright (C) 2009 Bernd Buschinski <b.buschinski@web.de>
*/

#ifndef TRANSFERPANEL_H
#define TRANSFERPANEL_H

#include "chatwindow.h"

#include <QModelIndex>
#include <QItemSelection>

#include <kdeversion.h>

#if KDE_IS_VERSION(4, 5, 0)
#   include <KDialog>
#   include <KLocalizedString>
#   include <kfilemetadatawidget.h>
#endif
#include <KDebug>

class QSplitter;
class KMenu;
class KToolBar;

namespace Konversation
{
    namespace DCC
    {
        class TransferDetailedInfoPanel;
        class TransferView;
        class Transfer;

#if KDE_IS_VERSION(4, 5, 0)
        class FileMetaDataDialog : public KDialog
        {
            public:
                FileMetaDataDialog(const KUrl& file, QWidget *parent = 0)
                    : KDialog(parent)
                {
                    setCaption( i18nc("File Information for %1", "%1=filename", file.fileName() ) );
                    setButtons( KDialog::Ok );

                    m_fileMetaDataWidget = new KFileMetaDataWidget(this);

                    KFileItemList fileList;
                    fileList.append(KFileItem(KFileItem::Unknown, KFileItem::Unknown, file));
                    m_fileMetaDataWidget->setItems(fileList);

                    setMainWidget(m_fileMetaDataWidget);

                    //known Qt problem, minium size is not set, limitation of X11 window manager
                    setMinimumSize(QSize(sizeHint().height()*2, sizeHint().width()));
                }

                ~FileMetaDataDialog()
                {
                    delete m_fileMetaDataWidget;
                }

            private:
                KFileMetaDataWidget* m_fileMetaDataWidget;
        };
#endif

        class TransferPanel : public ChatWindow
        {
            Q_OBJECT

            public:
                TransferPanel(QWidget *parent);
                ~TransferPanel();

                TransferView *getTransferView();

                void runFile(Transfer *transfer);
                void openLocation(Transfer *transfer);
                void openFileInfoDialog(Transfer *transfer);

            protected slots:
                void slotNewTransferAdded(Konversation::DCC::Transfer *transfer);
                void slotTransferStatusChanged();

                void acceptDcc();
                void abortDcc();
                void resendFile();
                void clearDcc();
                void clearCompletedDcc();
                void runDcc();
                void openLocation();
                void showFileInfo();
                void selectAll();
                void selectAllCompleted();

                void popupRequested (const QPoint &pos);

                void doubleClicked(const QModelIndex &index);

                void updateButton();

                void setDetailPanelItem (const QItemSelection &newindex, const QItemSelection &oldindex);

            protected:
                /** Called from ChatWindow adjustFocus */
                virtual void childAdjustFocus();

            private:
                inline void initGUI();

                TransferView *m_transferView;
                KMenu *m_popup;
                KToolBar *m_toolBar;

                TransferDetailedInfoPanel *m_detailPanel;
                QSplitter *m_splitter;

                QAction *m_abort;
                QAction *m_accept;
                QAction *m_clear;
                QAction *m_clearCompleted;
                QAction *m_info;
                QAction *m_open;
                QAction *m_openLocation;
                QAction *m_selectAll;
                QAction *m_selectAllCompleted;
                QAction *m_resend;
        };
    }
}

#endif
