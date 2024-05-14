/*E*/
/* Fichier : $Id: xzao491.c,v 1.4 2021/10/07 11:13:11 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2021/10/07 11:13:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao491.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	27/07/20	: Creation IMU 1.1 DEM-SAE155
* ABE	11/02/21	: Ajout SousType DEM-SAE155
* ABE	07/06/21	: Fix Autoroute & site gestion DEM-SAE155 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao491.h"

static char *version = "@(#)xzao491.c	1.1 1.1 27/07/2020 : xzao491.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO491SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO491_Lire_Liste_IMU (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt 	va_NomMachine_in,
				XDY_FonctionInt pa_FonctionUtilisateurIMU_in,
				XDY_Entier 	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in		
* XDY_NomEqt		va_NomMachine_in		
* XDY_FonctionInt	pa_FonctionUtilisateurIMU_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des IMU de la Machine 
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
* Pour chaque IMU appel de la fonction utilisateur avec XZAOT_IMU_491
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzao491.c	1.1 27/07/20 : XZAO491_Lire_Liste_IMU" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO491_RPC_NAME;
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
	int		i;
	XDY_Entier	SPstatus;
	XDY_Autoroute vl_Autoroute;
	
	/*A
	** Definition des variables locales
	*/
	
	XZAOT_IMU_491	vl_IMU;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO491_Lire_Liste_IMU : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao491: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao491: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao491: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao491: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}


	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao491: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao491: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao491 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao491 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao491 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao491: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao491: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao491: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao491: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao491: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurIMU pour
	** chaque type de Picto
	*/
	
	*va_Resultat_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;
	   
	   /*B
           ** Remplissage de la structure XZAOT_IMU_491 coorrespondant a la ligne resultat
           ** du select de la procedure stockee XZAO491
           */
				   
           vl_IMU.NumeroIMU 		= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
           strcpy(vl_IMU.NomIMU, (char *) vl_Tab_Ligne[i][1].pt_value);
           vl_Autoroute = * (XDY_Autoroute *) vl_Tab_Ligne[i][2].pt_value;
           vl_IMU.PR 			= * (XDY_PR *) vl_Tab_Ligne[i][3].pt_value;
           vl_IMU.Sens 			= * (XDY_Sens *) vl_Tab_Ligne[i][4].pt_value;
           strcpy(vl_IMU.AdresseEndPointOPCUA, (char *) vl_Tab_Ligne[i][5].pt_value);
           strcpy(vl_IMU.UserOPCUA, (char *) vl_Tab_Ligne[i][6].pt_value);
           strcpy(vl_IMU.PasswordOPCUA, (char *) vl_Tab_Ligne[i][7].pt_value);
           strcpy(vl_IMU.NomBICE, (char *) vl_Tab_Ligne[i][8].pt_value);
	   //strcpy(vl_IMU.NomServeur, (char *) vl_Tab_Ligne[i][9].pt_value);
	   vl_IMU.PortMaitre		= * (XDY_Entier *) vl_Tab_Ligne[i][9].pt_value;
	   vl_IMU.SousType		= * (XDY_Entier *) vl_Tab_Ligne[i][10].pt_value;
	   strcpy(vl_IMU.NomMachine,(char *) vl_Tab_Ligne[i][11].pt_value);
	   vl_IMU.SiteGestion 		= * (unsigned char *) vl_Tab_Ligne[i][12].pt_value;
	   sprintf(vl_IMU.Autoroute,"%d",vl_Autoroute);
	   XZST_03EcritureTrace(XZSTC_FONCTION,"IMU %d %s %s %d %d %s %s %s %s %d %d %s %d",
				vl_IMU.NumeroIMU,
				vl_IMU.NomIMU,
				vl_IMU.Autoroute,
				vl_IMU.PR,
				vl_IMU.Sens,
				vl_IMU.AdresseEndPointOPCUA,
				vl_IMU.UserOPCUA,
				vl_IMU.PasswordOPCUA,
				vl_IMU.NomBICE,
				vl_IMU.PortMaitre,
				vl_IMU.SousType,
				vl_IMU.NomMachine,
				vl_IMU.SiteGestion);
           
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurIMU
	   */
	  
	   if ( pa_FonctionUtilisateurIMU_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurIMU est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateurIMU_in) ( vl_IMU, *va_Resultat_out ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurIMU s'est mal deroule");
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
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO491_Lire_Liste_IMU retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




