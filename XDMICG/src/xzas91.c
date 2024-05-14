/*E*/
/*Fichier : $Id: xzas91.c,v 1.2 2010/12/07 12:52:53 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/12/07 12:52:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAS91 * FICHIER xzas91.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module appartient a TASTS. Il permet de fournir les derniers donnees RADT
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron     05 jan 2007     : Creation V1.1 DEM605
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : retour de XDC_NOK sur erreur conversion date  1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas91.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas91.c,v 1.2 2010/12/07 12:52:53 gesconf Exp $ : xzas06" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAS06_Fonction_De_T permet de demander l envoie des mesures Traffic
*	6MINS  afin d animer les courbes des Synoptiques.
*	Algo : Constitue un msg RTwks et
* 	envoie le message RTWks XDM_AS_FdeT.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS91_Derniere_Donnees_RADT (XDY_FonctionInt   va_FonctionUtilisateur_in,
					XDY_Entier 	*va_Resultat_out	) 
/*
* ARGUMENTS EN ENTREE : 
* XDY_Fonction  va_FonctionUtilisateur_in
*
* ARGUMENTS EN SORTIE : aucun
* CODE RETOUR : 
*
* XDY_Entier	va_Resultat_out
*
*	
* VALEUR RENDUE :
*
*  Liste des dernieres donnees RADT 
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
* Pour chaque donnee RADT appel de la fonction utilisateur
*
* MECANISMES :
*
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzas91.c,v 1.2 2010/12/07 12:52:53 gesconf Exp $ : XZAS91_Dernieres_Donnees_RADT" ;
	
	CS_CHAR *rpc_name = XZASC_XZAS91_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZASC_XZAS91_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	XDY_Horodate	vl_Horodate;
	XDY_Horodate	vl_horodate;
	XDY_Eqt		vl_station;
	XDY_Sens	vl_sens;
	XDY_Mot		vl_Q;
	XDY_Octet	vl_validite_Q;
	XDY_Octet	vl_V;
	XDY_Octet	vl_validite_V;
	XDY_Octet	vl_TO;
	XDY_Octet	vl_validite_TO;
	XDY_Octet	vl_PL;
	XDY_Octet	vl_validite_PL;
	XDY_Octet	vl_validite;
	
	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAS91_Dernieres_Donnees_RADT : " );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao131: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS91_Dernieres_Donnees_RADT retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao131: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS91_Dernieres_Donnees_RADT retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
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
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao131: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS91_Dernieres_Donnees_RADT retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao131: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao147 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao131 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao131 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS91_Dernieres_Donnees_RADT retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao131: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS91_Dernieres_Donnees_RADT retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao131: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS91_Dernieres_Donnees_RADT retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao131: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS91_Dernieres_Donnees_RADT retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateur
	** Affectation des parametres de sortie
	*/
	
	va_Resultat_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           va_Resultat_out ++;
	   
	   /*B
           ** Remplissage des variables coorrespondant a la ligne resultat
           ** du select de la procadure stockee XZAO;131
           */
	/*
		*A conversion de la date sybase en unix
							 */
	  retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)vl_Tab_Ligne[i][0].pt_value, &vl_Horodate);
	  if (retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,
					"acl02 : asql_Sybase2Ux(va_Horodate_in,&vl_Horodate) a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, 
						"OUT : a XZAS91_Dernieres_Donnees_RADT retourne %d",retcode);
			return (XDC_NOK);
		}
	   vl_horodate = (XDY_Horodate)vl_Horodate;
	   vl_station  = * (XDY_Eqt *) vl_Tab_Ligne[i][1].pt_value;
	   vl_sens = * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value;
	   vl_Q = * (XDY_Mot *) vl_Tab_Ligne[i][3].pt_value;
	   vl_validite_Q = * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
	   vl_V = * (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value;
	   vl_validite_V = * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
	   vl_TO = * (XDY_Octet *) vl_Tab_Ligne[i][7].pt_value;
	   vl_validite_TO = * (XDY_Octet *) vl_Tab_Ligne[i][8].pt_value;
	   vl_PL = * (XDY_Octet *) vl_Tab_Ligne[i][9].pt_value;
	   vl_validite_PL = * (XDY_Octet *) vl_Tab_Ligne[i][10].pt_value;

           if ( ( vl_validite_Q != XDC_RADT_CORRECT) || ( vl_validite_V != XDC_RADT_CORRECT) || ( vl_validite_TO != XDC_RADT_CORRECT) || ( vl_validite_PL != XDC_RADT_CORRECT))
		vl_validite=XDC_FAUX;
	   else
		vl_validite=XDC_VRAI;
XZST_03EcritureTrace(XZSTC_WARNING,"vl_horodate %f, vl_station %d, vl_sens %d, vl_Q %d, vl_V %d, vl_TO %d, vl_PL %d, vl_validite %d",vl_horodate, vl_station, vl_sens, vl_Q, vl_V, vl_TO, vl_PL, vl_validite);
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateur
	   */
	  
	   if ( va_FonctionUtilisateur_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"va_FonctionUtilisateur_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      free(Tab_donnees2);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*va_FonctionUtilisateur_in) ( vl_horodate, vl_station, vl_sens, vl_Q, vl_V, vl_TO, vl_PL, vl_validite ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de va_FonctionUtilisateur_in s'est mal deroule");
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS91_Dernieres_Donnees_RADT retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




