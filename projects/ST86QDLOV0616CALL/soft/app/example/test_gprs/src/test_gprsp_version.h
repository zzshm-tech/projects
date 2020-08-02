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

#ifndef _TEST_GPRS_VERSION_H_
#define _TEST_GPRS_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define TEST_GPRS_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef TEST_GPRS_VERSION_NUMBER
#define TEST_GPRS_VERSION_NUMBER                       (0)
#endif

#ifndef TEST_GPRS_VERSION_DATE
#define TEST_GPRS_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef TEST_GPRS_VERSION_STRING
#define TEST_GPRS_VERSION_STRING                       "TEST_GPRS version string not defined"
#endif

#ifndef TEST_GPRS_VERSION_STRING_WITH_BRANCH
#define TEST_GPRS_VERSION_STRING_WITH_BRANCH           TEST_GPRS_VERSION_STRING " Branch: " "none"
#endif

#define TEST_GPRS_VERSION_STRUCT                       {TEST_GPRS_VERSION_REVISION, \
                                                  TEST_GPRS_VERSION_NUMBER, \
                                                  TEST_GPRS_VERSION_DATE, \
                                                  TEST_GPRS_VERSION_STRING_WITH_BRANCH}

#endif // _TEST_GPRS_VERSION_H_
