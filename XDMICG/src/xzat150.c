/*E*/
/*  Fichier : 	$Id: xzat150.c,v 1.1 2017/02/10 09:23:38 pc2dpdy Exp $  Release : $Revision: 1.1 $ Date : $Date: 2017/02/10 09:23:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat05.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	26/1/16	creatio SAGA 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"

static char *version = "$Id: xzat150.c,v 1.1 2017/02/10 09:23:38 pc2dpdy Exp $ : xzat05" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAT05SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAT150_Utilisation_Eqt_SAGA  ( 	XDY_Horodate	va_Horodate_in,
				XDY_Mot		va_NumEqt_in,
				XDY_Octet 	*va_DispoPMV_out,
				XDY_Ligne_PMV 	va_Ligne1_out,
				XDY_Ligne_PMV 	va_Ligne2_out,
				XDY_Ligne_PMV 	va_Ligne3_out,
				XDY_Ligne_PMV 	va_Ligne4_out,
				XDY_Ligne_PMV 	va_Ligne5_out,
				XDY_Ligne_PMV	va_Alternat1_out,
				XDY_Ligne_PMV 	va_Alternat2_out,
				XDY_Ligne_PMV 	va_Alternat3_out,
				XDY_Ligne_PMV 	va_Alternat4_out,
				XDY_Ligne_PMV 	va_Alternat5_out,
				XDY_Booleen 	*va_Clignotement_out,
				char 		*va_Picto_out,
				XDY_Mot		*va_Luminosite_out,
				char 		*va_TextePicto_out,
				XDY_Mot		*va_Barriere_out,
				XDY_Mot		*va_Scenario_out,
				XDY_Entier 	*va_NumEvt_out,
				XDY_Octet 	*va_CleEvt_out,
				XDY_Octet 	*va_Priorite_out,
				XDY_Entier 	*va_DistanceEvt_out,
				XDY_Octet	*va_TypeSAGA_out)

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'utilisation reelle d'un PMV a une horodate donnee.
* 
* Sequence d'appel
* CL	XZAT150_Utilisation_Eqt_SAGA
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Mot	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet  	va_DispoPMV_out
* XDY_Octet  	va_DispoPicto_out
* XDY_Octet  	va_TypePMV_out
* XDY_Ligne_PMV	va_Ligne1_out
* XDY_Ligne_PMV	va_Ligne2_out
* XDY_Ligne_PMV	va_Ligne3_out
* XDY_Ligne_PMV	va_Alternat1_out
* XDY_Ligne_PMV	va_Alternat2_out
* XDY_Ligne_PMV	va_Alternat3_out
* XDY_Booleen	va_Clignotement1_out
* XDY_Booleen	va_Clignotement2_out
* XDY_Booleen	va_Clignotement3_out
* XDY_Booleen	va_Flash_out
* char[4]	va_TypePicto_out
* char[6]	va_TextePicto_out
* XDY_Octet	va_NumTypePicto_out
* XDY_Booleen	va_ClignotementPicto_out
* XDY_Booleen	va_ClignotementPicto_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_CleEvt_out
* XDY_Octet	va_Priorite_out
* XDY_Entier	va_DistanceEvt_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* select dans les tables EQUIPEMENT, E_PMV, ACTION et 
* INDISPONIBILITE
------------------------------------------------*/


{

	static char *version = "$Id: xzat150.c,v 1.1 2017/02/10 09:23:38 pc2dpdy Exp $ : XZAT150_Utilisation_Eqt_SAGA" ;

	CS_CHAR *rpc_name = XZATC_XZAT150_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZATC_XZAT150_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZATC_XZAT150_NB_PARM_RETURN];
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	XDY_Entier 	vl_Action;
	XDY_Octet	vl_SiteAction;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	tg_row_result   vl_Tab_Ligne = 0;
	XDY_Octet vl_cle, vl_type;
	int vl_numero,vl_dist;

	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAT150_Utilisation_Eqt_SAGA : Jours = %d\tHeure = %d\tNumEqt = %d\n" , vl_Horodate.dtdays,vl_Horodate.dttime,va_NumEqt_in );
			
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_smallint_input(NumEqt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_DispoPMV_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_DispoPMV_out,"@va_DispoPMV_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(DispoPMV) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[1].pt_value = (CS_VOID *)va_Ligne1_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Ligne1_out,"@va_Ligne1_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Ligne1) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[2].pt_value = (CS_VOID *)va_Ligne2_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Ligne2_out,"@va_Ligne2_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Ligne2) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[3].pt_value = (CS_VOID *)va_Ligne3_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Ligne3_out,"@va_Ligne3_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Ligne3) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	Tab_donnees2[4].pt_value = (CS_VOID *)va_Ligne4_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Ligne4_out,"@va_Ligne4_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Ligne4) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	Tab_donnees2[5].pt_value = (CS_VOID *)va_Ligne5_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Ligne5_out,"@va_Ligne5_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Ligne5) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	Tab_donnees2[6].pt_value = (CS_VOID *)va_Alternat1_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat1_out,"@va_Alternat1_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Alternat1) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[7].pt_value = (CS_VOID *)va_Alternat2_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat2_out,"@va_Alternat2_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Alternat2) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[8].pt_value = (CS_VOID *)va_Alternat3_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat3_out,"@va_Alternat3_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Alternat3) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	Tab_donnees2[9].pt_value = (CS_VOID *)va_Alternat4_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat4_out,"@va_Alternat4_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(va_Alternat4_out) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	Tab_donnees2[10].pt_value = (CS_VOID *)va_Alternat5_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat5_out,"@va_Alternat5_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(va_Alternat5_out) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	Tab_donnees2[11].pt_value = (CS_VOID *)va_Clignotement_out;
	
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_Clignotement_out,"@va_Clignotement_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_bit_output(Clignotement1) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[12].pt_value = (CS_VOID *)va_Picto_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Picto_out,"@va_Picto_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Picto) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	Tab_donnees2[13].pt_value = (CS_VOID *)va_Luminosite_out;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)va_Luminosite_out,"@va_Luminosite_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(va_Luminosite_out) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	Tab_donnees2[14].pt_value = (CS_VOID *)va_TextePicto_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_TextePicto_out,"@va_Bandeau_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(TextePicto) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}


	Tab_donnees2[15].pt_value = (CS_VOID *)va_Barriere_out;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)va_Barriere_out,"@va_Barriere_out")) != CS_SUCCEED)	
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(va_Barriere_out) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	Tab_donnees2[16].pt_value = (CS_VOID *)va_Scenario_out;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)va_Scenario_out,"@va_Scenario_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(va_Scenario_out) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	Tab_donnees2[17].pt_value = (CS_VOID *)&vl_numero;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&vl_numero,"@va_NumEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_int_output(NumEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[18].pt_value = (CS_VOID *)&vl_cle;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_cle,"@va_CleEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(CleEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[19].pt_value = (CS_VOID *)va_Priorite_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Priorite_out,"@va_Priorite_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[20].pt_value = (CS_VOID *)&vl_dist;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&vl_dist,"@va_DistanceEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_int_output(DistanceEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[21].pt_value = (CS_VOID *)&vl_Action;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&vl_Action,"@va_Action_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_int_output(Action) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[22].pt_value = (CS_VOID *)&vl_SiteAction;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_SiteAction,"@va_SiteAction_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(SiteAction) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[23].pt_value = (CS_VOID *)&vl_type;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_type,"@va_TypeSAGA_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(va_TypeSAGA_out) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzat05: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat05 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat05 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat05 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzat05 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzat05 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzat05 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzat05 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
	strcpy( va_Ligne1_out 	,  (char *)vl_Tab_Ligne[0][0].pt_value);
	strcpy( va_Ligne2_out 	,  (char *)vl_Tab_Ligne[0][1].pt_value);
	strcpy( va_Ligne3_out 	,  (char *)vl_Tab_Ligne[0][2].pt_value);
	strcpy( va_Ligne4_out 	,  (char *)vl_Tab_Ligne[0][3].pt_value);
	strcpy( va_Ligne5_out 	,  (char *)vl_Tab_Ligne[0][4].pt_value);
	strcpy( va_Alternat1_out 	,  (char *)vl_Tab_Ligne[0][5].pt_value);
	strcpy( va_Alternat2_out 	,  (char *)vl_Tab_Ligne[0][6].pt_value);
	strcpy( va_Alternat3_out 	,  (char *)vl_Tab_Ligne[0][7].pt_value);
	strcpy( va_Alternat4_out 	,  (char *)vl_Tab_Ligne[0][8].pt_value);
	strcpy( va_Alternat5_out 	,  (char *)vl_Tab_Ligne[0][9].pt_value);
	strcpy( va_Picto_out 	,  (char *)vl_Tab_Ligne[0][10].pt_value);
	strcpy( va_TextePicto_out 	,  (char *)vl_Tab_Ligne[0][11].pt_value);

	*va_CleEvt_out=vl_cle;
	*va_NumEvt_out = vl_numero;
	*va_DistanceEvt_out = vl_dist;
	*va_TypeSAGA_out=vl_type;
	XZST_03EcritureTrace(XZSTC_INTERFACE, "va_Ligne1_out %s//va_Ligne2_out %s//va_Ligne3_out %s",
		va_Ligne1_out,va_Ligne2_out,va_Ligne3_out);
	XZST_03EcritureTrace(XZSTC_INTERFACE, "va_NumEvt_out %d//va_CleEvt_out %d//va_Priorite_out %d  // va_Distance_out %d",
				*va_NumEvt_out,*va_CleEvt_out,*va_Priorite_out, *va_DistanceEvt_out);
	XZST_03EcritureTrace(XZSTC_INTERFACE, "va_TypeSAGA_out %d",
						*va_TypeSAGA_out);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT150_Utilisation_Eqt_SAGA retourne (dispo %d) %d", 
		*va_DispoPMV_out, retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




