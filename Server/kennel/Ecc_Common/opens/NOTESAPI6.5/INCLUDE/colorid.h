#ifndef COLORID_DEFS
#define COLORID_DEFS

#ifdef __cplusplus
extern "C" {
#endif

/*	Maximum number of colors that can be handled by Notes. */

#define MAX_NOTES_COLORS			240

/* 	Number of colors for V3 form background compatablilty */

#define V3_FORMCOLORS	21		

/*	Standard colors -- so useful they're available by name. */

#define MAX_NOTES_SOLIDCOLORS		16

#define NOTES_COLOR_BLACK		0
#define NOTES_COLOR_WHITE 		1
#define NOTES_COLOR_RED			2
#define NOTES_COLOR_GREEN		3
#define NOTES_COLOR_BLUE		4
#define NOTES_COLOR_MAGENTA		5
#define NOTES_COLOR_YELLOW		6
#define NOTES_COLOR_CYAN		7
#define NOTES_COLOR_DKRED		8
#define NOTES_COLOR_DKGREEN		9
#define NOTES_COLOR_DKBLUE		10
#define NOTES_COLOR_DKMAGENTA	11
#define NOTES_COLOR_DKYELLOW	12
#define NOTES_COLOR_DKCYAN		13
#define NOTES_COLOR_GRAY		14
#define NOTES_COLOR_LTGRAY		15

/*	The following FONT_COLOR_XXX are for compatibility with earlier 
	revs of the SDK.  New code should use NOTES_COLOR_XXX */ 

#define	FONT_COLOR_BLACK			NOTES_COLOR_BLACK
#define	FONT_COLOR_WHITE			NOTES_COLOR_WHITE
#define FONT_COLOR_RED				NOTES_COLOR_RED
#define FONT_COLOR_GREEN			NOTES_COLOR_GREEN
#define FONT_COLOR_BLUE				NOTES_COLOR_BLUE
#define	FONT_COLOR_CYAN				NOTES_COLOR_CYAN
#define	FONT_COLOR_YELLOW			NOTES_COLOR_YELLOW
#define	FONT_COLOR_MAGENTA			NOTES_COLOR_MAGENTA

/*	Color mapping routines */

WORD LNPUBLIC MapV3ColorToExtColor (WORD color);
WORD LNPUBLIC MapExtColorToV3Color (WORD color);

#ifdef __cplusplus
}
#endif

#endif /* COLORID_DEFS */

