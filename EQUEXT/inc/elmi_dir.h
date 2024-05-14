/*E Fichier : $Id: elmi_dir.h,v 1.5 1997/04/07 20:10:42 mismer Exp $      Release : $Revision: 1.5 $        Date : $Date: 1997/04/07 20:10:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER elmi_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TELMi.
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	20 Oct 1994	: Creation
* Mismer.D	version 1.2	27 Fev 1995	: Ajout variable simulation
* Mismer.D	version 1.3	19 Mai 1995	: Modif gestion deconnexion ST
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete
* Mismer.D      version 1.5     25 Mar 1997     : Ajout date pour TDP (DEM/1396)
*
------------------------------------------------------ */

#ifndef elmi_dir
#define elmi_dir

/* fichiers inclus */

#include "elmi_mdon.h"
#include "elmi_cfg.h"
#include "elmi_pro.h"
#include "ex_msok.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XZEXT_MASK	*vg_SockList 	= (XZEXT_MASK*)NULL;	/* liste globale des sockets */
XZEXT_ST_Config		vg_Config;	/* struct config tache */	
XZEXT_ST_Sockets	vg_DesSocks;	/* struct. descs. sockets typiques tache TELxi */
XZEXT_Liste_Cmdes_LCR	*pg_ListeCmdesLCR = (XZEXT_Liste_Cmdes_LCR *)NULL; /* liste cmdes LCR */
char			vg_idpassw[18];
int			vg_EtatST	  = XDC_OK;

/* declaration de fonctions externes */

         
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Lignes_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_LireLigneFichi( 	int    /* va_Fd */,
				char * /* pa_LigneFich */,
				int    /* va_NbCar */);

/*
* ARGUMENTS EN ENTREE :
*   va_Fd		: Descripteur fichier lecture
*   va_NbCar		: Nombre de cars. lus
*
* ARGUMENTS EN SORTIE :
*   pa_LignFich		: Contenu de la ligne lue
* 
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/

#endif
