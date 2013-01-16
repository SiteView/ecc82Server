// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef LABEL_H_
#define LABEL_H_

#include <WText>

using namespace Wt;

/**
 * @addtogroup composerexample
 */
/*@{*/

/*! \brief A label.
 *
 * A label is a WText that is styled as "label", and aligned
 * to the right in its parent.
 */
class Label : public WText
{
public:
  Label(const WMessage& text, WContainerWidget *parent);
};

/*@}*/

#endif // LABEL_H_
