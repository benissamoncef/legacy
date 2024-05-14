/*E*/
/*Fichier : $Id: xzah01.c,v 1.5 1996/09/09 09:09:46 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1996/09/09 09:09:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER xzah01.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*********************
*  XZAH01_Envoyer_Bouchon_Chaussee_Portion recoie un booleen en entree.
*  Appelle XZAH;01 
*  puis pour chaque ligne renvoyee par le select effectue par XZAH;01,
*  on appelle XZIS03.
*********************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
*****************************
* MERCIER	10 Nov 1994	: Modification		V1.2
*   	Blindage en cas de non prÅsence de la SP sur le SQL Srv.
*	et liberation de la structure cmd par le biais asql_clean_cmd.
*****************************
* gaborit	22 dec 1994	: bug tiny->mot (1.3)
***********
* mercier	06 Jan 1995	: correction code de retour xzis 	v 1.4
* niepceron	27 Aou 1996	: Ajout du site en arg  + arg d'entree de la proc BIT->TINYINT 
					(pour avoir un bool a 3 etats!) v1.5
------------------------------------------------------*/

/* fichiers inclus */

#include "xzah01.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah01.c,v 1.5 1996/09/09 09:09:46 gaborit Exp $ : xzah" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  XZAH01_Envoyer_Bouchon_Chaussee_Portion recoie un booleen en entree.
*  Appelle XZAH;01 
*  puis pour chaque ligne renvoyee par le select effectue par XZAH;01,
*  on appelle XZIS03.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAH01_Envoyer_Bouchon_Chaussee_Portion(	XDY_Booleen	va_Init_in ,XDY_Nom va_NomSite)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Booleen	va_Init_in		
* 
* XDY_Nom va_NomSIte
* PARAMETRES EN SORTIE : aucun
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
* Pour chaque ligne du select retournee par la SP, on constitue une chaine,
* que l on envoie a l IHM grace a XZAH01.
*
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR *rpc_name = XZAH01C_XZAH01_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	CS_RETCODE      vl_retcode;
	CS_INT          res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_num_Tab_donnees2 = XZAH01C_XZAH01_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	vl_SPstatus;
	
	XDY_Booleen	vl_Init_in = va_Init_in;
	/*A
	** Definition des variables locales
	*/	
		
	XDY_Evt		vl_Evt;
	XDY_Octet 	vl_TypeRaf;
        XDY_Mot 	vl_Idportion;
        XDY_Sens 	vl_Sens;
        XDY_Entier 	vl_CoefTranslation;
        XDY_Entier 	vl_CoefExtension;
        XDY_Octet  	vl_niveau;
		
	static char *version = "$Id: xzah01.c,v 1.5 1996/09/09 09:09:46 gaborit Exp $ : XZAH01_Envoyer_Bouchon_Chaussee_Portion" ;	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s : Init = %d\n",version,va_Init_in);			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&vl_Init_in,"@va_Init_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: asql_ctparam_tinyint_input(va_Init_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
	   asql_clean_cmd (pl_connection,pl_cmd);
	   return (XDC_NOK);
	}	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/
	while ((vl_retcode = ct_results(pl_cmd, &res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzah01: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzah01 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzah01 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzah01 : LE RESULTAT de STATUS DE LA PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAH01_Envoyer_Bouchon_Chaussee_Portion retourne 1");
 						   asql_clean_cmd (pl_connection,pl_cmd);
 						   return (XDC_NOK);
						}
						break;
				}				
				break;
				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return (XDC_NOK);
				}
	}

	
	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurTypPMV pour
	** chaque type de PMV
	*/
	
	
        for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{         	
           	/*B
           	** Remplissage de la structure XDY_Evt ainsi que des parametres pris par la XZIS03 avec 
           	** coorrespondant a la ligne resultat
           	** du select de la procedure stockee XZAH;01
           	*/
           	if(vl_Tab_Ligne[i][0].indicator == CS_INT_TYPE)
           	vl_Evt.numero		= * (XDY_Entier *) vl_Tab_Ligne[i][0].pt_value; 
           	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: Probleme sur le type de la colonne 1 de la ligne %d\n sur retour de select de XZAH;01",i);
           	if(vl_Tab_Ligne[i][1].indicator == CS_TINYINT_TYPE)
       	   	vl_Evt.cle			= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: Probleme sur le type de la colonne 2 de la ligne %d\n sur retour de select de XZAH;01",i);
           	if(vl_Tab_Ligne[i][2].indicator == CS_SMALLINT_TYPE)
       	   	vl_Idportion			= * (XDY_Mot *) vl_Tab_Ligne[i][2].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: Probleme sur le type de la colonne 3 de la ligne %d\n sur retour de select de XZAH;01",i);
		if(vl_Tab_Ligne[i][3].indicator == CS_TINYINT_TYPE)
       	   	vl_TypeRaf			= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: Probleme sur le type de la colonne 4 de la ligne %d\n sur retour de select de XZAH;01",i);
		if(vl_Tab_Ligne[i][4].indicator == CS_TINYINT_TYPE)
       	   	vl_Sens				= * (XDY_Sens *) vl_Tab_Ligne[i][4].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: Probleme sur le type de la colonne 5 de la ligne %d\n sur retour de select de XZAH;01",i);
		if(vl_Tab_Ligne[i][5].indicator == CS_INT_TYPE)
       	   	vl_CoefTranslation		= * (XDY_Entier *) vl_Tab_Ligne[i][5].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: Probleme sur le type de la colonne 6 de la ligne %d\n sur retour de select de XZAH;01",i);
		if(vl_Tab_Ligne[i][6].indicator == CS_INT_TYPE)
       	   	vl_CoefExtension		= * (XDY_Entier *) vl_Tab_Ligne[i][6].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: Probleme sur le type de la colonne 7 de la ligne %d\n sur retour de select de XZAH;01",i);
		if(vl_Tab_Ligne[i][7].indicator == CS_TINYINT_TYPE)
       	   	vl_niveau			= * (XDY_Octet *) vl_Tab_Ligne[i][7].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah01: Probleme sur le type de la colonne 8 de la ligne %d\n sur retour de select de XZAH;01",i);
         	/*A 
         	** pour chaque ligne retournee par XZAH;01 on appelle XZIS03
         	*/
         	if (XZIS03_Raf_Bouchon_Chaussee_Portion (vl_TypeRaf,vl_Evt,vl_Idportion,vl_Sens,vl_CoefTranslation,vl_CoefExtension,vl_niveau, va_NomSite)!=XDC_OK)
         	{
         		XZST_03EcritureTrace(XZSTC_WARNING,"xzah01 : XZIS03_Raf_Bouchon_Chaussee_Portion s est mal deroule");
         	}
         	else	XZST_03EcritureTrace(XZSTC_DEBUG1,"xzah01 : XZIS03_Raf_Bouchon_Chaussee_Portion : OK");          
	}
	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	if ((vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne))!= XDC_OK)
	{
	   	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           	asql_clean_cmd (pl_connection,pl_cmd);
           	return (XDC_NOK);
	}	
 		
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
	asql_clean_cmd (pl_connection,pl_cmd);	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
 	return(XDC_OK);
}


