

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Sun Oct 19 12:54:09 2014
 */
/* Compiler settings for ..\..\idl\Kinect.INPC.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Kinect2EINPC_h__
#define __Kinect2EINPC_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __INotifyPropertyChanged_FWD_DEFINED__
#define __INotifyPropertyChanged_FWD_DEFINED__
typedef interface INotifyPropertyChanged INotifyPropertyChanged;

#endif 	/* __INotifyPropertyChanged_FWD_DEFINED__ */


#ifndef __IPropertyChangedEventArgs_FWD_DEFINED__
#define __IPropertyChangedEventArgs_FWD_DEFINED__
typedef interface IPropertyChangedEventArgs IPropertyChangedEventArgs;

#endif 	/* __IPropertyChangedEventArgs_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "mmreg.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_Kinect2EINPC_0000_0000 */
/* [local] */ 


typedef INT_PTR WAITABLE_HANDLE;

typedef INT64 TIMESPAN;





extern RPC_IF_HANDLE __MIDL_itf_Kinect2EINPC_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Kinect2EINPC_0000_0000_v0_0_s_ifspec;

#ifndef __INotifyPropertyChanged_INTERFACE_DEFINED__
#define __INotifyPropertyChanged_INTERFACE_DEFINED__

/* interface INotifyPropertyChanged */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_INotifyPropertyChanged;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D27A5C77-32E9-4283-A046-9D693E29E3E7")
    INotifyPropertyChanged : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribePropertyChanged( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribePropertyChanged( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPropertyChangedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            UINT bufferSize,
            /* [annotation][out][retval] */ 
            _Out_writes_z_(bufferSize)  WCHAR *propertyName) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct INotifyPropertyChangedVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INotifyPropertyChanged * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INotifyPropertyChanged * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INotifyPropertyChanged * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribePropertyChanged )( 
            INotifyPropertyChanged * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribePropertyChanged )( 
            INotifyPropertyChanged * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetPropertyChangedEventData )( 
            INotifyPropertyChanged * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            UINT bufferSize,
            /* [annotation][out][retval] */ 
            _Out_writes_z_(bufferSize)  WCHAR *propertyName);
        
        END_INTERFACE
    } INotifyPropertyChangedVtbl;

    interface INotifyPropertyChanged
    {
        CONST_VTBL struct INotifyPropertyChangedVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INotifyPropertyChanged_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INotifyPropertyChanged_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INotifyPropertyChanged_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INotifyPropertyChanged_SubscribePropertyChanged(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribePropertyChanged(This,waitableHandle) ) 

#define INotifyPropertyChanged_UnsubscribePropertyChanged(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribePropertyChanged(This,waitableHandle) ) 

#define INotifyPropertyChanged_GetPropertyChangedEventData(This,waitableHandle,bufferSize,propertyName)	\
    ( (This)->lpVtbl -> GetPropertyChangedEventData(This,waitableHandle,bufferSize,propertyName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INotifyPropertyChanged_INTERFACE_DEFINED__ */


#ifndef __IPropertyChangedEventArgs_INTERFACE_DEFINED__
#define __IPropertyChangedEventArgs_INTERFACE_DEFINED__

/* interface IPropertyChangedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IPropertyChangedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("574E9321-9DD9-41C8-BC51-C11F3F38D6B5")
    IPropertyChangedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PropertyName( 
            UINT bufferSize,
            /* [annotation][out][retval] */ 
            _Out_writes_z_(bufferSize)  WCHAR *propertyName) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IPropertyChangedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPropertyChangedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPropertyChangedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPropertyChangedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PropertyName )( 
            IPropertyChangedEventArgs * This,
            UINT bufferSize,
            /* [annotation][out][retval] */ 
            _Out_writes_z_(bufferSize)  WCHAR *propertyName);
        
        END_INTERFACE
    } IPropertyChangedEventArgsVtbl;

    interface IPropertyChangedEventArgs
    {
        CONST_VTBL struct IPropertyChangedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyChangedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPropertyChangedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPropertyChangedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPropertyChangedEventArgs_get_PropertyName(This,bufferSize,propertyName)	\
    ( (This)->lpVtbl -> get_PropertyName(This,bufferSize,propertyName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPropertyChangedEventArgs_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


