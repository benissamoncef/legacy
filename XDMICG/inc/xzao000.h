/*E*/
/*Fichier : $Id: xzao000.h,v 1.1 1994/12/14 19:02:54 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/14 19:02:54 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao000.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	15 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao000
#define xzao000

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAO_TAB_SEPAR "\t"		/* Tabulation */

#define XZAO_COMMENT "#"		/* Commentaire */

#define XZAO_FIN_STRING '\0'		/* Fin d'une chaine de caractere */

#define XZAO_LINE_FEED '\n'		/* Fin de ligne */

#define XZAO_EOF 	-1		/* Fin de fichier */

#define XZAO_INT	1		/* Type entier */

#define XZAO_LONG_INT	5		/* Max d'un entier sybase */

#define XZAO_MAX_INT	32767		/* Max d'un entier sybase */

#define XZAO_LONG	2		/* Type long */

#define XZAO_LONG_LONG	10		/* Max d'un entier sybase */

#define XZAO_MAX_LONG	2147483647	/* Max d'un long sybase */

#define XZAO_UCHAR	3		/* Type unsigned char */

#define XZAO_LONG_UCHAR	3		/* Max d'un entier sybase */

#define XZAO_MAX_UCHAR	255		/* Max d'un unsigned char sybase */

#define XZAO_BIT	4		/* Type unsigned char */

#define XZAO_LONG_BIT	1		/* Max d'un entier sybase */

#define XZAO_MAX_BIT	1		/* Max d'un unsigned char sybase */


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

/* Lecture d'une ligne d'un fichier */

extern int XZAO000_LireLigneFich ( FILE * , char *, XDY_Entier, XDY_Entier * );

extern int XZAO000_Verif_Coherence_String ( char *, XDY_Entier, char * ); 
				        
extern int XZAO000_Verif_Coherence_Nombre ( char *, XDY_Entier, char * ); 
				        

#endif
