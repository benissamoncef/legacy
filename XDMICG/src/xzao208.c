/*E*/
/*Fichier : $Id: xzao208.c,v 1.3 2018/03/09 12:55:20 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/03/09 12:55:20 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao208.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       11 Oct 1994     : Creation
* volcic       01 Nov 1994     : Modification entete (v1.2)
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : adresses des parametres de la procedure stockee  1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao208.c,v 1.3 2018/03/09 12:55:20 devgfi Exp $ : xzao208.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO208SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO208_Liste_Capteurs (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt	va_NomTube_in,
				XDY_Octet	va_TypeCapteur_in,
				XDY_FonctionInt va_FonctionUtilisateur_in )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_NomEqt	va_NomTube_in
* XDY_Octet	va_TypeCapteur_in
* XDY_Fonction  va_FonctionUtilisateur_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des Capteurs d'un tube 
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
* Pour chaque Capteur appel de la fonction utilisateur avec XZAOT_Capteur
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao208.c,v 1.3 2018/03/09 12:55:20 devgfi Exp $ : xzao208_Lire_Capteurs" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO208_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO208_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XZAOT_Capteur	vl_Capteur;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO208_Liste_Capteurs : Basedd = %s\tTube = %d\tFonction = %ld\n",
			va_Basedd_in,
			va_NomTube_in,
			va_FonctionUtilisateur_in );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao208: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao208: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao208: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomTube_in,"@va_NomTube_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao208: asql_ctparam_char_input(NomTube) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_TypeCapteur_in,"@va_TypeCapteur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao208: asql_ctparam_tinyint_input(TypeCapteur) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao208: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao208: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao208 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao208 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao208 : STATUS REUSULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao208: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao208: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao208: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurCapteurs pour
	** chaque type de Capteurs
	** Affectation des parametres de sortie
	*/
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           /*B
           ** Remplissage de la structure XZAOT_Capteur coorrespondant a la ligne resultat
           ** du select de la procadure stockee XZAO;208
           */
           
           vl_Capteur.NumCapteur	= * (XDY_Mot *) vl_Tab_Ligne[i][0].pt_value;
           strcpy ( vl_Capteur.NomCapteur, (char *) vl_Tab_Ligne[i][1].pt_value );
           vl_Capteur.Autoroute		= * (XDY_Sens *) vl_Tab_Ligne[i][2].pt_value;
           vl_Capteur.PR		= * (XDY_PR *) vl_Tab_Ligne[i][3].pt_value;
           vl_Capteur.Sens		= * (XDY_Sens *) vl_Tab_Ligne[i][4].pt_value;
           
           
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateur
	   */
	  
	   if ( va_FonctionUtilisateur_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzao208 : va_FonctionUtilisateur_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      free(Tab_donnees2);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (int) (*va_FonctionUtilisateur_in) ( vl_Capteur ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao208 : appel de va_FonctionUtilisateur_in s'est mal deroule");
	         asql_clean_cmd(connection, cmd);
	   	 free(Tab_donnees2);
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao208 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO208_Liste_Capteurs retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




