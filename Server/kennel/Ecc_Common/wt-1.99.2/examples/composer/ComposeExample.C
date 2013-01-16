/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <WApplication>
#include <WBreak>
#include <WText>
#include <WPushButton>
#include <WContainerWidget>
#include <WStringUtil>
#include <unistd.h>

#include "Composer.h"
#include "ComposeExample.h"
#include "Contact.h"

ComposeExample::ComposeExample(WContainerWidget *parent)
  : WContainerWidget(parent)
{
  composer_ = new Composer(this);

  std::vector<Contact> addressBook;
  addressBook.push_back(Contact(L"Koen Deforche",
				L"koen.deforche@gmail.com"));
  addressBook.push_back(Contact(L"Koen alias1",
				L"koen.alias1@yahoo.com"));
  addressBook.push_back(Contact(L"Koen alias2",
				L"koen.alias2@yahoo.com"));
  addressBook.push_back(Contact(L"Koen alias3",
				L"koen.alias3@yahoo.com"));
  addressBook.push_back(Contact(L"Bartje",
				L"jafar@hotmail.com"));
  composer_->setAddressBook(addressBook);

  std::vector<Contact> contacts;
  contacts.push_back(Contact(L"Koen Deforche", L"koen.deforche@gmail.com"));

  composer_->setTo(contacts);
  composer_->setSubject(L"That's cool! Want to start your own google?");

  composer_->send.connect(SLOT(this, ComposeExample::send));
  composer_->discard.connect(SLOT(this, ComposeExample::discard));

  details_ = new WContainerWidget(this);

  new WText(tr("example.info"), details_);
}

void ComposeExample::send()
{
  WContainerWidget *feedback = new WContainerWidget(this);
  feedback->setStyleClass(L"feedback");

  WContainerWidget *horiz = new WContainerWidget(feedback);
  new WText(L"<p>We could have, but did not send the following email:</p>",
	    horiz);

  std::vector<Contact> contacts = composer_->to();
  if (!contacts.empty())
    horiz = new WContainerWidget(feedback);
  for (unsigned i = 0; i < contacts.size(); ++i) {
    WText *t = new WText(L"To: \"" + contacts[i].name + L"\" <"
			 + contacts[i].email + L">", horiz);
    t->setFormatting(WText::PlainFormatting);
    new WBreak(horiz);
  }

  contacts = composer_->cc();
  if (!contacts.empty())
    horiz = new WContainerWidget(feedback);
  for (unsigned i = 0; i < contacts.size(); ++i) {
    WText *t = new WText(L"Cc: \"" + contacts[i].name + L"\" <"
			 + contacts[i].email + L">", horiz);
    t->setFormatting(WText::PlainFormatting);
    new WBreak(horiz);
  }
  
  contacts = composer_->bcc();
  if (!contacts.empty())
    horiz = new WContainerWidget(feedback);
  for (unsigned i = 0; i < contacts.size(); ++i) {
    WText *t = new WText(L"Bcc: \"" + contacts[i].name + L"\" <"
			 + contacts[i].email + L">", horiz);
    t->setFormatting(WText::PlainFormatting);
    new WBreak(horiz);
  }

  horiz = new WContainerWidget(feedback);
  WText *t = new WText(L"Subject: \"" + composer_->subject() + L"\"", horiz);
  t->setFormatting(WText::PlainFormatting);

  std::vector<Attachment> attachments = composer_->attachments();
  if (!attachments.empty())
    horiz = new WContainerWidget(feedback);
  for (unsigned i = 0; i < attachments.size(); ++i) {
    WText *t = new WText(L"Attachment: \""
			 + attachments[i].fileName
			 + L"\" (" + attachments[i].contentDescription
			 + L")", horiz);
    t->setFormatting(WText::PlainFormatting);

    unlink(attachments[i].spoolFileName.c_str());

    t = new WText(L", was in spool file: "
		  + widen(attachments[i].spoolFileName), horiz);
    new WBreak(horiz);
  }

  std::wstring message = composer_->message();

  horiz = new WContainerWidget(feedback);
  t = new WText(L"Message body: ", horiz);
  new WBreak(horiz);

  if (!message.empty()) {
    t = new WText(message, horiz);
    t->setFormatting(WText::PlainFormatting);
  } else
    t = new WText(L"<i>(empty)</i>", horiz);

  delete composer_;
  delete details_;

  wApp->quit();
}

void ComposeExample::discard()
{
  WContainerWidget *feedback = new WContainerWidget(this);
  feedback->setStyleClass(L"feedback");

  WContainerWidget *horiz = new WContainerWidget(feedback);
  new WText(L"<p>Wise decision! Everyone's mailbox is already full anyway.</p>",
	    horiz);

  delete composer_;
  delete details_;

  wApp->quit();
}

WApplication *createApplication(const WEnvironment& env)
{
  WApplication *app = new WApplication(env);
  app->messageResourceBundle().use("composer");
  app->setTitle(L"Composer example");
  app->useStyleSheet("composer.css");

  app->root()->addWidget(new ComposeExample());

  return app;
}

int main(int argc, char **argv)
{
   return WRun(argc, argv, &createApplication);
}

