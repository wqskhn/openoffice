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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_filter.hxx"
#include <osl/mutex.hxx>
#include <vos/mutex.hxx>
#include <toolkit/awt/vclxwindow.hxx>

#include <osl/thread.h>
#include <cppuhelper/factory.hxx>
#include <cppuhelper/typeprovider.hxx>
#include <cppuhelper/component.hxx>
#include <com/sun/star/lang/XSingleServiceFactory.hpp>
#include <com/sun/star/frame/XDesktop.hpp>
#include <com/sun/star/frame/XTerminateListener.hpp>
#include <cppuhelper/implbase4.hxx>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/awt/XWindow.hpp>
#include <com/sun/star/ui/dialogs/XExecutableDialog.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <toolkit/awt/vclxwindow.hxx>
#include <tools/resmgr.hxx>
#include <vcl/svapp.hxx>

#include <svl/solar.hrc>

#include "xmlfiltersettingsdialog.hxx"

//using namespace ::comphelper;
using namespace ::rtl;
using namespace ::cppu;
using namespace ::osl;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::registry;
using namespace ::com::sun::star::frame;


class XMLFilterDialogComponentBase
{
protected:
	::osl::Mutex maMutex;
};


class XMLFilterDialogComponent :	public XMLFilterDialogComponentBase,
									public OComponentHelper,
									public ::com::sun::star::ui::dialogs::XExecutableDialog,
									public XServiceInfo,
									public XInitialization,
									public XTerminateListener
{
public:
	XMLFilterDialogComponent( const Reference< XMultiServiceFactory >& rxMSF );
	virtual ~XMLFilterDialogComponent();

protected:
	// XInterface
    virtual Any SAL_CALL queryInterface( const Type& aType ) throw (RuntimeException);
	virtual Any SAL_CALL queryAggregation( Type const & rType ) throw (RuntimeException);
    virtual void SAL_CALL acquire() throw ();
    virtual void SAL_CALL release() throw ();

	// XTypeProvider
	virtual Sequence< sal_Int8 > SAL_CALL getImplementationId() throw(RuntimeException);
	virtual Sequence< Type > SAL_CALL getTypes() throw (RuntimeException);

	// XServiceInfo
	virtual ::rtl::OUString SAL_CALL getImplementationName() throw(com::sun::star::uno::RuntimeException);
	virtual sal_Bool SAL_CALL supportsService(const ::rtl::OUString& ServiceName) throw(RuntimeException);
    virtual Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames(  ) throw (RuntimeException);

	// XExecutableDialog
	virtual void SAL_CALL setTitle( const ::rtl::OUString& aTitle ) throw(RuntimeException);
	virtual sal_Int16 SAL_CALL execute(  ) throw(RuntimeException);

	// XInitialization
	virtual void SAL_CALL initialize( const Sequence< Any >& aArguments ) throw(Exception, RuntimeException);

	// XTerminateListener
	virtual void SAL_CALL queryTermination( const EventObject& Event ) throw (TerminationVetoException, RuntimeException);
	virtual void SAL_CALL notifyTermination( const EventObject& Event ) throw (RuntimeException);
    virtual void SAL_CALL disposing( const EventObject& Source ) throw (RuntimeException);

	/** Called in dispose method after the listeners were notified.
    */
	virtual void SAL_CALL disposing();

private:
	com::sun::star::uno::Reference<com::sun::star::awt::XWindow> mxParent;	/// parent window
	com::sun::star::uno::Reference< XMultiServiceFactory > mxMSF;

	static ResMgr* mpResMgr;
	XMLFilterSettingsDialog* mpDialog;
};

//-------------------------------------------------------------------------

ResMgr* XMLFilterDialogComponent::mpResMgr = NULL;

XMLFilterDialogComponent::XMLFilterDialogComponent( const com::sun::star::uno::Reference< XMultiServiceFactory >& rxMSF ) :
	OComponentHelper( maMutex ),
	mxMSF( rxMSF ),
	mpDialog( NULL )
{
	Reference< XDesktop > xDesktop( mxMSF->createInstance( OUString::createFromAscii( "com.sun.star.frame.Desktop" ) ), UNO_QUERY );
	if( xDesktop.is() )
	{
		Reference< XTerminateListener > xListener( this );
		xDesktop->addTerminateListener( xListener );
	}
}

//-------------------------------------------------------------------------

XMLFilterDialogComponent::~XMLFilterDialogComponent()
{
}

//-------------------------------------------------------------------------

// XInterface
Any SAL_CALL XMLFilterDialogComponent::queryInterface( const Type& aType ) throw (RuntimeException)
{
	return OComponentHelper::queryInterface( aType );
}

//-------------------------------------------------------------------------

Any SAL_CALL XMLFilterDialogComponent::queryAggregation( Type const & rType ) throw (RuntimeException)
{
    if (rType == ::getCppuType( (Reference< ::com::sun::star::ui::dialogs::XExecutableDialog > const *)0 ))
    {
        void * p = static_cast< ::com::sun::star::ui::dialogs::XExecutableDialog * >( this );
        return Any( &p, rType );
    }
    else if (rType == ::getCppuType( (Reference< XServiceInfo > const *)0 ))
    {
        void * p = static_cast< XServiceInfo * >( this );
        return Any( &p, rType );
    }
    else if (rType == ::getCppuType( (Reference< XInitialization > const *)0 ))
    {
        void * p = static_cast< XInitialization * >( this );
        return Any( &p, rType );
    }
	else if (rType == ::getCppuType( (Reference< XTerminateListener > const *)0 ))
	{
        void * p = static_cast< XTerminateListener * >( this );
        return Any( &p, rType );
	}
    return OComponentHelper::queryAggregation( rType );
}

//-------------------------------------------------------------------------

void SAL_CALL XMLFilterDialogComponent::acquire() throw ()
{
    OComponentHelper::acquire();
}

//-------------------------------------------------------------------------

void SAL_CALL XMLFilterDialogComponent::release() throw ()
{
    OComponentHelper::release();
}

//-------------------------------------------------------------------------

OUString XMLFilterDialogComponent_getImplementationName() throw ( RuntimeException )
{
	return OUString( RTL_CONSTASCII_USTRINGPARAM( "XMLFilterDialogComponent" ) );
}

//-------------------------------------------------------------------------

Sequence< OUString > SAL_CALL XMLFilterDialogComponent_getSupportedServiceNames()  throw ( RuntimeException )
{
	OUString aServiceName( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.comp.ui.XSLTFilterDialog" ) );
	Sequence< ::rtl::OUString > aSupported( &aServiceName, 1 );
	return aSupported;
}

//-------------------------------------------------------------------------

sal_Bool SAL_CALL XMLFilterDialogComponent_supportsService( const OUString& ServiceName ) throw ( RuntimeException )
{
	Sequence< ::rtl::OUString > aSupported(XMLFilterDialogComponent_getSupportedServiceNames());
	const ::rtl::OUString* pArray = aSupported.getConstArray();
	for (sal_Int32 i = 0; i < aSupported.getLength(); ++i, ++pArray)
		if (pArray->equals(ServiceName))
			return sal_True;
	return sal_False;
}

//-------------------------------------------------------------------------

Reference< XInterface > SAL_CALL XMLFilterDialogComponent_createInstance( const Reference< XMultiServiceFactory > & rSMgr) throw ( Exception )
{
	return (OWeakObject*)new XMLFilterDialogComponent( rSMgr );
}

//-------------------------------------------------------------------------
::rtl::OUString SAL_CALL XMLFilterDialogComponent::getImplementationName() throw(com::sun::star::uno::RuntimeException)
{
	return XMLFilterDialogComponent_getImplementationName();
}

//-------------------------------------------------------------------------

Sequence< sal_Int8 > SAL_CALL XMLFilterDialogComponent::getImplementationId( void ) throw( RuntimeException )
{
	static OImplementationId* pId = 0;
	if( !pId )
	{
		MutexGuard aGuard( Mutex::getGlobalMutex() );
		if( !pId)
		{
			static OImplementationId aId;
			pId = &aId;
		}
	}
	return pId->getImplementationId();
}

//-------------------------------------------------------------------------

Sequence< Type > XMLFilterDialogComponent::getTypes() throw (RuntimeException)
{
	static OTypeCollection * s_pTypes = 0;
	if (! s_pTypes)
	{
		MutexGuard aGuard( Mutex::getGlobalMutex() );
		if (! s_pTypes)
		{
			static OTypeCollection s_aTypes(
				::getCppuType( (const Reference< XComponent > *)0 ),
				::getCppuType( (const Reference< XTypeProvider > *)0 ),
				::getCppuType( (const Reference< XAggregation > *)0 ),
				::getCppuType( (const Reference< XWeak > *)0 ),
				::getCppuType( (const Reference< XServiceInfo > *)0 ),
				::getCppuType( (const Reference< XInitialization > *)0 ),
				::getCppuType( (const Reference< XTerminateListener > *)0 ),
				::getCppuType( (const Reference< ::com::sun::star::ui::dialogs::XExecutableDialog > *)0 ));
			s_pTypes = &s_aTypes;
		}
	}
	return s_pTypes->getTypes();
}

//-------------------------------------------------------------------------

Sequence< ::rtl::OUString > SAL_CALL XMLFilterDialogComponent::getSupportedServiceNames() throw(com::sun::star::uno::RuntimeException)
{
	return XMLFilterDialogComponent_getSupportedServiceNames();
}

//-------------------------------------------------------------------------
sal_Bool SAL_CALL XMLFilterDialogComponent::supportsService(const ::rtl::OUString& ServiceName) throw(RuntimeException)
{
	return XMLFilterDialogComponent_supportsService( ServiceName );
}

//-------------------------------------------------------------------------

/** Called in dispose method after the listeners were notified.
*/
void SAL_CALL XMLFilterDialogComponent::disposing()
{
	vos::OGuard aGuard( Application::GetSolarMutex() );

	if( mpDialog )
	{
		delete mpDialog;
		mpDialog = NULL;
	}

	if( mpResMgr )
	{
		delete mpResMgr;
		mpResMgr = NULL;
	}
}

//-------------------------------------------------------------------------

// XTerminateListener
void SAL_CALL XMLFilterDialogComponent::queryTermination( const EventObject& /* Event */ ) throw (TerminationVetoException, RuntimeException)
{
	vos::OGuard aGuard( Application::GetSolarMutex() );

	// we will never give a veto here
	if( mpDialog && !mpDialog->isClosable() )
	{
		mpDialog->ToTop();
		throw TerminationVetoException();
	}
}

//-------------------------------------------------------------------------

void SAL_CALL XMLFilterDialogComponent::notifyTermination( const EventObject& /* Event */ ) throw (RuntimeException)
{
	// we are going down, so dispose us!
	dispose();
}

void SAL_CALL XMLFilterDialogComponent::disposing( const EventObject& /* Source */ ) throw (RuntimeException)
{
}

//-------------------------------------------------------------------------
void SAL_CALL XMLFilterDialogComponent::setTitle( const ::rtl::OUString& /* _rTitle */ ) throw(RuntimeException)
{
}

//-------------------------------------------------------------------------
sal_Int16 SAL_CALL XMLFilterDialogComponent::execute(  ) throw(RuntimeException)
{
	vos::OGuard aGuard( Application::GetSolarMutex() );

	if( NULL == mpResMgr )
	{
		ByteString aResMgrName( "xsltdlg" );

		mpResMgr = ResMgr::CreateResMgr( aResMgrName.GetBuffer(), Application::GetSettings().GetUILocale() );
	}

	if( NULL == mpDialog )
	{
		Window* pParent = NULL;
		if( mxParent.is() )
		{
			VCLXWindow* pImplementation = VCLXWindow::GetImplementation(mxParent);
			if (pImplementation)
				pParent = pImplementation->GetWindow();
		}

		Reference< XComponent > xComp( this );
		mpDialog = new XMLFilterSettingsDialog( pParent, *mpResMgr, mxMSF );
		mpDialog->ShowWindow();
	}
	else if( !mpDialog->IsVisible() )
	{
		mpDialog->ShowWindow();
	}
	mpDialog->ToTop();

	return 0;
}

//-------------------------------------------------------------------------
void SAL_CALL XMLFilterDialogComponent::initialize( const Sequence< Any >& aArguments ) throw(Exception, RuntimeException)
{
	const Any* pArguments = aArguments.getConstArray();
	for(sal_Int32 i=0; i<aArguments.getLength(); ++i, ++pArguments)
	{
		PropertyValue aProperty;
		if(*pArguments >>= aProperty)
		{
			if( aProperty.Name.compareToAscii( RTL_CONSTASCII_STRINGPARAM( "ParentWindow" ) ) == 0 )
			{
				aProperty.Value >>= mxParent;
			}
		}
	}
}


extern "C"
{
//==================================================================================================
void SAL_CALL component_getImplementationEnvironment(
	const sal_Char ** ppEnvTypeName, uno_Environment ** /* ppEnv */ )
{
	*ppEnvTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME;
}

//==================================================================================================
void * SAL_CALL component_getFactory(
	const sal_Char * pImplName, void * pServiceManager, void * /* pRegistryKey */ )
{
	void * pRet = 0;
	
	if( pServiceManager )
	{
		Reference< XSingleServiceFactory > xFactory;

		OUString implName = OUString::createFromAscii( pImplName );
		if ( implName.equals(XMLFilterDialogComponent_getImplementationName()) )
		{
			xFactory = createOneInstanceFactory(
				reinterpret_cast< XMultiServiceFactory * >( pServiceManager ),
				OUString::createFromAscii( pImplName ),
				XMLFilterDialogComponent_createInstance, XMLFilterDialogComponent_getSupportedServiceNames() );
			
		}

		if (xFactory.is())
		{
			xFactory->acquire();
			pRet = xFactory.get();
		}
	}

	return pRet;
}
}
