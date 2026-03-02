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
// $Date: 6.07.04 8:55 $
// $Revision: 1 $
// $Modtime: 5.07.04 14:55 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef DCP_TYPES_HPP
#define DCP_TYPES_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#ifdef WIN32
    #if defined(DCP06_EXPORTS) || defined(DCP05_EXPORTS)
    #define DCP_API __declspec(dllexport)
    #else
    #define DCP_API __declspec(dllimport)
    #endif
#else
    #define DCP_API
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================

// ================================================================================================
// ===========================================  enums  ============================================
// ================================================================================================
namespace DCP
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
        AT_DCP05 = 15750,   // DCP05 (legacy)
        AT_DCP06 = 15751    // DCP06 - onboard 3D measurement for Leica total stations
    }; // enum ApplTypeT
    enum CnfKeyT
    {
        CNF_KEY_INIT = 105,//2,
		CNF_KEY_DOM = 3,
		CNF_KEY_POM = 4,	 
		CNF_KEY_CHST = 5,
		CNF_KEY_USERDEF_OLD = 6,
		CNF_KEY_CIRCLE =70,//7,
		CNF_KEY_SHAFT =80,//8,
		CNF_KEY_USERDEF = 9,
		CNF_KEY_LICENSE = 101,
		CNF_KEY_LINE_FITTING = 104,//103//102
		CNF_KEY_DEMO_LICENSES = 110
    };

};


// ================================================================================================
// ==========================================  constants  =========================================
// ================================================================================================


// ================================================================================================
// ============================================  types  ===========================================
// ================================================================================================

#endif // DCP_TYPES_HPP


