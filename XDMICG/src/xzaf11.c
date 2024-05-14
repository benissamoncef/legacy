/*E*/
/* Fichier : $Id: xzaf11.c,v 1.2 1995/01/17 12:00:12 torregrossa Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/17 12:00:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzaf11.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       16/01/95     : Creation
* C.T.       17/01/95     : Modif + test (V 1.2)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf11.h"

static char *version = "$Id: xzaf11.c,v 1.2 1995/01/17 12:00:12 torregrossa Exp $ XZAF11_Animer_Bouchons_Inverses";

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

int XZAF11_Animer_Bouchons_Inverses ( XDY_FonctionInt	va_FonctionUtilisateur)
				

/*
* ARGUMENTS EN ENTREE :
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
* Appelle XZAF11SP
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzaf11.c,v 1.2 1995/01/17 12:00:12 torregrossa Exp $ XZAF11_Animer_Bouchons_Inverses";
	
	CS_CHAR 	*vl_RpcName =XZAFC_XZAF11_RPC_NAME;
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
	XDY_Evt		vl_EvtBasc;
	XDY_Evt		vl_EvtBou;
	XDY_Entier	vl_Translation;
	XDY_Entier	vl_Extension;
	XDY_PR		vl_PR;
	XDY_Sens	vl_Sens;
	XDY_Voie	vl_Voie;
	XDY_Mot		vl_Portion;
	XDY_Octet	vl_Niveau;
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAF11_Animer_Bouchons_Inverses ");
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzaf11: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF11_Animer_Bouchons_Inverses retourne %d", 
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
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf11: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF11_Animer_Bouchons_Inverses retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}
        
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf11: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF11_Animer_Bouchons_Inverses retourne %d", 
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf11: ROW RESULTS nb lignes:%d", vl_Nb_Ligne_Lue);
						for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						{
							vl_EvtBasc.numero = *(XDY_Entier *)vl_Tab_Ligne[i][0].pt_value;
							vl_EvtBasc.cle = *(XDY_Octet *)vl_Tab_Ligne[i][1].pt_value;
							vl_EvtBou.numero = *(XDY_Entier *)vl_Tab_Ligne[i][2].pt_value;
							vl_EvtBou.cle = *(XDY_Octet *)vl_Tab_Ligne[i][3].pt_value;
							vl_Portion = *(XDY_Mot *)vl_Tab_Ligne[i][4].pt_value;
							vl_PR = *(XDY_PR *)vl_Tab_Ligne[i][5].pt_value;
							vl_Sens = *(XDY_Sens *)vl_Tab_Ligne[i][6].pt_value;
							vl_Voie = *(XDY_Voie *)vl_Tab_Ligne[i][7].pt_value;
							vl_Translation = *(XDY_Entier *)vl_Tab_Ligne[i][8].pt_value;
							vl_Extension = *(XDY_Entier *)vl_Tab_Ligne[i][9].pt_value;
							vl_Niveau = *(XDY_Octet *)vl_Tab_Ligne[i][10].pt_value;

							if (va_FonctionUtilisateur == NULL)
							{
								XZST_03EcritureTrace(XZSTC_WARNING,
										"%s : Fonction utilisateur retourne XDC_NOK", version);
								asql_clean_cmd(vl_Connection, vl_Cmd);
								return XDC_NOK;
							}
							vl_Status = (*va_FonctionUtilisateur)(vl_EvtBasc, vl_EvtBou,
									vl_Portion, vl_PR, vl_Sens,
									vl_Voie, vl_Translation, 
									vl_Extension, vl_Niveau);
							if (vl_Status != XDC_OK)
								XZST_03EcritureTrace(XZSTC_WARNING, "xzaf11 : fonction utilisateur retourne %d", vl_Status);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf11 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf11 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf11 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF11_Animer_Bouchons_Inverses retourne 1");
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				
				if (vl_Retcode != CS_SUCCEED && vl_ResType != CS_ROW_RESULT)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf11: asql_fetch_data* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF11_Animer_Bouchons_Inverses retourne %d", vl_Retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzaf11: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF11_Animer_Bouchons_Inverses retourne %d", vl_Retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf11: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF11_Animer_Bouchons_Inverses retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf11: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF11_Animer_Bouchons_Inverses retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAF11_Animer_Bouchons_Inverses \n");
			
 	
 	asql_clean_cmd(vl_Connection, vl_Cmd);

	return(XDC_OK);
}

