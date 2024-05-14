/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	15 Nov 1994	: Creation
* gaborit	25 Nov 1994	: Modif
* gaborit	25 Nov 1994	: pb sccs 1.3
* gaborit       30 Aou 1995	: modif pour prendre en compte plusieurs mises
				  en file d attente (1.5)
* guilhou	27 mar 1997	: evt est un INT 1.6
* Guilhou	06 mar 1998	: ajout colonne sit dans EQT_FIL (dem/1595) 1.7
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.8
* GGY		26/09/23	: Ajout district CA (DEM483)
* JPL		20/10/23	: Pas de FMC d'Essai ou Exercice en attente dans EQT_FIL (SAE_525)  1.9
------------------------------------------------------*/


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Indique que le SAD desire et ne peut pas utiliser l'equipement pour l'evenement pour
*  lequel il calcule le plan d'actions
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAP;21
   @va_NumPA_in        	smallint = null,
   @va_NumEqt_in	smallint = null,
   @va_TypeEqt_in	tinyint = null,
   @va_PrioriteEvt_in	tinyint = null,
   @va_DistanceEvt_in	int = null,
   @va_Resultat_out	int = null output
   as 

/* 
*
* SP	XZAP21SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumPA_in
* XDY_NomEqt		va_NumEqt_in
* XDY_Octet		va_TypeEqt_in
* XDY_Octet		va_PrioriteEvt_in
* XDY_Entier		va_DistanceEvt_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Indique que le SAD desire et ne peut pas utiliser l'equipement pour l'evenement pour
* lequel il calcule le plan d'actions
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
*
* MECANISMES :
*
------------------------------------------------------*/
/*
#ifdef CI
*/
/* pas de file d'attente au CI */
/*return XDC_NOK*/
/*
#endif
#ifndef CI
*/
#ifdef HIST
declare @vl_Status int, @vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP;21
			@va_NumPA_in        	,
			@va_NumEqt_in	,
			@va_TypeEqt_in	,
			@va_PrioriteEvt_in	,
			@va_DistanceEvt_in	,
			@va_Resultat_out	 output

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;21
			@va_NumPA_in        	,
			@va_NumEqt_in	,
			@va_TypeEqt_in	,
			@va_PrioriteEvt_in	,
			@va_DistanceEvt_in	,
			@va_Resultat_out	 output

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Cle		tinyint,
   @vl_Evt		int,
   @vl_Eqt		smallint,
   @vl_Numero		smallint,
   @vl_Num		smallint,
   @vl_NumMod		smallint,
   @vl_Priorite		tinyint,
   @vl_Site		tinyint

   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumPA_in = null  
   or @va_NumEqt_in = null 
   or @va_TypeEqt_in = null 
   or @va_PrioriteEvt_in = null 
   or @va_DistanceEvt_in = null 
     
      return XDC_ARG_INV
   
   else   
   begin
      
      /*A
      ** Recherche de l'evenement associe au plan d'action
      */
      
      select @vl_Evt = evenement, @vl_Cle = cle from EXP..SAD_PLN
        where numero = @va_NumPA_in
      
      select @vl_Site = numero from CFG..RES_DIS
      where serveur = @@servername

      /*A
      ** Recherche si l'equipement qui voulait utiliser l'evenement
      */
      
      select @vl_Eqt = equipement from EXP..EQT_FIL_TRV
        where type_equipement 	= @va_TypeEqt_in 
          and equipement 	= @va_NumEqt_in
        
      if @vl_Eqt != null
      begin
      
      /*B
      ** L'equipement voulait utiliser l'evenement
      ** delete de la file d'attente de travail
      ** update de la priorite et de la distance de la fil d'attente equipement
      */   
      
         delete EXP..EQT_FIL_TRV
         where type_equipement 	= @va_TypeEqt_in 
           and equipement 	= @va_NumEqt_in 
      
         update EXP..EQT_FIL
         set distance = @va_DistanceEvt_in,
             priorite = @va_PrioriteEvt_in
         where evenement 	= @vl_Evt
           and cle		= @vl_Cle
           and type_equipement 	= @va_TypeEqt_in 
           and equipement 	= @va_NumEqt_in 
      end
          
      else
      
      /*B
      ** L'equipement ne voulait pas utiliser l'evenement
      ** insert dans la file d'attente equipement 
      */   
      begin
	select @vl_Priorite=priorite
	from EXP..EQT_FIL
	where evenement        = @vl_Evt
	  and cle              = @vl_Cle
	  and type_equipement  = @va_TypeEqt_in
	  and equipement       = @va_NumEqt_in

	/*B
	** L'equipement a deja ete mis en file au cours du meme plan d actions
	*/
	if @@rowcount=1 and @vl_Priorite<@va_PrioriteEvt_in
	begin

	 update EXP..EQT_FIL
	 set priorite=@va_PrioriteEvt_in
	 where evenement        = @vl_Evt
	  and cle              = @vl_Cle
	  and type_equipement  = @va_TypeEqt_in
	  and equipement       = @va_NumEqt_in

	end
	else
	begin
	   select 1 from EXP..FMC_GEN
	   where numero = @vl_Evt  and  cle = @vl_Cle
	   and origine_creation not like '%ESSAI%'
	   and origine_creation not like '%EXERCICE%'
	   and origine_creation not like '%SUPPRIMER%'

	   if @@rowcount = 1
	   begin
	       insert EXP..EQT_FIL ( evenement, cle, type_equipement, equipement, distance, priorite, sit )
	       values ( @vl_Evt, @vl_Cle, @va_TypeEqt_in, @va_NumEqt_in, @va_DistanceEvt_in, @va_PrioriteEvt_in, @vl_Site )
	   end
	end
      end
   end         
     
   return XDC_OK
/*#endif         */
#endif
go  
