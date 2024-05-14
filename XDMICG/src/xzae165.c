/*E*/
/*Fichier : $Id: xzae165.c,v 1.1 2001/04/19 16:26:10 gesconf Exp $    Release : $Revision: 1.1 $        Date : $Date: 2001/04/19 16:26:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae165.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae165.h"

static char *version = " $Id: xzae165.c,v 1.1 2001/04/19 16:26:10 gesconf Exp $ $Revision: 1.1 $ $Date: 2001/04/19 16:26:10 $ ";
	
/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X------------------------------------------------------
* SERVICE RENDU : 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAE165_Lire_Icones (  XDY_FonctionInt va_FonctionUtilisateur_in,
			XDY_Octet      va_niveau_in)


/*
*
* PARAMETRES EN ENTREE :
*
* int           va_NumEvt_in
* tinyint       va_CleEvt_in
* XDY_Fonction  va_FonctionUtilisateur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
*  Liste des autoroutes
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
* Pour chaque autoroute appel de la fonction utilisateur avec 
*
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR *rpc_name = XZAE165C_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAE165C_NB_PARM_RETURN;
       	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	char vl_identifiant[10];
	int vl_niveau;
	char vl_nom_vue[50];
	int vl_X;
	int vl_Y;
	int vl_facteurX;
	int vl_facteurY;
	int vl_rotation;
	char vl_icone[30];
	char vl_texte[255];
	char vl_horodate[30];
	XDY_Octet vl_type;
	int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE165_Lire_Icones : niveau %d ",
			va_niveau_in);
			
			
   	/*A
        **  test les parametres d'entree
        */

	if ( va_FonctionUtilisateur_in == NULL)
	{
	      XZST_03EcritureTrace(XZSTC_WARNING,"va_FonctionUtilisateur_in est nulle");
	      return (XDC_NOK);
	}

	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae165: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE165_Lire_Icones retourne %d", retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae165: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE165_Lire_Icones retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_niveau_in,"@va_niveau_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE150: asql_ctparam_tinyint_input(va_niveau_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE150_Message_Datex retourne %d", retcode);
	   	asql_clean_cmd (connection,cmd);
	   	return (XDC_NOK);
	}
		
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae165: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE165_Lire_Icones retourne %d", retcode);
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
						retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae165: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzae165 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae165 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae165 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE165_Lire_Icones retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae165: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE165_Lire_Icones retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
			 			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae165: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE165_Lire_Icones retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae165: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE165_Lire_Icones retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);
				}
	}

	
	XZST_03EcritureTrace(XZSTC_INTERFACE, "XZAE165_Lire_Icones %d lignes", vl_Nb_Ligne_Lue);

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
	           
		/*B
	         * recup des parametres de retour
	         */
	        strcpy(vl_identifiant, (char *) vl_Tab_Ligne[i][0].pt_value);
	        strcpy (vl_nom_vue, (char *) vl_Tab_Ligne[i][2].pt_value);
		vl_X	= * (XDY_Entier *) vl_Tab_Ligne[i][3].pt_value;
		vl_Y    = * (XDY_Entier *) vl_Tab_Ligne[i][4].pt_value;
		vl_facteurX = * (XDY_Entier *) vl_Tab_Ligne[i][5].pt_value;
		vl_facteurY = * (XDY_Entier *) vl_Tab_Ligne[i][6].pt_value;
		vl_rotation = * (XDY_Entier *) vl_Tab_Ligne[i][7].pt_value;
		strcpy (vl_icone, (char *) vl_Tab_Ligne[i][8].pt_value);
		strcpy (vl_texte, (char *) vl_Tab_Ligne[i][9].pt_value);
		vl_jour = * (XDY_Entier *) vl_Tab_Ligne[i][10].pt_value;
		vl_mois = * (XDY_Entier *) vl_Tab_Ligne[i][11].pt_value;
		vl_annee = * (XDY_Entier *) vl_Tab_Ligne[i][12].pt_value;
		vl_heure = * (XDY_Entier *) vl_Tab_Ligne[i][13].pt_value;
		vl_minute = * (XDY_Entier *) vl_Tab_Ligne[i][14].pt_value;
		vl_type = * (XDY_Octet *) vl_Tab_Ligne[i][15].pt_value;

		sprintf(vl_horodate,"%02d/%02d/%02d,%02d:%02d",
					vl_jour,
					vl_mois,
					vl_annee,
					vl_heure,
					vl_minute);

	        /*B
		 ** Appel de la fonction utilisateur FonctionUtilisateur
		 */
		  
		if (( retcode = (*va_FonctionUtilisateur_in) (vl_identifiant,
							      va_niveau_in,
							      vl_nom_vue,
							      vl_X,
							      vl_Y,
							      vl_facteurX,
							      vl_facteurY,
							      vl_rotation,
							      vl_icone,
							      vl_texte,
							      vl_horodate,
							      vl_type) != XDC_OK ))
		{
		        XZST_03EcritureTrace(XZSTC_FATAL,"appel de va_FonctionUtilisateur_in s'est mal deroule");
		        asql_clean_cmd(connection, cmd);
			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
			return (XDC_NOK);
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
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE165_Lire_Icones retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
