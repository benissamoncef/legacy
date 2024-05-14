/*E*/
/*Fichier : $Id: xzae554.c,v 1.1 2008/11/17 11:02:48 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date : $Date: 2008/11/17 11:02:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae554.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture de la liste des dernieres FMC modifiees
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 836
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae554.h"

static char *version = "$Id: xzae554.c,v 1.1 2008/11/17 11:02:48 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2008/11/17 11:02:48 $: xzae554.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE554SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE554_Recherche_FMC_WEB ( XDY_FonctionInt pa_FonctionUtilisateur_in,
				      XDY_Entier 	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Fonction  pa_FonctionUtilisateur_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* retourne la liste des fmc a exporter vers strada
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Pour chaque zone de parcours appel de la fonction utilisateur avec XDY_Destination
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzae554.c,v 1.1 2008/11/17 11:02:48 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2008/11/17 11:02:48 $: XZAE554_Lect_FMC_A_Exporter" ;
	
	CS_CHAR *rpc_name = XZAE554C_XZAE554_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i = 0;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Octet	vl_type;
	XDY_Entier	vl_numero;
	XDY_Octet	vl_cle;
	XDY_Horodate 	vl_validation;
	XDY_Octet	vl_VR;
	XDY_Octet	vl_VM2;
	XDY_Octet	vl_VM1;
	XDY_Octet	vl_VL;
	XDY_Octet	vl_BAU;
	XDY_Octet	vl_VR_I;
	XDY_Octet	vl_VM2_I;
	XDY_Octet	vl_VM1_I;
	XDY_Octet	vl_VL_I;
	XDY_Octet	vl_BAU_I;
	XDY_Autoroute	vl_autoroute;
	XDY_PR	vl_PR;
	XDY_Sens	vl_sens;
	XDY_Octet	vl_type_pt_car;
	XDY_Octet	vl_confirme;
	XDY_Octet	vl_num_pt_car;
	int	vl_longueur;

	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae554: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE554_Lire_Liste_Destination_TDP retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae554: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE554_Lire_Liste_Destination_TDP retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        /*if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae554: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE554_Lire_Liste_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}*/
        
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae554: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE554_Lire_Liste_Destination_TDP retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
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
					case  CS_ROW_RESULT:
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae554: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzae554 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae554 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae554 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE554 retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae554: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE554_Lire_Liste_Destination_TDP retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae554: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae554: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae554: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE554_Lire_Liste_Destination_TDP retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae554: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE554 retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur pour chaque FMC 
	*/
	
	*va_Resultat_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;
	   
	   /*B
	   * recup des parametres de retour
           */
           
	   vl_numero			= * (XDY_Entier *) vl_Tab_Ligne[i][0].pt_value;
	   vl_cle			= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
	   vl_type			= * (XDY_Mot *) vl_Tab_Ligne[i][2].pt_value;
	   vl_validation		= * (XDY_Horodate *) vl_Tab_Ligne[i][3].pt_value;
	   vl_VR		= * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
	   vl_VM2		= * (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value;
	   vl_VM1		= * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
	   vl_VL		= * (XDY_Octet *) vl_Tab_Ligne[i][7].pt_value;
	   vl_BAU		= * (XDY_Octet *) vl_Tab_Ligne[i][8].pt_value;
	   vl_VR_I		= * (XDY_Octet *) vl_Tab_Ligne[i][9].pt_value;
	   vl_VM2_I		= * (XDY_Octet *) vl_Tab_Ligne[i][10].pt_value;
	   vl_VM1_I		= * (XDY_Octet *) vl_Tab_Ligne[i][11].pt_value;
	   vl_VL_I		= * (XDY_Octet *) vl_Tab_Ligne[i][12].pt_value;
	   vl_BAU_I		= * (XDY_Octet *) vl_Tab_Ligne[i][13].pt_value;
	   vl_confirme		= * (XDY_Octet *) vl_Tab_Ligne[i][14].pt_value;
	   vl_autoroute		= * (XDY_Autoroute *) vl_Tab_Ligne[i][15].pt_value;
	   vl_PR		= * (XDY_PR *) vl_Tab_Ligne[i][16].pt_value;
	   vl_sens		= * (XDY_Octet *) vl_Tab_Ligne[i][17].pt_value;
	   vl_type_pt_car	= * (XDY_Octet *) vl_Tab_Ligne[i][18].pt_value;
	   vl_num_pt_car	= * (XDY_Octet *) vl_Tab_Ligne[i][19].pt_value;
	   vl_longueur	= * (int *) vl_Tab_Ligne[i][20].pt_value;
           
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae554 : %d %d %d",
		vl_numero,
		vl_cle,
		vl_type);

           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateur
	   */
	  
	   if ( pa_FonctionUtilisateur_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"XZAE554 : pa_FonctionUtilisateur_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateur_in) ( vl_numero, 
								vl_cle,
								vl_type,
                                                        vl_validation,
                                                        vl_VR,
                                                        vl_VM2,
                                                        vl_VM1,
                                                        vl_VL,
                                                        vl_BAU,
                                                        vl_VR_I,
                                                        vl_VM2_I,
                                                        vl_VM1_I,
                                                        vl_VL_I,
                                                        vl_BAU_I,
                                                        vl_confirme,
                                                        vl_autoroute,
                                                        vl_PR,
                                                        vl_sens,
                                                        vl_type_pt_car,
                                                        vl_num_pt_car,
                                                        vl_longueur) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateur_in s'est mal deroule");
	         asql_clean_cmd(connection, cmd);
	         return (XDC_NOK);
	      }
	   }	
	   
	}
	
	
	
	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE554 retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




