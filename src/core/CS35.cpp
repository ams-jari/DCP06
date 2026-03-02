
#include "stdafx.h"
#include <dcp06/core/CS35.hpp>
#include "dcp06/core/DCP_Defs.hpp"
#include <dcp06/core/DCP_MsgBox.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

 
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP06CS35C::DCP06CS35C()
{
	
}

// ****************************************************************************************
DCP::DCP06CS35C::~DCP06CS35C()
{
	
} 

 
// ****************************************************************************************
void DCP::DCP06CS35C::get_serialnumber(char* serialNumber)
{
	serialNumber[0] = '\0';

	//#if defined(HW_X86) && defined(CS35) 
    #if defined(CS35) 
		HRESULT lResult = CoInitializeSecurity(
            NULL,
            -1,
            NULL,
            NULL,
            RPC_C_AUTHN_LEVEL_DEFAULT,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE,
            NULL
      );

      CComPtr<IWbemLocator> pLocator;

      lResult = pLocator.CoCreateInstance( CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER );

      if ( FAILED( lResult ) ) {
            return; //("Failed to create IWbemLocator object.", lResult);
      }

      CComPtr<IWbemServices> pService;

      lResult = pLocator->ConnectServer( _bstr_t( L"ROOT\\CIMV2" ), NULL, NULL, 0, NULL, 0, 0, &pService );

      if ( FAILED( lResult ) ) {
            return ; // ("Could not connect.", lResult);
      }

      lResult = CoSetProxyBlanket(
            pService,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            NULL,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL, EOAC_NONE
      );

      if ( FAILED( lResult ) ) {
            return ; // ("Could not set proxy blanket.", lResult);
      }

      CComPtr<IEnumWbemClassObject> pEnumerator;

      lResult = pService->ExecQuery(
            L"WQL",
            L"SELECT * FROM Win32_ComputerSystemProduct",
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator
      );

      if ( FAILED( lResult ) ) {
            return; // ( "Query for operating system name failed.", lResult);
      }

      CComPtr<IWbemClassObject> pComputerProduct;

      ULONG                           uObjectCount = 0;

      lResult = pEnumerator->Next( WBEM_INFINITE, 1, &pComputerProduct, &uObjectCount );

      if ( FAILED( lResult ) ) {
            return; // ( "Failed iterating enumerator.", lResult);
      }

      if( uObjectCount == 0 ) {
            return ;
      }

      VARIANT /*vUuidProperty,*/ vSerialNumber;

      //lResult = pComputerProduct->Get( L"UUID", 0, &vUuidProperty, 0, 0 );
      //if ( FAILED( lResult ) ) {
      //    return; // ( "Failed obtaining UUID.", lResult);
      //}

      //(void)BSS::UTI::BSS_UTI_WCharToUtf8( vUuidProperty.bstrVal, strUuid );

      lResult = pComputerProduct->Get( L"IdentifyingNumber", 0, &vSerialNumber, 0, 0 );

      if ( FAILED( lResult ) ) {
            return ; // ( "Failed obtaining SerialNumber.", lResult);
      }

      //std::string strSerialNumber;
      //(void)BSS::UTI::BSS_UTI_WCharToUtf8( vSerialNumber.bstrVal, strSerialNumber );

	   BSS::UTI::BSS_UTI_WCharToAscii(vSerialNumber.bstrVal, serialNumber);

	   /*DCP06MsgBoxC msgBox;	
 	   msgBox.ShowMessageOk(StringC(serialNumber));*/
		
	   

	   //serialNumber = StringC(strSerialNumber);
      // Msg

     /* ABL::MsgDefC oMsgDef;

      oMsgDef.SetMsgText( strSerialNumber );

      oMsgDef.SetMsgType( GUI::MessageDialogC::MSG_Warning );

      oMsgDef.SetResultMask( GUI::MessageDialogC::MBR_Ok );

      (void)ABL::ShowMessage( oMsgDef );
	  */
	
	#endif
}
