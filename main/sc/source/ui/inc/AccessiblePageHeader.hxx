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

 


#ifndef _SC_ACCESSIBLEPAGEHEADER_HXX
#define _SC_ACCESSIBLEPAGEHEADER_HXX

#include "AccessibleContextBase.hxx"
#include <editeng/svxenum.hxx>

class ScPreviewShell;
class EditTextObject;
class ScAccessiblePageHeaderArea;
class ScPreviewShell;

class ScAccessiblePageHeader : public ScAccessibleContextBase
{
public:
	ScAccessiblePageHeader( const ::com::sun::star::uno::Reference<
						        ::com::sun::star::accessibility::XAccessible>& rxParent,
							ScPreviewShell* pViewShell, sal_Bool bHeader, sal_Int32 nIndex );

protected:
	virtual ~ScAccessiblePageHeader();

    using ScAccessibleContextBase::IsDefunc;

public:
    using ScAccessibleContextBase::disposing;
 	virtual void SAL_CALL disposing();

	//=====  SfxListener  =====================================================

	virtual void Notify( SfxBroadcaster& rBC, const SfxHint& rHint );

	//=====  XAccessibleComponent  ============================================

    virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SAL_CALL
    						getAccessibleAtPoint( const ::com::sun::star::awt::Point& aPoint )
    							throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL	grabFocus() throw (::com::sun::star::uno::RuntimeException);

	//=====  XAccessibleContext  ==============================================

    virtual sal_Int32 SAL_CALL getAccessibleChildCount() throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SAL_CALL
    						getAccessibleChild( sal_Int32 i )
    							throw (::com::sun::star::lang::IndexOutOfBoundsException,
    								::com::sun::star::uno::RuntimeException);
    virtual sal_Int32 SAL_CALL getAccessibleIndexInParent() throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessibleStateSet > SAL_CALL
    						getAccessibleStateSet() throw (::com::sun::star::uno::RuntimeException);

	//=====  XServiceInfo  ====================================================

	virtual ::rtl::OUString SAL_CALL getImplementationName()
								throw(::com::sun::star::uno::RuntimeException);
	virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames()
								throw(::com::sun::star::uno::RuntimeException);

    //=====  internal  ========================================================
    void SetCurrentIndexInParent(sal_Int32 nNew) { mnIndex = nNew; }

protected:
	virtual ::rtl::OUString SAL_CALL createAccessibleDescription(void) throw(::com::sun::star::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL createAccessibleName(void) throw (::com::sun::star::uno::RuntimeException);

	virtual Rectangle GetBoundingBoxOnScreen(void) const throw(::com::sun::star::uno::RuntimeException);
	virtual Rectangle GetBoundingBox(void) const throw (::com::sun::star::uno::RuntimeException);

private:
	ScPreviewShell*		mpViewShell;
	sal_Int32			mnIndex;
	sal_Bool			mbHeader;
    typedef std::vector< ScAccessiblePageHeaderArea* > ScHFAreas;
    ScHFAreas           maAreas;
    sal_Int32           mnChildCount;

	sal_Bool IsDefunc(
		const com::sun::star::uno::Reference<
		::com::sun::star::accessibility::XAccessibleStateSet>& rxParentStates);

    void AddChild(const EditTextObject* pArea, sal_uInt32 nIndex, SvxAdjust eAdjust);
};


#endif
