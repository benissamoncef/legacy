/*E*/
/*Fichier : $Id: xzad04.c,v 1.2 2010/12/07 12:52:45 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/12/07 12:52:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAD04 * FICHIER xzad04.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tacli
* permet de mettre en base les temps de parcours 6mn
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron	09 fev 1997	: Creation (dem/tdp)
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : retour de XDC_NOK sur erreur conversion date  1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzad04.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzad04.c,v 1.2 2010/12/07 12:52:45 gesconf Exp $ : xzad04" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAD04_Liste_Franchissements : recupere la liste de 
* franchissement pour un site ,une fmc et horodate donn≈s.
*  Appelle XZAD04SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD04_Liste_Franchissements (	XDY_Octet  	va_NumeroSite_in ,
					XDY_Horodate	va_Horodate_in,
					XDY_Entier	va_NoFMC_in,
					XDY_Octet	va_CleFMC_in,
					XDY_FonctionInt va_fct_in)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet               va_NumeroSite_in   : numero du site
* XDY_Datetime          va_Horodate_in
* XDY_TDP		va_NoFMC_in
* XDY_Octet		va_CleFMC_in
* XDY_FonctionInt va_fct_in

*				
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* - va_Numero_in, va_Horodate_in sont obligatoires
* - si une mesure est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
*

* FONCTION :
*
* Inserer ou Modifier dans la table ZDP_6MN
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzad04.c,v 1.2 2010/12/07 12:52:45 gesconf Exp $ : XZAD04_Liste_Franchissements" ;
	
	CS_CHAR 	*pl_rpc_name = XZAD04C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAD04C_NB_PARM_RETURN;
	tg_Colonne_donnees2 *vl_Tab_donnees2;
	tg_row_result 	vl_Tab_Ligne;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	
	XDY_Entier	vl_SPstatus;
	CS_DATETIME     vl_Horodate;
	XDY_Horodate	vl_HorodateFin;
	
	XZAD04_Ligne	vl_zdp;
	int 		vl_indice;
	XDY_Booleen	vl_Bool;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAD04_Liste_Franchissements : Debut d execution" );

	/*A
        ** Conversion des chaines de caracteres en entree du format Unix au format Sybase
        */

        if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_horodate_in,&vl_Horodate) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
		return (XDC_NOK);
        }
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_NumeroSite_in,"@va_NumeroSite_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: asql_ctparam_smallint_input(va_Numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: asql_ctparam_date_input(va_Horodate_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_NoFMC_in,"@va_NoFMC_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: asql_ctparam_int_input(va_NoFMC_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_CleFMC_in,"@va_CleFMC_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: asql_ctparam_tinyint_input(va_CleFMC_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_row_result (pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao204: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAD04 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAD04 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAD04 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAD04_Liste_Franchissements retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAD04: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	if(vl_SPstatus == XDC_OK)
	
	{
		for (vl_indice = 0 ; vl_indice<vl_Nb_Ligne_Lue;vl_indice++)
		{
			vl_zdp.Numero = * (XDY_Mot   *) vl_Tab_Ligne[vl_indice][0].pt_value;

			/*
			*A conversion de la date sybase en unix
			*/
			vl_retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)vl_Tab_Ligne[vl_indice][1].pt_value, &vl_HorodateFin);
			if (vl_retcode != XDC_OK)
				{
				XZST_03EcritureTrace(XZSTC_WARNING,
						"XZAD04:  : asql_Sybase2Ux(va_Horodate_in,&vl_Horodate) a echoue");
				XZST_03EcritureTrace(XZSTC_INTERFACE, 
						"OUT : XZAD04 retourne %d",vl_retcode);
				asql_clean_cmd(pl_connection, pl_cmd);
				return (XDC_NOK);
				}
			vl_zdp.HorodateFin = (XDY_Horodate)vl_HorodateFin;
			
	   	if ( va_fct_in == NULL)
	   	{
	      		XZST_03EcritureTrace(XZSTC_FATAL,"va_fct_in est nulle");
	      		asql_clean_cmd(pl_connection, pl_cmd);
	      		return (XDC_NOK);
	   	}
	   	else
	   	{
	      		if ( ( vl_retcode = (*va_fct_in) ( vl_zdp ) ) != XDC_OK )
	      		{
	         		XZST_03EcritureTrace(XZSTC_WARNING,"appel de va_fct_in s'est mal deroule");
	         		asql_clean_cmd(pl_connection, pl_cmd);
		 		return (XDC_NOK);
	      		}
	   	}	
	   
			
		}	
	}
	/*A
        ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
        ** si il y a lieu
        */

        if ((vl_retcode = asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne)) != XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
        }
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD04_Liste_Franchissements retourne %d", vl_retcode);
 	return(XDC_OK);
}



