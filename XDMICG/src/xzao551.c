/*E*/
/* Fichier : $Id: xzao551.c,v 1.1 2012/07/04 10:27:41 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:27:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao551.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	21/03/12	: creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao551.h"

static char *version = "@(#)xzao551.c	1.2 1.2 09/28/05 : xzao551.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO551SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


int XZAO551_Commandes_Scenarios (XDY_Basedd	va_Basedd_in,
				XDY_Entier  	va_NumeroScenario_in,
				XDY_FonctionInt pa_FonctionUtilisateur_in,
				XDY_Entier 	*va_Resultat_out )


/*
*
* PARAMETRES EN ENTREE :
*
*	XDY_Entier  	va_NumZone_in
*	XDY_Entier		va_NumScen_in
*	XDY_FonctionInt pa_FonctionUtilisateurPMV_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des commandes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR 	*pl_rpc_name 		= XZAOC_XZAO551_RPC_NAME;
	CS_CONTEXT      *pl_context;
	CS_CONNECTION   *pl_connection		= XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;

	CS_RETCODE      vl_retcode		= XDC_NOK;
	CS_INT          vl_res_type		= 0;
	CS_SMALLINT     vl_msg_id		= 0;
	int 		vl_num_param_retour 	= XZAOC_XZAO551_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_param_retour;
	
	int             vl_Nb_Ligne_Lue		= 0;
        int             vl_Nb_Col		= 0;
	int	i,vl_cr;
        tg_row_result   vl_Tab_Ligne;
	
	XDY_Entier      vl_SPstatus;
	CS_DATETIME	vl_Horodate;

	XZAOT_Commandes_Scenarios 	vl_cmdsce;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO551_Commandes_Scenarios : Basedd = %s\va_NumeroScenario_in = %d",
			va_Basedd_in,
			va_NumeroScenario_in);



	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/

	pl_Tab_param_retour = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_param_retour);

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"XZAO551_Commandes_Scenarios: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO551_Commandes_Scenarios retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAO551_Commandes_Scenarios: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO551_Commandes_Scenarios retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

    if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)

	{
 	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAO551_Commandes_Scenarios: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO551_Commandes_Scenarios retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}


	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_NumeroScenario_in,"@va_NumeroScenario_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO551_Commandes_Scenarios: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO551_Commandes_Scenarios retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAO551_Commandes_Scenarios: asql_ctparam_int_output() a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO551_Commandes_Scenarios retourne %d", XDC_NOK);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAO551_Commandes_Scenarios: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO551_Commandes_Scenarios retourne %d", XDC_NOK);
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
						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO551_Commandes_Scenarios: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:

						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO551_Commandes_Scenarios : PARAMETER RESULTS vl_retcode = %ld", vl_retcode);
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_param_retour, pl_Tab_param_retour);
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO551_Commandes_Scenarios : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO551_Commandes_Scenarios : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO551_Commandes_Scenarios retourne 1");
                                                   	asql_clean_cmd (pl_connection,pl_cmd);
                                                   	free(pl_Tab_param_retour);
							return (XDC_NOK);
                                                }

						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                	XZST_03EcritureTrace(XZSTC_FATAL,"XZAO551_Commandes_Scenarios: asql_fetch_* a echoue");
                                	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO551_Commandes_Scenarios retourne %d", XDC_NOK);
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
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAO551_Commandes_Scenarios: ct_res_info(CS_MSGTYPE) a echoue");
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAO551_Commandes_Scenarios: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO551_Commandes_Scenarios retourne %d", XDC_NOK);
						asql_clean_cmd (pl_connection,pl_cmd);
						free(pl_Tab_param_retour);
						return (XDC_NOK);
					}
					XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO551_Commandes_Scenarios: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO551_Commandes_Scenarios : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAO551_Commandes_Scenarios : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAO551_Commandes_Scenarios: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO551_Commandes_Scenarios retourne %d", XDC_NOK);
					free(pl_Tab_param_retour);
					return(XDC_NOK);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAO551_Commandes_Scenarios: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO551_Commandes_Scenarios retourne %d", XDC_NOK);
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return (XDC_NOK);
				}

	}

	XZST_03EcritureTrace(XZSTC_INFO,"XZAO551_Commandes_Scenarios : nb de regles = %d",vl_Nb_Ligne_Lue);

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

		vl_cmdsce.prv 		    = * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
		vl_cmdsce.scenario		= * (XDY_Eqt *) vl_Tab_Ligne[i][1].pt_value;
		vl_cmdsce.commande 	    = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
		vl_cmdsce.prv_critique	= * (XDY_Booleen *) vl_Tab_Ligne[i][3].pt_value;
		vl_cmdsce.autoroute	= * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
		vl_cmdsce.PR	= * (XDY_Entier *) vl_Tab_Ligne[i][5].pt_value;
		vl_cmdsce.sens	= * (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value;

	    XZST_03EcritureTrace(XZSTC_DEBUG1,"xzao551 : %d/%d/%d/%d",
	    		vl_cmdsce.prv,
	    		vl_cmdsce.scenario,
	    		vl_cmdsce.commande,
	    		vl_cmdsce.prv_critique);

              if ( ( vl_retcode = (*pa_FonctionUtilisateur_in) (
            		  	  	  	  	  vl_cmdsce,
					i+1 ) ) != XDC_OK )
              {
                 XZST_03EcritureTrace(XZSTC_WARNING,"XZAO551_Commandes_Scenarios appel de pa_FonctionUtilisateur_in s'est mal deroule");
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

        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO551_Commandes_Scenarios  %d", vl_retcode);
        asql_clean_cmd(pl_connection, pl_cmd);
        return(XDC_OK);

}
