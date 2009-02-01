/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  copyright: (C) 2004 by Peter Simonsson
  email:     psn@linux.se
*/
#include "identitydialog.h"
#include "application.h"
#include "awaymanager.h"
#include "irccharsets.h"

#include <KDialog>
#include <KMessageBox>
#include <KInputDialog>

namespace Konversation
{

    // FIXME: One of the latest changes introduced a crash on duplication
    // TODO: dialog closes if an error with a new identity occurs
    //    (e.g.: on missing items, after showing the KMessageBox)
    IdentityDialog::IdentityDialog(QWidget *parent)
        : KDialog(parent)
    {
        setCaption( i18n("Identities") );
        setButtons( Ok|Cancel );
        setDefaultButton( Ok );

        // Initialize the dialog widget
        QWidget* w = new QWidget(this);
        setupUi(w);
        setMainWidget(w);

        newBtn->setIcon(KIcon("list-add"));
        connect(newBtn, SIGNAL(clicked()), this, SLOT(newIdentity()));
        
        copyBtn->setIcon(KIcon("edit-copy"));
        connect(copyBtn, SIGNAL(clicked()), this, SLOT(copyIdentity()));
        
        m_editBtn->setIcon(KIcon("edit-rename"));
        connect(m_editBtn, SIGNAL(clicked()), this, SLOT(renameIdentity()));
        
        m_delBtn->setIcon(KIcon("edit-delete"));
        connect(m_delBtn, SIGNAL(clicked()), this, SLOT(deleteIdentity()));

        foreach(IdentityPtr id, Preferences::identityList()) {
            m_identityCBox->addItem(id->getName());
            m_identityList.append( IdentityPtr( id ) );
        }
        
        // add encodings to combo box
        m_codecCBox->insertItems(0, Konversation::IRCCharsets::self()->availableEncodingDescriptiveNames());

        // set values for the widgets
        updateIdentity(0);

        // Set up signals / slots for identity page
        //connect(m_identityCBox, SIGNAL(activated(int)), this, SLOT(updateIdentity(int)));

        setButtonGuiItem(KDialog::Ok, KGuiItem(i18n("&OK"), "dialog-ok", i18n("Change identity information")));
        setButtonGuiItem(KDialog::Cancel, KGuiItem(i18n("&Cancel"), "dialog-cancel", i18n("Discards all changes made")));

        AwayManager* awayManager = static_cast<KonversationApplication*>(kapp)->getAwayManager();
        connect(m_identityCBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIdentity(int)));
        connect(this, SIGNAL(identitiesChanged()), awayManager, SLOT(identitiesChanged()));
        connect(this, SIGNAL( okClicked() ), this, SLOT( slotOk() ) );
    }

    void IdentityDialog::updateIdentity(int index)
    {
        if(m_currentIdentity && (m_nicknameLBox->count() == 0))
        {
            KMessageBox::error(this, i18n("You must add at least one nick to the identity."));
            m_identityCBox->setItemText(0, m_currentIdentity->getName());
            return;
        }

        if (isVisible() && m_currentIdentity && m_realNameEdit->text().isEmpty())
        {
            KMessageBox::error(this, i18n("Please enter a real name."));
            m_identityCBox->setItemText(0, m_currentIdentity->getName());
            return;
        }

        refreshCurrentIdentity();

        m_currentIdentity = m_identityList[index];

        m_realNameEdit->setText(m_currentIdentity->getRealName());
        m_nicknameLBox->clear();
        m_nicknameLBox->insertStringList(m_currentIdentity->getNicknameList());
        m_botEdit->setText(m_currentIdentity->getBot());
        m_passwordEdit->setText(m_currentIdentity->getPassword());

        m_insertRememberLineOnAwayChBox->setChecked(m_currentIdentity->getInsertRememberLineOnAway());
        m_awayNickEdit->setText(m_currentIdentity->getAwayNick());
        awayMessageGroup->setChecked(m_currentIdentity->getShowAwayMessage());
        m_awayEdit->setText(m_currentIdentity->getAwayMessage());
        m_unAwayEdit->setText(m_currentIdentity->getReturnMessage());
        automaticAwayGroup->setChecked(m_currentIdentity->getAutomaticAway());
        m_awayInactivitySpin->setValue(m_currentIdentity->getAwayInactivity());
        m_automaticUnawayChBox->setChecked(m_currentIdentity->getAutomaticUnaway());

        m_sCommandEdit->setText(m_currentIdentity->getShellCommand());
        m_codecCBox->setCurrentIndex(Konversation::IRCCharsets::self()->shortNameToIndex(m_currentIdentity->getCodecName()));
        m_loginEdit->setText(m_currentIdentity->getIdent());
        m_quitEdit->setText(m_currentIdentity->getQuitReason());
        m_partEdit->setText(m_currentIdentity->getPartReason());
        m_kickEdit->setText(m_currentIdentity->getKickReason());

        if(index == 0)
        {
            m_editBtn->setEnabled(false);
            m_delBtn->setEnabled(false);
        }
        else
        {
            m_editBtn->setEnabled(true);
            m_delBtn->setEnabled(true);
        }
    }
    
    void IdentityDialog::refreshCurrentIdentity()
    {
        if(!m_currentIdentity)
        {
            return;
        }

        m_currentIdentity->setRealName(m_realNameEdit->text());
        QStringList nicks;

        for(unsigned int i = 0; i < m_nicknameLBox->count(); ++i)
        {
            nicks.append(m_nicknameLBox->text(i));
        }

        m_currentIdentity->setNicknameList(nicks);
        m_currentIdentity->setBot(m_botEdit->text());
        m_currentIdentity->setPassword(m_passwordEdit->text());

        m_currentIdentity->setInsertRememberLineOnAway(m_insertRememberLineOnAwayChBox->isChecked());
        m_currentIdentity->setAwayNick(m_awayNickEdit->text());
        m_currentIdentity->setShowAwayMessage(awayMessageGroup->isChecked());
        m_currentIdentity->setAwayMessage(m_awayEdit->text());
        m_currentIdentity->setReturnMessage(m_unAwayEdit->text());
        m_currentIdentity->setAutomaticAway(automaticAwayGroup->isChecked());
        m_currentIdentity->setAwayInactivity(m_awayInactivitySpin->value());
        m_currentIdentity->setAutomaticUnaway(m_automaticUnawayChBox->isChecked());

        m_currentIdentity->setShellCommand(m_sCommandEdit->text());
        m_currentIdentity->setCodecName(Konversation::IRCCharsets::self()->availableEncodingShortNames()[m_codecCBox->currentIndex()]);
        m_currentIdentity->setIdent(m_loginEdit->text());
        m_currentIdentity->setQuitReason(m_quitEdit->text());
        m_currentIdentity->setPartReason(m_partEdit->text());
        m_currentIdentity->setKickReason(m_kickEdit->text());
    }

    void IdentityDialog::slotOk()
    {
        
        if(m_nicknameLBox->count() == 0)
        {
            KMessageBox::error(this, i18n("You must add at least one nick to the identity."));
            m_identityCBox->setItemText(0, m_currentIdentity->getName());
            return;
        }

        if(m_realNameEdit->text().isEmpty())
        {
            KMessageBox::error(this, i18n("Please enter a real name."));
            m_identityCBox->setItemText(0, m_currentIdentity->getName());
            return;
        }

        refreshCurrentIdentity();
        Preferences::setIdentityList(m_identityList);
        static_cast<KonversationApplication*>(kapp)->saveOptions(true);
        emit identitiesChanged();
        accept();
    }

    void IdentityDialog::newIdentity()
    {
        bool ok = false;
        QString txt = KInputDialog::getText(i18n("Add Identity"), i18n("Identity name:"), QString(), &ok, this);

        if(ok && !txt.isEmpty())
        {
            KUser user(KUser::UseRealUserID);
            IdentityPtr identity=IdentityPtr(new Identity);
            identity->setName(txt);
            identity->setIdent(user.loginName());
            m_identityList.append(identity);
            m_identityCBox->insertItem(txt);
            m_identityCBox->setCurrentIndex(m_identityCBox->count() - 1);
            updateIdentity(m_identityCBox->currentIndex());
        }
        else if(ok && txt.isEmpty())
        {
            KMessageBox::error(this, i18n("You need to give the identity a name."));
            newIdentity();
        }
    }

    void IdentityDialog::renameIdentity()
    {
        bool ok = false;
        QString currentTxt = m_identityCBox->currentText();
        QString txt = KInputDialog::getText(i18n("Rename Identity"), i18n("Identity name:"), currentTxt, &ok, this);

        if(ok && !txt.isEmpty())
        {
            m_currentIdentity->setName(txt);
            m_identityCBox->setItemText(m_identityCBox->currentIndex(), txt);
        }
        else if(ok && txt.isEmpty())
        {
            KMessageBox::error(this, i18n("You need to give the identity a name."));
            renameIdentity();
        }
    }

    void IdentityDialog::deleteIdentity()
    {
        int current = m_identityCBox->currentIndex();

        if(current <= 0)
        {
            return;
        }

        ServerGroupList serverGroups = Preferences::serverGroupList();
        ServerGroupList::iterator it = serverGroups.begin();
        bool found = false;

        while((it != serverGroups.end()) && !found)
        {
            if((*it)->identityId() == m_currentIdentity->id())
            {
                found = true;
            }

            ++it;
        }

        QString warningTxt;

        if(found)
        {
            warningTxt = i18n("This identity is in use, if you remove it the network settings using it will"
                " fall back to the default identity. Should it be deleted anyway?");
        }
        else
        {
            warningTxt = i18n("Are you sure you want to delete all information for this identity?");
        }

        if(KMessageBox::warningContinueCancel(this, warningTxt, i18n("Delete Identity"),
            KGuiItem(i18n("Delete"), "editdelete")) == KMessageBox::Continue)
        {
            m_identityCBox->removeItem(current);
            m_identityList.remove(m_currentIdentity);
            m_currentIdentity = 0;
            updateIdentity(m_identityCBox->currentIndex());
        }
    }

    void IdentityDialog::copyIdentity()
    {
        bool ok = false;
        QString currentTxt = m_identityCBox->currentText();
        QString txt = KInputDialog::getText(i18n("Duplicate Identity"), i18n("Identity name:"), currentTxt, &ok, this);

        if(ok && !txt.isEmpty())
        {
            Identity* identity = new Identity;
            identity->copy(*m_currentIdentity);
            identity->setName(txt);
#ifndef Q_CC_MSVC
#warning "port kde4"
#endif
            //m_identityList.append(identity);
            m_identityCBox->insertItem(txt);
            m_identityCBox->setCurrentIndex(m_identityCBox->count() - 1);
            updateIdentity(m_identityCBox->currentIndex());
        }
        else if(ok && txt.isEmpty())
        {
            KMessageBox::error(this, i18n("You need to give the identity a name."));
            renameIdentity();
        }
    }

    void IdentityDialog::setCurrentIdentity(int index)
    {
        if (index >= m_identityCBox->count())
            index = 0;

        m_identityCBox->setCurrentIndex(index);
        updateIdentity(index);
    }

    IdentityPtr IdentityDialog::setCurrentIdentity(IdentityPtr identity)
    {
        int index = Preferences::identityList().indexOf(identity);
        setCurrentIdentity(index);

        return m_currentIdentity;
    }

    IdentityPtr IdentityDialog::currentIdentity() const
    {
        return m_currentIdentity;
    }
}
