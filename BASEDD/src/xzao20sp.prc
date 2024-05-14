/*E*/
/*Fichier : $Id: xzao20sp.prc,v 1.14 2009/10/22 15:48:58 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2009/10/22 15:48:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao20sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao20sp.h >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO20SP_Rech_Tunnels
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute		Autoroute_In
* XDY_PR		PR_In
* XDY_Sens		Sens_In
* XDY_Horodate		va_Horodate_in
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
*
* Recherche tous les tunnels (equipement) possedant une signalisation
* non inhibee dont la zone de competence englobe PR_In
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
* Select dans les tables EQT_GEN (EQUIPEMENT) et EQT_TUB (TUBE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* gaborit       16/11/94        : modif (1.6)
* C.T.  	10/01/95        : Ajout une date en entree (v1.8)
*              		          pour la formation
* gaborit       17/01/95        : modif curseur (1.9)
* gaborit       26/01/1995	: remplacement select par if exists (1.10)
* gaborit       14/09/1995	: teste tunnel entre pr tete et queue (1.11)
* PN	        29/06/1998	: separation EXP/HIS (1.12)
* PN	        13/07/2005	: suppression de l'appel de SQL_HIS au CI (1.13) DEM486
* JMG		25/09/2008 	: ajout nom tunnel en sortie (1.14)
------------------------------------------------------*/


use PRC
go

create procedure XZAO;20
	@va_Autoroute_in	tinyint = null,
   	@va_PR_in		int = null,
   	@va_Sens_in		tinyint = null,
	@va_Horodate_in		datetime = null,
	@va_PR_tete_in		int = null
as
declare @vl_NumEqt smallint, @vl_PR T_PR, @vl_Site T_SITE,
	@vl_Horodate datetime, @vl_Status int,
	@vl_nom char(10),
	@vl_VR tinyint,
	@vl_VM2 tinyint, @vl_VM1 tinyint, @vl_VL tinyint, @vl_BAU tinyint,
	@vl_VR_I tinyint, @vl_VM2_I tinyint,@vl_VM1_I tinyint,
	@vl_VL_I tinyint, @vl_BAU_I tinyint,
	@vl_influence int, @vl_zi tinyint


   /* Test si pas de pr tete alors tete=queue */
   if @va_PR_tete_in=null or @va_PR_tete_in=-1
   select @va_PR_tete_in=@va_PR_in

   /* Test des parametres d'entree :
      Si un des parametres d'entree est egal a null alors XDC_NOK   */
      
   if @va_Autoroute_in = null or @va_PR_in = null or @va_Sens_in = null
      return XDC_ARG_INV
   else   
      
      /* Recherche de tous les tunnels possedant une signalisation non inhibee
         dont la zone de competence englobe va_PR_in                              */  
	create table #EQT_TUB (numero smallint, nom char(10),PR int, influence int, VR tinyint, VM2
	tinyint, VM1 tinyint, VL tinyint, BAU tinyint)

	create unique index #EQT_TUB_PK on #EQT_TUB (numero)

	insert into #EQT_TUB
	select 	TUB.numero,EQT.nom,EQT.PR, TUB.PR_Influence,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE
	from 	CFG..EQT_TUB TUB,
		CFG..EQT_GEN EQT,
		EXP..EQT_DSP DSP
	where 	EQT.type=XDC_EQT_TUB and
		TUB.numero=EQT.numero and
		EQT.autoroute=@va_Autoroute_in and
		EQT.sens=@va_Sens_in and
		(
			@va_Sens_in<>XDC_SENS_SUD or
			(EQT.PR<=@va_PR_tete_in and TUB.PR_Influence>=@va_PR_in)
		) and
		(
			@va_Sens_in<>XDC_SENS_NORD or
			(EQT.PR>=@va_PR_tete_in and TUB.PR_Influence<=@va_PR_in)
		) and
		DSP.type=XDC_EQT_TUB and
		DSP.equipement=EQT.numero and
		DSP.dernier=XDC_OUI and
		DSP.HS=XDC_NON and
		DSP.inhibe=XDC_NON

/*appelle la XZAE860 pour la zone d'influence de chaque tunnel*/
        declare Eqt2 cursor for
        select  numero, PR, influence from #EQT_TUB
        for update of VR, VM2, VM1, VL, BAU
        open Eqt2
        fetch Eqt2 into @vl_NumEqt, @vl_PR, @vl_influence
        while (@@sqlstatus = 0)
        begin
                exec PRC..XZAE860 @va_Autoroute_in, @vl_PR, @va_Sens_in, @vl_influence,
                        @vl_VR output , @vl_VM2 output , @vl_VM1 output ,@vl_VL output ,@vl_BAU output , @vl_VR_I output , @vl_VM2_I output , @vl_VM1_I output ,@vl_VL_I output ,@vl_BAU_I output , @vl_zi output
                update #EQT_TUB
                set VR=@vl_VR, VM2=@vl_VM2, VM1 = @vl_VM1, VL = @vl_VL, BAU = @vl_BAU
                where numero = @vl_NumEqt
                fetch Eqt2 into @vl_NumEqt, @vl_PR, @vl_influence
        end
        close Eqt2
        select * from #EQT_TUB


	return XDC_OK
         
go  
