// -*- C++ -*-
//
// Package:     Core
// Class  :     CmsShowModelPopup
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:
//         Created:  Fri Jun 27 11:23:08 EDT 2008
// $Id: CmsShowModelPopup.cc,v 1.23 2010/05/31 15:44:01 eulisse Exp $
//

// system include file
#include <iostream>
#include <sstream>
#include <set>
#include <sigc++/sigc++.h>
#include <boost/bind.hpp>
#include "TClass.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGString.h"
#include "TColor.h"
#include "TG3DLine.h"
#include "TGFont.h"
#include "TGSlider.h"

// user include files
#include "Fireworks/Core/interface/CmsShowModelPopup.h"
#include "Fireworks/Core/interface/FWDisplayProperties.h"
#include "Fireworks/Core/src/FWColorSelect.h"
#include "Fireworks/Core/src/FWDialogBuilder.h"
#include "Fireworks/Core/interface/FWModelChangeSignal.h"
#include "Fireworks/Core/interface/FWModelChangeManager.h"
#include "Fireworks/Core/interface/FWColorManager.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWModelId.h"
#include "Fireworks/Core/interface/FWSelectionManager.h"
#include "Fireworks/Core/interface/FWDetailViewManager.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

void
CmsShowModelPopupDetailViewButtonAdapter::wasClicked()
{
   m_popup->clicked(m_index);
}

//
// constructors and destructor
//
CmsShowModelPopup::CmsShowModelPopup(FWDetailViewManager* iManager,
                                     FWSelectionManager* iSelMgr,
                                     const FWColorManager* iColorMgr,
                                     const TGWindow* p, UInt_t w, UInt_t h) :
   TGTransientFrame(gClient->GetDefaultRoot(),p,w,h),
   m_detailViewManager(iManager),
   m_colorManager(iColorMgr)
{
   m_changes = iSelMgr->selectionChanged_.connect(boost::bind(&CmsShowModelPopup::fillModelPopup, this, _1));

   SetCleanup(kDeepCleanup);

   FWDialogBuilder builder(this);

   // Dummy button for detail views. Can be overidden.
   TGTextButton *detailedViewButton;
   
   // Do the layouting of the various widgets.
   builder.indent(4)
          .addLabel(" ", 14, 1, &m_modelLabel)
          .indent(4)
          .addLabel("Color", 8)
          .addColorPicker(iColorMgr, &m_colorSelectWidget).expand(false)
          .addHSeparator()
          .addLabel("Opacity", 8)
          .addHSlider(150, &m_opacitySlider)
          .addHSeparator()
          .addCheckbox("Visible", &m_isVisibleButton)
          .addHSeparator()
          .addTextButton("Open Detailed View", &detailedViewButton)
          .vSpacer(15);

   m_openDetailedViewButtons.push_back(detailedViewButton);
   m_openDetailedViewButtons.back()->SetEnabled(kFALSE);

   m_adapters.push_back(new CmsShowModelPopupDetailViewButtonAdapter(this, 0));
   m_openDetailedViewButtons.back()->Connect("Clicked()","CmsShowModelPopupDetailViewButtonAdapter", m_adapters.back(), "wasClicked()");

   m_colorSelectWidget->Connect("ColorChosen(Color_t)", "CmsShowModelPopup", this, "changeModelColor(Color_t)");
   m_isVisibleButton->Connect("Toggled(Bool_t)", "CmsShowModelPopup", this, "toggleModelVisible(Bool_t)");
   m_opacitySlider->Connect("PositionChanged(Int_t)", "CmsShowModelPopup",
                            this, "changeModelOpacity(Int_t)");

   SetWindowName("Object Display Controller");
   Resize(GetDefaultSize());
   MapSubwindows();
   Layout();

   fillModelPopup(*iSelMgr);
}

// CmsShowModelPopup::CmsShowModelPopup(const CmsShowModelPopup& rhs)
// {
//    // do actual copying here;
// }

CmsShowModelPopup::~CmsShowModelPopup()
{
   m_changes.disconnect();
   m_colorSelectWidget->Disconnect("ColorSelected(Pixel_t)", this, "changeModelColor(Pixel_t)");
   m_opacitySlider->Disconnect("PositionChanged(Int_t)", this, "changeModelOpacity(Int_t)");
   m_isVisibleButton->Disconnect("Toggled(Bool_t)", this, "toggleModelVisible(Bool_t)");
   disconnectAll();
   
   for(size_t i = 0, e = m_adapters.size(); i != e; ++i)
      delete m_adapters[i];
}

//
// assignment operators
//
// const CmsShowModelPopup& CmsShowModelPopup::operator=(const CmsShowModelPopup& rhs)
// {
//   //An exception safe implementation is
//   CmsShowModelPopup temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
/** this updates the dialog when selection changes. It also handles multiple
    Selections by updating only the widgets which have non controversial 
    (i.e. different across selected objects) values.
  */
void
CmsShowModelPopup::fillModelPopup(const FWSelectionManager& iSelMgr)
{
   disconnectAll();
   // Handles the case in which the selection is empty.
   if (iSelMgr.selected().empty())
      return;
   
   // Handle the case in which the selection is not empty.
   bool multipleNames = false, multipleColors = false, multipleVis = false,
        multipleTransparecy = false;

   m_models = iSelMgr.selected();
   std::set<FWModelId>::const_iterator id = m_models.begin();
   const FWEventItem* item = id->item();
   const FWEventItem::ModelInfo info = item->modelInfo(id->index());
   const FWDisplayProperties &props = info.displayProperties();
   
   // The old logic was broken here. It was enought that the last item
   // was the same as the first one, and all of them would have been considered
   // equal. This should fix it. The idea is that if any of the elements in
   // models in [1, N] are different from the first, then we consider the
   // set with multipleXYZ.
   for (std::set<FWModelId>::const_iterator i = ++(m_models.begin()),
                                            e = m_models.end(); i != e; ++i) 
   {
      const FWEventItem::ModelInfo &nextInfo = i->item()->modelInfo(i->index());
      const FWDisplayProperties &nextProps = nextInfo.displayProperties();
      
      multipleNames = multipleNames || (item->name() != i->item()->name());
      multipleColors = multipleColors || (props.color() != nextProps.color());
      multipleVis = multipleVis || (props.isVisible() != nextProps.isVisible());
      multipleTransparecy = multipleTransparecy 
                            || (props.transparency() != nextProps.transparency());
   }
   
   // Handle the name.
   std::ostringstream s;
   if (multipleNames) 
      s << m_models.size() << " objects";
   else
      s << m_models.size() << " " << item->name();
   m_modelLabel->SetText(s.str().c_str());

   if (m_models.size()==1)
   {
      m_modelLabel->SetText(item->modelName(id->index()).c_str());
      std::vector<std::string> viewChoices = m_detailViewManager->detailViewsFor(*id);
      m_openDetailedViewButtons.front()->SetEnabled(viewChoices.size()>0);
      //be sure we show just the right number of buttons
      if(m_openDetailedViewButtons.size()<=viewChoices.size()) 
      {
         for(size_t i = 1, e = m_openDetailedViewButtons.size(); i != e; ++i)
            ShowFrame(m_openDetailedViewButtons[i]);

         //now we make additional buttons
         for(size_t index = m_openDetailedViewButtons.size(); index < viewChoices.size(); ++index)
         { 
            TGTextButton *button = new TGTextButton(this,"Open Detailed View");
            m_openDetailedViewButtons.push_back(button);
            AddFrame(button);
            m_adapters.push_back(new CmsShowModelPopupDetailViewButtonAdapter(this, index));
            button->Connect("Clicked()",
                            "CmsShowModelPopupDetailViewButtonAdapter", 
                            m_adapters.back(), "wasClicked()");
         }
      }
      else if (viewChoices.size()>0)
      {
         for (size_t i = 1, e = viewChoices.size(); i != e; ++i)
            ShowFrame(m_openDetailedViewButtons[i]);
      }
      
      //set the names
      for (size_t i = 0, e = viewChoices.size(); i != e; ++i)
         m_openDetailedViewButtons[i]->SetText(("Open " + viewChoices[i] + " Detail View ...").c_str());
   }
   
   // Set the various widgets.
   m_colorSelectWidget->SetColorByIndex(m_colorManager->colorToIndex(props.color()), kFALSE);
   m_opacitySlider->SetPosition(100 - props.transparency());
   m_isVisibleButton->SetDisabledAndSelected(props.isVisible());
   
   m_colorSelectWidget->SetEnabled(kTRUE);
   m_isVisibleButton->SetEnabled(kTRUE);
   
   m_modelChangedConn = item->changed_.connect(boost::bind(&CmsShowModelPopup::updateDisplay, this));
   m_destroyedConn = item->goingToBeDestroyed_.connect(boost::bind(&CmsShowModelPopup::disconnectAll, this));
   Layout();
}

/** Based on the actual models properties, update the GUI. 
  */
void
CmsShowModelPopup::updateDisplay()
{
   for (std::set<FWModelId>::iterator i = m_models.begin(), e = m_models.end(); 
        i != e; ++i)
   {
      const FWEventItem::ModelInfo &info = i->item()->modelInfo(i->index());
      const FWDisplayProperties &p = info.displayProperties();
      m_colorSelectWidget->SetColor(gVirtualX->GetPixel(p.color()), kFALSE);
      
      if (p.isVisible())
         m_isVisibleButton->SetState(kButtonDown, kFALSE);
      else
         m_isVisibleButton->SetState(kButtonUp, kFALSE);

      m_opacitySlider->SetPosition(100 - p.transparency());
   }
}

/** This is invoked to when no object is selected and sets the dialog in
    a disabled look.
  */
void
CmsShowModelPopup::disconnectAll() {
   m_modelChangedConn.disconnect();
   m_destroyedConn.disconnect();
   //  m_item = 0;
   //  m_model = 0;
   m_modelLabel->SetText("No object selected");
   m_colorSelectWidget->SetColor(gVirtualX->GetPixel(kRed),kFALSE);
   m_isVisibleButton->SetDisabledAndSelected(kTRUE);
   m_colorSelectWidget->SetEnabled(kFALSE);
   m_isVisibleButton->SetEnabled(kFALSE);
   m_openDetailedViewButtons.front()->SetEnabled(kFALSE);
   m_openDetailedViewButtons.front()->SetText("Open Detail View ...");
   assert(m_openDetailedViewButtons.size() > 0);
   for(size_t i = 1, e = m_openDetailedViewButtons.size(); i != e; ++i)
      HideFrame(m_openDetailedViewButtons[i]);
}

/** Change the color of the selected objects.

    NOTES:
    - Notice that the whole thing works with a "Copy old properties and modify"
      paradigm.
      
  */
void
CmsShowModelPopup::changeModelColor(Color_t color)
{
   if (m_models.empty())
      return;
      
   FWChangeSentry sentry(*(m_models.begin()->item()->changeManager()));
   for (std::set<FWModelId>::iterator i = m_models.begin(), e = m_models.end(); i != e; ++i)
   {
      const FWEventItem::ModelInfo &info = i->item()->modelInfo(i->index());
      FWDisplayProperties changeProperties = info.displayProperties();
      changeProperties.setColor(color);
      i->item()->setDisplayProperties(i->index(), changeProperties);
   }
}

/** Change the opacity of the selected objects. See above in changeModelColor 
    for additional notes.
  */
void
CmsShowModelPopup::changeModelOpacity(Int_t opacity) 
{
   if (m_models.empty())
      return;
   
   FWChangeSentry sentry(*(m_models.begin()->item()->changeManager()));
   for (std::set<FWModelId>::iterator i = m_models.begin(), e = m_models.end(); i != e; ++i)
   {
      const FWEventItem::ModelInfo &info = i->item()->modelInfo(i->index());
      FWDisplayProperties changeProperties = info.displayProperties();
      changeProperties.setTransparency(100 - opacity);
      i->item()->setDisplayProperties(i->index(), changeProperties);
   }
}

/** Change (not toggle actually) the visibility of selected objects. 
    See changeModelColor for additional notes.
  */
void
CmsShowModelPopup::toggleModelVisible(Bool_t on) {
   if(m_models.empty())
      return;
   
   FWChangeSentry sentry(*(m_models.begin()->item()->changeManager()));
   for (std::set<FWModelId>::iterator i = m_models.begin(); i != m_models.end(); ++i) 
   {
      const FWEventItem::ModelInfo &info = i->item()->modelInfo(i->index());
      FWDisplayProperties changeProperties = info.displayProperties();
      changeProperties.setIsVisible(on);
      i->item()->setDisplayProperties(i->index(), changeProperties);
   }
}

void
CmsShowModelPopup::openDetailedView()
{
   std::vector<std::string> viewChoices = m_detailViewManager->detailViewsFor(*(m_models.begin()));

   m_detailViewManager->openDetailViewFor( *(m_models.begin()), viewChoices.front() );
}

void
CmsShowModelPopup::clicked(int iIndex)
{
   std::vector<std::string> viewChoices = m_detailViewManager->detailViewsFor(*(m_models.begin()));
   
   m_detailViewManager->openDetailViewFor( *(m_models.begin()), *(viewChoices.begin()+iIndex) );
   
}   
//
// const member functions
//

//
// static member functions
//
