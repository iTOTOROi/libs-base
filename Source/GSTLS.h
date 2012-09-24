/** Interface for GSTLS classes for GNUStep
   Copyright (C) 2012 Free Software Foundation, Inc.

   Written by:  Richard Frith-Macdonald <rfm@gnu.org>
   Date: 2101

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02111 USA.

   */

#import "Foundation/NSObject.h"

@class  NSDate;
@class  NSDictionary;
@class  NSHost;
@class  NSString;

#if     defined(HAVE_GNUTLS)
/* Temporarily redefine 'id' in case the headers use the objc reserved word.
 */
#define	id	GNUTLSID
/* gcrypt uses __attribute__((deprecated)) to mark structure members that are
 * private.  This causes compiler warnings just from using the header.  Turn
 * them off...
 */
#define	_GCRYPT_IN_LIBGCRYPT
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#include <gcrypt.h>
#undef	id

@protocol       GSTLSOwner

/* Returns the optins dictionary set for this session.
 */
- (NSDictionary*) options;

/* Returns the host this session should be connected to.
 */
- (NSHost*) remoteHost;

@end

/* This class is used to ensure that the GNUTLS system is initialised
 * and thread-safe.  It also provides session verification.
 */
@interface      GSTLSObject : NSObject

/* Performs verification for the supplied session and returns a GNUTLS
 * error code in the event of verification failure or zero on success.<br />
 * The ponter set in the session with gnutls_session_set_ptr() must be
 * the owner of the session and must conform to the GSTLSOwner protocol. 
 */
+ (int) verify: (gnutls_session_t)session;
@end

/* This class provides the current DH paraqmeters for server negotiation.
 */
@interface      GSTLSDHParams : GSTLSObject
{
  gnutls_dh_params_t    params;
}
+ (GSTLSDHParams*) current;
- (gnutls_dh_params_t) params;
@end

/* Manage certificate lists (for servers and clients) and also provide
 * DH params.
 */
@interface      GSTLSCertificateList : GSTLSObject
{
  NSDate                *when;
  NSString              *path;
  gnutls_x509_crt_t     *crts;
  unsigned int          count;
}
+ (GSTLSCertificateList*) listFromFile: (NSString*)f;
- (gnutls_x509_crt_t*) certificateList;
- (unsigned int) count;
@end

/* This encapsulates private keys used to unlock certificates
 */
@interface      GSTLSPrivateKey : GSTLSObject
{
  NSDate                *when;
  NSString              *path;
  NSString              *password;
  gnutls_x509_privkey_t key;
}
+ (GSTLSPrivateKey*) keyFromFile: (NSString*)f withPassword: (NSString*)p;
- (gnutls_x509_privkey_t) key;
@end

#endif
