#include <WApplication>
#include <WEnvironment>
#include <WContainerWidget>
#include <WImage>

#include "Character.h"

using namespace Wt;

/**
 * \defgroup dragexample Drag and Drop example
 */
/*@{*/

/*! \brief Create an image which can be dragged.
 *
 * The image to be displayed when dragging is given by smallurl, and
 * configured with the given mime type
 */
WImage *createDragImage(const char *url, const char *smallurl,
			const char *mimeType,
			WContainerWidget *p)
{
  WImage *result = new WImage(url, p);
  WImage *dragImage = new WImage(smallurl, p);

  /*
   * Set the image to be draggable, showing the other image (dragImage)
   * to be used as the widget that is visually dragged.
   */
  result->setDraggable(mimeType, dragImage, true);

  return result;
}

WApplication *createApplication(const WEnvironment& env)
{
  WApplication *app = new WApplication(env);
  app->setTitle(L"Drag & drop");
  new WText("<h1>Wt Drag &amp; drop example.</h1>", app->root());

  new WText("<p>Help these people with their decision by dragging one of "
	    "the pills.</p>", app->root());

  if (!env.javaScript()) {
    new WText("<i>This examples requires that javascript support is "
	      "enabled.</i>", app->root());
  }

  WContainerWidget *pills = new WContainerWidget(app->root());
  pills->setContentAlignment(WWidget::AlignCenter);

  createDragImage("blue-pill.jpg",
		  "blue-pill-small.png",
		  "blue-pill", pills);
  createDragImage("red-pill.jpg",
		  "red-pill-small.png",
		  "red-pill", pills);

  WContainerWidget *dropSites = new WContainerWidget(app->root());

  new Character(L"Neo", dropSites);
  new Character(L"Morpheus", dropSites);
  new Character(L"Trinity", dropSites);

  app->useStyleSheet("dragdrop.css");

  return app;
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}

/*@}*/
