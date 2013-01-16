#ifdef __cplusplus
extern "C" {
#endif


/* Distinguished name handling package. */

#ifndef DNAME_DEFS
#define DNAME_DEFS


#define DN_DELIM '/'				/* Component delimiter between RDN's */
#define DN_DELIM_STR "/"
#define	DN_DELIM_RDN '+'			/* Component delimiter within an RDN */
#define	DN_DELIM_RDN_ABBREV '+'		/* Display component delimiter within an RDN (when abbreviating)*/
#define DN_DELIM_LDAP ','			/* LDAP delimiter within an RDN */
#define DN_TYPE_DELIM '='			/* Type name delimiter */
#define DN_TYPE_DELIM_STR "="
#define DN_OUNITS 4					/* Maximum number org units */
#define DN_MAX_COMMENTS 3			/* Maximum number of embedded comments in internet address */
#define DN_DCS 16					/* Maximum number of domain components */
#define DN_OUS_EXT 12				/* Maximum number of extended organizationalUnits */

/* Distinguished name parsing result data structure. Distinguished name is phrase part
   of Internet Sytle addresses for V5. */

typedef struct {
	DWORD Flags;					/* Parsing flags */
#define DN_NONSTANDARD      0x0001	/* Name includes non-standard components */
									/*  i.e., contains unrecognized Notes standard labels */
#define DN_NONDISTINGUISHED 0x0002	/* Non-distinguished name */
									/*  Ie., contains no delimiters or labeled attributes */
#define DN_CN_OU_RDN		0x0008	/* CN plus OU are relative distinguished name */
#define DN_O_C_RDN			0x0010	/* O plus C are relative distinguished name */
#define DN_NONABBREV		0x0020	/* Name includes components that cannot be abbreviated */
									/*  E.g., G, I, S, Q, P, A, UID, L, ST, STREET, DC */

	WORD CLength;					/* Country name length */
	char far *C;					/* Country name pointer */
	WORD OLength;					/* Organization name length */
	char far *O;					/* Organization name pointer */
	WORD OULength[DN_OUNITS];		/* Org Unit name lengths */
									/*  OULength[0] is rightmost org unit */
	char far *OU[DN_OUNITS];		/* Org unit name pointers */
									/*  OU[0] is rightmost org unit */
	WORD CNLength;					/* Common name length */
	char far *CN;					/* Common name pointer */
	WORD DomainLength;				/* Domain name length */
	char far *Domain;				/* Domain name pointer */

/*	Original V3 structure ended here.  The following fields were added in V4 */

	WORD PRMDLength;				/* Private management domain name length */
	char far *PRMD;					/* Private management domain name pointer */
	WORD ADMDLength;				/* Administration management domain name length */
	char far *ADMD;					/* Administration management domain name pointer */
	WORD GLength;					/* Given name length */
	char far *G;					/* Given name name pointer */
	WORD SLength;					/* Surname length */
	char far *S;					/* Surname pointer */
	WORD ILength;					/* Initials length */
	char far *I;					/* Initials pointer */
	WORD QLength;					/* Generational qualifier (e.g., Jr) length */
	char far *Q;					/* Generational qualifier (e.g., Jr) pointer */

/*	Original V4 structure ended here.  The following fields were added in V5 */

	WORD PhraseLength;				/* Internet Address Phrase Part length */
	char far *Phrase;				/* Internet Address Phrase Part pointer */
	WORD LPLength;					/* Internet Address Local Part length */
	char far *LP;					/* Internet Address Local Part pointer */
	WORD RLength;					/* Internet Address Route length */
	char far *R;					/* Internet Address Route pointer */
	WORD CMTLength[DN_MAX_COMMENTS];/* Internet Address Comment lengths */
	char far *CMT[DN_MAX_COMMENTS];	/* Internet Address Comment pointers */
	WORD Address821Length; 			/* Route address OR simple address portion of 822 style internet address length */
	char far *Address821;			/* Route address OR simple address portion of 822 style internet address pointer */
	WORD HierarchyOnlyLength;		/* Hierarchy only (all components after CN) length */
	char far *HierarchyOnly;		/* Hierarchy only (all components after CN) pointer */
	char far *UID;					/* LDAP/X.500 userid */
	WORD UIDLength;					/* LDAP/X.500 userid length */
	char far *L;					/* LDAP/X.500 localityName */
	WORD LLength;					/* LDAP/X.500 localityName length */
	WORD STLength;					/* LDAP/X.500 stateOrProvinceName length */
	char far *ST;					/* LDAP/X.500 stateOrProvinceName */
	WORD STREETLength;				/* LDAP/X.500 streetAddress length */
	char far *STREET;				/* LDAP/X.500 streetAddress */
	WORD DCLength[DN_DCS];			/* LDAP/X.500 domainComponent length */
	char far *DC[DN_DCS];			/* LDAP/X.500 domainComponent */
	WORD CN2Length;					/* LDAP/X.500 container type commonName length */
	char far *CN2;					/* LDAP/X.500 container type commonName */
	WORD OUExtLength[DN_OUS_EXT];	/* LDAP/X.500 organizationalUnit name lengths */
									/*  OUExtLength[0] is rightmost org unit, in addition to the OULength[DN_OUNITS] */
	char far *OUExt[DN_OUS_EXT];	/* LDAP/X.500 organizationalUnit name lengths */
									/*  OUExt[0] is rightmost org unit, in addition to the OU[DN_OUNITS] */
	} DN_COMPONENTS;


/* Functions */

#define DN_ABBREV_INCLUDEALL 0x00000001 /* Include all component types, even when same as template */

STATUS LNPUBLIC DNAbbreviate(DWORD Flags, const char far *TemplateName, const char far *InName,
								char far *OutName, WORD OutSize, WORD far *OutLength);
STATUS LNPUBLIC DNCanonicalize(DWORD Flags, const char far *TemplateName, const char far *InName,
								char far *OutName, WORD OutSize, WORD far *OutLength);

STATUS LNPUBLIC DNParse(DWORD Flags, const char far *TemplateName, 
							const char far *InName, DN_COMPONENTS far *Comp, WORD CompSize);

#endif

#ifdef __cplusplus
}
#endif

