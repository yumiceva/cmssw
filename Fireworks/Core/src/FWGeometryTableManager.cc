// -*- C++ -*-
//
// Package:     Core
// Class  :     FWGeometryTableManager
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Alja Mrak-Tadel, Matevz Tadel
//         Created:  Thu Jan 27 14:50:57 CET 2011
// $Id: FWGeometryTableManager.cc,v 1.15 2011/06/30 04:53:30 amraktad Exp $
//

//#define PERFTOOL

// user include files
#include <iostream>
#include <boost/bind.hpp>
#include <stack>
#ifdef PERFTOOL 
#include <google/profiler.h>
#endif
#include "Fireworks/Core/interface/FWGeometryTableManager.h"
#include "Fireworks/Core/interface/FWGeometryBrowser.h"
#include "Fireworks/Core/src/FWColorBoxIcon.h"
#include "Fireworks/TableWidget/interface/GlobalContexts.h"
#include "Fireworks/TableWidget/src/FWTabularWidget.h"
#include "Fireworks/Core/interface/fwLog.h"

#include "TMath.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoMatrix.h"
#include "TGeoShape.h"
#include "TGeoBBox.h"
#include "TEveManager.h"
#include "TEveGeoNode.h"
#include "TEveScene.h"

#include "TGFrame.h"

static const char* redTxt   = "\033[01;31m";
static const char* greenTxt = "\033[01;32m";
static const char* cyanTxt  = "\033[22;36m";
//static const char* whiteTxt = "\033[0m";

const char* FWGeometryTableManager::NodeInfo::name() const
{
   return m_node->GetName();
}

FWGeometryTableManager::ColorBoxRenderer::ColorBoxRenderer():
   FWTableCellRendererBase(),
   m_width(1),
   m_height(1),
   m_color(0xffffff),
   m_isSelected(false)
{
   GCValues_t gval; 
   gval.fMask       = kGCForeground | kGCBackground | kGCStipple | kGCFillStyle  | kGCGraphicsExposures;
   gval.fStipple    = gClient->GetResourcePool()->GetCheckeredBitmap();
   gval.fGraphicsExposures = kFALSE;
   gval.fBackground = gVirtualX->GetPixel(kGray);
   m_colorContext = gClient->GetResourcePool()->GetGCPool()->GetGC(&gval,kTRUE);

}

FWGeometryTableManager::ColorBoxRenderer::~ColorBoxRenderer()
{
   gClient->GetResourcePool()->GetGCPool()->FreeGC(m_colorContext->GetGC());
}

void FWGeometryTableManager::ColorBoxRenderer::setData(Color_t c, bool s)
{
   m_color = gVirtualX->GetPixel(c);
   m_isSelected = s;
}


void FWGeometryTableManager::ColorBoxRenderer::draw(Drawable_t iID, int iX, int iY, unsigned int iWidth, unsigned int iHeight)
{
   iX -= FWTabularWidget::kTextBuffer;
   iY -= FWTabularWidget::kTextBuffer;
   iWidth += 2*FWTabularWidget::kTextBuffer;
   iHeight += 2*FWTabularWidget::kTextBuffer;

   m_colorContext->SetFillStyle(kFillSolid);
   Pixel_t baq =  m_colorContext->GetForeground();
   m_colorContext->SetForeground(m_color);
   gVirtualX->FillRectangle(iID, m_colorContext->GetGC(), iX, iY, iWidth, iHeight);

   if (m_isSelected)
   {
      m_colorContext->SetFillStyle(kFillOpaqueStippled);
      gVirtualX->FillRectangle(iID, m_colorContext->GetGC(), iX, iY, iWidth, iHeight);
   }
   m_colorContext->SetForeground(baq);
}

//==============================================================================
//==============================================================================
//
// class FWGeometryTableManager
//
//==============================================================================
//==============================================================================

FWGeometryTableManager::FWGeometryTableManager(FWGeometryBrowser* browser)
:    m_selectedRow(-1),
     m_selectedIdx(0),
     m_selectedColumn(-1),
     m_browser(browser),
     m_filterOff(true),
     m_levelOffset(0),
     m_geoTopNodeIdx(-1)
{ 
   m_colorBoxRenderer.m_width  =  50;
   m_colorBoxRenderer.m_height =  m_renderer.height();
}

FWGeometryTableManager::~FWGeometryTableManager()
{
}


int FWGeometryTableManager::unsortedRowNumber(int unsorted) const
{
   return unsorted;
}

int FWGeometryTableManager::numberOfRows() const 
{
   return m_row_to_index.size();
}

int FWGeometryTableManager::numberOfColumns() const 
{
   return kNumCol;
}
   

std::vector<std::string> FWGeometryTableManager::getTitles() const 
{
   std::vector<std::string> returnValue;
   returnValue.reserve(numberOfColumns());

   if (m_browser->getVolumeMode() )
      returnValue.push_back("Volume Name");
   else
      returnValue.push_back("Node Name");

   returnValue.push_back("Color");
   returnValue.push_back("RnrSelf");
   returnValue.push_back("RnrChildren");
   returnValue.push_back("Material");
   returnValue.push_back("Position");
   returnValue.push_back("Diagonal");

   return returnValue;
}
  
void FWGeometryTableManager::setSelection (int row, int column, int mask) 
{
   changeSelection(row, column);
}

const std::string FWGeometryTableManager::title() const 
{
   return "Geometry";
}

int FWGeometryTableManager::selectedRow() const 
{
   return m_selectedRow;
}

int FWGeometryTableManager::selectedColumn() const 
{
   return m_selectedColumn;
}
 
bool FWGeometryTableManager::rowIsSelected(int row) const 
{
   return m_selectedRow == row;
}

void FWGeometryTableManager::changeSelection(int iRow, int iColumn)
{     
   if (iRow < 0) return; 


   m_selectedRow = iRow;
   m_selectedColumn = iColumn;
   if (m_row_to_index.size() > 0)   m_selectedIdx = m_row_to_index[iRow];
   visualPropertiesChanged();
}    

void  FWGeometryTableManager::setBackgroundToWhite(bool iToWhite )
{
   if(iToWhite) {
      m_renderer.setGraphicsContext(&TGFrame::GetBlackGC());
   } else {
      m_renderer.setGraphicsContext(&TGFrame::GetWhiteGC());
   }
   m_renderer.setBlackIcon(iToWhite);
}

FWTableCellRendererBase* FWGeometryTableManager::cellRenderer(int iSortedRowNumber, int iCol) const
{
   if (static_cast<int>(m_row_to_index.size()) <= iSortedRowNumber)
   {
      m_renderer.setData(std::string("FWGeometryTableManager::cellRenderer() Error!"), false);
      return &m_renderer;
   }       

   FWTextTreeCellRenderer* renderer = &m_renderer;

   int unsortedRow =  m_row_to_index[iSortedRowNumber];
   const NodeInfo& data = m_entries[unsortedRow];
   TGeoNode& gn = *data.m_node;

   bool isSelected =  (!m_filterOff &&  m_volumes[gn.GetVolume()].m_matches);//(m_selectedRow == unsortedRow);

   if (iCol == kName)
   {
      int nD = getNdaughtersLimited(data.m_node);
      if (m_browser->getVolumeMode())
         renderer->setData(Form("%s [%d]", gn.GetVolume()->GetName(), nD), isSelected);
      else    
         renderer->setData(Form("%s [%d]", gn.GetName(), nD), isSelected); 

      renderer->setIsParent((gn.GetNdaughters() > 0) && (m_filterOff || m_volumes[gn.GetVolume()].accepted()));

      renderer->setIsOpen(data.m_expanded);
      int level = data.m_level - m_levelOffset;

      if (data.m_node->GetNdaughters())
         renderer->setIndentation(20*level);
      else
         renderer->setIndentation(20*level + FWTextTreeCellRenderer::iconWidth());

      return renderer;
   }
   else
   {
      // printf("title %s \n",data.m_node->GetTitle() );
      renderer->setIsParent(false);
      renderer->setIndentation(0);
      if (iCol == kColor)
      {
         // m_colorBoxRenderer.setData(data.m_node->GetVolume()->GetLineColor(), isSelected);
         m_colorBoxRenderer.setData(data.m_color, isSelected);
         return  &m_colorBoxRenderer;
      }
      else if (iCol == kVisSelf )
      {
         const char* txt = gn.IsVisible() ? "On" : "-";
         renderer->setData( txt,  isSelected);
         return renderer;
      }
      else if (iCol == kVisChild )
      {
         renderer->setData( gn.IsVisDaughters() ? "On" : "-",  isSelected);
         return renderer;
      }
      else if (iCol == kMaterial )
      { 
         renderer->setData( gn.GetVolume()->GetMaterial()->GetName(),  isSelected);
         return renderer;
      }
      else if (iCol == kPosition )
      { 
         const Double_t* p = gn.GetMatrix()->GetTranslation();
         renderer->setData(Form("[%.3f, %.3f, %.3f]", p[0], p[1], p[2]),  isSelected);
         return renderer;
      }
      else// if (iCol == kPosition  )
      { 
         TGeoBBox* gs = static_cast<TGeoBBox*>( gn.GetVolume()->GetShape());
         renderer->setData( Form("%f", TMath::Sqrt(gs->GetDX()*gs->GetDX() + gs->GetDY()*gs->GetDY() +gs->GetDZ()*gs->GetDZ() )),  isSelected);
         return renderer;
      }
   }
}

//______________________________________________________________________________
void FWGeometryTableManager::firstColumnClicked(int row)
{
   if (row == -1)
      return;

   int idx = rowToIndex()[row];
   // printf("click %s \n", m_entries[idx].name());
   Entries_i it = m_entries.begin();
   std::advance(it, idx);
   NodeInfo& data = *it;
   data.m_expanded = !data.m_expanded;

   recalculateVisibility();
   dataChanged();
   visualPropertiesChanged();
}

//______________________________________________________________________________

void FWGeometryTableManager::recalculateVisibility()
{
   m_row_to_index.clear();
   for ( size_t i = 0,  e = m_entries.size(); i != e; ++i )
   {   
      NodeInfo &data = m_entries[i];
      if ( (m_geoTopNodeIdx == -1 && data.m_parent == -1) || (  (int)i == m_geoTopNodeIdx) )
      {
         data.m_visible = true;
         m_row_to_index.push_back(i);
         //    printf("top node %d visible %s OFFF %d\n",(int)i, data.name(),m_levelOffset );
         ++i;
         while (i < e)
         {
            if (m_entries[i].m_level >  m_levelOffset) {
               int pIdx =  m_entries[i].m_parent;

               //    printf("child %d level[%d] pexpan[%d] pvis[%d] \n", (int)i, m_entries[i].m_level,  m_entries[pIdx].m_expanded,  m_entries[pIdx].m_visible);

               if (m_entries[pIdx].m_expanded && m_entries[pIdx].m_visible)
               {
                  m_entries[i].m_visible = true;
                  m_row_to_index.push_back(i);
               }
               else
               {
                  m_entries[i].m_visible = false;
                  // printf("child vis node %d visible %s \n",(int)i, m_entries[i].name());
               }
               i++; 

               if (i == e) return;
            }
            else
            {
               break;
            }
         }
      }
      else 
      {
         data.m_visible = false;
      }
   }
   // printf("entries %d \n", m_entries.size());
} 

//______________________________________________________________________________

void FWGeometryTableManager::redrawTable() 
{
   changeSelection(0, 0);

   recalculateVisibility();
   dataChanged();
   visualPropertiesChanged();
}

//==============================================================================

void FWGeometryTableManager::checkUniqueVolume(TGeoVolume* v)
{
   Volumes_i it  = m_volumes.find(v);
   if (it == m_volumes.end())
   {
      m_volumes.insert(std::make_pair(v, Match()));
   }
   for (int i =0, nD = v->GetNdaughters(); i != nD; ++i) {
      checkUniqueVolume(v->GetNode(i)->GetVolume());
   }
}
//==============================================================================

void FWGeometryTableManager::loadGeometry()
{
  
#ifdef PERFTOOL  
   ProfilerStart("loadGeo");
#endif
  // fill table
   checkUniqueVolume(m_browser->geoManager()->GetTopNode()->GetVolume());
   if (!m_filterOff)
      updateFilter();
   
   m_browser->updateStatusBar(Form("FWGeometryTableManager::loadGeometry() %d unique volumes", (int)m_volumes.size()));
   
 
   // Prepare data for cell render.
   m_browser->updateStatusBar("Set table content ...");
   
   // clear entries
   m_entries.clear();
   m_row_to_index.clear();
   m_volumes.clear();

   m_levelOffset = 0;


   // add top node to init
   NodeInfo topNodeInfo;
   topNodeInfo.m_node   = m_browser->geoManager()->GetTopNode();

   topNodeInfo.m_level  = 0;
   topNodeInfo.m_parent = -1;
   m_entries.push_back(topNodeInfo);

   importChildren(0, true);
   for (Entries_i i = m_entries.begin(); i != m_entries.end(); ++i)
   {
      if (i->m_level > m_browser->getAutoExpand()) i->m_expanded = false;
   }   


   if (1)
      checkHierarchy();
 
   redrawTable();
   
#ifdef PERFTOOL  
   ProfilerStop();
#endif

   if (m_filterOff)
   {
      m_browser->updateStatusBar(Form("%d entries imported ", (int)m_entries.size()));
   }
   else
   {
      {
         // get status
         int na = 0;
         int n = 0;
         for (Volumes_i i = m_volumes.begin(); i!= m_volumes.end(); ++i) 
         {
            n++;
            if ( i->second.m_matches)
            {
               na++;
               // printf("[%d] %s matches material %s \n", na, i->first->GetName(), i->first->GetMaterial()->GetName());
            }
         }

         m_browser->updateStatusBar(Form("%d entries imported, filter: %d volumes (%.2f %%) selected ", (int)m_entries.size(), na, na*1.f/n));
      }
   }
}

//==============================================================================

void
FWGeometryTableManager::getNNodesTotal(TGeoNode* geoNode, int level, int& off, bool debug) const
{   
   // Get number of nested children recursively.
   
   if (debug) printf("getNNodesTotal %s %s (c:%d)\033[22;0m \n", cyanTxt, geoNode->GetName(), level);
   
   int nD =  getNdaughtersLimited(geoNode);
   std::vector<int> vi; vi.reserve(nD);
   vi.reserve(nD);
   for (int n = 0; n != nD; ++n)
   {
      TGeoVolume* vTmp = geoNode->GetDaughter(n)->GetVolume();
      if (m_volumes[vTmp].accepted())
      {
         bool toAdd = true;
         if (m_browser->getVolumeMode())
         {
            for (std::vector<int>::iterator u = vi.begin(); u != vi.end(); ++u )
            {
               TGeoVolume* neighbourVolume = geoNode->GetDaughter(*u)->GetVolume();
               if (neighbourVolume == vTmp)
               {
                  toAdd = false;
                  break;
               }
            }
         } // end volume mode
         if (toAdd) vi.push_back(n);
      }
   }
   
   int nV = vi.size();
   {
      off += nV;
      for (int i = 0; i < nV; ++i )
      {
         getNNodesTotal(geoNode->GetDaughter(vi[i]), level+1, off, false);
      }
      if (debug) printf("%d \n", off);
   }
}

//______________________________________________________________________________

void FWGeometryTableManager::importChildren(int parent_idx, bool /*recurse*/)
{
   bool debug = 0;
   
   int nEntries = (int)m_entries.size();
   assert( parent_idx < nEntries);

   // parnt index not valid in recursive import:  save parent info here

   NodeInfo& parent        = m_entries[parent_idx];
   TGeoNode* parentGeoNode = parent.m_node; 
   int       parentLevel   = parent.m_level;   
   if (debug) printf("%s START level[%d] >  %s[%d]   \033[0m\n" ,greenTxt,  parentLevel+1, parentGeoNode->GetName(), parent_idx);

   
   // get indices of accepted nodes
   int nD = getNdaughtersLimited(parentGeoNode);
   std::vector<int> vi; 
   vi.reserve(nD);
   TGeoVolume* vTmp;

   for (int n = 0; n != nD; ++n)
   {
      vTmp = parentGeoNode->GetDaughter(n)->GetVolume();
      
      if (m_filterOff || m_volumes[vTmp].accepted())
      {
         bool toAdd = true;
         if (m_browser->getVolumeMode())
         {
            // check duplicates in added
            for (std::vector<int>::iterator u = vi.begin(); u != vi.end(); ++u )
            {
               TGeoVolume* neighbourVolume =  parentGeoNode->GetDaughter(*u)->GetVolume();
               if (neighbourVolume == vTmp)
               {
                  toAdd = false;
                  break;
               }
            }
         } // end volume mode
         if (toAdd) vi.push_back(n);         
      } // end checke filters
      
      
   } // end daughter loop
   int nV =  vi.size();
   
   // add  accepted nodes
   Entries_i it = m_entries.begin();
   std::advance(it, parent_idx+1);
   m_entries.insert(it, nV, NodeInfo());
   nEntries += nV; 
   //   if (debug)  printf(" accpted %d of %d entries size %d \n", nV, nD, (int)m_entries.size());
   
   // child nodes setup
   for (int n = 0; n != nV; ++n)
   {
      int childIdx = vi[n];
      NodeInfo &nodeInfo = m_entries[parent_idx + 1 + n ];
      nodeInfo.m_node =   parentGeoNode->GetDaughter(childIdx);
      nodeInfo.m_level =  parentLevel + 1;
      nodeInfo.m_parent = parent_idx;
      // 3D
      nodeInfo.m_color =  nodeInfo.m_node->GetVolume()->GetLineColor();
      if (debug)  printf(" add %s\n", nodeInfo.name());
   }
   
  
   // change of autoExpand parameter
   int dOff = 0;
   {
      for (int n = 0; n != nV; ++n)
      {
         importChildren(parent_idx + n + 1 + dOff, true);       
         if (parentGeoNode->GetNdaughters() > 0)
         {
            getNNodesTotal(parentGeoNode->GetDaughter(vi[n]), parentLevel+1, dOff, debug);
         }
            
      }
   }

   fflush(stdout);
}// end importChildren

//==============================================================================

void FWGeometryTableManager::checkHierarchy()
{
   // Used for debug: in a NodeInfo entry look TGeoNode children from parent index and check
   // if child is found.
   
   for ( size_t i = 0,  e = m_entries.size(); i != e; ++i )
   {
      if ( m_entries[i].m_level > 0)
      {
         TGeoNode* pn = m_entries[m_entries[i].m_parent].m_node;
         bool ok = false;
         for (int d = 0; d < pn->GetNdaughters(); ++d )
         {
            if (m_entries[i].m_node ==  pn->GetDaughter(d))
            {
               ok = true;
               break;
            }
         }
         if (!ok) printf("%s!!!!!! node %s has false parent %s \n", redTxt, m_entries[i].name(), pn->GetName());
      }   
   }
}

void FWGeometryTableManager::checkChildMatches(TGeoVolume* vol,  std::vector<TGeoVolume*>& pstack)
{
   if (m_volumes[vol].m_matches)
   {
      for (std::vector<TGeoVolume*>::iterator i = pstack.begin(); i!= pstack.end(); ++i)
      {
         Match& pm =  m_volumes[*i];
         //  if (0 && pm.m_childMatches)
         //   break;

         pm.m_childMatches = true;         
      }
   }

   pstack.push_back(vol);

   int nD = TMath::Min(m_browser->getMaxDaughters(), vol->GetNdaughters());
   for (int i = 0; i!=nD; ++i)
      checkChildMatches(vol->GetNode(i)->GetVolume(), pstack);
   
   pstack.pop_back();
}

// callbacks ______________________________________________________________________________

void FWGeometryTableManager::updateFilter()
{/*
   std::string filterExp =  m_browser->getFilter();
   m_filterOff =  filterExp.empty();

   if (!m_browser->geoManager()) return;
   
#ifdef PERFTOOL
   ProfilerStart(filterExp);
#endif

   for (Volumes_i i = m_volumes.begin(); i!= m_volumes.end(); ++i) 
   {
      i->second.m_matches = m_filterOff || strstr(i->first->GetMaterial()->GetName(), filterExp.c_str() );
      i->second.m_childMatches = false;
   }  
  
   std::vector<TGeoVolume*> pstack;
   checkChildMatches(m_browser->geoManager()->GetCurrentNode()->GetVolume(), pstack);
 
   //   printf("filterChanged \n");
#ifdef PERFTOOL
   ProfilerStop();
#endif
setTableContent();*/
}


FWGeometryTableManager::NodeInfo& FWGeometryTableManager::refSelected()
{
   return  m_entries[m_selectedIdx];
}

void FWGeometryTableManager::getNodePath(int idx, std::string& path)
{
   std::vector<std::string> relPath;
   while(idx >= 0)
   { 
      relPath.push_back( m_entries[idx].name());
      // printf("push %s \n",m_entries[idx].name() );
      idx  =  m_entries[idx].m_parent;
   }

   size_t ns = relPath.size();
   for (size_t i = 1; i < ns; ++i )
   {
      path +="/";
      path += relPath[ns-i -1];
      // printf("push_back add to path %s\n", path.c_str());
   }
}

//______________________________________________________________________________

void  FWGeometryTableManager::checkExpandLevel()
{
   // check expabd state
   int ae = m_browser->getAutoExpand() +  m_levelOffset;
   int cnt = 0;
   for (Entries_i i = m_entries.begin(); i != m_entries.end(); ++i)
   {
      i->m_expanded =  (i->m_level  < ae );
      cnt++;
   } 
}

//______________________________________________________________________________


void  FWGeometryTableManager::topGeoNodeChanged(int idx)
{
   m_geoTopNodeIdx = idx;

   // cached 
   if (idx >= 0)
      m_levelOffset = m_entries[idx].m_level;
   else
      m_levelOffset = 0;
}

void FWGeometryTableManager::printChildren(int idx) const
{
  
   //   static double pnt[3];

   for (int i =0, k = m_entries.size(); i < k; ++i)
   {
     const  NodeInfo&  ni = m_entries[i];
      if (ni.m_parent == idx)
      {
         std::string indent(ni.m_level*3 , ' ');

         //  printf("%s%s (%.2f, %.2f, %.2f)\n",indent.c_str(), ni.name(), pnt[0], pnt[1], pnt[2]);
         printf("%s%s \n",indent.c_str(), ni.name());

         printChildren(i);
      }
   }

}
