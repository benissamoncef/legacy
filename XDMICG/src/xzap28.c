/*E*/
/*  Fichier : $Id: xzap28.c,v 1.4 2012/11/08 09:53:22 pc2dpdy Exp $      Release : $Revision: 1.4 $       Date : $Date: 2012/11/08 09:53:22 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap28.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Plan d'actions
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	 Creation
* JPL		06/12/10 : Migration architecture HP ia64 (DEM 961) : Cte. pour arg. 5 de 'ct_param' pour Sybase 15.x  1.3
* VR		08/06/12 : Ajout destination TDP PMVA (DEM/1014 PMA)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzap28.h"

static char *version = "$Id: xzap28.c,v 1.4 2012/11/08 09:53:22 pc2dpdy Exp $ $Revision: 1.4 $ $Date: 2012/11/08 09:53:22 $ : xzap28" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAP28SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAP28_Ajout_Prop_PMVA   (   XDY_Mot 	NumPA_In,
				XDY_Mot 	Ordre_In,
				XDY_District	District_In,
				XDY_Phrase	Remarque_In,
				XDY_Octet 	PrioriteEvt_In,
				XDY_Eqt 	NoEqt_In,
				XDY_Commentaire	Explication_In,
				XDY_Ligne_PMVA	Texte1PMVA_In,
				XDY_Ligne_PMVA	Texte2PMVA_In,
				XDY_Ligne_PMVA	Texte3PMVA_In,
				XDY_Ligne_PMVA	Texte4PMVA_In,
				XDY_Ligne_PMVA	Texte5PMVA_In,
				XDY_Ligne_PMVA	Alternat1PMVA_In,
				XDY_Ligne_PMVA	Alternat2PMVA_In,
				XDY_Ligne_PMVA	Alternat3PMVA_In,
				XDY_Ligne_PMVA	Alternat4PMVA_In,
				XDY_Ligne_PMVA	Alternat5PMVA_In,
				XDY_Octet	Flash_In,
				XDY_Octet	Dest_un_proche_In,
				XDY_Octet	Dest_un_loin_In,
				XDY_Octet	Dest_deux_proche_In,
				XDY_Octet	Dest_deux_loin_In,
				XDY_Nom		Libelle_In,
				XDY_Booleen	Realisable_In )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Phrase		Remarque_In
* XDY_Octet 		PrioriteEvt_In
* XDY_Eqt 		NoEqt_In
* XDY_Commentaire	Explication_In
* XDY_Ligne_PMVA		Texte1PMVA_In
* XDY_Ligne_PMVA		Texte2PMVA_In
* XDY_Ligne_PMVA		Texte3PMVA_In
* XDY_Ligne_PMVA		Texte4PMVA_In
* XDY_Ligne_PMVA		Texte5PMVA_In
* XDY_Ligne_PMVA		Texte6PMVA_In
* XDY_Nom		Libelle_In
* XDY_TxtPicto		TextePicto_In
* XDY_NomPicto		NomPicto_In
* XDY_Mot         	Destination_In
* XDY_Mot         	DestinationAlternat_In
* XDY_Booleen		Realisable_In
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzap28.c,v 1.4 2012/11/08 09:53:22 pc2dpdy Exp $ $Revision: 1.4 $ $Date: 2012/11/08 09:53:22 $ : XZAP28_Ajout_Prop_PMVA" ;

	CS_CHAR *rpc_name =XZAOC_XZAP28_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	XDY_Entier	SPstatus;
	
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAP28_Ajout_Prop_PMVA : NumPA_In=%d Ordre_In=%d District_In=%d Remarque_In=%s PrioriteEvt_In=%d NoEqt_In=%d Explication_In=%s TextePMVA1_In=%s Texte2PMVA_In=%s Texte3PMVA_In=%s Texte4PMVA_In=%s Texte5PMVA_In=%s Alternat1PMVA_In =%s Alternat2PMVA_In =%s Alternat3PMVA_In =%s Alternat4PMVA_In =%s Alternat5PMVA_In =%s Libelle_In=%s",NumPA_In,Ordre_In,District_In,Remarque_In ,PrioriteEvt_In,NoEqt_In,Explication_In ,Texte1PMVA_In ,Texte2PMVA_In ,Texte3PMVA_In ,Texte4PMVA_In ,Texte5PMVA_In ,Alternat1PMVA_In ,Alternat2PMVA_In,Alternat3PMVA_In,Alternat4PMVA_In,Alternat5PMVA_In,Libelle_In);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/


        strcpy(datafmt.name ,"@NumPA_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&NumPA_In,
			CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(@NumPA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Ordre_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&Ordre_In,
			CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(@Ordre_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	

	strcpy(datafmt.name ,"@District_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&District_In,
			CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(District_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d",retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	strcpy(datafmt.name ,"@Remarque_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Remarque_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.format    = CS_FMT_PADBLANK; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Remarque_In,
			strlen(Remarque_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(Remarque_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d",retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@PrioriteEvt_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&PrioriteEvt_In,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(PrioriteEvt_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	strcpy(datafmt.name ,"@NoEqt_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&NoEqt_In,
		CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(NoEqt_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@Explication_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Explication_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Explication_In,
		strlen(Explication_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Explication_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@Texte1PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Texte1PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Texte1PMVA_In,
		strlen(Texte1PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Texte1PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@Texte2PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Texte2PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Texte2PMVA_In,
		strlen(Texte2PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Texte2PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@Texte3PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Texte3PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Texte3PMVA_In,
		strlen(Texte3PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Texte3PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@Texte4PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Texte4PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Texte4PMVA_In,
		strlen(Texte4PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Texte4PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@Texte5PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Texte5PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Texte5PMVA_In,
		strlen(Texte5PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Texte5PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	
	strcpy(datafmt.name ,"@Alternat1PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Alternat1PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Alternat1PMVA_In,
		strlen(Alternat1PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Alternat1PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Alternat2PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Alternat2PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Alternat2PMVA_In,
		strlen(Alternat2PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Alternat2PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Alternat3PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Alternat3PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Alternat3PMVA_In,
		strlen(Alternat3PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Alternat3PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Alternat4PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Alternat4PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Alternat4PMVA_In,
		strlen(Alternat4PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Alternat4PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	strcpy(datafmt.name ,"@Alternat5PMV_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Alternat5PMVA_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Alternat5PMVA_In,
		strlen(Alternat5PMVA_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Alternat5PMVA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Flash_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&Flash_In,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Flash_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Dest_un_proche_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&Dest_un_proche_In,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Dest_un_proche_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Dest_un_loin_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&Dest_un_loin_In,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Dest_un_loin_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	strcpy(datafmt.name ,"@Dest_deux_proche_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE;
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE;
	datafmt.locale    = NULL;

		if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&Dest_deux_proche_In,
			CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
		{

			XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Dest_deux_proche_In) a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
			asql_clean_cmd(connection, cmd);
			return (XDC_NOK);
		}

	strcpy(datafmt.name ,"@Dest_deux_loin_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE;
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE;
	datafmt.locale    = NULL;

		if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&Dest_deux_loin_In,
			CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
		{

			XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Dest_deux_loin_In) a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
			asql_clean_cmd(connection, cmd);
			return (XDC_NOK);
		}

		
	strcpy(datafmt.name ,"@Libelle_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Libelle_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Libelle_In,
		strlen(Libelle_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_param(Libelle_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Realisable_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&Realisable_In,
		CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(Realisable_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP03_Ajout_Prop_NAV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap28: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_Prop_PMVA retourne %d", retcode);
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
						retcode = asql_fetch_data (cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"xzap28: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzap28 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzap28 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzap28 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_PMVA retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzap01: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_PMVA retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzap01: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_PMVA retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzap01: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_PMVA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP28_Ajout_PMVA retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
