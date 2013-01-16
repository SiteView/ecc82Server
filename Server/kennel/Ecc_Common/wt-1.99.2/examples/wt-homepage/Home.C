/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <fstream>

#include <boost/tokenizer.hpp>

#include <WApplication>
#include <WEnvironment>
#include <WImage>
#include <WMenu>
#include <WStackedWidget>
#include <WTable>
#include <WTableCell>
#include <WText>
#include <WTreeNode>
#include <WIconPair>

#include "Home.h"
#include "RoundedWidget.h"
#include "TreeListExample.h"

Home::Home(WContainerWidget *parent)
  : WContainerWidget(parent)
{
  WTable *topLayout = new WTable(this);

  WTable *header = new WTable(topLayout->elementAt(0, 0));
  WImage *icon = new WImage("uiloog.jpg", header->elementAt(0, 0));
  icon->setMargin(WLength(15));

  new WText("<h1>Wt: a C++ Web Toolkit</h1>", header->elementAt(0, 1));
  if (!wApp->environment().javaScript()) {
    new WText("<i>(non-javascript version)</i>",
	      header->elementAt(0, 1));
  }

  WTable *body = new WTable(topLayout->elementAt(1, 0));
  WStackedWidget *contents = new WStackedWidget(body->elementAt(0, 1));

  /*
   * Wrap the menu in a rounded widget.
   */
  RoundedWidget *rounded = new RoundedWidget(RoundedWidget::All,
					     body->elementAt(0, 0));
  rounded->setBackgroundColor(WColor(247,236,111));
  rounded->resize(WLength(18, WLength::FontEx), WLength());
  rounded->setMargin(WLength(15), Right);

  WMenu *menu = new WMenu(contents, WMenu::Vertical, rounded->contents());

  menu->setStyleClass("menu");

  body->elementAt(0, 0)->setContentAlignment(AlignTop | AlignLeft);
  body->elementAt(0, 1)->setContentAlignment(AlignTop | AlignLeft);

  menu->addItem("Introduction", introduction());
  menu->addItem("News", news(), WMenuItem::PreLoading);
  menu->addItem("Status", status(), WMenuItem::PreLoading);
  menu->addItem("Features", features(), WMenuItem::PreLoading);
  menu->addItem("Documentation", documentation(), WMenuItem::PreLoading);
  menu->addItem("Examples", examples(), WMenuItem::PreLoading);
  menu->addItem("Download", download(), WMenuItem::PreLoading);
  menu->addItem("Community", community(), WMenuItem::PreLoading);

  menu->enableBrowserHistory("main");

  menu->itemSelected.connect(SLOT(this, Home::changeTitle));
}

void Home::changeTitle(WMenuItem *item)
{
  wApp->setTitle(L"Wt, C++ Web Toolkit - " + item->text().value());
}

WWidget *Home::introduction()
{
  return new WText(tr("home.intro"));
}

void Home::refresh()
{
  readNews(recentNews_, "latest-news.txt");
  readNews(historicalNews_, "historical-news.txt");
  readReleases(releases_, "releases.txt");

  WContainerWidget::refresh();
}

WWidget *Home::news()
{
  WContainerWidget *result = new WContainerWidget();

  result->addWidget(new WText("<h2><span>Latest News</span></h2>"));
  recentNews_ = new WTable();
  readNews(recentNews_, "latest-news.txt");
  result->addWidget(recentNews_);

  result->addWidget(new WText("<h2><span>Historical News</span></h2>"));
  historicalNews_ = new WTable();
  readNews(historicalNews_, "historical-news.txt");
  result->addWidget(historicalNews_);

  return result;
}

WWidget *Home::status()
{
  return new WText(tr("home.status"));
}

WWidget *Home::features()
{
  return new WText(tr("home.features"));
}

WWidget *Home::documentation()
{
  return new WText(tr("home.documentation"));
}

WWidget *Home::helloWorldExample()
{
  WContainerWidget *result = new WContainerWidget();

  new WText(tr("home.examples.hello"), result);

  WTreeNode *tree = makeTreeMap("Hello world", 0);
  makeTreeFile("hello.C", tree);

  tree->expand();

  result->addWidget(tree);

  return result;
}

WWidget *Home::homepageExample()
{
  WContainerWidget *result = new WContainerWidget();

  new WText(tr("home.examples.wt"), result);

  WTreeNode *tree = makeTreeMap("Wt Homepage", 0);
  WTreeNode *home = makeTreeMap("class Home", tree);
  makeTreeFile("Home.h", home);
  makeTreeFile("Home.C", home);
  WTreeNode *treeexample = makeTreeMap("class TreeListExample", tree);
  makeTreeFile("TreeListExample.h", treeexample);
  makeTreeFile("TreeListExample.C", treeexample);
  makeTreeFile("wt-home.xml", tree);

  tree->expand();

  result->addWidget(tree);

  return result;
}

WWidget *Home::formExample()
{
  WContainerWidget *result = new WContainerWidget();

  new WText(tr("home.examples.form"), result);

  WTreeNode *tree = makeTreeMap("Form example", 0);
  WTreeNode *form = makeTreeMap("class Form", tree);
  makeTreeFile("Form.h", form);
  makeTreeFile("Form.C", form);
  WTreeNode *formexample = makeTreeMap("class FormExample", tree);
  makeTreeFile("FormExample.h", formexample);
  makeTreeFile("FormExample.C", formexample);
  WTreeNode *datevalidator = makeTreeMap("class DateValidator", tree);
  makeTreeFile("DateValidator.h", datevalidator);
  makeTreeFile("DateValidator.C", datevalidator);
  makeTreeFile("form-example.xml", tree);
  makeTreeFile("form-example_nl.xml", tree);

  tree->expand();

  result->addWidget(tree);

  return result;
}

WWidget *Home::treelistExample()
{
  WContainerWidget *result = new WContainerWidget();

  new WText(tr("home.examples.treelist"), result);
  new TreeListExample(result);
  new WText(tr("home.examples.treelist-remarks"), result);

  return result;
}

WWidget *Home::hangmanExample()
{
  WContainerWidget *result = new WContainerWidget();

  new WText(tr("home.examples.hangman"), result);

  WTreeNode *tree = makeTreeMap("Hangman game", 0);
  WTreeNode *widgets = makeTreeMap("Widgets", tree);
  WTreeNode *loginwidget = makeTreeMap("class LoginWidget", widgets);
  makeTreeFile("LoginWidget.h", loginwidget);
  makeTreeFile("LoginWidget.C", loginwidget);
  WTreeNode *hangmanwidget = makeTreeMap("class HangmanWidget", widgets);
  makeTreeFile("HangmanWidget.h", hangmanwidget);
  makeTreeFile("HangmanWidget.C", hangmanwidget);
  WTreeNode *highscoreswidget = makeTreeMap("class HighScoresWidget", widgets);
  makeTreeFile("HighScoresWidget.h", highscoreswidget);
  makeTreeFile("HighScoresWidget.C", highscoreswidget);
  WTreeNode *hangmangame = makeTreeMap("class HangmanGame", widgets);
  makeTreeFile("HangmanGame.h", hangmangame);
  makeTreeFile("HangmanGame.C", hangmangame);
  WTreeNode *other = makeTreeMap("Other", tree);
  WTreeNode *hangmandb = makeTreeMap("class HangmanDb", other);
  makeTreeFile("HangmanDb.h", hangmandb);
  makeTreeFile("HangmanDb.C", hangmandb);
  WTreeNode *dictionary = makeTreeMap("class Dictionary", other);
  makeTreeFile("Dictionary.h", dictionary);
  makeTreeFile("Dictionary.C", dictionary);
  makeTreeFile("hangman.C", other);

  tree->expand();

  result->addWidget(tree);

  return result;
}

WWidget *Home::styleExample()
{
  WContainerWidget *result = new WContainerWidget();

  new WText(tr("home.examples.style"), result);

  WTreeNode *tree = makeTreeMap("Style example", 0);
  WTreeNode *cornerimage = makeTreeMap("class CornerImage", tree);
  makeTreeFile("CornerImage.h", cornerimage);
  makeTreeFile("CornerImage.C", cornerimage);
  WTreeNode *roundedwidget = makeTreeMap("class RoundedWidget", tree);
  makeTreeFile("RoundedWidget.h", roundedwidget);
  makeTreeFile("RoundedWidget.C", roundedwidget);
  WTreeNode *styleexample = makeTreeMap("class StyleExample", tree);
  makeTreeFile("StyleExample.h", styleexample);
  makeTreeFile("StyleExample.C", styleexample);

  tree->expand();

  result->addWidget(tree);

  return result;
}

WWidget *Home::composerExample()
{
  WContainerWidget *result = new WContainerWidget();

  new WText(tr("home.examples.composer"), result);

  WTreeNode *tree = makeTreeMap("Mail composer example", 0);

  WTreeNode *classMap;
  classMap = makeTreeMap("class AddresseeEdit", tree);
  makeTreeFile("AddresseeEdit.h", classMap);
  makeTreeFile("AddresseeEdit.C", classMap);
  classMap = makeTreeMap("class AttachmentEdit", tree);
  makeTreeFile("AttachmentEdit.h", classMap);
  makeTreeFile("AttachmentEdit.C", classMap);
  classMap = makeTreeMap("class ComposeExample", tree);
  makeTreeFile("ComposeExample.h", classMap);
  makeTreeFile("ComposeExample.C", classMap);
  classMap = makeTreeMap("class Composer", tree);
  makeTreeFile("Composer.h", classMap);
  makeTreeFile("Composer.C", classMap);
  classMap = makeTreeMap("class ContactSuggestions", tree);
  makeTreeFile("ContactSuggestions.h", classMap);
  makeTreeFile("ContactSuggestions.C", classMap);
  classMap = makeTreeMap("class Label", tree);
  makeTreeFile("Label.h", classMap);
  makeTreeFile("Label.C", classMap);
  classMap = makeTreeMap("class Option", tree);
  makeTreeFile("Option.h", classMap);
  makeTreeFile("Option.C", classMap);
  classMap = makeTreeMap("class OptionList", tree);
  makeTreeFile("OptionList.h", classMap);
  makeTreeFile("OptionList.C", classMap);
  makeTreeFile("Contact.h", tree);
  makeTreeFile("Attachment.h", tree);
  makeTreeFile("composer.xml", tree);
  makeTreeFile("composer.css", tree);

  tree->expand();

  result->addWidget(tree);

  return result;
}

WWidget *Home::dragdropExample()
{
  WContainerWidget *result = new WContainerWidget();

  new WText(tr("home.examples.dragdrop"), result);

  WTreeNode *tree = makeTreeMap("DragDrop example", 0);

  WTreeNode *classMap;
  classMap = makeTreeMap("class Character", tree);
  makeTreeFile("Character.h", classMap);
  makeTreeFile("Character.C", classMap);
  makeTreeFile("DragExample.C", tree);
  makeTreeFile("dragdrop.css", tree);

  tree->expand();

  result->addWidget(tree);

  return result;
}

WWidget *Home::fileExplorerExample()
{
  WContainerWidget *result = new WContainerWidget();

  new WText(tr("home.examples.fileexplorer"), result);

  WTreeNode *tree = makeTreeMap("File explorer example", 0);

  WTreeNode *classMap;
  classMap = makeTreeMap("class FileTreeTableNode", tree);
  makeTreeFile("FileTreeTableNode.h", classMap);
  makeTreeFile("FileTreeTableNode.C", classMap);
  classMap = makeTreeMap("class FileTreeTable", tree);
  makeTreeFile("FileTreeTable.h", classMap);
  makeTreeFile("FileTreeTable.C", classMap);
  makeTreeFile("FileTreeExample.C", tree);
  makeTreeFile("filetree.css", tree);

  tree->expand();

  result->addWidget(tree);

  return result;
}

WWidget *Home::examples()
{
  WContainerWidget *result = new WContainerWidget();

  result->addWidget(new WText(tr("home.examples")));

  WStackedWidget *exampleStack = new WStackedWidget();

  /*
   * Wrap the menu in a rounded widget.
   */
  RoundedWidget *rounded = new RoundedWidget(RoundedWidget::All, result);
  rounded->setBackgroundColor(WColor(0xE0,0xE8,0xFF));

  WMenu *exampleMenu = new WMenu(exampleStack, WMenu::Horizontal,
				 rounded->contents());
  exampleMenu->setStyleClass("menu");

  result->addWidget(exampleStack);

  exampleMenu->addItem("Hello world", helloWorldExample());
  exampleMenu->addItem("Form", formExample());
  exampleMenu->addItem("Wt homepage", homepageExample());
  exampleMenu->addItem("Treelist", treelistExample());
  exampleMenu->addItem("Hangman", hangmanExample());
  exampleMenu->addItem("Style", styleExample());
  exampleMenu->addItem("Mail composer", composerExample());
  exampleMenu->addItem("Drag &amp; Drop", dragdropExample());
  exampleMenu->addItem("File explorer", fileExplorerExample());

  exampleMenu->enableBrowserHistory("example");

  return result;
}

WWidget *Home::download()
{
  WContainerWidget *result = new WContainerWidget();
  result->addWidget(new WText("<h2><span>Download</span></h2>"));
  result->addWidget(new WText(tr("home.download.license")));
  result->addWidget(new WText(tr("home.download.requirements")));
  result->addWidget(new WText("<h3>Available packages</h3>"));

  releases_ = new WTable();
  readReleases(releases_, "releases.txt");
  result->addWidget(releases_);

  result->addWidget
    (new WText("<p>Older releases are still available at "
	       + href("http://sourceforge.net/project/showfiles.php?"
		      "group_id=153710#files",
		      "sourceforge.net")
	       + "</p>"));

  return result;
}

std::string Home::href(const std::string url, const std::string description)
{
  return "<a href=\"" + url + "\" TARGET=\"_blank\">" + description + "</a>";
}

WWidget *Home::community()
{
  return new WText(tr("home.community"));
}

void Home::readNews(WTable *newsTable, const std::string newsfile)
{
  std::ifstream f(newsfile.c_str());

  newsTable->clear();

  int row = 0;

  while (f) {
    std::string line;
    getline(f, line);

    if (f) {
      typedef boost::tokenizer<boost::escaped_list_separator<char> >
	CsvTokenizer;
      CsvTokenizer tok(line);

      CsvTokenizer::iterator i=tok.begin();

      newsTable->elementAt(row, 0)->
	addWidget(new WText("<p><b>" + *i + "</b></p>"));
      newsTable->elementAt(row, 0)
	->setContentAlignment(AlignCenter | AlignTop);
      newsTable->elementAt(row, 0)
	->resize(WLength(16, WLength::FontEx), WLength());
      newsTable
	->elementAt(row, 1)->addWidget(new WText("<p>" + *(++i) + "</p>"));

      ++row;
    }
  }
}

void Home::readReleases(WTable *releaseTable, const std::string releasefile)
{
  std::ifstream f(releasefile.c_str());

  releaseTable->clear();

  releaseTable->elementAt(0, 0)->addWidget(new WText("<b>Version</b>"));
  releaseTable->elementAt(0, 1)->addWidget(new WText("<b>Date</b>"));
  releaseTable->elementAt(0, 2)->addWidget(new WText("<b>Description</b>"));

  releaseTable->elementAt(0, 0)->resize(WLength(10, WLength::FontEx),
					WLength());
  releaseTable->elementAt(0, 1)->resize(WLength(15, WLength::FontEx),
					WLength());

  int row = 1;

  while (f) {
    std::string line;
    getline(f, line);

    if (f) {
      typedef boost::tokenizer<boost::escaped_list_separator<char> >
	CsvTokenizer;
      CsvTokenizer tok(line);

      CsvTokenizer::iterator i=tok.begin();

      std::string version = *i;
      releaseTable->elementAt(row, 0)->addWidget
	(new WText(href("http://prdownloads.sourceforge.net/witty/wt-"
			+ version + ".tar.gz?download", "Wt " + version)));
      releaseTable->elementAt(row, 1)->addWidget(new WText(*(++i)));
      releaseTable->elementAt(row, 2)->addWidget(new WText(*(++i)));

      ++row;
    }
  }
}

WTreeNode *Home::makeTreeMap(const std::string name, WTreeNode *parent)
{
  WIconPair *labelIcon
    = new WIconPair("icons/yellow-folder-closed.png",
		    "icons/yellow-folder-open.png", false);

  WTreeNode *node = new WTreeNode(name, labelIcon, parent);
  node->label()->setFormatting(WText::PlainFormatting);

  if (!parent) {
    node->setImagePack("icons/");
    node->expand();
    node->setLoadPolicy(WTreeNode::NextLevelLoading);
  }

  return node;
}

WTreeNode *Home::makeTreeFile(const std::string name, WTreeNode *parent)
{
  WIconPair *labelIcon
    = new WIconPair("icons/document.png",
		    "icons/yellow-folder-open.png", false);

  return new WTreeNode("<a href=\"" + name + "\" TARGET=\"_BLANK\">"
		       + name + "</a>", labelIcon, parent);
}

WApplication *createApplication(const WEnvironment& env)
{
  WApplication *app = new WApplication(env);
  app->messageResourceBundle().use("wt-home", false);
  app->useStyleSheet("home.css");
  app->setTitle("Wt, C++ Web Toolkit");

  new Home(app->root());
  return app;
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}

