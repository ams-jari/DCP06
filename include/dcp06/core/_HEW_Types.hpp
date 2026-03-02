// ====================================================================================================================
//
// Project  : Pluto/Venus
//
// Component: Hello World project for loadable applications
//
// $Workfile: HEW_Types.hpp $
//
// Summary  : definition of datatypes and enums
//
// ------------------------------------------------------------------------------------------------
//
// Copyright by Leica Geosystems AG, Heerbrugg
//
// ================================================================================================


// $Author: Hlar $
// $Date: 9.06.04 14:37 $
// $Revision: 1 $
// $Modtime: 26.05.04 15:41 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef HEW_TYPES_HPP
#define HEW_TYPES_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#ifdef WIN32
    #ifdef HELLOWORLD_EXPORTS
    #define HEW_API __declspec(dllexport)
    #else
    #define HEW_API __declspec(dllimport)
    #endif
#else
    #define HEW_API
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================

// ================================================================================================
// ===========================================  enums  ============================================
// ================================================================================================
namespace HEW
{
    // Name       : ApplTypeT
    // Description: This enumeration type represents all application types predefined by Leica.
    // Remarks    : To create an instance of an DbApplicationC having such a predefined type
    //              you have to use a constructor, that take an ApplType parameter.
    //              You cannot change the type later using the set-method.
    //              The set method can be used only to change the type of a user application,
    //              which is an application having type AT_Null or type greater or equal to
    //              AT_FirstUserApp.
    enum ApplTypeT
    {
        AT_DCP05 = 15750     // Hello World id (startable application & DbApplicationC id)
    }; // enum ApplTypeT
};


// ================================================================================================
// ==========================================  constants  =========================================
// ================================================================================================


// ================================================================================================
// ============================================  types  ===========================================
// ================================================================================================

#endif // HEW_TYPES_HPP


