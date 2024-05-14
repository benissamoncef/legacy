/*E*/
/* Fichier : $Id: xzao790.c,v 1.2 2017/03/31 17:14:03 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2017/03/31 17:14:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao790.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* liste des zones
********************** 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	30/0317 creation 1.1 DEM1191
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao730.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzao790.c	1.5 1.5 12/09/10 : xzao790" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO790_Liste_Eqt_SAGA( XDY_FonctionInt pa_FonctionUtilisateur_in,
					 XDY_Basedd        va_Basedd_in,
					 XDY_Entier      *va_Resultat_out					 
					)


/*
* ARGUMENTS EN ENTREE :CFG
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION 
* XZAO19 appelle XZAO;19
* AOSP19 select dans les tables EQUIPEMENT et PORTION
* XZAO19 retourne les parametres
*
------------------------------------------------------*/
{

	CS_CHAR 	*pl_rpc_name 		= XZAOC_XZAO790_RPC_NAME;
	CS_CONTEXT      *pl_context;
	CS_CONNECTION   *pl_connection		= XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode		= XDC_NOK;
	CS_INT          vl_res_type		= 0;
	CS_SMALLINT     vl_msg_id		= 0;
	int 		vl_num_param_retour 	= XZAOC_XZAO790_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_param_retour;
	
	int             vl_Nb_Ligne_Lue		= 0;
        int             vl_Nb_Col		= 0;
	int	i,vl_cr;
        tg_row_result   vl_Tab_Ligne;
	
	XDY_Entier      vl_SPstatus;
	CS_DATETIME	vl_Horodate;
	XZAOT_ConfEqtSAGA	vl_eqt;
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"XZAO790_Liste_Eqt_SAGA: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO790_Liste_Eqt_SAGA retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAO790_Liste_Eqt_SAGA: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO790_Liste_Eqt_SAGA retourne %d", XDC_NOK);
		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	
	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO790_Liste_Eqt_SAGA: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO790_Liste_Eqt_SAGA retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAO790_Liste_Eqt_SAGA: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO790_Liste_Eqt_SAGA retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO790_Liste_Eqt_SAGA: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_param_retour, pl_Tab_param_retour);
						
						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO790_Liste_Eqt_SAGA : PARAMETER RESULTS\n");
						
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO790_Liste_Eqt_SAGA : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO790_Liste_Eqt_SAGA : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO790_Liste_Eqt_SAGA retourne 1");
                                                   	asql_clean_cmd (pl_connection,pl_cmd);
                                                   	free(pl_Tab_param_retour);
							return (XDC_NOK);
                                                }

						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                	XZST_03EcritureTrace(XZSTC_FATAL,"XZAO790_Liste_Eqt_SAGA: asql_fetch_* a echoue");
                                	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO790_Liste_Eqt_SAGA retourne %d", XDC_NOK);
                                        asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return (XDC_NOK);
                                }

				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAO790_Liste_Eqt_SAGA: ct_res_info(CS_MSGTYPE) a echoue");
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAO790_Liste_Eqt_SAGA: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO790_Liste_Eqt_SAGA retourne %d", XDC_NOK);
						asql_clean_cmd (pl_connection,pl_cmd);
						free(pl_Tab_param_retour);
						return (XDC_NOK);
					}
					XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO790_Liste_Eqt_SAGA: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO790_Liste_Eqt_SAGA : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO790_Liste_Eqt_SAGA : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAO790_Liste_Eqt_SAGA: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO790_Liste_Eqt_SAGA retourne %d", XDC_NOK);
					free(pl_Tab_param_retour);
					return(XDC_NOK);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAO790_Liste_Eqt_SAGA: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO790_Liste_Eqt_SAGA retourne %d", XDC_NOK);
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return (XDC_NOK);
				}
		
	}
	
	*va_Resultat_out = 0;

	XZST_03EcritureTrace(XZSTC_INFO,"xzao790 : nb de eqt saga = %d",vl_Nb_Ligne_Lue);

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
        {
           /*B
           ** Appel de la fonction utilisateur FonctionUtilisateur
           */

           if ( pa_FonctionUtilisateur_in == NULL)
           {
              XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateur_in est nulle");
              asql_clean_cmd(pl_connection, pl_cmd);
              return (XDC_NOK);
           }

	else
           {
	      vl_eqt.Numero = * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
	      strcpy(vl_eqt.NomEqtSAGA, (char *) vl_Tab_Ligne[i][1].pt_value);
	      vl_eqt.TypeSAGA = * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value;
	      strcpy(vl_eqt.Instance, (char *) vl_Tab_Ligne[i][3].pt_value);
	      strcpy(vl_eqt.FelsCode, (char *) vl_Tab_Ligne[i][4].pt_value);
	      strcpy(vl_eqt.EqtCode, (char *) vl_Tab_Ligne[i][5].pt_value);
	      strcpy(vl_eqt.CodeCmdMessage, (char *) vl_Tab_Ligne[i][6].pt_value);
	      strcpy(vl_eqt.CodeCmdLuminosite, (char *) vl_Tab_Ligne[i][7].pt_value);
	      strcpy(vl_eqt.CodeCmdPicto, (char *) vl_Tab_Ligne[i][8].pt_value);
	      strcpy(vl_eqt.CodeCmdHeure, (char *) vl_Tab_Ligne[i][9].pt_value);
	      strcpy(vl_eqt.CodeCmdBandeau, "cdu");
	      vl_eqt.NumeroSite = * (XDY_Octet *) vl_Tab_Ligne[i][10].pt_value;
	      strcpy(vl_eqt.Alarme, (char *) vl_Tab_Ligne[i][11].pt_value);
	      strcpy(vl_eqt.NomSite, (char *) vl_Tab_Ligne[i][12].pt_value);
	      vl_eqt.Sens = * (XDY_Octet *) vl_Tab_Ligne[i][13].pt_value;
	      XZST_03EcritureTrace(XZSTC_INFO,"xzao790 : [num %d/nom %s/type %d/instance %s/fels %s/eqt %s/msg %s/lumi %s/ picto %s/ heure %s / site %d(%s) / alarme %s/ sens %d]",
					vl_eqt.Numero,
					vl_eqt.NomEqtSAGA,
					vl_eqt.TypeSAGA,
					vl_eqt.Instance,
					vl_eqt.FelsCode,
					vl_eqt.EqtCode,
					vl_eqt.CodeCmdMessage,
					vl_eqt.CodeCmdLuminosite,
					vl_eqt.CodeCmdPicto,
					vl_eqt.CodeCmdHeure,
					vl_eqt.NumeroSite,
					vl_eqt.NomSite,
					vl_eqt.Alarme,
					vl_eqt.Sens);
                  if ( ( vl_retcode = (*pa_FonctionUtilisateur_in) ( vl_eqt) ) != XDC_OK )
              {
                 XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateur_in s'est mal deroule");
                 asql_clean_cmd(pl_connection, pl_cmd);
                 return (XDC_NOK);
              }

           }
	}
	if (vl_Nb_Ligne_Lue>0)
          asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

        /*A
        ** Retourne le resultat et les parametres de sorties renseignes
        */

        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO790_Rechercher_PAL_amont retourne  %d", vl_retcode);
        asql_clean_cmd(pl_connection, pl_cmd);
        return(XDC_OK);

}
