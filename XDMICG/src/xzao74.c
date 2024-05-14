/*E*/
/*Fichier : $Id: xzao74.c,v 1.6 2018/10/08 14:43:33 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2018/10/08 14:43:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao74.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       11 Oct 1994     : Creation
* volcic       14 Oct 1994     : Modification de XDY_FonctionInt et suppr typedef (v1.2)
* volcic       27 Dec 1994     : Modification de NumType (v1.3)
* volcic       27 Dec 1994     : Modification de NumType (v1.4)
* volcic       29 Nov 1995     : Modif allocation Tab_donees2 (v1.5)
* JMG		02/08/	18 	: ajout picto suppression SAGA DEM1306 1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao74.c,v 1.6 2018/10/08 14:43:33 pc2dpdy Exp $ : xzao74.c" ;
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO74SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO74_Lire_Types_PMV (	XDY_Basedd	va_Basedd_in,
				XDY_FonctionInt pa_FonctionUtilisateurTypPMV_in,
				XDY_Entier	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Retourne la liste des types de PMV
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* INTERFACE :
*
* Pour chaque type de PMV appel de la fonction utilisateur
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao74.c,v 1.6 2018/10/08 14:43:33 pc2dpdy Exp $ : XZAO74_Lire_Types_PMV" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO74_RPC_NAME;
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
	
	/*A
	** Definition des variables locales
	*/
	
	XZAOT_TypePMV	vl_TypePMV;
   	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO74_Lire_Types_PMV : Basedd = %s",
			va_Basedd_in );
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao74: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO74_Lire_Types_PMV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao74: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO74_Lire_Types_PMV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao74: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO74_Lire_Types_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao74: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO74_Lire_Types_PMV retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao74: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao74 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao117 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao117 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao74: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO74_Lire_Types_PMV retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao74: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO74_Lire_Types_PMV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao74: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO74_Lire_Types_PMV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	
	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurTypPMV pour
	** chaque type de PMV
	*/
	
	*va_Resultat_out = 0;
	
        for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           *va_Resultat_out ++;
           
           /*B
           ** Remplissage de la structure XZAOT_TypPMV coorrespondant a la ligne resultat
           ** du select de la procadure stockee XZAO;74
           */
           
           vl_TypePMV.NumType		= * (XDY_Octet *) vl_Tab_Ligne[i][0].pt_value;
           vl_TypePMV.NombreLignes 	= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
           vl_TypePMV.NombreCaracteres	= * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value;
           vl_TypePMV.Flash		= * (XDY_Booleen *) vl_Tab_Ligne[i][3].pt_value;
           vl_TypePMV.Temperature	= * (XDY_Booleen *) vl_Tab_Ligne[i][4].pt_value;
           vl_TypePMV.Picto	= * (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value;
           
           
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurTypPMV
	   */
	  
	   if ( pa_FonctionUtilisateurTypPMV_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurTypPMV est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateurTypPMV_in) ( vl_TypePMV , i) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurTypPMV s'est mal deroule");
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
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO74_Lire_Types_PMV retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




