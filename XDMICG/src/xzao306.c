/*E*/
/* Fichier : $Id: xzao306.c,v 1.3 1995/03/10 17:00:12 niepceron Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/03/10 17:00:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzao306.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/03/95     : Creation
* C.T. 	10 Mar 1995  : Modif include fichier (V 1.2)
* C.T. 	10 Mar 1995  : Modif test asql_fetch (V 1.3)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao306.c,v 1.3 1995/03/10 17:00:12 niepceron Exp $ XZAO306_Liste_RADT";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne la liste des stations RADT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO306_Liste_RADT ( XDY_Basedd 	va_Base, 
			XDY_Nom 	va_NomTroncon,
			XDY_Mot		va_NumPortion,
			XDY_FonctionInt	va_FonctionUtilisateur,
			XDY_Entier	*va_NbStations)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Basedd		va_Base
* XDY_Nom		va_NomTroncon
* XDY_Mot		va_NumPortion
* XDY_FonctionInt	va_FonctionUtilisateur
* XDY_Entier		va_NbStations
*
* ARGUMENTS EN SORTIE :
*
* XDY_Entier		va_NbStations
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*  XDC_ARG_INV	: argument invalide
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* Appelle XZA0306SP
*
* MECANISMES :
*
------------------------------------------------------*/
{
	static char *version = "$Id: xzao306.c,v 1.3 1995/03/10 17:00:12 niepceron Exp $ XZAO306_Liste_RADT";
	
	CS_CHAR 	*vl_RpcName =XZAOC_XZAO306_RPC_NAME;
	CS_CONNECTION   *vl_Connection=XZAGV_Connexion;
	CS_COMMAND	*vl_Cmd;
	CS_RETCODE      vl_Retcode;
	CS_INT          vl_ResType;
	CS_SMALLINT     vl_MsgId;
	tg_row_result	vl_Tab_Ligne;
	XZAOT_Station	vl_Station;
	int		i,
			vl_Nb_Ligne_Lue,
			vl_Nb_Col,
			vl_Status;
	XDY_Entier	SPstatus;
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAO306_Liste_RADT : base=%s NomTroncon=%s NumPortion=%d\n",

			va_Base,
			va_NomTroncon,
			va_NumPortion);

	if (va_FonctionUtilisateur == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s : Fonction utilisateur est NULL");
		return XDC_NOK;
	}
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", 
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
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_Retcode = asql_ctparam_char_input(vl_Cmd,(CS_CHAR *)va_Base,
						"@va_Basedd_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: asql_ctparam_char_input(Base) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_char_input(vl_Cmd,(CS_CHAR *)va_NomTroncon,
						"@va_NomTroncon_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: asql_ctparam_char_input(NomTroncon) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_smallint_input(vl_Cmd,(CS_SMALLINT *)&va_NumPortion,
						"@va_NumPortion_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: asql_ctparam_smallint_input(NumPortion) echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", 
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
						vl_Retcode = asql_fetch_row_result(vl_Cmd , &vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao306: ROW RESULTS nb lignes:%d\n",vl_Nb_Ligne_Lue);
						*va_NbStations = 0;
						for (i = 0; i < vl_Nb_Ligne_Lue; i++)
						{
							*va_NbStations = i + 1;

							vl_Station.NumeroStation = *((XDY_Eqt *)vl_Tab_Ligne[i][0].pt_value);
							vl_Station.NumeroAutoroute = *((XDY_Octet *)vl_Tab_Ligne[i][1].pt_value);
							vl_Station.Sens = *((XDY_Sens *)vl_Tab_Ligne[i][2].pt_value);
							vl_Station.PR = *((XDY_PR *)vl_Tab_Ligne[i][3].pt_value);
							vl_Status = (*va_FonctionUtilisateur)(vl_Station);
							if (vl_Status != XDC_OK)
								XZST_03EcritureTrace(XZSTC_WARNING, 
									"xzao306 : fonction utilisateur retourne %d", vl_Status);
						}
						vl_Retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,
									vl_Nb_Col,&vl_Tab_Ligne);
						if (vl_Retcode != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_WARNING,
									"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
							asql_clean_cmd(vl_Connection, vl_Cmd);
							return XDC_NOK;
						}
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao306 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao306 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao306 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", SPstatus);
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
						   return (SPstatus);
						}
						break;
				}
				
				
				if (vl_Retcode != CS_SUCCEED && ((int)vl_ResType != CS_ROW_RESULT))
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: asql_fetch a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", vl_Retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", vl_Retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao306: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO306_Liste_RADT retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAO306_Liste_RADT retourne %d\n", SPstatus);
			
 	
 	asql_clean_cmd(vl_Connection, vl_Cmd);

	return(XDC_OK);
}

