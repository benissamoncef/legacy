/*E*/
/*Fichier : $Id: xzah08.c,v 1.7 1997/05/02 16:28:57 gaborit Exp $      Release : $Revision: 1.7 $        Date : $Date: 1997/05/02 16:28:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER xzah08.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	XZAH08 : Envoie a l IHM les modifications des variables 
*  d animation synoptiques
*
*  Appelle XZAH;08 : Lecture et ecriture des synoptiques pour animation.
* 
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
*********************
* mercier	03 Nov 1994	: Modification		V 1.2
*     	passage par adresse a asql_ctparam_tinyint_input de 
*	du booleen va_Init_in effectue par une variable intermediaire
*	Ajout de l appel a asql_clean_cmd.
* gaborit	22 dec 1994	: modif NOK xzis09
*************
* mercier	06 Jan 1995	: Correction bug test xzis	V 1.4
* niepceron	27 Aou 1996	: Ajout du site en arg  + arg d'entree de la proc BIT->TINYINT 
					(pour avoir un bool a 3 etats!) v1.5
* niepceron     09 Jan 1997	: Ajout d'un num ordre (dem/1285) v1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzah08.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah08.c,v 1.7 1997/05/02 16:28:57 gaborit Exp $ : xzah08" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*	XZAH08 : Envoie a l IHM les modifications des variables d animation synoptiques
*
*  Appelle XZAH;08 : Lecture et ecriture des synoptiques pour animation
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAH08_Envoyer_Objet_Mobile_Aff_Voies(XDY_Booleen	va_Init_in ,XDY_Nom va_NomSite)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Booleen	va_Init_in	
* XDY_Nom va_NomSite	
*
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
* que l on envoie a l IHM grace a XZAH08_Envoyer_Objet_Mobile_Aff_Voies.
*
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR *rpc_name = XZAH08C_XZAH08_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	CS_RETCODE      vl_retcode;
	CS_INT          res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_num_Tab_donnees2 = XZAH08C_XZAH08_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	tg_row_result 	vl_Tab_Ligne;
	int		i =0;
	int		j =0;
	XDY_Entier	vl_SPstatus;
	XDY_Booleen	vl_Init_in=va_Init_in;
	/*A
	** Definition des variables locales a passer a XZIS09
	*/	
	XDY_Octet 	vl_TypeRaf; 
        XDY_Evt 	vl_Idfmc; 
        XDY_Booleen 	vl_DebFin;
        XDY_Mot 	vl_Idportion;
        XDY_Entier 	vl_CoefTranslation;
        XDY_Octet  	vl_Avoie1;
        XDY_Octet  	vl_Nvoie1;
        XDY_Octet  	vl_Avoie2;
        XDY_Octet  	vl_Nvoie2;
        XDY_Octet  	vl_Avoie3;
        XDY_Octet  	vl_Nvoie3;
        XDY_Octet  	vl_Avoie4;
        XDY_Octet  	vl_Nvoie4;
        XDY_Octet  	vl_Avoie5;
        XDY_Octet  	vl_Nvoie5;
        XDY_Octet  	vl_Avoie6;
        XDY_Octet  	vl_Nvoie6;
        XDY_Octet  	vl_Avoie7;
        XDY_Octet  	vl_Nvoie7;
        XDY_Octet  	vl_Avoie8;
        XDY_Octet  	vl_Nvoie8;
        XDY_Octet  	vl_Avoie9;
        XDY_Octet  	vl_Nvoie9;
        XDY_Octet  	vl_Avoie10;
        XDY_Octet  	vl_Nvoie10;
	XDY_Octet       vl_numordre;
	
		
	static char *version = "$Id: xzah08.c,v 1.7 1997/05/02 16:28:57 gaborit Exp $ : XZAH08_Envoyer_Objet_Mobile_Aff_Voies" ;	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s : Init = %d\n",version,va_Init_in);			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&vl_Init_in,"@va_Init_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: asql_ctparam_tinyint_input(vl_Init_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
	   asql_clean_cmd (pl_connection,pl_cmd);
	   return (XDC_NOK);
	}	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies : LE RESULTAT de STATUS DE LA PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAH08_Envoyer_Objet_Mobile_Aff_Voies retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
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
           	j=0;
           	/*B
           	** Remplissage des parametres pris par la XZIS09 avec 
           	** coorrespondant a la ligne resultat
           	** du select de la procedure stockee XZAH;08
           	*/
           	if(vl_Tab_Ligne[i][j].indicator == CS_INT_TYPE)
           	vl_Idfmc.numero		= * (XDY_Entier *) vl_Tab_Ligne[i][j++].pt_value; 
           	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
           	
           	if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Idfmc.cle		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
           	
           	if(vl_Tab_Ligne[i][j].indicator == CS_BIT_TYPE)
       	   	vl_DebFin		= * (XDY_Booleen *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_SMALLINT_TYPE)
       	   	vl_Idportion		= * (XDY_Mot *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_TypeRaf		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_INT_TYPE)
       	   	vl_CoefTranslation	= * (XDY_Entier *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie1		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie1		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie2		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie2		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie3		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie3		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie4		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie4		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie5		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie5		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie6		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie6		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie7		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie7		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie8		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie8		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie9		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie9		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Nvoie10		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
       	   	vl_Avoie10		= * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);
		if(vl_Tab_Ligne[i][j].indicator == CS_TINYINT_TYPE)
		vl_numordre              = * (XDY_Octet *) vl_Tab_Ligne[i][j++].pt_value;
		else XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08_Envoyer_Objet_Mobile_Aff_Voies: Probleme sur le type de la colonne %d de la ligne %d\n sur retour de select de XZAH;08",j,i);

		
         	/*A
         	** Commande d animation synoptique pour chaque ligne retourne par XZAH;08
         	*/
         	if ((XZIS09_Raf_Objet_Mobile_Port_Aff_Voies(
         						vl_TypeRaf, 
                                            		vl_Idfmc, 
                                            		vl_DebFin,
                                            		vl_Idportion,
                                            		vl_CoefTranslation,
                                            		vl_Avoie1,vl_Nvoie1,
                                            		vl_Avoie2, vl_Nvoie2,
                                            		vl_Avoie3, vl_Nvoie3,
                                            		vl_Avoie4, vl_Nvoie4,
                                            		vl_Avoie5, vl_Nvoie5,
                                            		vl_Avoie6, vl_Nvoie6,
                                            		vl_Avoie7, vl_Nvoie7,
                                            		vl_Avoie8, vl_Nvoie8,
                                            		vl_Avoie9, vl_Nvoie9,
                                            		vl_Avoie10,vl_Nvoie10,
							vl_numordre,
                                            		va_NomSite))!=XDC_OK)
                {
         		XZST_03EcritureTrace(XZSTC_WARNING,"XZAH08 : XZIS09_Raf_Objet_Mobile_Port_Aff_Voies s est mal deroule");
         	}
         	else	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAH08 : XZIS09_Raf_Objet_Mobile_Port_Aff_Voies : OK");          
	}
	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (vl_retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd (pl_connection,pl_cmd);
           return (XDC_NOK);
	}	
 		
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
	asql_clean_cmd (pl_connection,pl_cmd);	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
 	return(XDC_OK);
}

