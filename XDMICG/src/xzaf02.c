/*E*/
/* Fichier : $Id: xzaf02.c,v 1.7 2010/12/09 00:29:12 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2010/12/09 00:29:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzaf02.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       13/01/95     : Creation
* C.T.       17/01/95     : Modif + test
* C.T.       24/01/95     : Ajout correspondance type 
*                           entre base et syn (V 1.3)
* C.T.       25/03/95	  : Ajout de la prise en compte des
*                           evenements sur echangeur (V 1.4)
* C.T.	     30/03/1995	  : Suppression de l'appel a la fonction
*                           de conversion de types d'objet par
*                           un tableau de correspondance (V 1.5)
* C.T.	     29/05/1995	  : Correction trace (V 1.6)
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf.h"
#include "xzah_utils.h"

static char *version = "$Id: xzaf02.c,v 1.7 2010/12/09 00:29:12 gesconf Exp $ XZAF02_Animer_Objet_Mobile";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Rechercher les evenements et les actions clos en cours a
*  l'heure specifiee pour mettre a jour les tables de formation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAF02_Animer_Objet_Mobile ( XDY_Horodate		va_Horodate,
				 XDY_FonctionInt	va_FonctionUtilPasEch,
				 XDY_FonctionInt	va_FonctionUtilSurEch)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate		va_Horodate
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* Appelle XZAF02SP
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzaf02.c,v 1.7 2010/12/09 00:29:12 gesconf Exp $ XZAF02_Animer_Objet_Mobile";
	
	CS_CHAR 	*vl_RpcName =XZAFC_XZAF02_RPC_NAME;
	CS_CONNECTION   *vl_Connection=XZAGV_Connexion;
	CS_COMMAND	*vl_Cmd;
	CS_RETCODE      vl_Retcode;
	CS_INT          vl_ResType;
	CS_SMALLINT     vl_MsgId;
	int		vl_Status;
	int		i;
	tg_row_result	vl_Tab_Ligne;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	XDY_Entier	SPstatus;
	XDY_Evt		vl_Evenement;
	XDY_Act		vl_Action;
	XDY_Entier	vl_TranslOuPosEch;
	XDY_Sens	vl_Sens;
	XDY_Voie	vl_Voie;
	XDY_Mot		vl_Portion;
	XDY_Octet	vl_TypeObjet;
	XDY_Octet	vl_TypeTrans;
	XDY_Nom		vl_Echangeur;
	
	
	/*A
	** Definition des variables intermediaires pour la conversion des dates au format Sybase
	*/
	CS_DATETIME	vl_Horodate_in;
	
	
        /*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_Retcode = asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf02: asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne %d", 
					vl_Retcode);
		return (XDC_NOK);
	}
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAF02_Animer_Objet_Mobile : vl_Horodate_in=%d va_Time_in=%d ",
			vl_Horodate_in.dtdays,vl_Horodate_in.dttime);
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzaf02: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_Retcode = ct_command(vl_Cmd, CS_RPC_CMD, vl_RpcName, 
				CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf02: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_Retcode = asql_ctparam_date_input(vl_Cmd,(CS_DATETIME *)&vl_Horodate_in,
						"@va_Horodate_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaf02: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}
        
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf02: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}
	
	
	
	/*A
	** Traitement du resultat
	*/

	while ((vl_Retcode = ct_results(vl_Cmd, &vl_ResType)) == CS_SUCCEED)
	{
		switch ((int)vl_ResType)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_ResType)
				{
					case  CS_ROW_RESULT:
						vl_Retcode = asql_fetch_row_result(vl_Cmd, &vl_Nb_Ligne_Lue, &vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf02: ROW RESULTS nb lignes:%d", vl_Nb_Ligne_Lue);
						for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						{
							vl_Evenement.numero = *(XDY_Entier *)vl_Tab_Ligne[i][0].pt_value;
							vl_Evenement.cle = *(XDY_Octet *)vl_Tab_Ligne[i][1].pt_value;
							vl_Portion = *(XDY_Mot *)vl_Tab_Ligne[i][2].pt_value;
							vl_Action.numero = *(XDY_Entier *)vl_Tab_Ligne[i][3].pt_value;
							vl_Action.cle = *(XDY_Octet *)vl_Tab_Ligne[i][4].pt_value;
							vl_TypeObjet = *(XDY_Octet *)vl_Tab_Ligne[i][5].pt_value;
							vl_Sens = *(XDY_Octet *)vl_Tab_Ligne[i][6].pt_value;
							vl_Voie = *(XDY_Voie *)vl_Tab_Ligne[i][7].pt_value;
							vl_TranslOuPosEch = *(XDY_Entier *)vl_Tab_Ligne[i][8].pt_value;
							strcpy(vl_Echangeur, vl_Tab_Ligne[i][9].pt_value);

							if (va_FonctionUtilPasEch == NULL || 
							    va_FonctionUtilSurEch == NULL)
							{
								XZST_03EcritureTrace(XZSTC_WARNING,
										"%s : Fonction utilisateur est NULL", version);
								asql_clean_cmd(vl_Connection, vl_Cmd);
								return XDC_NOK;
							}

							/*A
							 * Conversion du type Objet de BaseDD 
							 * vers les constantes de XDC et envoi
							 */
         						vl_TypeTrans = XZAHC_ConvTypesMob[vl_TypeObjet];
         						if( vl_TypeTrans == (XDY_Octet) XDC_SYN_OBJ_INCONNU)
         						{
         							XZST_03EcritureTrace(XZSTC_WARNING,"le tableau XZAHC_ConvTypesMob n a pas de correspondance pour le type obj =%d",vl_TypeObjet);
							}
							else if (vl_Sens == XDC_SENS_SORTIE_NORD ||
								vl_Sens == XDC_SENS_ENTREE_NORD ||
								vl_Sens == XDC_SENS_SORTIE_SUD ||
								vl_Sens == XDC_SENS_ENTREE_SUD)
							{
								vl_Status = (*va_FonctionUtilSurEch)(vl_TypeTrans,
									vl_Echangeur, vl_Sens, 
									vl_TranslOuPosEch, vl_Evenement,
									vl_Action);
								if (vl_Status != XDC_OK)
									XZST_03EcritureTrace(XZSTC_WARNING, 
										"xzaf02 : fonction utilisateur sur echangeur retourne %d", vl_Status);
							}
							else
							{
								vl_Status = (*va_FonctionUtilPasEch)(vl_TypeTrans,
									vl_Portion, vl_Sens, vl_Voie, 
									vl_TranslOuPosEch, vl_Evenement,
									vl_Action);
								if (vl_Status != XDC_OK)
									XZST_03EcritureTrace(XZSTC_WARNING, 
										"xzaf02 : fonction utilisateur pas sur echangeur retourne %d", vl_Status);
							}
						}
						vl_Retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,
									vl_Nb_Col,&vl_Tab_Ligne);
						if (vl_Retcode != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
							asql_clean_cmd(vl_Connection, vl_Cmd);
							return XDC_NOK;
						}
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf02 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf02 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf02 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne 1");
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				
				if (vl_Retcode != CS_SUCCEED && ((int)vl_ResType != CS_ROW_RESULT))
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf02: asql_fetch a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_Retcode = ct_res_info(vl_Cmd, CS_MSGTYPE,(CS_VOID *)&vl_MsgId, CS_UNUSED, NULL);
					if (vl_Retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzaf02: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne %d", vl_Retcode);
						asql_clean_cmd(vl_Connection, vl_Cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf02: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf02: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF02_Animer_Objet_Mobile retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAF02_Animer_Objet_Mobile \n");
			
 	
 	asql_clean_cmd(vl_Connection, vl_Cmd);

	return(XDC_OK);
}

