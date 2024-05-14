/*E*/
/*Fichier :  $Id: evid_cyc.c,v 1.10 2017/02/28 14:54:10 devgfi Exp $      Release : $Revision: 1.10 $        Date : $Date: 2017/02/28 14:54:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module gÅrant la fenetre LCR cyclique
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	23 Dec 1994	: Creation
* Mismer.D	version 1.2	17 Jan 1995	:
* Volcic.F	version 1.3	24 Jan 1995	:
* Fontaine.C	version 1.4	23 Mai 1995	:
* Mismer.D	version 1.5	18 Jan 1998	: Modif pour basculement PC2
* Cluchague.P   version 1.6     15 Fev 2000     : Correction de l'erreur dans la commande LCR
* Mismer.D	version 1.7	29 Mai 2000	: Modif pour basculement PC2 Suite et fin (DEM/65)
* JMG		version 1.8	02/10/09	: secto DEM887
* JPL		25/02/11 : Migration architecture HP ia64 (DEM 975) : retour de C.R. OK ou NOK (ey_traitement_cyclique)  1.9
* JPL		08/11/16 : Tester les connexions par une commande d'etat; tenter de retablir celles coupees (DEM 1189)  1.10
------------------------------------------------------*/

/* fichiers inclus */

#include "evid_com.h"

#include "evid_cyc.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: evid_cyc.c,v 1.10 2017/02/28 14:54:10 devgfi Exp $      : evid_cyc";

static char *vm_cmd_etat = "P";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Gestion des traitements cycliques de la tache.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ey_traitement_cyclique ( int va_NumSock )

/*
* ARGUMENTS EN ENTREE :
*va_NumSock : Socket provenant de telmi
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*
------------------------------------------------------*/
{
	static char *version = "$Id: evid_cyc.c,v 1.10 2017/02/28 14:54:10 devgfi Exp $      : ey_traitement_cyclique";

	static int		vl_periode = 0;

	int			vl_IndCnx = 0,
				vl_LgMsg  = 0;
	XZEXT_MSG_SOCKET	pl_Msg;
	XDY_Horodate		vl_Horodate;
	XZSMT_Horodate       vl_Horodatedecompo;
	int			vl_Delai;
	XZEXT_CLE_PROG		pl_CleProg;
	int			vl_noeqt;
	char			vl_Cas;
	int			vl_num_sysV;
	int			vl_encore = XDC_VRAI;

	/*A
	**  Lire le message recu dans la socket appelante
	*/
	if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, " Erreur a la lecture d'un message cyclique." );
		return (XDC_NOK);
	}


	/* Si le message est correct et si ce n'est pas une deconnexion */
	if ( ( vl_LgMsg != 0 ) && ( vl_IndCnx != XZEXC_DECONNEC ) )
	{
		/*A Si il s'agit d'un reveil cyclique */
		if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
		{
			sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
			             &vl_Horodate, &vl_Delai, pl_CleProg );

			/*! DEM1189 Video IP : les commandes au Systeme video ne sont plus envoyees selon le protocole LCR */
//			if (strstr(pl_CleProg,"VID_FLcr_NoEqt:")!=NULL)
//			{
//				sscanf(pl_CleProg, "VID_FLcr_NoEqt:%c%d", &vl_Cas, &vl_noeqt);
//				ex_relance_lcr (vl_noeqt,
//				                vl_Cas,
//				                &pg_DebutListeVIDFLcr,
//				                vl_Horodate,
//				                pl_CleProg,
//				                va_NumSock );
//				return (XDC_OK);
//			}


			/*A Traitement du reveil 1MN */
			if ( strstr ( pl_CleProg, EVIDC_TIMER_1MN ) != NULL )
			{
				XZST_03EcritureTrace ( XZSTC_DEBUG3, "%s : %d", pl_CleProg, vl_Delai );

				/*B Decompter la periode de reconnexion a un point d'acces au Systeme video */
				if (++vl_periode >= EVIDC_PERIODE_CNX) {
					vl_periode = 0;
				}

				/*A Pour chacun des points d'acces au Systeme video, */
				vl_num_sysV = EVIDC_SYSV1;
				do {
					/*A s'il est connecte alors envoyer une demande d'etat */
					if (estConnecte (vl_num_sysV)) {
						XZST_03EcritureTrace (XZSTC_DEBUG2, "Demande d'etat au Systeme video %d", vl_num_sysV);
						ev_sysv_commande_sysv (vl_num_sysV, EVIDC_DEMANDE_ETAT, vm_cmd_etat, strlen(vm_cmd_etat));
					} else if ( (estConfigure (vl_num_sysV))  &&  (0 == vl_periode) ) {
						/*A sinon tenter de reconnecter la tache a la periode de reconnexion */
						XZST_03EcritureTrace (XZSTC_DEBUG2, "Reconnexion au Systeme video %d", vl_num_sysV);
						ec_sysv_connecter (vl_num_sysV);

						if (estConnecte (vl_num_sysV)) {
							/*A et signaler un retablissement de la liaison */
							XZST_03EcritureTrace (XZSTC_INFO, "Liaison retablie avec Systeme video %d", vl_num_sysV);
							ec_message_operateurs ("Liaison retablie avec Systeme video %d", vl_num_sysV);
						}
					}

					if (vl_num_sysV == EVIDC_SYSV1) {
						vl_num_sysV = EVIDC_SYSV2;
					} else {
						vl_encore = XDC_FAUX;
					}
				} while (vl_encore);
			}
		}
	}

	return (XDC_OK);
}
