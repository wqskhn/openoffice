/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

#include "precompiled_sfx2.hxx"

#include "Panel.hxx"
#include "PanelTitleBar.hxx"
#include "PanelDescriptor.hxx"
#include "sfx2/sidebar/Theme.hxx"
#include "Paint.hxx"
#include "ResourceManager.hxx"

#ifdef DEBUG
#include "sfx2/sidebar/Tools.hxx"
#include "Deck.hxx"
#endif

#include <tools/svborder.hxx>
#include <toolkit/helper/vclunohelper.hxx>

#include <com/sun/star/awt/XWindowPeer.hpp>
#include <com/sun/star/awt/PosSize.hpp>
#include <com/sun/star/ui/XToolPanel.hpp>

#include <boost/bind.hpp>


using namespace css;
using namespace cssu;



namespace sfx2 { namespace sidebar {

Panel::Panel (
    const PanelDescriptor& rPanelDescriptor,
    Window* pParentWindow,
    const bool bIsInitiallyExpanded,
    const ::boost::function<void(void)>& rDeckLayoutTrigger,
    const ::boost::function<Context(void)>& rContextAccess)
    : Window(pParentWindow),
      msPanelId(rPanelDescriptor.msId),
      mpTitleBar(new PanelTitleBar(
              rPanelDescriptor.msTitle,
              pParentWindow,
              this)),
      mbIsTitleBarOptional(rPanelDescriptor.mbIsTitleBarOptional),
      mxElement(),
      mxPanelComponent(),
      mbIsExpanded(bIsInitiallyExpanded),
      maDeckLayoutTrigger(rDeckLayoutTrigger),
      maContextAccess(rContextAccess)
{
    SetBackground(Theme::GetPaint(Theme::Paint_PanelBackground).GetWallpaper());

#ifdef DEBUG
    SetText(A2S("Panel"));
#endif
}




Panel::~Panel (void)
{
    Dispose();
}




void Panel::Dispose (void)
{
    mxPanelComponent = NULL;

    {    
        Reference<lang::XComponent> xComponent (mxElement, UNO_QUERY);
        mxElement = NULL;
        if (xComponent.is())
            xComponent->dispose();
    }

    {
        Reference<lang::XComponent> xComponent (GetElementWindow(), UNO_QUERY);
        if (xComponent.is())
            xComponent->dispose();
    }

    mpTitleBar.reset();
}




PanelTitleBar* Panel::GetTitleBar (void) const
{
    return mpTitleBar.get();
}




bool Panel::IsTitleBarOptional (void) const
{
    return mbIsTitleBarOptional;
}




void Panel::SetUIElement (const Reference<ui::XUIElement>& rxElement)
{
    mxElement = rxElement;
    if (mxElement.is())
    {
        mxPanelComponent.set(mxElement->getRealInterface(), UNO_QUERY);
    }
}




void Panel::SetExpanded (const bool bIsExpanded)
{
    if (mbIsExpanded != bIsExpanded)
    {
        mbIsExpanded = bIsExpanded;
        maDeckLayoutTrigger();

        if (maContextAccess)
            ResourceManager::Instance().StorePanelExpansionState(
                msPanelId,
                bIsExpanded,
                maContextAccess());
    }
}




bool Panel::IsExpanded (void) const
{
    return mbIsExpanded;
}




bool Panel::HasIdPredicate (const ::rtl::OUString& rsId) const
{
    if (this == NULL)
        return false;
    else
        return msPanelId.equals(rsId);
}




const ::rtl::OUString& Panel::GetId (void) const
{
    return msPanelId;
}




void Panel::Paint (const Rectangle& rUpdateArea)
{
    Window::Paint(rUpdateArea);
}




void Panel::Resize (void)
{
    Window::Resize();

    // Forward new size to window of XUIElement.
    Reference<awt::XWindow> xElementWindow (GetElementWindow());
    if (xElementWindow.is())
    {
        const Size aSize (GetSizePixel());
        xElementWindow->setPosSize(
            0,
            0,
            aSize.Width(),
            aSize.Height(),
            awt::PosSize::POSSIZE);
    }
}




void Panel::Activate (void)
{
    Window::Activate();
}





void Panel::DataChanged (const DataChangedEvent& rEvent)
{
    (void)rEvent;
    SetBackground(Theme::GetPaint(Theme::Paint_PanelBackground).GetWallpaper());
}




Reference<ui::XSidebarPanel> Panel::GetPanelComponent (void) const
{
    return mxPanelComponent;
}




void Panel::PrintWindowTree (void)
{
#ifdef DEBUG
    Window* pElementWindow = VCLUnoHelper::GetWindow(GetElementWindow());
    if (pElementWindow != NULL)
    {
        OSL_TRACE("panel parent is %x", pElementWindow->GetParent());
        Deck::PrintWindowSubTree(pElementWindow, 2);
    }
    else
        OSL_TRACE("    panel is empty");
#endif
}




Reference<awt::XWindow> Panel::GetElementWindow (void)
{
    if (mxElement.is())
    {
        Reference<ui::XToolPanel> xToolPanel(mxElement->getRealInterface(), UNO_QUERY);
        if (xToolPanel.is())
            return xToolPanel->getWindow();
    }

    return NULL;
}


} } // end of namespace sfx2::sidebar
