/*
 * Copyright (C) 2005 Wim Dumon
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/lexical_cast.hpp>
#include <sstream>

#include <WBreak>
#include <WText>
#include <WTable>
#include <WTableCell>
#include <WCssDecorationStyle>

#include "HighScoresWidget.h"
#include "HangmanDb.h"

using namespace std;

HighScoresWidget::HighScoresWidget(const std::wstring &user,
				   WContainerWidget *parent):
   WContainerWidget(parent),
   User(user)
{
   setContentAlignment(AlignCenter);
   setStyleClass(L"highscores");
}

void HighScoresWidget::update()
{
   clear();

   WText *title = new WText(L"Hall of fame", this);
   title->decorationStyle().font().setSize(WFont::XLarge);
   title->setMargin(WLength(10), Top | Bottom);

   new WBreak(this);

   HangmanDb::Score s = HangmanDb::getUserPosition(User);

   std::wstring yourScore;
   if (s.number == 1)
     yourScore = L"Congratulations! You are currently leading the pack.";
   else {
     yourScore = L"You are currently ranked number "
       + boost::lexical_cast<std::wstring>(s.number)
       + L". Almost there !";
   }

   WText *score = new WText(L"<p>" + yourScore + L"</p>", this);
   score->decorationStyle().font().setSize(WFont::Large);

   std::vector<HangmanDb::Score> top = HangmanDb::getHighScores(20);

   WTable *table = new WTable(this);
   new WText(L"Rank", table->elementAt(0, 0));
   new WText(L"User", table->elementAt(0, 1));
   new WText(L"Games", table->elementAt(0, 2));
   new WText(L"Score", table->elementAt(0, 3));
   new WText(L"Last game", table->elementAt(0, 4));
   for(unsigned int i = 0; i < top.size(); ++i) {
      new WText(boost::lexical_cast<wstring>(top[i].number),
		table->elementAt(i + 1, 0));
      new WText(top[i].user, table->elementAt(i + 1, 1));
      new WText(boost::lexical_cast<std::wstring>(top[i].numgames),
		table->elementAt(i+ 1, 2));
      new WText(boost::lexical_cast<std::wstring>(top[i].score),
		table->elementAt(i + 1, 3));
      new WText(top[i].lastseen, table->elementAt(i + 1, 4));
   }

   table->resize(WLength(60, WLength::FontEx), WLength());
   table->setMargin(WLength(20), Top | Bottom);
   table->decorationStyle().setBorder(WBorder(WBorder::Solid));

   /*
    * Apply cell styles
    */
   for (int row = 0; row < table->numRows(); ++row) {
     for (int col = 0; col < table->numColumns(); ++col) {
       WTableCell *cell = table->elementAt(row, col);
       cell->setContentAlignment(AlignMiddle | AlignCenter);

       if (row == 0)
	 cell->setStyleClass(L"highscoresheader");

       if (row == s.number)
	 cell->setStyleClass(L"highscoresself");
     }
   }

   WText *fineprint
     = new WText(L"<p>For each game won, you gain 20 points, "
		 "minus one point for each wrong letter guess.<br />"
		 "For each game lost, you loose 10 points, so you "
		 "better try hard to guess the word!</p>", this);
   fineprint->decorationStyle().font().setSize(WFont::Smaller);
}
