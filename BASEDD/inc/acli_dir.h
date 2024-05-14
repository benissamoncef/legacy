/*E Fichier : $Id: acli_dir.h,v 1.21 2021/03/08 14:56:56 pc2dpdy Exp $      Release : $Revision: 1.21 $        Date : $Date: 2021/03/08 14:56:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE CLI * FICHIER CLI.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module directeur de la tache TACLI:
*  TACLI est une tache qui se met en reception de message
*  et effectue des actions sur la Base via le mecanisme 
*  de l Open Client.
*
------------------------------------------------------
* HISTORIQUE :
*
* MERCIER O.	04 Oct 1994	: Creation
* MERCIER O. 	24 Oct 1994	: Ajout de acli_acl.h   V 1.2
* GABORIT B. 	19 dec 1994	: modif constantes timer (1.4)
* GABORIT B. 	15 jan 1995	: ajout constantes timer (1.6)
* TORREGROSSA  	27 jan 1995	: Modif nom fichier d'include acli_XZEA_XZAS_FU.h
*                                 par acli_EA_AS_F.h (1.7)
* MERCIER	06 Fev 1995	: Ajout des constantes de noms de taches V 1.8
* TORREGROSSA  	10 Aou 1995	: Modif heures des purges histo et exploitation V 1.9
* GABORIT B.    13 sep 1995	: modif timer ihm (1.11)
* NIEPCERON P.  26 Aou 1996	: Ajout des cle des timer pour PC simplifie (1.12)
* NIEPCERON P.  26 Aou 1996	: Ajout des cle des timer pour PC simplifie (1.13)
* NIEPCERON P.  04 Fev 1997	: Ajout  pour TDP (1.14)
* NIEPCERON P.  05 Mai 1998     : ajout ACLIDC_TIMERCLE_XZAE101 (1.15)
* JMG		16/11/04	: inclusion de acli_xzac6*  1.16
* NIEPCERON P.  15/10/19	: Modif heure de purge et delta inter site 1.17
* NIEPCERON P.  15/10/19	: Ajout acli_xzao_cb.h DEM SAE-152 1.18
------------------------------------------------------ */

#ifndef CLI
#define CLI

/* fichiers inclus */

	/* include system */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>

	/* include system necessaire pour Open Server et Open Client. */
	#include        <ospublic.h>
	#include        <oserror.h>

	/* Include service migrazur */
	 #include "xdy.h"
	 #include "xdm.h"
	 #include "xzst.h"
	 #include "xzsm.h"
	 #include "xzsc.h"
	 #include "acli_xzah10.h"
	 #include "acli_xzae102.h"
	 #include "acli_xzan34_cb.h"
	 #include "acli_xzag05_cb.h"
	 #include "acli_xzag06_cb.h"
	 #include "acli_xzac66_cb.h"
	 #include "acli_xzac67_cb.h"
	 #include "acli_xzac62_cb.h"
	 #include "acli_xzac03_cb.h"
	 #include "acli_xzac_cb.h"
	 #include "acli_xzao_cb.h"
	 #include "acli_EA_AS_F.h"
	 
	/* Include des CallBacks RTwks de type Open Client*/ 
	 #include "acli_tcl.h"
	 #include "acli_acl.h"
	 #include "acli_hcl.h"
	 #include "acli_mcl.h"
	 #include "acli_scl.h"
	 #include "xzan34.h"
	 #include "xzag15.h"
	 #include "xzag25.h"
	 #include "xzea.h"
	 #include "xzao.h"
	 
/* definitions de constantes exportees */
/* nombre de secondes d'un jour */
	#define ACLIDC_OFFSET_24H	(24. * 3600.)	
	
	#define ACLIDC_TIMERDELAI_XZAH10 900.0
	#define ACLIDC_TIMERCLE_XZAH10 "Cle timer cyclique IHM"
	
	#define ACLIDC_TIMERCLE_XZAH10_BLOC "Cle timer cyclique IHM Bloc"
	#define ACLIDC_TIMERCLE_XZAE101 "Cle timer cyclique FMC locales"

	#define ACLIDC_TIMERDELAI_AHCL21 905.0
	#define ACLIDC_TIMERCLE_AHCL21 "Cle timer Radt Dai"

	#define ACLIDC_TIMERDELAI_AHCL27 910.0
	#define ACLIDC_TIMERCLE_AHCL27 "Cle timer Satir"

	#define ACLIDC_HEURE_DECLENCHEMENT_XZAN34 "00:30:00"
	#define ACLIDC_TIMERCLE_XZAN34 "Cle timer PURGE calendrier des astreintes"
	
	#define ACLIDC_HEURE_DECLENCHEMENT_XZAG05 "01:00:00" 
	#define ACLIDC_TIMERCLE_XZAG05 "Cle timer PURGE Base Exploitation"
	
	#define ACLIDC_HEURE_DECLENCHEMENT_XZAG06 "01:00:00" 
	#define ACLIDC_TIMERCLE_XZAG06 "Cle timer PURGE Base Historique"
	#define ACLIDC_DELTA_PURGE_INTER_SITE 	(60. * 60.)	/* 60 mn */
	
	#define ACLIDC_TIMERDELAI_TRV 3600.0
	#define ACLIDC_TIMERCLE_TRV "Cle timer cyclique travaux en cours"
	
	#define ACLIDC_TACLI	"tacli.x"
	#define ACLIDC_TAPUR	"tapur.x"
	#define ACLIDC_TACPC	"tacpc.x"

/* definitions de types exportes */

/* definition de macro exportees */
XDY_NomSite	vg_NomSite;

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void acli_sortir();
#endif
