/*E*/
/*Fichier :  $Id: ex_msta.h,v 1.15 2012/07/04 14:30:20 pc2dpdy Exp $      Release : $Revision: 1.15 $        Date : $Date: 2012/07/04 14:30:20 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_msta.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de status commun au 
*  Sous-Systeme EQUEXT.
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	28 Oct 1994	: Creation
* Mismer.D	version 1.2	09 Nov 1994	:
* Mismer.D	version 1.3	23 Nov 1994	:
* Nagiel.E	version 1.4	23 Nov 1994	:
* Mismer.D	version 1.5	23 Nov 1994	:
* Nagiel.E	version 1.6	28 Nov 1994	:
* Mismer.D	version 1.7	09 Dec 1994	:
* Fontaine.C	version 1.8	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.9	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D	version 1.10	25 Mar 1997	: Modif TDP suite (DEM/1396)
* Mismer.D      version 1.11    08 Jan 1998     : Modif new alerte DAI (DEM/APG17) fiche ANA74
*
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	23/02/11 : Migration architecture HP ia64 (DEM 975) : declaration type du des fonctions (ex_traitXXX)  1.14
* VR	19/12/11		: ajout PRV (DEM/1016) 1.15
------------------------------------------------------ */

#ifndef EX_MSTA
#define EX_MSTA

/* fichiers inclus */

#include <stdio.h>
#include <sys/types.h>

#include "xdm.h"
#include "xdy.h"
#include "xdf.h"
#include "xzst.h"
#include "xzss.h"
#include "xzsc.h"
#include "xzamc.h"
#include "ex_mdon.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int ex_decode_status  ( char *			pa_TrameStatus,
			XZEXT_TEXTE_STATUS *	pa_TabStatus,
			int			va_SizeTab,
            		XDY_Rgs			va_adrrgs,
			XDY_Horodate		va_horodate,
            		int 			va_typeeqt,
            		XDY_Mot	        	va_noeqt,
            		XDY_NomMachine		va_NomMachine ,
			XDY_District		va_SiteGestion); 

extern int ex_decode_status_comp  ( char *			pa_TrameStatus,
			     char *			pa_Status,
			     int *			va_NumAlarme );
			     
extern	void ex_liberation_memoire_status (void);

extern void ex_traitERI_PMV(XZEXT_TEXTE_STATUS	va_tabstatus,
                   XDY_Horodate		va_horodate,
                   int 			va_typeeqt,
                   XDY_Mot	        va_noeqt,
            	   XDY_NomMachine	va_NomMachine,
		   XDY_District		va_SiteGestion);
		   
extern void ex_traitERI_PRV(XZEXT_TEXTE_STATUS	va_tabstatus,
                   XDY_Horodate		va_horodate,
                   int 			va_typeeqt,
                   XDY_Mot	        va_noeqt,
            	   XDY_NomMachine	va_NomMachine,
		   XDY_District		va_SiteGestion);


extern void ex_traitERI_NAV(XZEXT_TEXTE_STATUS va_tabstatus,
            	   XDY_Horodate		va_horodate,
            	   int 			va_typeeqt,
            	   XDY_Mot	        va_noeqt,
            	   XDY_NomMachine	va_NomMachine,
		   XDY_District		va_SiteGestion);
            	   
extern void ex_traitBAT(XZEXT_TEXTE_STATUS	va_tabstatus,
                   XDY_Horodate		va_horodate,
                   int 			va_typeeqt,
                   XDY_Mot	        va_noeqt,
            	   XDY_NomMachine	va_NomMachine,
		   XDY_District		va_SiteGestion);

extern void ex_traitBTR(XZEXT_TEXTE_STATUS	va_tabstatus,
                   XDY_Horodate		va_horodate,
                   int 			va_typeeqt,
                   XDY_Mot	        va_noeqt,
            	   XDY_NomMachine	va_NomMachine,
		   XDY_District		va_SiteGestion);

extern int ex_decode_alerte_dai  ( char *pa_Trame,
	           int 			*va_NumAlerte,
            	   int			*va_Type );

#endif
