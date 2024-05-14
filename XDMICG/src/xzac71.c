/*E*/
/*  Fichier : 	$Id: xzac71.c,v 1.1 1999/02/26 11:33:11 gaborit Exp $  Release : $Revision: 1.1 $ Date : $Date: 1999/02/26 11:33:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac71.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron       09 Dec 1998     : Creation
* GGY	28/04/23 : Augmentation taille vl_bandeau a (XDC_LGR_TXT_PICTO) et vl_picto (XDC_LGR_NOM_PICTO) (DEM-473)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac71.h"


static char *version = "$Id: xzac71.c,v 1.1 1999/02/26 11:33:11 gaborit Exp $ : xzac71" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC71SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC71_Recherche_derniers_PMV (	XDY_FonctionInt	pa_FonctionUtil_in )

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recapitule l'affichage actuellement pose en gare pour un evenement.
* 
* Sequence d'appel
* CL	XZAC71_Recherche_derniers_PMV
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet	va_NumPMV_out
* char(45)	va_Message_out
* char(45)	va_Alternat_out
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Les arguments en sortie sont retournes dans une
* liste
* 
* Fonction
* select dans les tables ACTION et AFFICHAGE_GARE 
* where evt, heure_succes<Horodate<heure_fin
* 
------------------------------------------------*/


{

	static char *version = "$Id: xzac71.c,v 1.1 1999/02/26 11:33:11 gaborit Exp $ : XZAC71_Recherche_derniers_PMV" ;

	CS_CHAR *rpc_name = XZACC_XZAC71_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	XDY_Entier 	vl_NumAct,vl_Fmc;
	XDY_Octet 	vl_Site,vl_Cle,vl_Autoroute;
	XDY_PR		vl_PR;
	XDY_Sens	vl_Sens;
	XZSMT_Horodate	vl_succes,vl_fin;
	char		vl_ligne1[16];
	char		vl_ligne2[16];
	char		vl_ligne3[16];
	char		vl_ligne4[16];
	char		vl_picto[XDC_LGR_NOM_PICTO];
	char 		vl_bandeau[XDC_LGR_TXT_PICTO];
	XDY_Horodate 	vl_Horodate_succes,vl_Horodate_fin;
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAC71_Recherche_derniers_PMV : "  );
			
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac71: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC71_Recherche_derniers_PMV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac71: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC71_Recherche_derniers_PMV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac71: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC71_Recherche_derniers_PMV retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzac71: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzac71 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac71 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac71 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC71_Recherche_derniers_PMV retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzac71 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC71_Recherche_derniers_PMV retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzac71: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC71_Recherche_derniers_PMV retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzac71 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac71 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac71 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac71: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC71_Recherche_derniers_PMV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac71: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC71_Recherche_derniers_PMV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
        
           /*B
           ** Affectation de la variable vl_TypeAstreinte
           ** Appel de la fonction utilisateur FonctionUtili
           */ 
           
           vl_NumAct = *(XDY_Entier *) vl_Tab_Ligne[i][0].pt_value;
           vl_Site = *(XDY_Octet *)  vl_Tab_Ligne[i][1].pt_value;
	if ((retcode = asql_date_Sybase2Ux(*(CS_DATETIME *)vl_Tab_Ligne[i][2].pt_value,&vl_Horodate_succes))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC70_Recherche_derniers_TFM retourne %d", retcode);
	}

	if ((retcode = asql_date_Sybase2Ux(*(CS_DATETIME *)vl_Tab_Ligne[i][3].pt_value,&vl_Horodate_fin))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ZAC70_Recherche_derniers_TFMr retourne %d", retcode);
	}
           vl_Autoroute = *(XDY_Octet *)  vl_Tab_Ligne[i][4].pt_value;
           vl_PR = *(XDY_PR *)  vl_Tab_Ligne[i][5].pt_value;
           vl_Sens = *(XDY_Sens *)  vl_Tab_Ligne[i][6].pt_value;
           strcpy ( vl_picto, (char *) vl_Tab_Ligne[i][7].pt_value );
           strcpy ( vl_bandeau, (char *) vl_Tab_Ligne[i][8].pt_value );
           strcpy ( vl_ligne1, (char *) vl_Tab_Ligne[i][9].pt_value );
           strcpy ( vl_ligne2, (char *) vl_Tab_Ligne[i][10].pt_value );
           strcpy ( vl_ligne3, (char *) vl_Tab_Ligne[i][11].pt_value );
           strcpy ( vl_ligne4, (char *) vl_Tab_Ligne[i][12].pt_value );
           vl_Fmc = *(XDY_Entier *)  vl_Tab_Ligne[i][13].pt_value;
           vl_Cle = *(XDY_Octet *)  vl_Tab_Ligne[i][14].pt_value;
           
        
           if ( pa_FonctionUtil_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzac71 : pa_FonctionUtil est nulle");
	      asql_clean_cmd(connection, cmd);
	      return ( XDC_NOK );
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtil_in) ( vl_NumAct,vl_Site,vl_Horodate_succes,vl_Horodate_fin,
	      						vl_Autoroute,
	      						vl_PR,
	      						vl_Sens,
	      						vl_picto,
	      						vl_bandeau,
	      						vl_ligne1,
	      						vl_ligne2,
	      						vl_ligne3,
	      						vl_ligne4,
	      						vl_Fmc,vl_Cle) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzac71 : appel de pa_FonctionUtil s'est mal deroule");
	         asql_clean_cmd(connection, cmd);
	         return ( XDC_NOK );
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac71 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC71_Recherche_derniers_PMV retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




