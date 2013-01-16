#ifdef __cplusplus
extern "C" {
#endif


/*	Definitions for NIF structures stored in a view/design note */

#ifndef NIF_COLLATE_DEFS
#define NIF_COLLATE_DEFS


/*	Description of codes used in the "$Totals" view note item, used to
	specify whether/which subtotals are to be kept for each summary buffer
	item.  The $Totals item is a TYPE_TEXT_LIST item, where each member
	of the text list is a string which can be converted into one of the
	following codes by doing a "atoi" function on it.  The order of the
	members of the text list MUST MATCH the order of the entries in the
	summary buffer, even if it means putting a blank string for a column
	which doesn't have any totalling.
	All of these statistics are kept in each subindex header, and
	apply to all entries of that subindex AND ALL SUBINDEXES BELOW IT. */

#define NIF_STAT_NONE			0		/* No subtotalling */
#define NIF_STAT_TOTAL			1		/* Total of values in subtree */
#define NIF_STAT_AVG_PER_CHILD	2		/* Total / # direct entries in parent's index (1 level only below parent) */
#define NIF_STAT_PCT_OVERALL	3		/* Total / total of values in entire index */
#define NIF_STAT_PCT_PARENT		4		/* Total / total of values in parent's index */
#define NIF_STAT_AVG_PER_ENTRY	5		/* Total / # descendants in parent's index (all levels below parent) */

#define NIF_STAT_AVERAGE		NIF_STAT_AVG_PER_CHILD	/* Obsolete symbol */


/*	Description of collating specification */

typedef struct
	{
	USHORT	BufferSize;				/* Size of entire buffer in bytes */
	USHORT	Items;					/* number of items following */
	BYTE	Flags;
#define	COLLATION_FLAG_UNIQUE	0x01	/* Flag to indicate unique keys. */
	BYTE	signature;				/* Must be COLLATION_SIGNATURE */
#define COLLATION_SIGNATURE 0x44

/*	COLLATE_DESCRIPTOR desc[];		** repeated COLLATE_DESCRIPTOR follows... */
/*	char text_area[];				** followed by variable length text */
	} COLLATION;


/*	This entry is repeated in a collating specification for each "key"
	which wants to be collated.  The key name is the name string of the item
	in the note summary information.

	The entries are placed in the list in the order in which the collating
	is to be done.  Thus, the first entry in the list has the highest
	priority, second has next highest priority, etc.  The collating
	routine only continues down the list of items as long as the higher
	priority items match exactly.

	The KEY,TUMBLER,CATEGORY collating types compares two entries using
	the value of the specified item as the collating value.  The others
	use data found within the NODE itself to collate two entries, rather
	than using item values.

	There are special meanings associated with the TUMBLER and CATEGORY
	types.  Like the KEY, they sort based on a summary item's value.
	However:

	1) TUMBLER is used with "list" item datatypes (non-list datatypes are
		treated exactly the same as a KEY).  Each value of the list
		corresponds to a level in a hierarchical (outline) index.  As
		collation is performed, only the "i"th list value is collated
		if we are collating the "i"th level of the hierarchical index.
		This causes the new index entry to be placed as many levels deep
		as there are list values.

		As an example, a number list value of "1:2:3" places the index
		entry 3 levels down in the hierarchy.  If the new index entry
		requires a subtree which does not yet exist, a "ghost" entry
		is created to act as a parent for the new entry at intermediate
		levels.  The result is an hierarchical outline where index entries
		are created at a variety of different levels in the index depending
		on the number of values in their list datatype.

	2) CATEGORY is used with any datatypes (list or non-list) to create
		a hierarchical index.  For each CATEGORY in the collating spec,
		"ghost" entries are created for each UNIQUE value of the specified
		item (only as many "ghost" entries as there are unique values),
		and all duplicate values are placed at the next lower level.

		Unlike TUMBLER, CATEGORY collates list datatypes exactly the
		same way as KEY, which is that each list value is compared, and
		only if equal, the next one is compared to break ties, and so on
		until the list is exhausted.

		As an example, if a collating spec consists of 1) CATEGORY "Folder",
		2) CATEGORY "Author", and 3) KEY "Date", the top level of the index
		will only contain as many "ghost" entries as there are unique
		Folder names, and below it, the next level will contain all unique
		Author names within that folder, and below each Author, the next
		level will contain all the actual index entries for each Author
		sorted by Date.  The result is a 3-level hierarchical outline
		where all index entries are always at the "n"th level, and all
		intermediate "category" levels always contain only "ghost" entries.
*/



#define COLLATE_TYPE_KEY 0			/* Collate by key in summary buffer */
									/* (requires key name string) */
#define COLLATE_TYPE_NOTEID 3		/* Collate by note ID */
#define COLLATE_TYPE_TUMBLER 6		/* Collate by "tumbler" summary key */
									/* (requires key name string) */
#define COLLATE_TYPE_CATEGORY 7		/* Collate by "category" summary key */
									/* (requires key name string) */
#define COLLATE_TYPE_MAX 7


typedef struct
	{
	BYTE	Flags;
#define	CDF_S_descending			0		/* True if descending */
#define	CDF_M_descending			0x01	/* False if ascending order (default) */
#define CDF_M_caseinsensitive		0x02	/* Obsolete - see new constant below */
#define CDF_M_accentinsensitive 	0x04	/* Obsolete - see new constant below */
#define CDF_M_permuted		 		0x08	/* If set, lists are permuted */
#define CDF_M_permuted_pairwise		0x10	/* Qualifier if lists are permuted; if set, lists
											   are pairwise permuted, otherwise lists are
											   multiply permuted. */
#define CDF_M_flat_in_v5			0x20	/* If set, treat as permuted */
#define CDF_M_casesensitive_in_v5	0x40	/* If set, text compares are case-sensitive */
#define CDF_M_accentsensitive_in_v5	0x80	/* If set, text compares are accent-sensitive */
	BYTE	signature;				/* Must be COLLATE_DESCRIPTOR_SIGNATURE */
#define COLLATE_DESCRIPTOR_SIGNATURE 0x66
	BYTE	keytype;				/* Type of key (COLLATE_TYPE_xxx) */
	WORD	NameOffset;				/* Offset to the name string */
									/* (relative to text area of buffer) */
	WORD	NameLength;				/* Length of the name string */
	} COLLATE_DESCRIPTOR;

#endif



#ifdef __cplusplus
}
#endif

