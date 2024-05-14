/*E Fichier : $Id: etmi_dir.h,v 1.3 1995/05/22 13:15:26 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/22 13:15:26 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TETMi * FICHIER etmi_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition des variables et fonctions du fichier directeur
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Fontaine C.   Version 1.2     03 Mai 1995     : Reprise entete
* D.Mismer	Version 1.3	19 Mai 1995	: Modif gestion deconnexion ST
------------------------------------------------------ */

#ifndef etmi_dir
#define etmi_dir

/* fichiers inclus */

#include "etmi_common.h"
#include "ex_msok.h"
#include "etmi_pro.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XZEXT_MASK		*vg_SockList 	= (XZEXT_MASK*)NULL;	/* liste globale des sockets */
XZEXT_ST_Config		vg_Config  ;	/* struct config tache */	
XZEXT_ST_Sockets	vg_DesSocks  ;	/* struct. descs. sockets typiques tache TELxi */
XZEXT_Liste_Cmdes_LCR	*pg_ListeCmdesLCR = (XZEXT_Liste_Cmdes_LCR *)NULL; /* liste cmdes LCR */
XDY_Entier       	vg_sock_TETEL = -1 ;			/* socket acces TETEL */
int			vg_EtatST     = XDC_OK;

float	vg_val_timer = 32. ;

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
