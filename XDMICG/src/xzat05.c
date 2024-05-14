/*E*/
/*  Fichier : 	$Id: xzat05.c,v 1.5 2021/01/06 09:44:41 akkaakka Exp $  Release : $Revision: 1.5 $ Date : $Date: 2021/01/06 09:44:41 $
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
* volcic       03 Nov 1994     : Creation
* volcic       23 Nov 1994     : Modif parametre sortie char (v1.2)
*				 Ajout un parametre de sortie
* volcic       19 Avr 1995	: suppression malloc tabdonnees2 (1.3)
* JMG		13/08/12 : correction param string sortie sybasre V15 linux 1.4
* GGY	28/04/23 : Augmentation taille va_TextePicto_out a (XDC_LGR_TXT_PICTO) et va_typePicto_out (XDC_LGR_NOM_PICTO) (DEM-473)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"

static char *version = "$Id: xzat05.c,v 1.5 2021/01/06 09:44:41 akkaakka Exp $ : xzat05" ;

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

int XZAT05_Utilisation_PMV  ( 	XDY_Horodate	va_Horodate_in,
				XDY_Mot		va_NumEqt_in,
				XDY_Octet 	*va_DispoPMV_out,
				XDY_Octet 	*va_DispoPicto_out,
				XDY_Octet 	*va_TypePMV_out,
				XDY_Ligne_PMV 	va_Ligne1_out,
				XDY_Ligne_PMV 	va_Ligne2_out,
				XDY_Ligne_PMV 	va_Ligne3_out,
				XDY_Ligne_PMV	va_Alternat1_out,
				XDY_Ligne_PMV 	va_Alternat2_out,
				XDY_Ligne_PMV 	va_Alternat3_out,
				XDY_Booleen 	*va_Clignotement1_out,
				XDY_Booleen 	*va_Clignotement2_out,
				XDY_Booleen 	*va_Clignotement3_out,
				XDY_Booleen 	*va_Flash_out,
				char 		*va_TypePicto_out,
				char 		*va_TextePicto_out,
				XDY_Octet 	*va_NumTypePicto_out,
				XDY_Booleen 	*va_ClignotementPicto_out,
				XDY_Entier 	*va_NumEvt_out,
				XDY_Octet 	*va_CleEvt_out,
				XDY_Octet 	*va_Priorite_out,
				XDY_Entier 	*va_DistanceEvt_out )

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'utilisation reelle d'un PMV a une horodate donnee.
* 
* Sequence d'appel
* CL	XZAT05_Utilisation_PMV
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
* char[XDC_LGR_NOM_PICTO]	va_TypePicto_out
* char[XDC_LGR_TXT_PICTO]	va_TextePicto_out
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

	static char *version = "$Id: xzat05.c,v 1.5 2021/01/06 09:44:41 akkaakka Exp $ : XZAT05_Utilisation_PMV" ;

	CS_CHAR *rpc_name = XZATC_XZAT05_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZATC_XZAT05_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZATC_XZAT05_NB_PARM_RETURN];
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	XDY_Entier 	vl_Action;
	XDY_Octet	vl_SiteAction;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	tg_row_result   vl_Tab_Ligne = 0;
	char	va_AlternatDir_out[20];
	char	va_LigneDir_out[20];

	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAT05_Utilisation_PMV : Jours = %d\tHeure = %d\tNumEqt = %d\n" , vl_Horodate.dtdays,vl_Horodate.dttime,va_NumEqt_in );
			
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
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
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_smallint_input(NumEqt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_DispoPMV_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_DispoPMV_out,"@va_DispoPMV_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(DispoPMV) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[1].pt_value = (CS_VOID *)va_DispoPicto_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_DispoPicto_out,"@va_DispoPicto_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(DispoPicto) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[2].pt_value = (CS_VOID *)va_TypePMV_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_TypePMV_out,"@va_TypePMV_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(TypePMV) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[3].pt_value = (CS_VOID *)va_Ligne1_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Ligne1_out,"@va_Ligne1_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Ligne1) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[4].pt_value = (CS_VOID *)va_Ligne2_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Ligne2_out,"@va_Ligne2_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Ligne2) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[5].pt_value = (CS_VOID *)va_Ligne3_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Ligne3_out,"@va_Ligne3_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Ligne3) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[6].pt_value = (CS_VOID *)va_Alternat1_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat1_out,"@va_Alternat1_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Alternat1) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[7].pt_value = (CS_VOID *)va_Alternat2_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat2_out,"@va_Alternat2_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Alternat2) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[8].pt_value = (CS_VOID *)va_Alternat3_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat3_out,"@va_Alternat3_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(Alternat3) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[9].pt_value = (CS_VOID *)va_Clignotement1_out;
	
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_Clignotement1_out,"@va_Clignotement1_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_bit_output(Clignotement1) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[10].pt_value = (CS_VOID *)va_Clignotement2_out;
	
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_Clignotement2_out,"@va_Clignotement2_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_bit_output(Clignotement2) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[11].pt_value = (CS_VOID *)va_Clignotement3_out;
	
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_Clignotement3_out,"@va_Clignotement3_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_bit_output(Clignotement3) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[12].pt_value = (CS_VOID *)va_Flash_out;
	
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_Flash_out,"@va_Flash_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_bit_output(Flash) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[13].pt_value = (CS_VOID *)va_TypePicto_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_TypePicto_out,"@va_TypePicto_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(TypePicto) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[14].pt_value = (CS_VOID *)va_TextePicto_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_TextePicto_out,"@va_TextePicto_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(TextePicto) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[15].pt_value = (CS_VOID *)va_NumTypePicto_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_NumTypePicto_out,"@va_NumTypePicto_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(NumTypePicto) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[16].pt_value = (CS_VOID *)va_ClignotementPicto_out;
	
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_ClignotementPicto_out,"@va_ClignotementPicto_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_bit_output(ClignotementPicto) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[17].pt_value = (CS_VOID *)va_NumEvt_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_NumEvt_out,"@va_NumEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_int_output(NumEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[18].pt_value = (CS_VOID *)va_CleEvt_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_CleEvt_out,"@va_CleEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(CleEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[19].pt_value = (CS_VOID *)va_Priorite_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Priorite_out,"@va_Priorite_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[20].pt_value = (CS_VOID *)va_DistanceEvt_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistanceEvt_out,"@va_DistanceEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_int_output(DistanceEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[21].pt_value = (CS_VOID *)&vl_Action;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&vl_Action,"@va_Action_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_int_output(Action) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[22].pt_value = (CS_VOID *)&vl_SiteAction;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_SiteAction,"@va_SiteAction_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_tinyint_output(SiteAction) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	Tab_donnees2[23].pt_value = (CS_VOID *)va_LigneDir_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat1_out,"@va_LigneDir1_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(LigneDir1) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[24].pt_value = (CS_VOID *)va_AlternatDir_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Alternat2_out,"@va_AlternatDir2_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: asql_ctparam_char_output(AlternatDir2) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
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
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzat05 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat05: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
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
	strcpy( va_Alternat1_out 	,  (char *)vl_Tab_Ligne[0][3].pt_value);
	strcpy( va_Alternat2_out 	,  (char *)vl_Tab_Ligne[0][4].pt_value);
	strcpy( va_Alternat3_out 	,  (char *)vl_Tab_Ligne[0][5].pt_value);
	strcpy( va_TypePicto_out 	,  (char *)vl_Tab_Ligne[0][6].pt_value);
	strcpy( va_TextePicto_out 	,  (char *)vl_Tab_Ligne[0][7].pt_value);

 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT05_Utilisation_PMV retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




