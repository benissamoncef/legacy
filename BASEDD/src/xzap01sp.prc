/*E*/
/*Fichier : $Id: xzap01sp.prc,v 1.10 2020/05/26 13:16:05 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/05/26 13:16:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Sep 1994	: Creation
* volcic	15 Sep 1994	: Modification pour SAD (v1.2)
* gaborit	21 Sep 1994	: Rien (v1.3)
* gaborit	21 Sep 1994	: Rien (v1.4)
* volcic	23 Sep 1994	: Modification pour SAD (v1.5)
* volcic	13 Oct 1994	: Modification (v1.5)
* volcic	28 Oct 1994	: Modification reception chaine nulle (v1.6)
* gaborit	04 dec 1994	: Modification (file d'attente) (v1.7)
* torregrossa	25 sep 1995	: Modification calcul de numero : numerotage cyclique (v1.8)
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.9
* JMG		03/12/19 	: DEM1347 plus de EQT_FIL ici 1.10
* PNI 	14/09/2022	: n'enregistre pas le calcul du PA si numero FMC = 0 (cas d'appel keepalive) SAE-395
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Cree un nouveau plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP;01

	@Horodate_In 		datetime,
	@PosteOperateur_In	smallint,
	@NumEvt_In		int,
	@NumEvtCle_In		tinyint,
	@NumPA_Out		smallint output

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate	Horodate_In 
* XDY_Eqt	PosteOperateur_In 
* XDY_Entier	NumEvt_In
* XDY_Octet	NumEvtCle_In
*
* ARGUMENTS EN SORTIE :
*
* XDY_Mot	NumPA_Out
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* Au niveau du C.I., creation uniquement du plan d'actio
* (pas de gestion de file d'attente)
*
* FONCTION :
*
* DELETE FILE_D_ATTENTE_TRAVAIL
* SELECT type_equipement, equipement FROM FILE_D_ATTENTE_EQUIPEMENT
*     WHERE evenement = NumEvt.numero AND cle = NumEvt.cle
*
* Ajoute dans la table PLAN_D_ACTION et retourne le numero du plan
* d'actio cree
*
* MECANISMES :
*
------------------------------------------------------*/


as
declare @vl_horodateMaxMoins1Jour datetime,  @vl_horodateMax datetime,
	@vl_Bouclage bit, @vl_Erreur bit,
    	@vl_Status int, @vl_Site tinyint
 
#ifdef HIST
	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD
		
	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP;01
		@Horodate_In,
		@PosteOperateur_In,
		@NumEvt_In	,
		@NumEvtCle_In	,
		@NumPA_Out	output

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;01
		@Horodate_In,
		@PosteOperateur_In,
		@NumEvt_In	,
		@NumEvtCle_In	,
		@NumPA_Out	output

    if @vl_Status = null
        return XDC_PRC_INC
    else
		return @vl_Status
#else

   if @Horodate_In = null or @PosteOperateur_In = null or @NumEvt_In = null or @NumEvtCle_In = null
      return XDC_NOK
   else   
      begin
	  	if @NumEvt_In =0
	  	begin
			select @NumPA_Out=0 
			return XDC_OK
		end
	  
   
      /*A
      ** Recherche du numero du dernier plan d'actions cree
      ** avec numerotation cyclique et test sur date en cas de changement d'heure
      */

      begin tran
	      select @vl_horodateMax = max(horodate) from EXP..SAD_PLN
	
	      if @vl_horodateMax is null
		select @NumPA_Out = 0
	      else
	      begin
	      	select @vl_horodateMaxMoins1Jour = dateadd(day, -1, @vl_horodateMax)
	
	      	select @NumPA_Out = max(numero) from EXP..SAD_PLN
	      	where horodate between @vl_horodateMaxMoins1Jour and @vl_horodateMax
	      end
	
	      if @NumPA_Out = null  or @NumPA_Out > 32000
	         select @NumPA_Out = 0
	         
	      select @NumPA_Out = @NumPA_Out + 1

	      /*A
	      ** si le numero existe en base alors prendre le prochain numero libre
	      */
	      if exists (select * from EXP..SAD_PLN where numero = @NumPA_Out)
	      begin
		select @vl_Bouclage = XDC_VRAI

		while @vl_Bouclage = XDC_VRAI
		begin
			select @NumPA_Out = @NumPA_Out + 1
			if not exists (select * from EXP..SAD_PLN where numero = @NumPA_Out)
				select @vl_Bouclage = XDC_FAUX
		end
	      end
	      
	      /*A
	      ** Insertion dans la table SAD_PLN (PLAN_D_ACTION) d'un nouveau plan d'actions
	      */
	     
	      insert into EXP..SAD_PLN
	      values ( @NumPA_Out, @NumEvt_In, @NumEvtCle_In, @PosteOperateur_In, @Horodate_In ) 

	      if @@rowcount != 1
		select @vl_Erreur = XDC_VRAI
      commit tran
      
      if @vl_Erreur = XDC_VRAI
	return XDC_NOK

      /*A mise a jour de la file d'attente de travail */
      /*
      delete EXP..EQT_FIL_TRV
      
      insert EXP..EQT_FIL_TRV
      select type_equipement,equipement
      from EXP..EQT_FIL
      where evenement=@NumEvt_In
        and cle=@NumEvtCle_In
	*/

      return XDC_OK
      
      end
#endif
go
