/*E*/
/*Fichier : $Id: xzad07.c,v 1.2 2012/02/14 14:26:30 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/02/14 14:26:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAD * FICHIER xzad07.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer       27 Fev 1997     : Creation (DEM/1395)
* JMG		13/01/12	: tdp etendu DEM/1014 1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzad07.h"

static char *version = "$Id: xzad07.c,v 1.2 2012/02/14 14:26:30 pc2dpdy Exp $ : xzad07.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAD07SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD07_Liste_PMV_TDP_Actif ( XDY_FonctionInt pa_FonctionUtilPMVActif_in,
				 XDY_Entier 	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Fonction  pa_FonctionUtilPMVActif_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des PMV avec un TDP actif 
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
* Pour chaque PMV appel de la fonction utilisateur avec XZADT_PMV_TDP
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzad07.c,v 1.2 2012/02/14 14:26:30 pc2dpdy Exp $ : XZAD07_Liste_PMV_TDP_Actif" ;
	
	CS_CHAR *rpc_name = XZADC_XZAD07_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i, vl_Index = 0;
	XDY_Eqt		vl_Dest = 0;
	XDY_Eqt		vl_DestAlt = 0;
	XDY_Entier	SPstatus;
	XDY_Entier	vl_NumAction = 0;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Etat_PMV	vl_PMV;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAD07_Liste_PMV_TDP_Actif" );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzad07: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD07_Liste_PMV_TDP_Actif retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzad07: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD07_Liste_PMV_TDP_Actif retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

		
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzad07: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD07_Liste_PMV_TDP_Actif retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzad07: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzad07 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzad07 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzad07 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzad07 retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzad07: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD07_Liste_PMV_TDP_Actif retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzad07: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzad07: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzad07: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD07_Liste_PMV_TDP_Actif retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzad07: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD07_Liste_PMV_TDP_Actif retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurPMV pour
	** chaque PMV
	*/
	
	*va_Resultat_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;
	   
	   /*B
           ** Remplissage de la structure XZADT_PMV_TDP correspondant a la ligne resultat
           ** du select de la procadure stockee XZAD;07
           */
           
           vl_NumAction 	= * (XDY_Entier *) vl_Tab_Ligne[i][0].pt_value;
           vl_PMV.Numero 	= * (XDY_Eqt *) vl_Tab_Ligne[i][1].pt_value;
           strcpy(vl_PMV.Texte1, (char *) vl_Tab_Ligne[i][2].pt_value);
           strcpy(vl_PMV.Texte2, (char *) vl_Tab_Ligne[i][3].pt_value);
           strcpy(vl_PMV.Texte3, (char *) vl_Tab_Ligne[i][4].pt_value);
           strcpy(vl_PMV.Alternat1, (char *) vl_Tab_Ligne[i][5].pt_value);
           strcpy(vl_PMV.Alternat2, (char *) vl_Tab_Ligne[i][6].pt_value);
           strcpy(vl_PMV.Alternat3, (char *) vl_Tab_Ligne[i][7].pt_value);
	   vl_PMV.Clign1	= * (XDY_Booleen *) vl_Tab_Ligne[i][10].pt_value;
	   vl_PMV.Clign2	= * (XDY_Booleen *) vl_Tab_Ligne[i][11].pt_value;
	   vl_PMV.Clign3	= * (XDY_Booleen *) vl_Tab_Ligne[i][12].pt_value;
           strcpy(vl_PMV.Texte4, (char *) vl_Tab_Ligne[i][15].pt_value);
           strcpy(vl_PMV.Alternat4, (char *) vl_Tab_Ligne[i][16].pt_value);
	   vl_PMV.Destination		= * (XDY_Mot *) vl_Tab_Ligne[i][17].pt_value;
	   vl_PMV.DestinationAlternat	= * (XDY_Mot *) vl_Tab_Ligne[i][18].pt_value;
	   vl_PMV.DestinationNoeud	= * (XDY_Mot *) vl_Tab_Ligne[i][20].pt_value;

           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurPMV
	   */
	  
	   if ( pa_FonctionUtilPMVActif_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilPMVActif_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilPMVActif_in) ( vl_PMV, vl_NumAction ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilPMVActif_in s'est mal deroule");
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
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD07_Liste_PMV_TDP_Actif retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




