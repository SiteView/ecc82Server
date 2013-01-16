#ifdef __cplusplus
extern "C" {
#endif

#ifndef INTL_DEFS
#define INTL_DEFS

/* Country ISO codes (partial list) */

#define ISO_COUNTRY_ARG		32
#define ISO_COUNTRY_AUS 	36
#define ISO_COUNTRY_AUT		40
#define ISO_COUNTRY_BHR		48
#define ISO_COUNTRY_BEL		56
#define ISO_COUNTRY_BRA		76
#define ISO_COUNTRY_GBR		826
#define ISO_COUNTRY_BGR		100
#define ISO_COUNTRY_CAN		124
#define ISO_COUNTRY_CHN		156
#define ISO_COUNTRY_CYP		196
#define ISO_COUNTRY_CZE		203
#define ISO_COUNTRY_DNK		208
#define ISO_COUNTRY_EST		233
#define ISO_COUNTRY_FIN		246
#define ISO_COUNTRY_FRA		250
#define ISO_COUNTRY_DEU		276
#define ISO_COUNTRY_GRC		300
#define ISO_COUNTRY_HKG		344
#define ISO_COUNTRY_HUN		348
#define ISO_COUNTRY_IND		356
#define ISO_COUNTRY_IDN		360
#define ISO_COUNTRY_IRL		372
#define ISO_COUNTRY_ITL		380
#define ISO_COUNTRY_JPN		392
#define ISO_COUNTRY_KOR		410
#define ISO_COUNTRY_LVA		428
#define ISO_COUNTRY_LTU		440
#define ISO_COUNTRY_LUX		442
#define ISO_COUNTRY_MYS		458
#define ISO_COUNTRY_MLT		470
#define ISO_COUNTRY_MEX		484
#define ISO_COUNTRY_NLD		528
#define ISO_COUNTRY_NZL		554
#define ISO_COUNTRY_NOR		578
#define ISO_COUNTRY_OMN		512
#define ISO_COUNTRY_PAK		586
#define ISO_COUNTRY_POL		616
#define ISO_COUNTRY_PRT		620
#define ISO_COUNTRY_QAT		634
#define ISO_COUNTRY_ROM		642
#define ISO_COUNTRY_RUS		643
#define ISO_COUNTRY_SAU		682
#define ISO_COUNTRY_SGP		702
#define ISO_COUNTRY_SVK		703
#define ISO_COUNTRY_SVN		705
#define ISO_COUNTRY_ZAF		710
#define ISO_COUNTRY_ESP		724
#define ISO_COUNTRY_LKA		144
#define ISO_COUNTRY_SWE		752
#define ISO_COUNTRY_CHE		756
#define ISO_COUNTRY_TWN		158
#define ISO_COUNTRY_THA		764
#define ISO_COUNTRY_TUR		792
#define ISO_COUNTRY_ARE		784
#define ISO_COUNTRY_USA		840
#define ISO_COUNTRY_VEN		862

/*	International Environment Parameter Definitions */

#define	CURRENCY_SUFFIX 		0x0001
#define	CURRENCY_SPACE			0x0002
#define	NUMBER_LEADING_ZERO		0x0004
#define	CLOCK_24_HOUR			0x0008
#define	DAYLIGHT_SAVINGS		0x0010
#define	DATE_MDY				0x0020
#define	DATE_DMY				0x0040
#define	DATE_YMD				0x0080
#define DATE_4DIGIT_YEAR		0x0100
#define TIME_AMPM_PREFIX		0x0400 

#define	ISTRMAX 5
#define	YTSTRMAX 32

typedef struct {
	WORD Flags;							/* Flags (see above) */
	BYTE CurrencyDigits;
	BYTE Length;						/* Length of this structure */
										/* THIS MUST BE SET TO THE EXACT */
										/* SIZE OF THE STRUCTURE WHEN ITS */
										/* POINTER IS PASSED AS AN ARGUMENT */
	int	 TimeZone;
	char AMString[ISTRMAX];
	char PMString[ISTRMAX];
	char CurrencyString[ISTRMAX];
	char ThousandString[ISTRMAX];
	char DecimalString[ISTRMAX];
	char DateString[ISTRMAX];
	char TimeString[ISTRMAX];
	char YesterdayString[YTSTRMAX];
	char TodayString[YTSTRMAX];
	char TomorrowString[YTSTRMAX];
} INTLFORMAT;

void	LNPUBLIC OSGetIntlSettings(INTLFORMAT far *retIntlFormat,
									WORD BufferSize);
WORD LNPUBLIC OSGetExtIntlFormat (char item, char index, void *buff, WORD bufSize);
/* Item values to pass into OSGetExtIntlFormat(..)  */
#define EXT_AM_STRING		1	/* Request for AM String */
#define EXT_PM_STRING		2	/* Request for PM String */
#define EXT_CURRENCY_STRING	3	/* Request for Currency String */
#define MONTH_NAMES			4	/* Request for Month Names */
#define ABBR_MONTH_NAMES	5	/* Request for abbreviated month names */
#define WEEKDAY_NAMES		6	/* Request for weekday names */
#define ABBR_WEEKDAY_NAMES	7	/* Request for abbreviated weekday names */
#define CALENDARTYPE		8	/* Request for Calendar Type, see CALENDAR_XXX types below */
#define ERANAME				9	/* Request for Asian Native Calendar Name */
#define ABBRERANAME			10  /* Request for abbreviated Asian Native Calendar Name*/

/* CalendarType */
#define CALENDAR_NONE	0
#define CALENDAR_JAPAN	1
#define CALENDAR_TAIWAN	2
#define CALENDAR_THAI	3
#define CALENDAR_KOREA	4


/*	End of International Environment Parameter Definitions */

#endif

#ifdef __cplusplus
}
#endif

