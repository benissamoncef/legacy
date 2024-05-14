/*E*/
/*Fichier : $Id: xzah10.c,v 1.11 2021/07/01 15:22:57 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2021/07/01 15:22:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAH10 * FICHIER xzah10.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* xzah10  appartient A BASEDD et au module TACLI.
* permet de rafraichir les synoptiques de l IHM.
*	Positionne XZAH10G_Init a XDC_FAUX.
*	Appel XZAH01 XZAH08 XZAH07
*
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	27 Oct 1994	: Creation
* C.T.		20 Fev 1995	: Ajout de la construction
*                                 des objets (V 1.3)
* T.milleville  17 Jul 1995 	: Ajout fonction asql_clean_cmd
*	dans le module d'interface XZAH10 V1.4
* niepceron  	27 Aou 1996	: Modif de XZAH10 pour les pc simplifie V1.5
* niepceron  	23 Dec 1996	: On ne fait pas l'xzah30 si trait par bloc v1.6
* Hilmarcher    16/03/05	: ajout appel a XZAH02 v1.7
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.8
* JMG	15/02/11	 : envoi du message de fin d init aux synoptiques flash 1.9
* LCL	01/07/21	 : Fix liberation memoire en fin de fonction, ajout de asql_Free_Tab_Ligne_Results 1.10
* LCL	01/07/21	 : Unfix asql_Free_Tab_Ligne_Results, provoque le plantage de tacli 1.11
------------------------------------------------------*/

/* fichiers inclus */

#include "xzah10.h"

#include "xzah02.h"
#include "xzah03.h"
#include "xzah04.h"
#include "xzah05.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah10.c,v 1.11 2021/07/01 15:22:57 pc2dpdy Exp $ : xzah10";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*	Positionne XZAH10G_Init a XDC_FAUX.
*	Appel XZAH01 XZAH08 XZAH07
*
*	XZAH08 : Envoie a l IHM les modifications des variables d animation synoptiques
*
*  Appelle XZAH;08 : Lecture et ecriture des synoptiques pour animation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAH10_Animer_IHM(XDY_Nom va_NomSite)

/*
*
* PARAMETRES EN ENTREE : Nom du Site
* PARAMETRES EN SORTIE : aucun
* VALEUR RENDUE :
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*	Rtserver en etat de marche, SQL Serveur en etat de marche avec les SP connus.
* INTERFACE :
*
* MECANISMES : Lit la variable globale XZAH10G_Init significative du type
* de rafraichissement a effectuer a l IHM :
* Si XZAH10G_Init (au moment de l appel) == XDC_VRAI on effectue un rafraichissement
* de type INITIALISATION de l IHM sinon on ne rafraichie que les deltas.
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzah10.c,v 1.11 2021/07/01 15:22:57 pc2dpdy Exp $ : XZAH10_Animer_IHM";
	int vl_retcode = XDC_OK;
	int vl_retcode_general = XDC_OK;

	XDY_Booleen vl_Init = XZAH10G_Init; /* Initialise avec la globale XZAH10G_Init */

	CS_CHAR *rpc_name = XZAH10C_XZAH30_RPC_NAME;

	CS_CONNECTION *pl_connection = XZAGV_Connexion;
	CS_COMMAND *pl_cmd;
	CS_INT res_type;
	CS_SMALLINT vl_msg_id;
	int vl_num_Tab_donnees2 = XZAH10C_XZAH30_NB_PARAM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2;
	int vl_Nb_Ligne_Lue;
	int vl_Nb_Col;
	tg_row_result vl_Tab_Ligne;
	int i;
	XDY_Entier vl_SPstatus;

	XDY_Booleen vl_Init_in = XZAH10G_Init;
	/*A
	** Definition des variables locales
	*/

	XDY_Evt vl_Evt;
	XDY_Octet vl_TypeRaf;
	XDY_Mot vl_Idportion;
	XDY_Sens vl_Sens;
	XDY_Entier vl_CoefTranslation;
	XDY_Entier vl_CoefExtension;
	XDY_Octet vl_niveau;
	XDY_Datagroup vl_datagroup;

	XZST_03EcritureTrace(XZSTC_INTERFACE, "IN : XZAH10_Animer_IHM : Debut d execution ");

	/*A Nettoyage des synoptiques si necessaire */

	if (vl_Init == XDC_VRAI)
		XZIS16_Detruire_Mobiles();

	/*A Si on ne fait pas un traitement par bloc (ie on n'est pas tacpc) on maj les table SYN_xxx
			sinon on laisse sce boulot � tacli.x */
	if (vl_Init != XZAH_TRAIT_BLOC)
	{
		/*A
		**  Construction des objets a animer
		*/

		/*A
		** Initialisation de la commande
		*/

		if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "xzah01: asql_cmd_alloc() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version, vl_retcode);
			asql_clean_cmd(pl_connection, pl_cmd);
			return (XDC_NOK);
		}

		/*A
		** Envoie de la commande RPC pour notre stored procedure.
		*/

		if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "xzah01: ct_command() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version, vl_retcode);
			asql_clean_cmd(pl_connection, pl_cmd);
			return (XDC_NOK);
		}

		/*A
		** Envoie de la commande au Sql server
		*/
		if (ct_send(pl_cmd) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "xzah01: ct_send() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version, vl_retcode);
			asql_clean_cmd(pl_connection, pl_cmd);
			return (XDC_NOK);
		}

		/*A
		** Traitement du resultat
		*/
		while ((vl_retcode = ct_results(pl_cmd, &res_type)) == CS_SUCCEED)
		{
			switch ((int)res_type)
			{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)res_type)
				{
				case CS_ROW_RESULT:
					vl_retcode = asql_fetch_row_result(pl_cmd, &vl_Nb_Ligne_Lue, &vl_Nb_Col, &vl_Tab_Ligne);
					XZST_03EcritureTrace(XZSTC_INFO, "xzah30: ROW RESULTS");
					break;

				case CS_PARAM_RESULT:
					vl_retcode = asql_fetch_param_result(pl_cmd, vl_num_Tab_donnees2, pl_Tab_donnees2);
					XZST_03EcritureTrace(XZSTC_INFO, "xzah30 : PARAMETER RESULTS\n");
					break;

				case CS_STATUS_RESULT:
					vl_retcode = asql_fetch(pl_cmd, &vl_SPstatus);
					XZST_03EcritureTrace(XZSTC_INFO, "xzah30 : CS_STATUS_RESULT = %ld", vl_retcode);
					XZST_03EcritureTrace(XZSTC_INFO, "xzah30 : LE RESULTAT de STATUS DE LA PROCEDURE STOCKEE = %ld", vl_SPstatus);

					if (vl_SPstatus != XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAH30_Modif_Synoptique retourne 1");
						asql_clean_cmd(pl_connection, pl_cmd);
						return (XDC_NOK);
					}
					break;
				}
				break;
			case CS_MSG_RESULT:
				/*
				**
				*/
				vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE, (CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
				if (vl_retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "xzah30: ct_res_info(msgtype) a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version, vl_retcode);
					asql_clean_cmd(pl_connection, pl_cmd);
					return (XDC_NOK);
				}
				break;

			case CS_CMD_SUCCEED:
				/*
				** Pas de colonnes retournees.
				*/
				break;

			case CS_CMD_DONE:

				/*
				** Commande effectuee : positionnement des parametres de retour.
				*/
				break;

			case CS_CMD_FAIL:
				/*
				** Le serveur SQL a detecte une ereeur en executant notre commande.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING, "xzah30: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version, vl_retcode);
				break;

			default:
				/*
				** Il s est passe quelque chose d inattendu.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING, "xzah30: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version, vl_retcode);
				asql_clean_cmd(pl_connection, pl_cmd);
				return (XDC_NOK);
			}
		}

		/* Modification T.M pour resoudre probleme de fuite */
		asql_clean_cmd(pl_connection, pl_cmd);
		/****************************************************/

		/*A
		** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
		** si il y a lieu
		*/

		/*        vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

				if (vl_retcode != XDC_OK)
				{
						XZST_03EcritureTrace( XZSTC_WARNING, "xzah10 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire" );
						asql_clean_cmd( pl_connection, pl_cmd );
						return ( XDC_NOK );
				}
		*/
	} /* fin if ( vl_Init != XZAH_TRAIT_BLOC ) */

	/*A
	**  Positionne a XDC_FAUX la variable globale XZAH10G_Init.
	**  Si XZAH10G_Init reste a XDC_FAUX la BaseDD rafraichit que les delta sur les synoptiques.
	**  sinon c est un rafraichissement de type initialisation.
	**  Pour que l animation synoptique soit de type initialisation
	**  il faut que XZAH11 soit appelee avant XZAH10 ou que XZAH10 soit
	**  a l initialisation de migrazur.
	*/

	/* Si on ne fait pas de traitement alors reinit de XZAH10G_Init = XDC_FAUX
	   sinon envoi du msg de debut des envois */
	if (vl_Init != XZAH_TRAIT_BLOC)
		XZAH10G_Init = XDC_FAUX;
	else
	{
		if (!TipcSrvMsgWrite(XDG_IRAF_DEBUT, XDM_IdentMsg(XDM_IRAF_DEBUT), XDM_FLG_SRVMSGWRITE, NULL))
		{
			/* Affichage Trace */
			XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10_Anim:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)", XDG_IRAF_DEBUT, XDM_IdentMsg(XDM_IRAF_DEBUT));
			vl_retcode = XDC_NOK;
			vl_retcode_general = vl_retcode_general + vl_retcode;
		}
	}

	/*A
	** Declenchement des fonctions d animation de l IHM
	*/
	/*B
	**	Appel de XZAH01_Envoyer_Bouchon_Chaussee_Portion.
	*/
	if ((vl_retcode = XZAH01_Envoyer_Bouchon_Chaussee_Portion(vl_Init, va_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10 : XZAH01 a echoue");
		vl_retcode_general = vl_retcode_general + vl_retcode;
	}
	/*B
	**	Appel de XZAH03_Envoyer_Voies_Neutralisees_Segment.
	*/
	if ((vl_retcode = XZAH03_Envoyer_Voies_Neutralisees_Segment(vl_Init, va_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10 : XZAH03 a echoue");
		vl_retcode_general = vl_retcode_general + vl_retcode;
	}

	/*B
	**	Appel de XZAH02_Envoyer_Bouchon_Chaussee_Segment.
	*/
	if ((vl_retcode = XZAH02_Envoyer_Bouchon_Chaussee_Segment(vl_Init, va_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10 : XZAH02 a echoue");
		vl_retcode_general = vl_retcode_general + vl_retcode;
	}

	/*B
	**	Appel de XZAH04_Envoyer_Voie_Inversee .
	*/
	if ((vl_retcode = XZAH04_Envoyer_Voie_Inversee(vl_Init, va_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10 : XZAH04 a echoue");
		vl_retcode_general = vl_retcode_general + vl_retcode;
	}
	/*B
	**	Appel de XZAH05_Envoyer_Basculement_Bouchon .
	*/
	if ((vl_retcode = XZAH05_Envoyer_Basculement_Bouchon(vl_Init, va_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10 : XZAH05 a echoue");
		vl_retcode_general = vl_retcode_general + vl_retcode;
	}

	/*B
	**	Appel de XZAH08_Envoyer_Objet_Mobile_Aff_Voies.
	*/
	if ((vl_retcode = XZAH08_Envoyer_Objet_Mobile_Aff_Voies(vl_Init, va_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10 : XZAH08 a echoue");
		vl_retcode_general = vl_retcode_general + vl_retcode;
	}
	/*B
	**	Appel de XZAH07_Envoyer_Objet_Mobile_Portion.
	*/
	if ((vl_retcode = XZAH07_Envoyer_Objet_Mobile_Portion(vl_Init, va_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10 : XZAH07 a echoue");
		vl_retcode_general = vl_retcode_general + vl_retcode;
	}

	if (vl_Init == XZAH_TRAIT_BLOC)
	{
		if (!TipcSrvMsgWrite(XDG_IRAF_FIN, XDM_IdentMsg(XDM_IRAF_FIN), XDM_FLG_SRVMSGWRITE, NULL))
		{
			/* Affichage Trace */
			XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10_Anim:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)", XDG_IRAF_FIN, XDM_IdentMsg(XDM_IRAF_FIN));
			vl_retcode = XDC_NOK;
			vl_retcode_general = vl_retcode_general + vl_retcode;
		}
	}

	/*envoi du message de fin d init aux synoptiques flash*/
	XDG_EncodeDG2(vl_datagroup, XDG_IRAF_FIN, "LX");
	if (!TipcSrvMsgWrite(vl_datagroup, XDM_IdentMsg(XDM_IRAF_FIN), XDM_FLG_SRVMSGWRITE, NULL))
	{
		/* Affichage Trace */
		XZST_03EcritureTrace(XZSTC_WARNING, "XZAH10_Anim:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)", vl_datagroup, XDM_IdentMsg(XDM_IRAF_FIN));
	}

	XZST_03EcritureTrace(XZSTC_INTERFACE, "IN : XZAH10_Animer_IHM : Fin d execution et retourne %d ", vl_retcode_general);
	return (vl_retcode_general);
}
