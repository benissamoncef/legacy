/* fichier : $RCSfile: itre_dir.h,v $   Release : $Revision: 1.23 $   Date : $Date: 2021/06/08 18:19:49 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
* FICHIER : itre_dir.h				*	
*************************************************
* DESCRIPTION :
*
*
*************************************************
* HISTORIQUE :
*
* J.M. Guilhou	29 Sep 1994	Creation V1.1
* Niepceron     05 Oct 1995	ajout de CM_FPARAM v1.12
* Niepceron     121 Jan 1996	ajout de xzss.h, xzsa.h ITR_AXPROF_NB ITR_AXPROF_CL 
				suppression de CM_FPARAM V1.14
* Guilhou	27 aou 1996	inclusion xzae103.h (RADT) V1.15
* Penot		07 Mai 1997	include xzic.h
* JPL		23/12/2011	Utilite, visibilite et nommage des constantes  1.17
* JMG		04/07/12 :	tidbg 1.18
* PFE		21/02/17 :	#define ITREC_RELAIS_LT_DBG     "titltdbg.x" tidbg 1.20
* JPL		12/02/20 :	Commande de lancement d'Applix avec numero d'ecran en parametre  1.21
* ABE 		26/01/20 : 	Ajout gestion fichier de config TOPS, pour IP:DISPLAY.PORT pour l'IHM DEM-SAE228 V1.22
* jmG		10/02/21 MAGISTRA 1.23
***********************************************/

#ifndef MTRE
#define MTRE

/***********************************************/
/* Acces aux services                          */
/***********************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <strings.h>

#include "xzst.h"
#include "itre_com.h"
#include "itre_ax.h"
#include "xdc.h"
#include "xdm.h"
#include "xdg.h"
#include "xzsm.h"
#include "xzsc.h"
#include "xzss.h"
#include "xzsa.h"
#include "xzae103.h"
#include "xzic.h"


#include "itre_rtw.h"

/***********************************************/
/* Constantes 				       */
/***********************************************/

#define	VAR_MONOECRAN	"MIGRAZUR_MONO"

#define	ITREC_RELAIS_BI		"titbi.x"
#define	ITREC_RELAIS_LT		"titlt.x"
#define	ITREC_RELAIS_LT_DBG	"titltdbg.x"
#define	ITREC_RELAIS_TITRE	"titre.x"


/* definition des points de communication de la tache */

#define	ITREC_NUMPORT_MCE	51230
#define	ITREC_NUMPORT_DIAL	53230


/* commande de lancement de l'applixation */

#define	NOMAPPLI	"migrazur"

#define	CMDAPPLIX	"applix -macro ITMA_TAR01_Emission -stay -display %s:%d.%d  -pass %d %d <&- >/dev/null 2>&1"

#define	CMDAPPLIJER	"itme.x -clearPersistedState -display %s:0.%d  -port %d %d <&- >/dev/null 2>&1 &"


/* Fichiers copie en axhome/ax_prof4 a l'init  */

#define ITR_AXPROF_NB				"/produits/Applix2/axlocal/ax_prof4_NB"		/* ax_prof4 postscript */
#define ITR_AXPROF_CL				"/produits/Applix2/axlocal/ax_prof4_COUL"       /* ax_prof4 PCL5 */

/* Fichier de configuration TOPS */
								
#define CO_FIC_TOPS_GEN_PATH "/produits/migrazur/appliSD/fichiers/adm/TOPS_"
#define CO_FIC_TOPS_DISPLAY1 "display1"
#define CO_FIC_TOPS_DISPLAY2 "display2"

/***********************************************/
/* Type 				       */
/***********************************************/

/***********************************************/
/* acces au variables globales 		       */
/***********************************************/
extern int vg_titre;

/***********************************************/
/* acces aux fonctions                         */
/***********************************************/

#endif
