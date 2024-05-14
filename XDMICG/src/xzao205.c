/*E*/
/*Fichier : $Id: xzao205.c,v 1.4 2020/11/03 17:57:11 pc2dpdy Exp $      Release : $Revision: 1.4 $      Date : $Date: 2020/11/03 17:57:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao205.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer       04 Nov 1996     : Creation (DEM/1232)
* Mismer       12 Dec 1996     : Modif nom du type ("_" pour " ") (DEM/1314) v 1.2
* JMG   13/11/07        : ajout site de gestion DEM715
* LCL	10/03/20	: Correction pour passage sous Linux64 (MOVIS DEM-SAE93 v1.4)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao205.c	1.2 12/13/96 : xzao205.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO205SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO205_Lire_Liste_Eqt_Generique_LCR (	XDY_Basedd	va_Basedd_in,
						XDY_NomEqt  	va_NomMachine_in,
						XDY_FonctionInt pa_FonctionUtilisateurEQT_LCR_in,
						XDY_Entier 	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
* XDY_Fonction  pa_FonctionUtilisateurEQT_LCR_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des PMV de la Machine 
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
* Pour chaque equipement appel de la fonction utilisateur avec XZAOT_EQT_LCR
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzao205.c	1.2 12/13/96 : XZAO205_Lire_Liste_Eqt_Generique_LCR" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO205_RPC_NAME;
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
	int		i, vl_Index = 0;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XZAOT_EQT_LCR	vl_Eqt;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO205_Lire_Liste_Eqt_Generique_LCR : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao205: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO205_Lire_Liste_Eqt_Generique_LCR retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao205: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO205_Lire_Liste_Eqt_Generique_LCR retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao205: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO205_Lire_Liste_Eqt_Generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao205: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO205_Lire_Liste_Eqt_Generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao205: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO205_Lire_Liste_Eqt_Generique_LCR retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao205: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao205 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao205 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao205 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao205: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO205_Lire_Liste_Eqt_Generique_LCR retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao205: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao205: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao205: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO205_Lire_Liste_Eqt_Generique_LCR retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao205: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO205_Lire_Liste_Eqt_Generique_LCR retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurPMV pour
	** chaque type de Picto
	*/
	
	*va_Resultat_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;
	   
	   /*B
           ** Remplissage de la structure XZAOT_EQT_LCR correspondant a la ligne resultat
           ** du select de la procadure stockee XZAO;205
           */
           
           vl_Eqt.Numero 		= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
           vl_Eqt.Type 			= * (XDY_TypeEqt *) vl_Tab_Ligne[i][1].pt_value;
           strncpy(vl_Eqt.LibType, (char *) vl_Tab_Ligne[i][2].pt_value,25);
           for ( vl_Index = strlen(vl_Eqt.LibType) - 1; vl_Index >= 0; vl_Index-- )
           {
              if ( vl_Eqt.LibType[vl_Index] != ' ' ) 
              {
                 vl_Eqt.LibType[vl_Index+1] = '\0';
                 break;
              }
           }
           for ( vl_Index = 0; vl_Eqt.LibType[vl_Index] != '\0'; vl_Index++ )
           {
              if ( vl_Eqt.LibType[vl_Index] == ' ' ) vl_Eqt.LibType[vl_Index] = '_';
           }
           strcpy(vl_Eqt.RGS, strtok((char *) vl_Tab_Ligne[i][3].pt_value, " "));
           vl_Eqt.LiaisonMaitre		= * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
           vl_Eqt.LiaisonEsclave	= * (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value;
           vl_Eqt.Code			= * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
           vl_Eqt.NumeroAutoroute	= * (XDY_Autoroute *) vl_Tab_Ligne[i][7].pt_value;
           strcpy(vl_Eqt.NomAutoroute, strtok((char *) vl_Tab_Ligne[i][8].pt_value, " "));
           vl_Eqt.PR			= * (XDY_PR *) vl_Tab_Ligne[i][9].pt_value;
           vl_Eqt.Sens			= * (XDY_Sens *) vl_Tab_Ligne[i][10].pt_value;
           strcpy(vl_Eqt.NomServeur, strtok((char *) vl_Tab_Ligne[i][11].pt_value, " "));
           strcpy(vl_Eqt.NomMachine, strtok((char *) vl_Tab_Ligne[i][12].pt_value, " "));
           vl_Eqt.SiteGestion       	= * (XDY_Octet *) vl_Tab_Ligne[i][13].pt_value;
           
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurPMV
	   */
	  
	   if ( pa_FonctionUtilisateurEQT_LCR_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurEQT_LCR_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateurEQT_LCR_in) ( vl_Eqt, *va_Resultat_out ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurEQT_LCR_in s'est mal deroule");
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
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO205_Lire_Liste_Eqt_Generique_LCR retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




