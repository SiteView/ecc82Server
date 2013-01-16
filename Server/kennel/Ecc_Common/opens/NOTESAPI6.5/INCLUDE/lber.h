
#ifndef _LBER_H
#define _LBER_H

#ifdef __cplusplus
extern "C" {
#endif


#define LBER_BIG_TAG_MASK	0x1f
#define LBER_MORE_TAG_MASK	0x80

/*
 * Note that LBER_ERROR and LBER_DEFAULT are values that can never appear
 * as valid BER tags, and so it is safe to use them to report errors.  In
 * fact, any tag for which the following is true is invalid:
 *     (( tag & 0x00000080 ) != 0 ) && (( tag & 0xFFFFFF00 ) != 0 )
 */

#define LBER_ERROR			0xffffffffL
#define LBER_DEFAULT		0xffffffffL

/* general BER types we know about */

#define LBER_BOOLEAN		0x01L
#define LBER_INTEGER		0x02L
#define LBER_BITSTRING		0x03L
#define LBER_OCTETSTRING	0x04L
#define LBER_NULL			0x05L
#define LBER_OBJECTID 		0x06L
#define LBER_ENUMERATED		0x0aL
#define LBER_IA5STRING 		0x16L
#define LBER_SEQUENCE		0x30L	/* constructed */
#define LBER_SET			0x31L	/* constructed */

typedef struct berelement	BerElement;

#define NULLBER	((BerElement *) 0)


typedef unsigned long ber_tag_t;
typedef unsigned long ber_len_t;
typedef int ber_int_t;
typedef unsigned int ber_uint_t;
typedef long ber_slen_t;

/*  Structure for returning a sequence of octet strings + length */

typedef struct berval 
	{
	ber_len_t		bv_len;
	char			*bv_val;
	} Berval;

typedef Berval BerValue;


/*
*   Function Prototypes
*/

/* for V3 API */
ber_tag_t LNPUBLIC ber_skip_tag( BerElement *ber, ber_len_t *len );
ber_tag_t LNPUBLIC ber_peek_tag( BerElement *ber, ber_len_t *len );
ber_tag_t LNPUBLIC ber_first_element( BerElement *ber, ber_len_t *len, char **last);
ber_tag_t LNPUBLIC ber_next_element( BerElement *ber, ber_len_t *len, char *last);
ber_tag_t LNVARARGS  ber_scanf( BerElement *ber, const char *fmt, ... );

void LNPUBLIC ber_bvfree( struct berval *bv );
void LNPUBLIC ber_bvecfree( struct berval **bv );
struct berval *	LNPUBLIC ber_bvdup( const struct berval *bv );
int	LNVARARGS ber_printf( BerElement *ber, const char *fmt, ... );
void LNPUBLIC ber_free( BerElement *ber, int freebuf );
BerElement * LNPUBLIC ber_alloc_t( int options );
BerElement * LNPUBLIC ber_init( const struct berval *bv );
int LNPUBLIC ber_flatten(BerElement *ber, struct berval **bvPtr);
#ifdef __cplusplus
}
#endif
#endif /* _LBER_H */
