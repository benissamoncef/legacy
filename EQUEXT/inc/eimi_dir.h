/*E Fichier : $Id: eimi_dir.h,v 1.2 2021/03/08 15:43:19 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2021/03/08 15:43:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER eimi_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TELMi.
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   01/03/18 :      Creation CTRL_FEUX 1.1 DEM1284
* JMG 	03/02/21 : config esclave 1.2
------------------------------------------------------ */

#ifndef eimi_dir
#define eimi_dir

/* fichiers inclus */

#include "eimi_mdon.h"
#include "eimi_cfg.h"
#include "eimi_pro.h"
#include "ex_msok.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XZEXT_MASK	*vg_SockList 	= (XZEXT_MASK*)NULL;	/* liste globale des sockets */
XZEXT_IP_Config		vg_Config;	/* struct config tache */	
XZEXT_IP_Config		vg_ConfigE;	/* struct config tache */	
XZEXT_ST_Sockets	vg_DesSocks;	/* struct. descs. sockets typiques tache TELxi */
XZEXT_Liste_Cmdes_LCR_IP	*pg_ListeCmdesLCR = (XZEXT_Liste_Cmdes_LCR_IP *)NULL; /* liste cmdes LCR */
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
