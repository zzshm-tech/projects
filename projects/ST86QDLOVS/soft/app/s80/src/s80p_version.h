//==============================================================================
//                                                                              
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.              
//                            All Rights Reserved                               
//                                                                              
//      This source code is the property of Coolsand Technologies and is        
//      confidential.  Any  modification, distribution,  reproduction or        
//      exploitation  of  any content of this file is totally forbidden,        
//      except  with the  written permission  of  Coolsand Technologies.        
//                                                                              
//==============================================================================
//                                                                              
//    THIS FILE WAS GENERATED AUTOMATICALLY BY THE MAKE PROCESS.
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                             
//                                                                              
//==============================================================================

#ifndef _S80_VERSION_H_
#define _S80_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define S80_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef S80_VERSION_NUMBER
#define S80_VERSION_NUMBER                       (0)
#endif

#ifndef S80_VERSION_DATE
#define S80_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef S80_VERSION_STRING
#define S80_VERSION_STRING                       "S80 version string not defined"
#endif

#ifndef S80_VERSION_STRING_WITH_BRANCH
#define S80_VERSION_STRING_WITH_BRANCH           S80_VERSION_STRING " Branch: " "none"
#endif

#define S80_VERSION_STRUCT                       {S80_VERSION_REVISION, \
                                                  S80_VERSION_NUMBER, \
                                                  S80_VERSION_DATE, \
                                                  S80_VERSION_STRING_WITH_BRANCH}

#endif // _S80_VERSION_H_
